#pragma once
#include"model.h"
int addCard(Card card);
void addCardInfo();
int doLogon(const char* pName, const char* pPwd);
int doSettle(const char* pName, const char* pPwd);
int doRecharge(const char* pName, const char* pPwd, float money, float* fbalance);
int doRefund(const char* pName, const char* pPwd, float refundMoney);
void refundMoney(Card*);
void releaseList();
void ShowInfo(Card* pCard);
void present();