#pragma once
#include"model.h"
Card* queryCard(const char*pName);
void releaseCardList();
Card* queryCards(const char* pName, int* pIndex);
Card* checkCard(const char* pName, const char* pPwd, int* index);
int doStatistics12(int*mt,int* mp, int* mop);
