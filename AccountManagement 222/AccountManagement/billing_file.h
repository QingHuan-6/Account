#pragma once
#include "model.h"
int saveBilling(const Billing* pBilling, const char* pPath);
Billing praseBilling(char* pbuf);
int readBilling(Billing* pBilling, const char* pPath);
int updateBillingfile(Billing* pBilling, const char* pPath, int count);