#include "tarmaker.h"
#include <getopt.h>

static string g_ConfigFile("");
static const struct option long_options[] = {
    {"help",    no_argument,        NULL, 'h'},
    {"config",  required_argument,  NULL, 'c'},
    {0,         0,                  NULL,  0 }
};

void print_help_txt()
{
    cout << endl;
    cout << "Usage: RAVTAR [OPTION]... [FILE]..." << endl;
    cout << endl;
    cout << "Mandatory arguments to long options are mandatory for short options too." << endl;
    cout << "    -c, --config=FILE   Set config file of your target tar file." << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    cout << "XML format:" << endl;
    cout << "<root name = \"t.tar\">            //[optional p_name] to make tar file named"<< endl
         << "                                   //t.tar. Default name: esmlog.tar."<<endl;
    cout << "   <tmppath>...</tmppath>          //[optional] To set temp file's path." << endl
         << "                                   //Default tmppath: /tmp ." << endl;
    cout << "   <destpath>...</destpath>        //[optional] To set tar file's path." << endl
         << "                                   //Default destpath: . ." << endl;
    cout << "   <source>...<source>             //To set source path of its branch." << endl;
    cout << "   <dir name = \"...\">...</dir>   //To create a dir" << endl;
    cout << "   <file>...</file>                //To create a file from source. The source" << endl
         << "                                   //path is either from its father element," << endl
         << "                                   //or from its node value." << endl;
    cout << "   <file cmd = \"...\">...</file>  //To create file from cmd output." << endl;
    cout << "</root>" << endl;
    cout << endl;
}

int parse_args(int argc, char *argv[])
{
    int pi = 0, ret = 0;
    optind = optind ? optind : 1; 
    while((ret = getopt_long(argc, argv, "hc:", long_options, &pi)) != -1){
        switch (ret){
            case 'h':
                print_help_txt();
                return 1;
            case 'c':
                g_ConfigFile = optarg;
                break;
            case '?':
                cout << "invalid param: " << argv[pi] << endl;
                return -1;
            default :
                return 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) 
{
    int err = parse_args(argc, argv);
    if(err){
        return err;
    }

    TarMaker tar(g_ConfigFile);
    err = tar.Start();

    return err;
}