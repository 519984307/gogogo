#!/usr/bin/python
# coding:utf-8
# package script, from 250 get file and pack it to install pkg
# //193.168.10.250/资源文件/svn on $major_folder/smb250 type cifs (rw)

import time
import hashlib
import urllib
import urllib2
import json
import shutil
import os
import sys
import platform
import tarfile
import commands
import glob


config = """{
    "kernel": [
        {
            "name": "Netfilter",
            "pid": 10081,
            "ko": ["ravfilter.ko"]
        },
        {
            "name": "FileDefen",
            "pid": 10080,
            "ko": ["ravdefen.ko"]
        }
    ],
    "platform": [
        {
            "name": "Netfilter",
            "pid": 10081,
            "lib": ["libravfilterbinder.so"]
        },
        {
            "name": "FileDefen",
            "pid": 10080,
            "lib": ["libravdefenbinder.so"]
        },
        {
            "name": "CommonAPI",
            "pid": 10063,
            "lib": ["libravcommon.so"]
        },
        {
            "name": "Communicator",
            "pid": 10064,
            "lib": ["libravcommunicator.so"]
        },
        {
            "name": "ScanEngine",
            "pid": 10076,
            "lib": ["libravscanengine.so"]
        },
        {
            "name": "Storage",
            "pid": 10077,
            "lib": ["libravstorage.so"]
        },
        {
            "name": "Upgrade",
            "pid": 10078,
            "bin": ["RAVGLTX2"],
            "lib": ["libravupgrade.so"]
        },
        {
            "name": "thirdpart",
            "pid": 10177,
            "lib": [
                "libravz.so",
                "libravssl.so",
                "libravcrypto.so",
                "libravcurl.so",
                "libravxml2.so",
                "libravncurses.so",
                "libraviconv.so",
                "libqxcb.so",
                "libQt5Gui.so",
                "libQt5Core.so",
                "libQt5Widgets.so",
                "libQt5XcbQpa.so"
            ]
        },
                {
            "name": "unix_engine2013",
            "pid": 10085,
            "lib": [
                "revm.so",
                "rxmltk.so",
                "rxxman.so",
                "scanpe.so",
                "vex86.so",
                "rxarch.so",  
                "rxoffice.so",
                "scanelf.so",
                "scansct.so",
                "librxbase.so",
                "rxcore.so",
                "rxvision.so",
                "scanexec.so",
                "scriptci.so",
                "rxfsm.so",   
                "rxwinpe.so",   
                "scanex.so",    
                "uroutine.so",
                "methodex.so",   
                "rxmail.so",  
                "rxxdu0.so",    
                "scanmac.so",   
                "urutils.so"
            ]
        },
        {
            "name": "RavConsole_MainApp",
            "pid": 10065,
            "bin": ["rav.bin"]
        },
        {
            "name": "RavDaemon",
            "pid": 10067,
            "bin": ["RAVGJCZ0","RAVGLTX0","RAVZDFY0", "RAVRJJH0", "RAVRJJH00"]
        },
        {
            "name": "RavQt_Config",
            "pid": 10069,
            "bin": ["RAVRJJH4"]
        },
        {
            "name": "RavQt_LogViewer",
            "pid": 10070,
            "bin": ["RAVRJJH6"]
        },
        {
            "name": "RavQt_MainApp",
            "pid": 10068,
            "bin": ["RAVRJJH2"]
        },
        {
            "name": "RavQt_Notifier",
            "pid": 10071,
            "lib": ["libravqtsupport.so"]
        },
        {
            "name": "RavConsole_Setup",
            "pid": 10184,
            "bin": ["ravsetup.bin","downloader.bin","crossupdater.bin"]
        },
        {
            "name": "RavConsole_Uninstall",
            "pid": 10066,
            "bin": ["ravuninst.bin"]
        }
    ],
    "common": [{
        "name": "Resources",
        "pid": 10185,
        "file": ["RJJHetc", "default.ini"]
                },
                {
        "name": "Resources",
        "pid": 10185,
        "file": ["LOGO", "rising.png"]
                },
                {
        "name": "Resources",
        "pid": 10185,
        "file": ["plugins", "config.xml"]
                },
                {   
        "name": "Resources",
        "pid": 10185,
        "file": ["plugins", "RsConfig.cfg"]
                },
                {
        "name": "Resources",
        "pid": 10185,
        "file": [".", "setup.sh"]
        }]
}"""

config_json = """{
    "eid": "123456",
    "dl_rpk": ["ep", "xav", "xfw"],
    "dl_silence": "1",
    "dl_path": "",
    "no.logfile": "0",
    "server": ["rd3", "tool"],
    "packmode": 0
}"""

cpfile_json = """
{
    "static": [
        {"setup":[
            {"bin":[
                {"path":"."},
                {"files":["ravsetup.bin"]}
            ]},
            {"shell":[
                {"path": "."},
                {"files": ["setup.sh"]}
            ]}
        ]},
        {"common":[
            {"ravdb":[
                {"path": "RJJHGJCZ"},
                {"files": ["malware.rmd"]}
            ]},
            {"ini":[
                {"path": "RJJHetc"},
                {"files": ["default.ini"]}
            ]},
            {"xml":[
                {"path": "plugins"},
                {"files": ["config.xml", "RsConfig.cfg"]}
            ]},
            {"logo":[
                {"path": "LOGO"},
                {"files": ["rising.png"]}
            ]}   
         ]}
    ],
    "ep": [
        {"bin":[
            {"path": "bin"},
            {"files": [
                "RAVRJJH0",
                "RAVRJJH00",
                "RAVRJJH2",
                "RAVRJJH4",
                "RAVRJJH6",
                "RAVRJJH8",
                "rav.bin",
                "RAVGLTX0",
                "RAVGLTX2",
                "RAVZDFY0",
                "ravuninst.bin"
            ]}
         ]},
         {"lib":[
            {"path": "lib"},
            {"files": [
                "libravcommunicator.so",
                "libravcommon.so",
                "libravcryto.so",
                "libravcrypto.so",
                "libravssl.so",
                "libravcurl.so",
                "libravz.so",
                "libravxml2.so",
                "libqxcb.so",
                "libravqtsupport.so",
                "libQt5Widgets.so",
                "libQt5Core.so",
                "libQt5Gui.so",
                "libQt5XcbQpa.so",
                "libraviconv.so",
                "libravupgrade.so",
                "libravscanengine.so",
                "libravstorage.so",
                "libravncurses.so"
            ]}
         ]}
    ],
    "xav": [
        {"lib":[
            {"path": "lib"},
            {"files": [
                "libravdefenbinder.so"
            ]}
        ]},
        {"bin":[
            {"path": "bin"},
            {"files": [
                "RAVGJCZ0"
            ]}
        ]},
        {"kernel":[
            {"path": "plugins"},
            {"files": [
                "ravdefen.ko"
            ]}
        ]},
        {"unix_engine2013": [
            {"path": "lib"},
            {"files": [
               "revm.so",
                "rxmltk.so",
                "rxxman.so",
                "scanpe.so",
                "vex86.so",
                "rxarch.so",  
                "rxoffice.so",
                "scanelf.so",
                "scansct.so",
                "librxbase.so",
                "rxcore.so",
                "rxvision.so",
                "scanexec.so",
                "scriptci.so",
                "rxfsm.so",   
                "rxwinpe.so",   
                "scanex.so",    
                "uroutine.so",
                "methodex.so",   
                "rxmail.so",  
                "rxxdu0.so",    
                "scanmac.so",   
                "urutils.so"
            ]}
        ]}
    ],
    "xfw": [
        {"lib": [
            {"path": "lib"},
            {"files": [
                "libravfilterbinder.so"
            ]}
        ]},

        {"bin": [
            {"path": "bin"},
            {"files": [
                "RAVZDFY2"
            ]}
        ]},
        {"kernel": [
            {"path": "plugins"},
            {"files": [
                "ravfilter.ko"
            ]}
        ]}
    ],
    "tools": [
        {"bin": [
            {"path": "tools"},
            {"files": [
                "downloader.bin",
                "crossupdater.bin",
                "ravsetup.bin"
            ]}
        ]}
    ]
}
"""

class CpFiles:
    def __init__(self, sjson):
        cjson = json.loads(sjson)
        self.static = cjson["static"] if cjson.has_key("static") else None
        self.ep = cjson["ep"] if cjson.has_key("ep") else None
        self.xav = cjson["xav"] if cjson.has_key("xav") else None
        self.xfw = cjson["xfw"] if cjson.has_key("xfw") else None
        self.tools = cjson["tools"] if cjson.has_key("tools") else None
        assert self.static
        assert self.ep
        assert self.xav
        assert self.xfw
        assert self.tools

    def checkStaticOutCopyFile(self, filePath, outMainPath, outCommonPath, outConfigPath):
        inFile = os.path.basename(filePath)
        if self.static:
            for obj in self.static:
                for obj1 in obj:
                    if obj1 == "setup":
                        for obj2 in obj["setup"]:
                            for obj3 in obj2:
                                for obj4 in obj2[obj3]:
                                    if obj3 == "bin":
                                        if "path" in obj4.keys():
                                            binPath = obj4["path"] 
                                        elif "files" in obj4.keys():
                                            binFiles = obj4["files"]        
                                            for obj5 in binFiles:
                                                if obj5 == inFile:
                                                    return os.path.join(outMainPath, binPath, inFile)
                                    if obj3 == "shell":
                                       if "path" in obj4.keys():
                                            shPath = obj4["path"]
                                       elif "files" in obj4.keys():
                                            shFiles = obj4["files"]
                                            for obj5 in shFiles:
                                                if obj5 == inFile:
                                                    return os.path.join(outMainPath, shPath, inFile)

                    if obj1 == "common":
                        for obj2 in obj["common"]:
                            for obj3 in obj2:
                                for obj4 in obj2[obj3]:
                                    if (obj3 == "ini") or (obj3 == "xml"):
                                       if "path" in obj4.keys():
                                            configPath = obj4["path"]
                                       elif "files" in obj4.keys():
                                            configFiles = obj4["files"]
                                            for obj5 in configFiles:
                                                if obj5 == inFile:
                                                    return os.path.join(outCommonPath, configPath, inFile)
                                    if obj3 == "ravdb":
                                        if "path" in obj4.keys():
                                            commonPath = obj4["path"] 
                                        elif "files" in obj4.keys():
                                            commonFiles = obj4["files"]        
                                            for obj5 in commonFiles:
                                                if obj5 == inFile:
                                                    return os.path.join(outCommonPath, commonPath, inFile)
                                    if obj3 == "logo":
                                       if "path" in obj4.keys():
                                            logoPath = obj4["path"]
                                       elif "files" in obj4.keys():
                                            logoFiles = obj4["files"]
                                            for obj5 in logoFiles:
                                                if obj5 == inFile:
                                                    return os.path.join(outCommonPath, logoPath, inFile)
        return None

    def checkEpOutCopyFile(self, filePath, outPath):
        inFile = os.path.basename(filePath)
        if self.ep:
            for obj in self.ep:
                for obj1 in obj:
                    if obj1 == "bin":
                        for obj2 in obj["bin"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    binPath = obj2[obj3] 
                                if obj3 == "files":
                                    binFiles = obj2[obj3]        
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, binPath, inFile)
                    if obj1 == "lib":
                        for obj2 in obj["lib"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    libPath = obj2[obj3]
                                if obj3 == "files":
                                    libFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, libPath, inFile)

        return None


    def checkXavOutCopyFile(self, filePath, outPath):
        inFile = os.path.basename(filePath)
        if self.xav:
            for obj in self.xav:
                for obj1 in obj:
                    if obj1 == "bin":
                        for obj2 in obj["bin"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    binPath = obj2[obj3] 
                                if obj3 == "files":
                                    binFiles = obj2[obj3]        
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, binPath, inFile)
                    if obj1 == "lib":
                        for obj2 in obj["lib"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    libPath = obj2[obj3]
                                if obj3 == "files":
                                    libFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, libPath, inFile)
                    if obj1 == "kernel":
                        for obj2 in obj["kernel"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    koPath = obj2[obj3]
                                if obj3 == "files":
                                    koFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, koPath, inFile)
                    if obj1 == "unix_engine2013":
                        for obj2 in obj["unix_engine2013"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    enginePath = obj2[obj3]
                                if obj3 == "files":
                                    engineFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, enginePath, inFile)
        return None

    def checkXfwOutCopyFile(self, filePath, outPath):
        inFile = os.path.basename(filePath)
        if self.xfw:
            for obj in self.xfw:
                for obj1 in obj:
                    if obj1 == "lib":
                        for obj2 in obj["lib"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    libPath = obj2[obj3]
                                if obj3 == "files":
                                    libFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, libPath, inFile)

                    if obj1 == "bin":
                        for obj2 in obj["bin"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    binPath = obj2[obj3] 
                                if obj3 == "files":
                                    binFiles = obj2[obj3]        
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, binPath, inFile)
                    if obj1 == "kernel":
                        for obj2 in obj["kernel"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    libPath = obj2[obj3]
                                if obj3 == "files":
                                    libFiles = obj2[obj3]
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, libPath, inFile)

        return None

    def checkToolsOutCopyFile(self, filePath, outPath):
        inFile = os.path.basename(filePath)
        if self.tools:
            for obj in self.tools:
                for obj1 in obj:
                    if obj1 == "bin":
                        for obj2 in obj["bin"]:
                            for obj3 in obj2:
                                if obj3 == "path":
                                    binPath = obj2[obj3] 
                                if obj3 == "files":
                                    binFiles = obj2[obj3]        
                                    for obj4 in obj2["files"]:
                                        if obj4 == inFile:
                                            return os.path.join(outPath, binPath, inFile)

        return None




    def copyFile(self, srcFile, dstFile):
        #print "copyFile: " + srcFile + " -> " + dstFile
        if not os.path.exists(srcFile):
            print "error, CpFiles::copyFile: not find file " + srcFile
            return
        if os.path.exists(dstFile):
            os.remove(dstFile)
        print "copyFile:" + srcFile + " -> " + dstFile
        shutil.copy(srcFile, dstFile)


class Component():
    def __init__(self, json, folder):
        self.name = json["name"] if json.has_key("name") else None
        self.pid = json["pid"] if json.has_key("pid") else None
        self.bin = json["bin"] if json.has_key("bin") else None
        self.lib = json["lib"] if json.has_key("lib") else None
        self.file = json["file"] if json.has_key("file") else None
        self.ko = json["ko"] if json.has_key("ko") else None 
        self.version = None
        self.mountPath = folder

    def setMountPath(self, mountPath):
        self.mountPath = mountPath

    def getMountFilePath(self, arch):
        assert self.mountPath
        assert self.name
        pathName = self.name + "_" + str(self.pid)
        pidPath = self.findMaxVersionFilePath(arch, pathName)
        if pidPath == None:
            print "findMaxVersionFilePath fail, pathName=" + pathName
            exit(-1)
            return None
        if os.path.exists(pidPath):
            return pidPath
        return None

    def findMaxVersionFilePath(self, arch, path):  # returned value only include path
        maxval = [0,0,0,0]
        dirlen = 4;
        versionPath = ""
        parents = os.listdir(os.path.join(self.mountPath, path))
        for parent in parents:
            child = os.path.join(self.mountPath, path, parent)
            if os.path.isdir(child):
                dirs = os.path.basename(parent).split(".")
                dirlen = len(dirs)
                if dirlen == 4:
                    if int(dirs[0]) > maxval[0]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), int(dirs[3])]
                        continue
                    elif int(dirs[1]) > maxval[1]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), int(dirs[3])]
                        continue
                    elif int(dirs[2]) > maxval[2]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), int(dirs[3])]
                        continue
                    elif  int(dirs[3]) > maxval[3]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), int(dirs[3])]
                        continue
                elif dirlen == 3:
                    if int(dirs[0]) > maxval[0]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), 0]
                        continue
                    elif int(dirs[1]) > maxval[1]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), 0]
                        continue
                    elif int(dirs[2]) > maxval[2]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), 0]
                        continue
                    elif  int(dirs[3]) > maxval[3]:
                        maxval = [int(dirs[0]), int(dirs[1]), int(dirs[2]), 0]
                        continue

        if len(dirs) == 3:
            versionPath = "{0}.{1}.{2}".format(str(maxval[0]),str(maxval[1]),str(maxval[2]))
        elif len(dirs) == 4:
            versionPath = "{0}.{1}.{2}.{3}".format(str(maxval[0]),str(maxval[1]),str(maxval[2]),str(maxval[3]))
        if arch == None:
            filePath = os.path.join(self.mountPath, path, versionPath)
        else:
            filePath = os.path.join(self.mountPath, path, versionPath, arch)
        if os.path.exists(filePath):
            return filePath
        print "Find max path, but not find file, filePath=" + filePath
        return None


#
#    def getComponents(self, arch=None):
#        if self.bin:
#            for binfile in self.bin:
#                self.getComponentFile(binfile, "bin", arch)
#
#        if self.lib:
#            for libfile in self.lib:
#                self.getComponentFile(libfile, "lib", arch)
#
#        if self.file:
#            assert len(self.file) == 2
#            self.getComponentFile(self.file[1], self.file[0], arch)


class PackageLib(object):
    def __init__(self, config, packPath, mountPath):
        self.kernel = []
        self.components = []
        self.common = []
        self.arch = []
        self.ep_md5 = ""
        self.ep_size = 0
        self.xav_md5 = ""
        self.xav_size = 0
        self.xfw_md5 = ""
        self.xfw_size = 0
        self.tools_md5 = "";
        self.tools_size = 0;
        self.vlib_md5 = ""
        self.vlib_size = 0
        self.packTools = ""
        self.packDir = packPath
        self.mountPath = mountPath
        self._parseConfig(config)

    def _parseConfig(self, config):
        cjson = json.loads(config)
        for pl in cjson["kernel"]:
            self.kernel.append(Component(pl, self.mountPath))

        for pl in cjson["platform"]:
            self.components.append(Component(pl, self.mountPath))

        for pl in cjson["common"]:
            self.common.append(Component(pl, self.mountPath))
        
        self.cfs = CpFiles(cpfile_json)

    def setOutDir(self, outDir):
        self.outDir = outDir

    def setMountPath(self, remotePath, localPath):
        self.mountPath = localPath
        self.remotePath = remotePath
        if not os.path.exists(localPath):
            os.makedirs(localPath)
        status, output = commands.getstatusoutput("mount |grep smb250|wc -l")
        if int(output) == 0:
            command = "mount -t cifs -o username=a,password=a //193.168.10.250/资源文件/svn " + self.mountPath
            status, output = commands.getstatusoutput(command)
            print "setMountPath: mount " + self.mountPath + " return " + str(status)

    def setVirusDbPath(self, vpath):
        pathname = os.path.join(vpath, "malware.rmd_*")
        for filename in glob.glob(pathname):
            self.virusdb = filename
        print self.virusdb
        assert os.path.exists(self.virusdb)

    def makeRAVDirs(self, destDir):
        if not os.path.exists(destDir):
            os.makedirs(destDir)
        os.makedirs(os.path.join(destDir, "bin"))
        os.mkdir(os.path.join(destDir, "tools"))
        os.mkdir(os.path.join(destDir, "lib"))
        os.mkdir(os.path.join(destDir, "log"))
        os.mkdir(os.path.join(destDir, "LOGO"))
        os.mkdir(os.path.join(destDir, "plugins"))
        os.mkdir(os.path.join(destDir, "GJCZetc"))
        os.mkdir(os.path.join(destDir, "RJJHetc"))
        os.mkdir(os.path.join(destDir, "RJJHGJCZ"))
        os.mkdir(os.path.join(destDir, "RJJHGLTX"))
        os.mkdir(os.path.join(destDir, "RJJHZDFY"))

    def refreshPackDir(self):
        if os.path.exists(self.packDir):
            shutil.rmtree(self.packDir)
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.tools"))
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.common"))
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.config"))
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.ep"))
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.xav"))
        self.makeRAVDirs(os.path.join(self.packDir, "RAV.xfw"))

    def refreshOutDir(self):
        if os.path.exists(self.outDir):
            shutil.rmtree(self.outDir)
        os.makedirs(os.path.join(self.outDir, "common"))
        os.makedirs(os.path.join(self.outDir, "setup"))

    def copyPackageFile(self, filepath):
        namefile = filepath
        if filepath.find("malware.rmd") != -1:
            dirname = os.path.dirname(filepath)
            basename = os.path.basename(filepath)
            eles = basename.split("_")
            namefile = os.path.join(dirname, eles[0])
            
        dfile = self.cfs.checkStaticOutCopyFile(namefile, self.packDir, os.path.join(self.packDir, "RAV.common"), os.path.join(self.packDir, "RAV.config"))
        if dfile != None:
            self.cfs.copyFile(filepath, dfile)
            return
        dfile = self.cfs.checkEpOutCopyFile(filepath, os.path.join(self.packDir, "RAV.ep"))
        if dfile != None:
            self.cfs.copyFile(filepath, dfile)
            return
        dfile = self.cfs.checkXavOutCopyFile(filepath, os.path.join(self.packDir, "RAV.xav"))
        if dfile != None:
            slen = len(dfile)
            if dfile[slen-3:slen] == ".ko":
                ddir = os.path.dirname(filepath)
                basename = os.path.basename(filepath)
                eles = basename.split(".")
                parents = os.listdir(ddir)
                for parent in parents:
                    if eles[0] != parent[0:len(eles[0])]:
                        continue;
                    child = os.path.join(ddir, parent)
                    if os.path.isfile(child) and child.find(".ko") >= 0:
                        self.cfs.copyFile(child, os.path.join(os.path.dirname(dfile), parent))
            else:
                self.cfs.copyFile(filepath, dfile)
            return
        dfile = self.cfs.checkXfwOutCopyFile(filepath, os.path.join(self.packDir, "RAV.xfw"))
        if dfile != None:
            slen = len(dfile)
            if dfile[slen-3:slen] == ".ko":
                ddir = os.path.dirname(filepath)
                basename = os.path.basename(filepath)
                eles = basename.split(".")
                parents = os.listdir(ddir)
                for parent in parents:
                    if eles[0] != parent[0:len(eles[0])]:
                        continue;
                    child = os.path.join(ddir, parent)
                    if os.path.isfile(child) and child.find(".ko") >= 0:
                        self.cfs.copyFile(child, os.path.join(os.path.dirname(dfile), parent))
            else:
                self.cfs.copyFile(filepath, dfile)
            return
        dfile = self.cfs.checkToolsOutCopyFile(filepath, os.path.join(self.packDir, "RAV.tools"))
        if dfile != None:
            self.cfs.copyFile(filepath, dfile)
            return
        print "Error, copyPackageFile: from json not find file " + filepath

    # for test 
    def cyclePath(self, srcPath, outPath):
        parents = os.listdir(srcPath)
        for parent in parents:
            child = os.path.join(srcPath, parent)
            if os.path.isdir(child):
                cyclePath(child, outPath)
            else:
                self.copyPackageFile(child)
                
    def preparePackFiles(self, arch):
        self.copyPackageFile(self.virusdb)
        for comp in self.kernel:
            comp.setMountPath(self.mountPath)
            if comp.ko:
                for kofile in comp.ko:
                    self.copyPackageFile(os.path.join(comp.getMountFilePath(arch), kofile))

        for comp in self.common:
            comp.setMountPath(self.mountPath)
            if comp.bin:
                for binfile in comp.bin:
                    self.copyPackageFile(os.path.join(comp.getMountFilePath(arch), binfile.bin))
            if comp.lib:
                for libfile in comp.lib:
                    print "xxxx2 " + libfile.lib
            if comp.file:
                assert len(comp.file) == 2
                self.copyPackageFile(os.path.join(comp.getMountFilePath(None), comp.file[1]))

        for comp in self.components:
            comp.setMountPath(self.mountPath)
            if comp.bin:
                for binfile in comp.bin:
                    self.copyPackageFile(os.path.join(comp.getMountFilePath(arch), binfile))
            if comp.lib:
                for libfile in comp.lib:
                    self.copyPackageFile(os.path.join(comp.getMountFilePath(arch), libfile))
            if comp.file:
                assert len(comp.file) == 2
                print "xxx6 " + os.path.join(comp.getMountFilePath(arch), comp.file)

    def getBigFileMD5(self,filepath):
        if os.path.exists(filepath):
            md5obj = hashlib.md5()
            maxbuf = 8192
            f = open(filepath,'rb')
            while True:
                buf = f.read(maxbuf)
                if not buf:
                    break
                md5obj.update(buf)
            f.close()
            hash = md5obj.hexdigest()
            return str(hash).upper()
        return None

    def pack(self, version, arch=None):
        assert self.outDir
        assert self.packDir
        assert self.packTools
        assert self.mountPath

        self.refreshPackDir()
        self.preparePackFiles(arch)
        state, commpck = self.packCommon()
        if state:
            print "pack:created package file " + commpck 
        else:
            print "pack:Create common file fail"
            return
        #state, confpck = self.packConfig()
        #if state:
        #    print "pack:created package file " + confpck 
        #else:
        #    print "pack:Create config file fail"
        #    return
        state, epfile = self.packEP(arch)
        if state:
            print "pack:Created ep file " + epfile
            self.ep_md5 = self.getBigFileMD5(epfile)
            self.ep_size = os.path.getsize(epfile)
        else:
            print "pack:Create ep file fail"
            return

        state, xavfile = self.packXAV(arch)
        if state:
            print "pack:Created xav file " + xavfile
            self.xav_md5 = self.getBigFileMD5(xavfile)
            self.xav_size = os.path.getsize(xavfile)
        else:
            print "pack:Create xav file fail"
            return

        state, xfwfile = self.packXFW(arch)
        if state:
            print "pack:Created xfw file " + xfwfile
            self.xfw_md5 = self.getBigFileMD5(xfwfile)
            self.xfw_size = os.path.getsize(xfwfile)
        else:
            print "pack:Create xfw file fail"
            return

        state, toolsfile = self.packTool(arch)
        if state:
            print "pack:Created tools file " + toolsfile
            self.tools_md5 = self.getBigFileMD5(toolsfile)
            self.tools_size = os.path.getsize(epfile)
        else:
            print "pack:Create ep file fail"
            return


        setupfile = os.path.join(self.packDir, "ravsetup.bin")
        self.cfs.copyFile(setupfile, os.path.join(self.outDir, "setup", arch, "ravsetup.bin"))
        
    def packCommon(self):
        rpkfile = os.path.join(self.outDir, "common", "common.rpk")
        vlibfile = os.path.join(self.packDir, "RAV.common/RJJHGJCZ/malware.rmd")
        self._packFunc(os.path.join(self.packDir, "RAV.common"), rpkfile)
        self.vlib_md5 = self.getBigFileMD5(vlibfile)
        self.vlib_size = os.path.getsize(vlibfile)
        return os.path.exists(rpkfile), rpkfile

    def packConfig(self):
        rpkfile = os.path.join(self.outDir, "common", "config.rpk")
        self._packFunc(os.path.join(self.packDir, "RAV.config"), rpkfile)
        return os.path.exists(rpkfile), rpkfile

    def packEP(self, arch):
        rpkfile = os.path.join(self.outDir, "setup", arch, "ep.rpk")
        self._packFunc(os.path.join(self.packDir, "RAV.ep"), rpkfile)
        return os.path.exists(rpkfile), rpkfile

    def packXAV(self, arch):
        rpkfile = os.path.join(self.outDir, "setup", arch, "xav.rpk")
        self._packFunc(os.path.join(self.packDir, "RAV.xav"), rpkfile)
        return os.path.exists(rpkfile), rpkfile

    def packXFW(self, arch):
        rpkfile = os.path.join(self.outDir, "setup", arch, "xfw.rpk")
        self._packFunc(os.path.join(self.packDir, "RAV.xfw"), rpkfile)
        return os.path.exists(rpkfile), rpkfile

    def packTool(self, arch):
        rpkfile = os.path.join(self.outDir, "setup", arch, "tools.rpk")
        self._packFunc(os.path.join(self.packDir, "RAV.tools"), rpkfile)
        return os.path.exists(rpkfile), rpkfile


    def _packFunc(self, path, outfile):
        import subprocess
        outPath = os.path.dirname(outfile)
        if not os.path.exists(outPath):
           os.makedirs(outPath) 
        command = "%s %s %s" % (self.packTools, path, outfile)
        print "_packFunc: before PackTools command " + path + " ==> " + outfile
        p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        for line in p.stdout.readlines():
            print line

    def _getVirusDbVersion(self):
        basename = os.path.basename(self.virusdb)
        eles = basename.split("_")
        return eles[1]

    def generateProductConfigFile(self, version):
        timestr = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
        f = open(os.path.join(self.outDir, "product.xml"), "w")
        f.write("<rpklist version=\"%s\" virver=\"%s\" updatetime=\"%s\">\n" % (version, self._getVirusDbVersion(), timestr))
        f.write("<item name=\"ep\" version=\"%s\" md5=\"%s\" size=\"%d\" updatetime=\"%s\" server=\"0\" client=\"1\" mustinstall=\"1\"/>\n" % \
                    (version, self.ep_md5, self.ep_size, timestr))
        f.write("<item name=\"xav\" version=\"%s\" md5=\"%s\" size=\"%d\" updatetime=\"%s\" server=\"0\" client=\"0\" mustinstall=\"0\"/>\n" % \
                    (version, self.xav_md5, self.xav_size, timestr))
        f.write("<item name=\"xfw\" version=\"%s\" md5=\"%s\" size=\"%d\" updatetime=\"%s\" server=\"0\" client=\"0\" mustinstall=\"0\"/>\n" % \
                    (version, self.xfw_md5, self.xfw_size, timestr))
        f.write("<item name=\"tools\" version=\"%s\" md5=\"%s\" size=\"%d\" updatetime=\"%s\" server=\"0\" client=\"0\" mustinstall=\"0\"/>\n" % \
                    (version, self.tools_md5, self.tools_size, timestr))
        f.write("</rpklist>\n")
        f.close()

    def tarFile(self, path, outfile):
        print "tarFile:" + path + " -> " + outfile
        t = tarfile.open(outfile, "w:gz")
        for root, _, files in os.walk(path):
            for file in files:
                fullpath = os.path.join(root, file)
                t.add(fullpath, arcname=os.path.relpath(fullpath, start=path))
        t.close()

def build_package(version, folder, archs, virusdb, packtool):
    major_folder = folder
    pack = PackageLib(config, os.path.join(major_folder, "packet.dir"), os.path.join(major_folder, "smb250"))
    pack.setOutDir(os.path.join(major_folder, "package", version))
    cmfolder = os.path.join(major_folder, "package")

    if os.path.exists(cmfolder):
        shutil.rmtree(cmfolder)
    os.makedirs(cmfolder)

    pack.refreshOutDir()
    pack.setMountPath("//193.168.10.250/资源文件/svn", os.path.join(major_folder, "smb250"))
    pack.packTools = packtool
    pack.setVirusDbPath(virusdb)

    for arch in archs:
        pack.pack(version, arch)

    pack.generateProductConfigFile(version)
    pack.cfs.copyFile(os.path.join(pack.packDir, "setup.sh"), os.path.join(pack.outDir, "setup.sh"))
    print "pack:tarFile " + cmfolder + " -> " + os.path.join(cmfolder, "RAV_%s.tar.gz"%(version))
    pack.tarFile(cmfolder, os.path.join(cmfolder, "RAV_%s.tar.gz"%(version)))
    shutil.move(os.path.join(cmfolder, "RAV_%s.tar.gz"%(version)), os.path.join(major_folder, "RAV_%s.tar.gz"%(version)))
    print "move result file to " + os.path.join(major_folder, "RAV_%s.tar.gz"%(version))
    shutil.rmtree(pack.outDir)
    shutil.rmtree(cmfolder)
    shutil.rmtree(pack.packDir)

if __name__ == "__main__":
    version = "3.0.0.2"
    folder = "/home/rising/UnixRavWorkDir"
    archs = ["x86_64"]
    virusdb = os.path.join(folder, "virusdb3")
    packtool = os.path.join(folder, "build/3.0.0.1/tools/PackTools")

    build_package(version, folder, archs, virusdb, packtool)



#if __name__ == "__main__":
#    version = "3.0.0.2"
#    major_folder = "/home/rising/UnixRavWorkDir"
#    pack = PackageLib(config, os.path.join(major_folder, "packet.dir"), os.path.join(major_folder, "smb250"))
#    pack.setOutDir(os.path.join(major_folder, "package", version))
#    cmfolder = os.path.join(major_folder, "package")
#
#    pack.refreshOutDir()
#    pack.setMountPath("//193.168.10.250/资源文件/svn", os.path.join(major_folder, "smb250"))
#    pack.packTools = (os.path.join(major_folder, "build/3.0.0.1/tools/PackTools"))
#    pack.setVirusDbPath(os.path.join(major_folder, "virusdb3"))
#
#    archs = ["x86_64"]
#    for arch in archs:
#        pack.pack(version, arch)
#
#    pack.generateProductConfigFile(version)
#    pack.cfs.copyFile(os.path.join(pack.packDir, "setup.sh"), os.path.join(pack.outDir, "setup.sh"))
#    print "pack:tarFile " + cmfolder + " -> " + os.path.join(cmfolder, "RAV_%s.tar.gz"%(version))
#    pack.tarFile(cmfolder, os.path.join(cmfolder, "RAV_%s.tar.gz"%(version)))
#    shutil.move(os.path.join(cmfolder, "RAV_%s.tar.gz"%(version)), os.path.join(major_folder, "RAV_%s.tar.gz"%(version)))
#    print "move result file to " + os.path.join(major_folder, "RAV_%s.tar.gz"%(version))
#    shutil.rmtree(pack.outDir)
#    shutil.rmtree(cmfolder)
#    shutil.rmtree(pack.packDir)




