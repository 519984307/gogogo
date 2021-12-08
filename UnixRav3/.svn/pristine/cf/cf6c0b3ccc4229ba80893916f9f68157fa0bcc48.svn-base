#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <string>

#include "models/modeldefine.h"
#include "json/json.h"

class CommandModel
{
public:
    static CommandModel *NEW(Json::Value json, bool decrypted=true);

    enum RemoteCommand {
        QUICK_SCAN_START = 0x3000,
        QUICK_SCAN_STOP  = 0x3001,
        FILE_DEFEN_OPEN  = 0x3100,
        FILE_DEFEN_STOP  = 0x3101,
        MAIL_DEFEN_OPEN  = 0x3200,
        MAIL_DEFEN_STOP  = 0x3201,
        ALL_SCAN_START   = 0x3302,
        ALL_SCAN_STOP    = 0x3303,
        UPDATE_START     = 0x2000,
        SHOW_MESSAGE     = 0x1000,
        REMOTE_EXE       = 0x1100,
    };

    void operator =(const CommandModel& model);
public:
    CommandModel();
    int Contruct(Json::Value json, bool decrypted);

    bool validate();
    std::string JSONString();

    std::string GetMessageContent();
public:
    /**
     * 描述命令所属的产品,固定不变，现在已提供
     * 基础平台命令EB8AFFA5-0710-47e6-8F53-55CAE55E1915，
     * 部署组件命令50BAC747-7D02-4969-AF79-45EE47365C81，
     * 杀毒子产品命令D49170C0-B076-4795-B079-0F97560485AF，
     * 如开发其他命令需研发提供具体对应关系。
     */
    std::string   mProductId;
    /** 命令GUID，服务器端动态生成 */
    std::string   mId;
    /**
     * 命令类型，需研发提供具体对应内容
     */
    int mType;

    CommandType mCmdType;
    /** 基础平台命令各个子产品对应命令流水号 */
    std::string   mCmdVer;
    /** 研发提供研发为排除重复命令用 */
    int      mCmdId;
    /** 具体策略xml，经过base64编码 */
    std::string   mCmdContentBuf;

private:
    bool mContentCrypted;
};

#endif // COMMANDMODEL_H
