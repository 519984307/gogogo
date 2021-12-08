#include "builder.h"
#include "commonapi.h"
#include "libxml/parser.h"

#include <fstream>
#include <queue>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

extern int rpmbuild_func(int argc, char *argv[]);
extern int deb_func(int argc, char *argv[]);

int builder::build(const string& config, const string& outfile) {
    Utils::Path::CheckPath(source_folder);
    this->outfile = outfile;

    entry pkg;
    if (init_entry(config, pkg)) {
        return -1;
    }

    if (make_install_folder(pkg)) {
        return -3;
    }

    if (make_install_script(pkg)) {
        return -2;
    }

    if (package(pkg)) {
        return -4;
    }

    return 0;
}

int builder::init_entry(const string& config, entry& pkg) {
    if (config.empty()) {
        return -1;
    }

    xmlDocPtr doc = xmlReadFile(config.c_str(), NULL, 0);
    if (NULL == doc){
        return -1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (NULL == root){
        xmlFreeDoc(doc);
        return -2;
    }

    for(xmlNodePtr ptr = root->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "name",ptr->name) == 0) {
            pkg.name = (const char*)xmlNodeGetContent(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "version",ptr->name) == 0) {
            pkg.version = (const char*)xmlNodeGetContent(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "release",ptr->name) == 0) {
            pkg.release = atoi((const char*)xmlNodeGetContent(ptr));
        }
        else if(xmlStrcmp(BAD_CAST "summary",ptr->name) == 0) {
            pkg.summary = (const char*)xmlNodeGetContent(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "section",ptr->name) == 0) {
            pkg.section = (const char*)xmlNodeGetContent(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "size",ptr->name) == 0) {
            pkg.size = atoi((const char*)xmlNodeGetContent(ptr));
        }
        else if(xmlStrcmp(BAD_CAST "arch",ptr->name) == 0) {
            pkg.arch = (const char*)xmlNodeGetContent(ptr);
        }
        else if(xmlStrcmp(BAD_CAST "depends",ptr->name) == 0) {
            parse_node(ptr, pkg.depends);
        }
        else if(xmlStrcmp(BAD_CAST "preinst",ptr->name) == 0) {
            parse_node(ptr, pkg.preinst);
        }
        else if(xmlStrcmp(BAD_CAST "postinst",ptr->name) == 0) {
            parse_node(ptr, pkg.postinst);
        }
        else if(xmlStrcmp(BAD_CAST "prerm",ptr->name) == 0) {
            parse_node(ptr, pkg.prerm);
        }
        else if(xmlStrcmp(BAD_CAST "postrm",ptr->name) == 0) {
            parse_node(ptr, pkg.postrm);
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int builder::parse_node(xmlNodePtr node, list<string>& data) {
    for(xmlNodePtr ptr = node->children; ptr; ptr = ptr->next) {
        if(XML_ELEMENT_NODE != ptr->type) {
            continue;
        }
        else if(xmlStrcmp(BAD_CAST "item",ptr->name) == 0) {
            data.push_back((const char*)xmlNodeGetContent(ptr));
        }
    }
    return 0;
}

int builder::copy_folder(const string& source, const string& dest) {
    queue<string> traval;
    traval.push(source);

    struct dirent *tmp = NULL;
    while (!traval.empty()) {
        string top = traval.front();
        return 0;
        DIR *dir = opendir(top.c_str());
        while (NULL != (tmp = readdir(dir))) {
            if (DT_REG == tmp->d_type) {
                string sfile = Utils::Path::JoinPath(top.c_str(), tmp->d_name, NULL);
                string dfile = Utils::Path::JoinPath(dest.c_str(), sfile.substr(source.size()).c_str(), NULL);

                string path, name;
                Utils::Path::SplitPath(dfile, path, name);
                Utils::Path::CheckPath(path);

                Utils::Path::CopyFile(sfile, dfile);
            } else if (DT_DIR == tmp->d_type) {
                if (0 == strcmp(tmp->d_name, ".") ||
                        0 == strcmp(tmp->d_name, "..")) {
                    continue;
                }

                string child = Utils::Path::JoinPath(top.c_str(), tmp->d_name, NULL);
                traval.push(child);
            }
        }
        closedir(dir);
        traval.pop();
    }
    return 0;
}

int deb_builder::make_install_script(entry& pkg) {
    string control = Utils::Path::JoinPath(debian.c_str(), "control", NULL);
    ofstream ofs(control);
    if (ofs.bad()) {
        return -1;
    }

    ofs << "Package: com.rising.antivirus" << endl;
    ofs << "Priority: extra" << endl;
    ofs << "Section: utils" << endl;
    ofs << "Installed-Size:" << pkg.size << endl;
    ofs << "Maintainer: Rising" << endl;
    ofs << "Architecture: " << pkg.arch << endl;;
    ofs << "Version: " << pkg.version << endl;
    ofs << "Description: " << pkg.description << endl;

    ofs.close();

    string preinst = Utils::Path::JoinPath(debian.c_str(), "preinst", NULL);
    ofstream pifs(preinst);
    if (!pifs){
        return -1;
    }
    pifs << "#!/bin/sh" << endl;
    for (auto item : pkg.preinst) {
        pifs << item << endl;
    }
    pifs.close();

    string postinst = Utils::Path::JoinPath(debian.c_str(), "postinst", NULL);
    ofstream pofs(postinst);
    if (!pofs){
        return -1;
    }
    pofs << "#!/bin/sh" << endl;
    for (auto item : pkg.postinst) {
        pofs << item << endl;
    }
    pofs.close();

    string prerm = Utils::Path::JoinPath(debian.c_str(), "prerm", NULL);
    ofstream prfs(prerm);
    if (!prfs){
        return -1;
    }
    prfs << "#!/bin/sh" << endl;
    for (auto item : pkg.prerm) {
        prfs << item << endl;
    }
    prfs.close();

    string postrm = Utils::Path::JoinPath(debian.c_str(), "postrm", NULL);
    ofstream porm(postrm);
    if (!porm){
        return -1;
    }
    porm << "#!/bin/sh" << endl;
    for (auto item : pkg.postrm) {
        porm << item << endl;
    }
    porm.close();
    return 0;
}

int deb_builder::make_install_folder(entry& pkg) {
    debian = Utils::Path::JoinPath(source_folder.c_str(), "DEBIAN", NULL);
    Utils::Path::CheckPath(debian);
    return 0;
}

int deb_builder::package(entry& pkg) {
    copy_folder(pkgfolder, source_folder);

    char command[4][128];
    strcpy(command[0], "dpkg");
    strcpy(command[1], "-b");
    strcpy(command[2], outfile.c_str());
    strcpy(command[3], source_folder.c_str());

    // dpkg -b rav.deb rav
    char *comm[] = {command[0], command[1], command[2], command[3], NULL};
    return deb_func(5, comm);
}


int rpm_builder::make_install_script(entry& pkg) {
    string spec = Utils::Path::JoinPath(spec_folder.c_str(), "rav.spec", NULL);
    ofstream ofs(spec);
    if (!ofs) {
        return -1;
    }
    ofs << "Name: RAV" << endl;
    ofs << "Version: " << pkg.version << endl;
    ofs << "Release: " << pkg.release << endl;
    ofs << "Summary: " << pkg.summary << endl;
    ofs << "Group: Applications" << endl;
    ofs << "URL: http://www.rising.com.cn" << endl;
    ofs << "Packager: Rising" << endl;
    ofs << endl;
    ofs << "%description" << endl;
    ofs << pkg.description << endl;
    ofs << endl;
    ofs << "%pre" << endl;
    for (auto item : pkg.preinst) {
        ofs << item << endl;
    }
    ofs << endl;
    ofs << "%post" << endl;
    for (auto item : pkg.postinst) {
        ofs << item << endl;
    }
    ofs << endl;
    ofs << "%preun" << endl;
    for (auto item : pkg.prerm) {
        ofs << item << endl;
    }
    ofs << endl;
    ofs << "%postun" << endl;
    for (auto item : pkg.postrm) {
        ofs << item << endl;
    }
    ofs << endl;
    ofs << endl;
    ofs << "%files" << endl;
    ofs << "%defattr(-,root,root,-)" << endl;

    queue<string> traval;
    traval.push(pkgfolder);

    struct dirent *tmp = NULL;
    while (!traval.empty()) {
        string top = traval.front();
        DIR *dir = opendir(top.c_str());
        while (NULL != (tmp = readdir(dir))) {
            if (DT_REG == tmp->d_type) {
                string file = Utils::Path::JoinPath(top.c_str(), tmp->d_name, NULL);
                ofs << file.substr(pkgfolder.size()) << endl;
            } else if (DT_DIR == tmp->d_type) {
                if (0 == strcmp(tmp->d_name, ".") ||
                        0 == strcmp(tmp->d_name, "..")) {
                    continue;
                }

                string child = Utils::Path::JoinPath(top.c_str(), tmp->d_name, NULL);
                traval.push(child);
            }
        }
        closedir(dir);
        traval.pop();
    }
    ofs.close();
    return 0;
}

int rpm_builder::make_install_folder(entry& pkg) {
    string control = Utils::Path::JoinPath(source_folder.c_str(), "BUILDROOT", NULL);
    Utils::Path::CheckPath(control);

    struct utsname un;
    if (uname(&un)) {
        return -1;
    }

    arch = un.machine;
    build_name = string("rav") + "-" + pkg.version + "-" + Utils::String::itoa(pkg.release) + "." + arch;

    build_folder = Utils::Path::JoinPath(control.c_str(), build_name.c_str(), NULL);
    Utils::Path::CheckPath(build_folder);

    spec_folder = Utils::Path::JoinPath(source_folder.c_str(), "SPECS", NULL);
    Utils::Path::CheckPath(spec_folder);

    rpm_folder = Utils::Path::JoinPath(source_folder.c_str(), "RPMS", NULL);
    Utils::Path::CheckPath(rpm_folder);
    return 0;
}

int rpm_builder::package(entry& pkg) {
    string dest = Utils::Path::JoinPath(source_folder.c_str(), "BUILDROOT", build_name.c_str(), NULL);
    copy_folder(pkgfolder, dest);

    char command[8][128];
    strcpy(command[0], "rpmbuild");
    strcpy(command[1], "--define");
    sprintf(command[2], "${dir:-%s}", source_folder.c_str());
    strcpy(command[3], "-bb");
    strcpy(command[4], "SPECS/rav.spec");
    strcpy(command[5], "--noclean");
    strcpy(command[6], "--target");
    strcpy(command[7], arch.c_str());

    //rpmbuild --define "_topdir ${dir:-/home/miwoo/workspace/esm_dev/package_tools/rpm-4.14.1/test/rpmbuild}" -bb SPECS/rav.spec  --noclean --target mips64el
    char *comm[] = {command[0], command[1], command[2], command[3],command[4], command[5], command[6], command[7], NULL};
    if (0 == rpmbuild_func(8, comm)) {
        string dest = Utils::Path::JoinPath(rpm_folder.c_str(), arch.c_str(), (build_name+".rpm").c_str(), NULL);
        rename(dest.c_str(), outfile.c_str());
        return 0;
    }
    return -1;
}
