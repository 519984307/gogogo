#ifndef TARMAKER_H
#define TARMAKER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include "baseconfig.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;

#define XML_NODE_STRING_VALUE(node, value) do {\
                                                 xmlChar *data = xmlNodeGetContent(node); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);
#define XML_PROP_STRING_VALUE(node, name, value) do {\
                                                 xmlChar *data = xmlGetProp(node, BAD_CAST(name)); \
                                                 value = (char*)data;\
                                                 xmlFree(data);\
                                              }while(0);


class TarMaker{
private:
    //Value of paths
    string TmpPath;
    string DestPath;

    string TarFileName;
    string TarFile;

    string DefaultConfigName;
    string ConfigFile;

private:
    //Path operation
    class Path {
        public:
            static bool     FileAvailed(const string &str);
            static string   GetCurrentDir();
            static int      ParsePath(string &str);
            static int      SetPath(string &str, string &path);
            static int      CheckPath(const string &str, mode_t mode = 0755);
            static int      MakeDirs(const string &str, mode_t mode = 0755);
    };

    class File;
    //To make Directories
    class Directory {
        public:
            string      Name;
            string      SourcePath;
            string      DestPath;

            Directory   *Next;
            Directory   *FirstChild;
            File        *FirstFile;

            Directory();
            Directory(const string &name, Directory *dir);
            virtual int ParseDir(xmlNodePtr &node);
            int MakeDir();
    };

    class Root : public Directory {
        public:
            TarMaker *Tar;

            Root(TarMaker *tar);
            int ParseDir(xmlNodePtr &node);
    };

    //To make files
    class File {
        public:
            string      DestPath;
            string      SourcePath;
            string      ShellCmd;

            File        *Next;

            File(string &path, Directory *dir);
            int     ParseFiles(xmlNodePtr &node);
            string  GetFileName(const string &path);
            int     MakeFile();
    };

    string      ConfigXml;
    Directory   *RootDir;

public:
    TarMaker(const string &tarconfig="");

    //Xml operation
    int      ParseXmlAndMakeFiles();
    int      ParseRoot(xmlNodePtr &Node);

    void     SetConfigFile(const string &tarconfig);
    int      LoadConfigXml();
    int      MakeTar();
    int      Start();
};

#endif