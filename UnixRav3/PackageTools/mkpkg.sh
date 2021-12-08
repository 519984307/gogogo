#!/bin/sh

VERSION=$1
PACKDIR=$2

if [ -z $VERSION ]
then
    echo "please confirm the version, eg. 26.03.00"
    echo 'FORMAT: ./mkpkg.sh version pkgdir'
    exit
fi

if [ -z $PACKDIR ]
then
    echo 'please input the packet dir'
    echo 'FORMAT: ./mkpkg.sh version pkgdir'
    exit
fi

MakeInstallDir()
{
    # make the package dir
    # the dictionary is just like the install dir
    mkdir $PACKDIR/$VERSION
    DIRS="bin lib GGFW plugins LOGO"
    for DIR in $DIRS
    do
        if [ ! -d $PACKDIR/$VERSION/$DIR ]; then
            mkdir $PACKDIR/$VERSION/$DIR
        fi
    done
    mkdir $PACKDIR/$VERSION/GGFW/virusdb
    return 0
}

ClearEnv()
{
    if [ -d $PACKDIR/$VERSION ]; then
        rm -rf $PACKDIR/$VERSION
    fi
}

CopyCompileFile()
{
    TARGET="bin lib plugin"
    for DIR in $TARGET
    do
        if [ -d $PACKDIR/$DIR ]; then
            cp $PACKDIR/$DIR/* $PACKDIR/$VERSION/$DIR
        fi
    done
}

CopyVirusDb()
{
    if [ ! -d $PACKDIR/$VERSION/GGFW/virusdb ];then
        echo "the ",$PACKDIR," must have virusdb dir"
        exit
    fi

    cp $PACKDIR/virusdb/malware.rmd $PACKDIR/$VERSION/GGFW/virusdb/malware.rmd
}

CopyUnixEngine2013()
{
    if [ ! -d $PACKDIR/unix_engine2013 ];then
        echo "the ",$PACKDIR," must have unix_engine2013 dir"
        exit
    fi
    cp $PACKDIR/unix_engine2013/rame.bin/*.so $PACKDIR/$VERSION/lib
}

ClearEnv              # clear the exist foler
MakeInstallDir        # create the install dir
CopyCompileFile       # copy the compile file to dest
CopyVirusDb           # copy the virus db file to dest
CopyUnixEngine2013    # copy the unix scan engine to dest


