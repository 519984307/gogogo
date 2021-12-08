#!/bin/sh
if [ $# -ne 1 ];then
echo "usage: argument:[CMD] [.pro.user file name]"
exit 1
fi

PRO_USER_FILE=$1
ACTIVE_BUILD_ID=0
BUILD_PATH=""
MAKE_PATH=`dirname $PRO_USER_FILE`
PRO_FILE=`echo $PRO_USER_FILE|awk -F'.user' '{print $1}'`
ORIGINAL_PATH=`pwd`

echo "Create makefile..."
ACTIVE_BUILD_ID=`grep "ProjectExplorer.Target.ActiveBuildConfiguration" ${PRO_USER_FILE}|awk -F'>' '{print $2}'|awk -F'<' '{print $1}'`

case $ACTIVE_BUILD_ID in
0)
    BUILD_PATH=`cat ${PRO_USER_FILE}|awk '/ProjectExplorer.Target.BuildConfiguration.0/{F=1}{if(F)print}/ProjectExplorer.BuildConfiguration.BuildStepList.0/{F=0}'|sed -n /ProjectExplorer.BuildConfiguration.BuildDirectory/p|awk -F'>' '{print $2}'|awk -F'<' '{print $1}'`
    ;;
1)
    BUILD_PATH=`cat ${PRO_USER_FILE}|awk '/ProjectExplorer.Target.BuildConfiguration.1/{F=1}{if(F)print}/ProjectExplorer.BuildConfiguration.BuildStepList.0/{F=0}'|sed -n /ProjectExplorer.BuildConfiguration.BuildDirectory/p|awk -F'>' '{print $2}'|awk -F'<' '{print $1}'`
    ;;
2)
    BUILD_PATH=`cat ${PRO_USER_FILE}|awk '/ProjectExplorer.Target.BuildConfiguration.2/{F=1}{if(F)print}/ProjectExplorer.BuildConfiguration.BuildStepList.0/{F=0}'|sed -n /ProjectExplorer.BuildConfiguration.BuildDirectory/p|awk -F'>' '{print $2}'|awk -F'<' '{print $1}'`
    ;;
*)
    ehco "Some error"
    exit
    ;;
esac

cat ${PRO_FILE} >${PRO_FILE}.qmake
echo "DESTDIR=${BUILD_PATH}">>${PRO_FILE}.qmake
echo "MOC_DIR=${BUILD_PATH}">>${PRO_FILE}.qmake
echo "OBJECTS_DIR=${BUILD_PATH}">>${PRO_FILE}.qmake

qmake -o $MAKE_PATH/Makefile ${PRO_FILE}.qmake
cd $MAKE_PATH
make 
cd $ORIGINAL_PATH

echo "Done."
