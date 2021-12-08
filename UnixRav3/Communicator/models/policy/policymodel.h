#ifndef POLICYMODEL_H
#define POLICYMODEL_H

#include <string>

#include "json/json.h"
#include "Entry/baseentry.h"

class PolicyModel : public Serializable
{
public:
    static PolicyModel* NEW(Json::Value value, bool crypted=true);

public:
    PolicyModel();
    virtual ~PolicyModel() {}

    int Construct(Json::Value value, bool crypted);

    std::string JSONString();
    
    std::string GetPolicyContent();

public:
    bool validate();

private:
    bool mContentCrypted;

public:
    /**
     * 策略guid研发定义
     * 现在已知基础平台策略EB8AFFA5-0710-47E6-8F53-55CAE55E1915，
     * 防病毒策略D49170C0-B076-4795-B079-0F97560485AF
     * 部署组件策略50BAC747-7D02-4969-AF79-45EE47365C81
     * 其他增加其他子产品需研发提供
     */
    std::string   mPolicyProductID;
    /** 产品分配类型0企业策略，1组策略，2个人策略 */
    int      mGroupType;
    /** 策略下的小类型，目前只有1，后续开发研发补充其他具体值 */
    int      mPolicyType;
    /** 产品策略版本号，时间戳 */
    std::string  mPolicyVer;
    /** 具体策略的xml，经过base64编码 */
    std::string   mPolicyMainContent;

private:
    void InitializeJson(Json::Value& json);
    void ExternlizeJson(Json::Value& json);
};

#endif // POLICYMODEL_H
