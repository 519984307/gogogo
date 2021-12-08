//#pragma once

#ifndef __RSAALG_H__
#define __RSAALG_H__

#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GenerateKeys(int bits , std::string& pubkeys , std::string& prikeys);

std::string RSA_Public_Encrypt(std::string& sPubkey , std::string& sMessage);
std::string RSA_Private_Encrypt(std::string& sPriKey , std::string& sMessage);

std::string RSA_Public_Decrypt(std::string& sPubkey , std::string& sMessage);
std::string RSA_Private_Decrypt(std::string& sPubkey , std::string& sMessage);

bool Generate_Machine_GUID(std::string& sGUID);

#ifdef __cplusplus
}
#endif

#endif
