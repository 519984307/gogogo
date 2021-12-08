#include "operationpower.h"


OperationPower::OperationPower(const uint32_t power)
    :m_nPower(power)
{

}

OperationPower::~OperationPower()
{

}

void OperationPower::SetValue(const uint32_t &power)
{
    m_nPower = power;
}

bool OperationPower::GetPower(const enumOpPowerType &power_type) const
{
    return m_nPower>>power_type&0x01;
}

RavQtUser::RavQtUser(const uint32_t power)
{
    SetPower(power);
}

RavQtUser::~RavQtUser()
{

}

void RavQtUser::SetPower(const uint32_t &power)
{
    m_Power.SetValue(power);
}

bool RavQtUser::HavePower(OperationPower::enumOpPowerType op_type) const
{
    return m_Power.GetPower(op_type);
}
