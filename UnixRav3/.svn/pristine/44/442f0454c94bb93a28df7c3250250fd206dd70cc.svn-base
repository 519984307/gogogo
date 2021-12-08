功能说明:
UnixRav2017 是对应于新版本的杀毒软件
主要进程包括以下:
RAVRJJH0  托盘, 用于后台进程的消息中转 (RavDaemon/RJJH)
RAVRJJH2  QT主界面 (RavQt/MainApp)
RAVRJJH4  QT设置界面  (RavQt/Config)
RAVRJJH6  QT日志界面  (RavQt/LogViewer)
RAVRJJH10 ncurses操作界面 (RavConsole/MainApp)
RAVGJCZ0  后台进程, 用于扫描文件  (RavDaemon/GJCZ)
RAVGLTX0  后台进程,用于访问网络   (RavDaemon/GLTX)
RAVZDFY0  后台进程,用于和内核通信 (RavDaemon/ZDFY)
libravcommon.so    常用功能API的封装 (CommonAPI)
libravcommunicator.so 和中心通信的功能封装 (Communicator)
libravdefenbinder.so  文件监控内核与ZDFY通讯的封装 (DefenMod)
libravfilterbinder.so 网络监控内核与ZDFY通讯的封装 (Netfilter)
libravqtsupport.so  RJJH0加载托盘的动态库封装 (RavQt/Notifier)
libravscanengine.so  杀毒引擎的封装 (ScanEngine)
libravstorage.so  数据存储的封装 (Storage)
libravupgrade.so  升级模块的封装 (Upgrade)

功能结构图:
///////////////////////////////////////////////////////////////////////////////////
ScanEngine

MultiVirusEngine (扫描引擎包括以下几部分)
├── CountFileVari   *mScanFileThread;  (遍历文件的功能)
├── CountFileVari   *mCountFileThread; (统计文件个数的功能,用于快速统计个数,以便画进度)
├── CloudEngine     *mCloudEngine;     (云引擎查杀,用于生成相应的json串)
└── vector<ScanThreadVari*> mScanThreadVari;  (多个查杀功能模块, 应对于多线程查杀)
    ├── ScanConfig      *mScanConfig;  (扫描的参数设置)
    └── ScanEngine      *mScanEngine;  (封装引擎的模块,用于对V13引擎的封装)


///////////////////////////////////////////////////////////////////////////////////
RavDaemon:
后台功能设计如下
其中 GJCZ, GLTX, ZDFY三个进程为ROOT进程, 全部从 StorageBridge,ServiceBinder继承下来
StorageBridge   用于操作各自相应的数据库
ServiceBinder	用于和RJJH/MainEngine通信
    ├── GJCZ/MainEngine
        └── Controller   用于处理引擎扫描上报的各种消息
    ├── GLTX/MainEngine
        ├── Communicator   用于和中心服务器通讯
        └── ScheduleEngine 用于定时的调度
    └── ZDFY/MainEngine
        ├── libravdefenbinder.so   动态加载此库,用于和文件监控内核通讯
        └── libravfilterbinder.so  动态加载此库,用于和网络监控内核通讯

StorageBridge
    ├── RJJH/MainEngine	用户进程, 用于和后台ROOT进程通讯,并转发消息,
        └── libravqtsupport.so 动态加载此库,用于显示QT托盘, 并弹一些信息提示框


//////////////////////////////////////////////////////////////////////////////////




