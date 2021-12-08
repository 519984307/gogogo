#include "tarmaker.h"

TarMaker::TarMaker(const string &tarconfig)
{
    TmpPath = "/tmp";
    DestPath = Path::GetCurrentDir();

    DefaultConfigName = "defaulttarconfig.xml";
    SetConfigFile(tarconfig);

    TarFileName = "esmlog.tar";
    Path::SetPath(TarFileName, TarFile);
}

void TarMaker::SetConfigFile(const string &tarconfig){
    if(tarconfig.size()>0){
        string::size_type pos = tarconfig.find("/");
        if(pos == 0)
            ConfigFile = tarconfig;
        else
            ConfigFile = DestPath + "/" + tarconfig;
    }
    else if(Path::FileAvailed(Path::GetCurrentDir() + \
                    "/../plugins/" + DefaultConfigName))
        ConfigFile = Path::GetCurrentDir() + "/../plugins/" + DefaultConfigName;

    else
        ConfigFile = Path::GetCurrentDir() + '/' + DefaultConfigName;
}

//Load XML config file, and save the content in ConfigXml
int TarMaker::LoadConfigXml()
{
    ifstream file(ConfigFile.c_str());
    if(!file){
        return -1;
    }
    else{
        istreambuf_iterator<char> beg(file), end;
        ConfigXml.assign(string(beg, end));
        file.close();
        return 0;
    }
}

//Parse XML and find root element
int TarMaker::ParseXmlAndMakeFiles()
{
    xmlDocPtr doc = xmlReadMemory(ConfigXml.c_str(),\
                    ConfigXml.size(), NULL, NULL, 0);
    if (NULL == doc){
        return -1;
    }
    xmlNodePtr root = xmlDocGetRootElement(doc);

    int ret = -1;
    if(xmlStrcmp(BAD_CAST "root", root->name) == 0){
        ret = ParseRoot(root);
    }
    return ret;
}

//Get TarFileName if was set in Xml, parse its branch and set TarFile
int TarMaker::ParseRoot(xmlNodePtr &node)
{   
    string str("");
    if (node->properties){
        XML_PROP_STRING_VALUE(node, "name", str);
    }
    if (xmlStrcmp(BAD_CAST "root", node->name) == 0){
        RootDir = new Root(this);
        RootDir->ParseDir(node);
    }
    else return -1;

    if(str.size() > 0){
        TarFileName = str;
        string tarfile = DestPath + "/" + TarFileName;
        Path::SetPath(tarfile, TarFile);
    }
    return 0;
}

int TarMaker::MakeTar()
{
    set<string> paths;
    string tmpdirs("");
    int dirlength = 0;
    //Add relative Path of eliminate the redundant folders that
    //contains in absolute path
    for(File *f = RootDir->FirstFile; f; f = f->Next){
        dirlength = f->DestPath.size() - TmpPath.size() \
                    - f->GetFileName(f->DestPath).size() -2;
        if(dirlength != 0)
            paths.insert(f->DestPath.substr(TmpPath.size()+1, dirlength));
        else
            paths.insert(f->GetFileName(f->DestPath));
    }
    for(Directory *d = RootDir->FirstChild; d; d = d->Next){
        paths.insert(d->DestPath.substr(TmpPath.size()+1));
    }
    for(set<string>::iterator iter=paths.begin(); iter!=paths.end(); ++iter){
        tmpdirs += *iter + " ";
    }

    chdir(TmpPath.c_str());
    string shellCmd = "tar -zcvf " + TarFile + " " + tmpdirs;
    system(shellCmd.c_str());

    shellCmd = "rm -rf " + tmpdirs;
    system(shellCmd.c_str());
    return 0;
}

int TarMaker::Start()
{   
    LoadConfigXml();
    ParseXmlAndMakeFiles();
    MakeTar();
    return 0;
}

bool TarMaker::Path::FileAvailed(const string &str){
    struct stat status;
    if(stat(str.c_str(), &status)==0 && !S_ISDIR(status.st_mode)) {
        return true;
    }
    return false;
}

string TarMaker::Path::GetCurrentDir()
{
    char *p             = NULL;
    const int len       = 256;
    ///Keep the absolute path of executable's path
    char tmparr[len]    = {0};
    
    readlink("/proc/self/exe", tmparr, len);
    if (NULL != (p = strrchr(tmparr,'/')))
        *p = '\0';
    else{
        return string("");
    }

    return string(tmparr);
}

//Replace path macros, replace '\' with '/' and remove '/' at 
//the end of the str
int TarMaker::Path::ParsePath(string &str)    
{
    string::size_type pos = str.find("%EXEPATH%");
    if (pos != string::npos){
        str.replace(pos, 9, GetCurrentDir());
    }
    for(string::size_type i(str.find("\\")); i!=string::npos;\
                                        i=str.find("\\",++i)){
        str.replace(i, 1, "/");
    }
    pos = str.rfind('/');
    if(pos == str.size() - 1){
        str = str.substr(0, str.size() - 1);
    }
    return 0;
}

//Set path to a absolute path, with given str
int TarMaker::Path::SetPath(string &str, string &path)
{
    string::size_type pos = path.find("/");
    if(pos != 0){
        path = GetCurrentDir() + "/" + str;
    }
    else
        path = str;
    return 0;
}

int TarMaker::Path::CheckPath(const string &path, mode_t mode){
    struct stat status;
    ////if path don't exist, mkdir
    if(stat(path.c_str(), &status)!=0 || S_ISDIR(status.st_mode)==0) {
        MakeDirs(path, mode);
    }
    return chmod(path.c_str(), mode);
}

//from Utils::Path::mkdirs
int TarMaker::Path::MakeDirs(const string &path, mode_t mode){
    size_t pre=0,pos;
    std::string dir;
    int mdret = 0;

    string target = path;
    if(target[target.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        target += "/";
    }
    if(target[0] == '/') {
        pre = 1;
    }

    while((pos=target.find_first_of('/',pre))!=std::string::npos) {
        dir=target.substr(0,pos++);
        if(dir.size()==0)
            continue;

        pre=pos;
        // if leading / first time is 0 length
        umask(0);
        if((mdret=::mkdir(dir.c_str(), mode)) && errno!=EEXIST) {
            return mdret;
        }
    }
    return mdret;
}

TarMaker::Directory::Directory()
{
    Name        = "";
    SourcePath  = "";
    DestPath    = "";
    Next        = NULL;
    FirstChild  = NULL;
    FirstFile   = NULL;
}

TarMaker::Directory::Directory(const string &name, Directory *dir)
{
    Name        = name;
    SourcePath  = dir->SourcePath + "/" + Name;
    DestPath    = dir->DestPath   + "/" + Name;
    Next        = NULL;
    FirstChild  = NULL;
    FirstFile   = NULL;
}

//Parse the child nodes to get info of this directory
int TarMaker::Directory::ParseDir(xmlNodePtr &node)
{
    for(xmlNodePtr p = node->children; p; p = p->next){
        if     (xmlStrcmp(BAD_CAST "text"    , p->name) == 0){
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "source"  , p->name) == 0){
            string str("");
            XML_NODE_STRING_VALUE(p, str);
            if(str.size()>0){
                Path::ParsePath(str);
                Path::SetPath(str, SourcePath);
            }
        }
        else if(xmlStrcmp(BAD_CAST "dir"     , p->name) == 0){
            string name("");
            if(p->properties){
                XML_PROP_STRING_VALUE(p, "name", name);
            }
            else return -1;

            if(FirstChild == NULL){
                FirstChild  = new Directory(name, this);
            }
            else{
                Directory *tmp = FirstChild;
                FirstChild = new Directory(name, this);
                FirstChild->Next = tmp;
            }
            Path::CheckPath(FirstChild->DestPath);
            FirstChild->ParseDir(p);
        }
        else if(xmlStrcmp(BAD_CAST "file"    , p->name) == 0){
            string name("");
            XML_NODE_STRING_VALUE(p, name);

            if(FirstFile == NULL){
                FirstFile = new File(name, this);
            }
            else{
                File *tmp = FirstFile;
                FirstFile = new File(name, this);
                FirstFile -> Next = tmp;
            }
            FirstFile->ParseFiles(p);
            FirstFile->MakeFile();
        }
        else continue;
    }
    return 0;    
}

TarMaker::Root::Root(TarMaker *tar):Directory()
{
    SourcePath  = Path::GetCurrentDir();
    DestPath    = tar->TmpPath;
    Tar         = tar;
}

int TarMaker::Root::ParseDir(xmlNodePtr &node)
{
    for(xmlNodePtr p = node->children; p; p = p->next){
        if     (xmlStrcmp(BAD_CAST "text"    , p->name) == 0){
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "tmppath" , p->name) == 0){
            string path("");
            XML_NODE_STRING_VALUE(p, path);
            Path::ParsePath(path);
            Path::SetPath(path, Tar->TmpPath);
            DestPath = Tar->TmpPath;
        }
        else if(xmlStrcmp(BAD_CAST "destpath", p->name) == 0){
            XML_NODE_STRING_VALUE(p, Tar->DestPath);
            Path::ParsePath(Tar->DestPath);
            Path::SetPath(Tar->DestPath, Tar->DestPath);
        }
        else if(xmlStrcmp(BAD_CAST "source"  , p->name) == 0){
            string str("");
            XML_NODE_STRING_VALUE(p, str);
            if(str.size()>0){
                Path::ParsePath(str);
                Path::SetPath(str, SourcePath);
            }
        }
        else if(xmlStrcmp(BAD_CAST "dir"     , p->name) == 0){
            string name("");
            if(p->properties){
                XML_PROP_STRING_VALUE(p, "name", name);
            }
            else return -1;

            if(FirstChild == NULL){
                FirstChild  = new Directory(name, this);
            }
            else{
                Directory *tmp = FirstChild;
                FirstChild = new Directory(name, this);
                FirstChild->Next = tmp;
            }
            Path::CheckPath(FirstChild->DestPath);
            FirstChild->ParseDir(p);
        }
        else if(xmlStrcmp(BAD_CAST "file"    , p->name) == 0){
            string name("");
            XML_NODE_STRING_VALUE(p, name);

            if(FirstFile == NULL){
                FirstFile = new File(name, this);
            }
            else{
                File *tmp = FirstFile;
                FirstFile = new File(name, this);
                FirstFile -> Next = tmp;
            }
            FirstFile->ParseFiles(p);
            FirstFile->MakeFile();
        }
        else continue;
    }
    return 0;
}

TarMaker::File::File(string &path, Directory *dir)
{
    Path::ParsePath(path);

    string::size_type pos = path.find("/");
    if(pos == 0){                       //Absolute path
        SourcePath = path;
        //without creating its path
        //DestPath   = dir->DestPath + "/" + GetFileName(path); 
        DestPath   = dir->DestPath + path;
    }
    else{                               //Relative path
        SourcePath = dir->SourcePath + "/" + path;
        DestPath   = dir->DestPath   + "/" + path;
    }
    ShellCmd = "";
}

string TarMaker::File::GetFileName(const string &path)
{
    string::size_type pos = path.rfind("/");
    if(pos == string::npos)
        return path;
    else
        return path.substr(pos+1);
}

int TarMaker::File::ParseFiles(xmlNodePtr &node)
{
    string cmd("");
    if(node->properties){
        XML_PROP_STRING_VALUE(node, "cmd", cmd);
    }
    ShellCmd = cmd;
    return 0;
}

int TarMaker::File::MakeFile()
{
    int pathLength = DestPath.size() - GetFileName(DestPath).size();
    string filedir = DestPath.substr(0, pathLength);
    Path::CheckPath(filedir);
    if(GetFileName(DestPath).find('*') != string::npos){
        DestPath = filedir;
    }

    if(ShellCmd.size() > 0){
        ShellCmd += (" >> " + DestPath);
        system(ShellCmd.c_str());
        return 0;
    }
    else if(SourcePath.size() > 0){
        ShellCmd = "cp " + SourcePath + " " + DestPath;
        system(ShellCmd.c_str());
        return 0;
    }
    return -1;
}