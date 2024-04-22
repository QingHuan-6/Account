#pragma once
#include"model.h"
int addBilling(const char* pName, Billing* billing);
int initBillingList();
void releaseBillingList();
Billing* checkBilling(const char* pName, int* index);
int doStatistics3(float* ta);