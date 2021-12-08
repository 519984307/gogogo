#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <list>

#include "libxml/parser.h"

using namespace std;

class builder
{
public:
    struct entry {
        string name;
        string version;
        int release;
        string summary;
        string description;
        string section;
        long size;
        string arch;
        list<string> depends;
        list<string> preinst;
        list<string> postinst;
        list<string> prerm;
        list<string> postrm;
    };

public:
    builder() {}
    virtual ~builder() {}
    void set_pkg_folder(const string& folder) {
        pkgfolder = folder;
    }
    void set_temp_folder(const string& folder) {
        source_folder = folder;
    }
    int build(const string& config, const string& outfile);

protected:
    virtual int make_install_script(entry& pkg) { return 0; }
    virtual int make_install_folder(entry& pkg) { return 0; }
    virtual int package(entry& pkg) { return 0; }

    int copy_folder(const string& source, const string& dest);

private:
    int init_entry(const string& config, entry& pkg);
    int parse_node(xmlNodePtr node, list<string>& data);

protected:
    entry   install_entry;

    string source_folder;
    string arch;
    string pkgfolder;
    string outfile;
};

class deb_builder : public builder {
public:
    deb_builder() {}
protected:
    int make_install_script(entry& pkg);
    int make_install_folder(entry& pkg);
    int package(entry& pkg);

private:
    string debian;
};

class rpm_builder : public builder {
public:
    rpm_builder() {}
protected:
    int make_install_script(entry& pkg);
    int make_install_folder(entry& pkg);
    int package(entry& pkg);

private:
    string spec_folder;
    string build_folder;
    string rpm_folder;
    string build_name;
};

#endif // BUILDER_H
