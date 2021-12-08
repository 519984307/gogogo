#include <QCoreApplication>

//#include "../Communicator/communicator.h"
#include "../ScanEngine/scanengine.h"
#include "../ScanEngine/multivirusengine.h"


//class TestCase : public CommunicatorObserver
//{
//public:
//    void onCommunicateCallback(COMM_EVENT event, COMM_TYPE type, void *object) {
//        int a = 0;
//        a++;
//    }

//    int requestClientInfo(ClientInfoType type, string& data) {
//        switch (type) {
//        case CLIENT_IP:
//            data = "193.168.11.50";
//            break;
//        case CLIENT_NAME:
//            data = "hello";
//            break;
//        case CLIENT_ARCH:
//            data = "x86_64";
//            break;
//        case CLIENT_OS:
//            data = "linux";
//            break;
//        case CLIENT_VER:
//            data = "26.02.50";
//            break;
//        default:
//            break;
//        }
//        data = "hello";
//        return 0;
//    }
//};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    TestCase *test = new TestCase();
//    Communicator *communicator = Communicator::NEW("http://193.168.11.77:8088/Api/Api.php",
//                                                   "85BFBA0745835596",
//                                                   "038D0240-045C-0558-6506-230700080009",
//                                                   /*"1BC58A0F91DDAE1FBF26208FE1B4CE32"*/"",
//                                                   "26.02.50",
//                                                   "linux:l25i");
//    communicator->setObserver(test);
//    communicator->test();

    ScanEngine *engine = ScanEngine::NEW("/home/miwoo/fuck/virusdb/malware.rmd");
//    HRESULT err = engine->ScanOneFile("/home/miwoo/Desktop/suSamples/rookit/Rootkit.Win32.Agent.aay");
    HRESULT err = engine->ScanOneFile("/home/miwoo/duyuan.cer");
    delete engine;

//    MultiVirusEngine *engine = MultiVirusEngine::NEW("/home/miwoo/fuck/virusdb/malware.rmd");
//    engine->Start("/home/miwoo/", -1, 2);

//    sleep(20);
//    engine->Pause();

//    sleep(20);
//    engine->Continue();

//    sleep(20);
//    engine->Stop();

    return a.exec();
}
