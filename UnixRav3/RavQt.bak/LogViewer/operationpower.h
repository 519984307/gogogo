#ifndef OPERATIONPOWER_H
#define OPERATIONPOWER_H

#include <sys/types.h>
#include <stdint.h>

#include <string>

using namespace std;

class OperationPower{
public:
    typedef enum {
        OP_POWER_ALLDISKSCAN = 0,
        OP_POWER_CUSTOMSCAN,
        OP_POWER_TIMERSCAN,
        OP_POWER_RIGHTKEYSCAN,
        OP_POWER_VIEWVIRUSINFO,
        OP_POWER_HANDLEVIRUSFILE,
        OP_POWER_VIEWLOG,
        OP_POWER_SETFILESCANTYPE,
        OP_POWER_SETVIRUSHANDLETYPE,
        OP_POWER_SETCOMPRESSSCAN
    }enumOpPowerType;

public:
    explicit OperationPower(const uint32_t power = 0);
    ~OperationPower();

    void SetValue(const uint32_t &power);
    bool GetPower(const enumOpPowerType &power_type) const;//return true or false



private:
    uint32_t  m_nPower;

};


//defined power value:
//ROOT      0x003F
//SECADM,   0x03FF
//AUDITADM, 0x003B
//USER      0x003F

class RavQtUser{
public:
    explicit RavQtUser(const uint32_t power = 0);
    ~RavQtUser();

    void SetPower(const uint32_t &power);
    bool HavePower(OperationPower::enumOpPowerType op_type) const;


public:
    OperationPower m_Power;
};


#endif // OPERATIONPOWER_H
