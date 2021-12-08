#include <cstdio>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include "upgrademaker.h"
#include "commonapi.h"
#include "libxml/parser.h"

upgrademaker::upgrademaker(const string& id, const string& type, const string& version, const string& folder, const string& out)
            : mID(id), mVersion(version), mType(type) {
    mSource = folder;
    mDest = out;

    m7zPath = Utils::Path::JoinPath(mDest.c_str(),  "7z", type.c_str(), NULL);
    mMd5Path = Utils::Path::JoinPath(mDest.c_str(), "md5", type.c_str(), NULL);
    mRpkPath = Utils::Path::JoinPath(mDest.c_str(), "rpk", type.c_str(), NULL);
    mXmlPath = Utils::Path::JoinPath(mDest.c_str(), "xml", type.c_str(),  NULL);
    mSrcPath = Utils::Path::JoinPath(mDest.c_str(), "src",  type.c_str(), NULL);

    Utils::Path::CheckPath(m7zPath);
    Utils::Path::CheckPath(mMd5Path);
    Utils::Path::CheckPath(mRpkPath);
    Utils::Path::CheckPath(mXmlPath);
    Utils::Path::CheckPath(mSrcPath);
}

int upgrademaker::general_xml(const string& file) {
    xmlDocPtr doc = NULL; 
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;  
     
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, BAD_CAST "name", BAD_CAST(mType.c_str()));
    xmlNewProp(root_node, BAD_CAST "version", BAD_CAST(mVersion.c_str()));
    string now = Utils::Date::FormatTimeStamp(Utils::Date::GetTimeStamp());
    xmlNewProp(root_node, BAD_CAST "updatetime", BAD_CAST now.c_str());
    xmlNewProp(root_node, BAD_CAST "id", BAD_CAST mID.c_str());

    xmlNodePtr component = xmlNewNode(NULL, BAD_CAST "componet");
    xmlNodePtr kernel = xmlNewNode(NULL, BAD_CAST "kernel");
    for (auto c : mKernelPool) {
        xmlNodePtr file = xmlNewNode(NULL, BAD_CAST "file");
        xmlNewProp(file, BAD_CAST "name", BAD_CAST c.first.c_str());
        xmlNewProp(file, BAD_CAST "dstpath", BAD_CAST c.second.path.c_str());
        xmlNewProp(file, BAD_CAST "norestart", BAD_CAST c.second.norestart.c_str());
        xmlNewProp(file, BAD_CAST "md5", BAD_CAST c.second.md5.c_str());
        xmlNewProp(file, BAD_CAST "crc", BAD_CAST c.second.crc32.c_str());
        xmlNewProp(file, BAD_CAST "size", BAD_CAST c.second.size.c_str());
        xmlNewProp(file, BAD_CAST "ver", BAD_CAST mVersion.c_str());

        xmlAddChild(kernel, file);
    }
    xmlAddChild(component, kernel);

    xmlNodePtr child = xmlNewNode(NULL, BAD_CAST mType.c_str());
    for (auto c : mHashPool) {
        xmlNodePtr file = xmlNewNode(NULL, BAD_CAST "file");
        xmlNewProp(file, BAD_CAST "name", BAD_CAST c.first.c_str());
        xmlNewProp(file, BAD_CAST "dstpath", BAD_CAST c.second.path.c_str());
        xmlNewProp(file, BAD_CAST "norestart", BAD_CAST c.second.norestart.c_str());
        xmlNewProp(file, BAD_CAST "md5", BAD_CAST c.second.md5.c_str());
        xmlNewProp(file, BAD_CAST "crc", BAD_CAST c.second.crc32.c_str());
        xmlNewProp(file, BAD_CAST "size", BAD_CAST c.second.size.c_str());
        xmlNewProp(file, BAD_CAST "ver", BAD_CAST mVersion.c_str());

        xmlAddChild(child, file);
    }
    xmlAddChild(component, child);
    xmlAddChild(root_node, component);

    string mainxml = file;
    xmlSaveFormatFileEnc(mainxml.c_str(), doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return 0;
}

int upgrademaker::make() {
    if (0 == traval_dir(mSource.c_str())) {
        string xmlfile = mType + "_" + mVersion + ".xml";
        string xml = Utils::Path::JoinPath(mXmlPath.c_str(), xmlfile.c_str(), NULL);
        string path, file;
        Utils::Path::SplitPath(xml, path, file);
        Utils::Path::CheckPath(path);
        if(0 == general_xml(xml)) {
            string validxml = mType + ".xml";
            return Utils::Path::CopyFile(xml, Utils::Path::JoinPath(mXmlPath.c_str(), validxml.c_str(), NULL))?0:-1;
        }
    }
    return -1;
}

int upgrademaker::traval_dir(const char *path) {
    struct dirent *tmp = NULL;
    char tempPath[1024] = {0};

    DIR *dir = opendir(path);
    while (NULL != (tmp = readdir(dir))) {
        if (DT_REG == tmp->d_type) {
            string file = Utils::Path::JoinPath(path, tmp->d_name, NULL);
            if (make_upgrade(file) < 0) {
                return -1;
            }
        } else if (DT_DIR == tmp->d_type) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }

            sprintf(tempPath, "%s/%s", path, tmp->d_name);
            if (traval_dir(tempPath) < 0) {
                return -1;
            }
        }
    }
    closedir(dir);
    return 0;
}

int upgrademaker::make_upgrade(const string& file) {
    string path, name;
    Utils::Path::SplitPath(file, path, name);
    string subpath = file.substr(mSource.size());
    if (subpath.compare(0, 1, "/") == 0) {
        subpath = subpath.substr(1);
    }

    string zipfile = Utils::Path::JoinPath(m7zPath.c_str(), subpath.c_str(), NULL);
    string dpath, dname;
    Utils::Path::SplitPath(zipfile, dpath, dname);
    Utils::Path::CheckPath(dpath);

    zipfile += ".rz";
    if (Utils::SevenZip::lzma_compress(file, zipfile)) {
        return -1;
    }
    
    string srcd = Utils::Path::JoinPath(mSrcPath.c_str(), subpath.c_str(), NULL);
    Utils::Path::SplitPath(srcd, dpath, dname);
    Utils::Path::CheckPath(dpath);
    if (!Utils::Path::CopyFile(file, srcd)) {
        return -1;
    }

    Utils::Path::SplitPath(subpath, dpath, dname);
    string md5hash = Utils::Hash::GetFileMD5(file);
    if (md5hash.empty()) {
        return -1;
    }
    md5hash = Utils::String::ToUpper(md5hash);
    string md5file = dname + "_" + md5hash + ".md5";
    string md5d = Utils::Path::JoinPath(mMd5Path.c_str(), dpath.c_str(), md5file.c_str(), NULL);

    Utils::Path::SplitPath(md5d, dpath, dname);
    Utils::Path::CheckPath(dpath);
    
    if (!Utils::Path::CopyFile(file, md5d)) {
        return -1;
    }

    string crc32 = Utils::Hash::CRC32(file);
    if (crc32.empty()) {
        return -1;
    }


    file_entry entry;
    entry.md5 = md5hash;
    entry.crc32 = crc32;
    long size = get_file_size(file);
    entry.size = Utils::String::itoa(size);
    entry.path = subpath;
    if (Utils::String::EndsWith(name, "ko")) {
        entry.norestart = "1";
        mKernelPool.insert(make_pair(name, entry));
    }
    else {
        entry.norestart = "0";
        mHashPool.insert(make_pair(name, entry));
    }
    return 0;
}

long upgrademaker::get_file_size(const string& file) {
    FILE *f = fopen(file.c_str(), "rb");
    if (f) {
        long size = 0;
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fclose(f);
        return size;
    }
    return 0;
}
