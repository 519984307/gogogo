#!/bin/sh

# global define
SVN_PATH="http://193.168.10.222/svn/esm/unixrav2017"
SVN_BIN=`which svn`

# sync file from svn
# @params : file svn path
# @return : 1 error, 0 success
func_sync_svn()
{
    svn_bin="${SVN_BIN=empyt}"
    if [ "$svn_bin"x = "empty"x ]; then
        return 1
    fi

    command="${svn_bin} export $1 $2 --username=pangxiaoliang --password=pangxl"
    echo $(command)


    return 0
}

# sync the file from svn
# @params: 1 the platform arch
# @prrams: 2 the version
# @return: 1 error, 0 success
func_sync_file_from_svn_by_arch()
{
product_so=(basecontroller/$1/libravbase.so
             commonapi/$1/libravcommon.so
             communicator/$1/libravcommunicate.so
             scanengine/$1/libravscanengine.so
             storage/$1/libravstorage.so
             upgrade/$1/libravupdate.so)

product_bin=(ravconsole_mainapp/$1/RAVRJJH0
            ravconsole_setup/$1/ravcmdsetup.bin
            ravconsole_uninstall/$1/ravcmduninstall.bin
            ravdaemon/$1/RAVGGFW0
            ravqt_config/$1/RAVRJJH4
            ravqt_logviewer/$1/RAVRJJH6
            ravqt_mainapp/$1/RAVRJJH2
            ravqt_notifier/$1/RAVGJCZ0
            ravqt_setup/$1/ravqtsetup.bin
            ravqt_uninstall/$1/ravqtuninstall.bin
            ravqt_update/$1/RAVRJJH8
            ravwatcher/$1/RAVGJCZ)

engine2013=(unix_engine2013/$1/methodex.so
            unix_engine2013/$1/rxoffice.so
            unix_engine2013/$1/scanmac.so
            unix_engine2013/$1/librxcoml.so
            unix_engine2013/$1/revm.so
            unix_engine2013/$1/rxvision.so
            unix_engine2013/$1/scanpe.so
            unix_engine2013/$1/libcurl.so
            unix_engine2013/$1/librxffr.so
            unix_engine2013/$1/rxarch.so
            unix_engine2013/$1/rxwinpe.so
            unix_engine2013/$1/scansct.so
            unix_engine2013/$1/librxmss.so
            unix_engine2013/$1/rxauth.so
            unix_engine2013/$1/rxxdu0.so
            unix_engine2013/$1/scriptci.so
            unix_engine2013/$1/librxruntim.so
            unix_engine2013/$1/rxcore.so
            unix_engine2013/$1/rxxman.so
            unix_engine2013/$1/uroutine.so
            unix_engine2013/$1/librxsig64.so
            unix_engine2013/$1/rxfsm.so
            unix_engine2013/$1/scanelf.so
            unix_engine2013/$1/urutils.so
            unix_engine2013/$1/rxmail.so
            unix_engine2013/$1/scanexec.so
            unix_engine2013/$1/vex86.so
            unix_engine2013/$1/rxmltk.so
            unix_engine2013/$1/scanex.so)

third_part=(third_part/$1/libravcrypto.so
            third_part/$1/libravz.so
            third_path/$1/libravssl.so
            third_part/$1/libravcurl.so
            third_part/$1/libravxml2.so
            third_part/$1/libravncurses.so
            third_part/$1/libraviconv.so)

    for product in ${product_so[*]}; do
        product_path=$SVN_PATH/build/$product
        func_sync_svn $product_path
    done

    for product in ${product_bin[*]}; do
        product_path=$SVN_PATH/build/$product
        func_sync_svn $product_path
    done

    for engine in ${engine2013[*]}; do
        product_path=$SVN_PATH/$engine
        func_sync_svn $product_path
    done
}

func_sync_file_from_svn_by_arch x86_64
