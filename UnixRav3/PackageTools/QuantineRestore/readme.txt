此工具是恢复隔离区文件的工具，一次性将隔离区的文件全部恢复

将文件 restore 拷贝至安装目录下 lib 目录， 和librxbase.so在同一目录

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
./restore -d /usr/local/Rising/Rav/virbak(隔离区目录)
