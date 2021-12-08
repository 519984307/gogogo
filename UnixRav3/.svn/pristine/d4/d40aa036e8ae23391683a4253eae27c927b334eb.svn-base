#include "ravconsolesetup.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("no arch\n");
        return -1;
    }

    std::string arch, pack_file, dest_path;
    int opt = 0;
    bool islpserver = false;
    bool file_usb_monitor = false;

//    bool build_deb = false;
//    bool using_x11_uninstall = false;
//    bool build_rpk = false;
//    std::string deb_kversion = "";

    while ((opt = getopt(argc, argv, "a:c:ft:k:dmsx")) != -1) {
        switch(opt) {
        case 'a':
            arch = optarg;
            break;
        case 'm':
            file_usb_monitor = true;
            break;
//        case 'c':
//            build_rpk = true;
//            dest_path = optarg;
//            break;
//        case 't':
//            dest_path = optarg;
//            break;
//        case 'k':
//            deb_kversion = optarg;
//            break;
//        case 'd'://build deb
//            build_deb = true;
//            break;
//        case 'x'://using x11_uninstall,now only valid in build_deb mode(2019-05-17)
//            using_x11_uninstall = true;
//            break;
//        case 's':
//            islpserver = true;
//            break;
        }
    }

    if (arch.empty()) {
		printf("Invalid arch\n");
        return -1;
    }

    RavConsoleSetup setup;
    setup.SetArch(arch);
    setup.SetLPSecServer(islpserver);
    setup.SetFileUsbMonitor(file_usb_monitor);

    //.SetFileMonitor(file_monitor);

//    //build rpk file
//    if (build_rpk && dest_path != "" && pack_file != "") {
//        setup.BuildRpkFile(dest_path.c_str(), pack_file.c_str());
//        return 0;
//    }
//    //unpack file
//    if (pack_file != "" && dest_path != "") {
//        printf("UnpackFile:%s -> %s \n", pack_file.c_str(), dest_path.c_str());
//        int ret = setup.UnpackFileToPath(pack_file, dest_path);
//        printf("setup.UnpackFileToPath returned:%d\n", ret);
//        return 0;
//    }
    //build debian install directory
//    if(build_deb && dest_path != "") {
//        setup.SetUsingX11Uninstall(using_x11_uninstall);
//        setup.SetDebKervelVersion(deb_kversion);
//        setup.BuildDeb(dest_path);
//        return 0;
//    }

    return setup.StartInstall();
}

