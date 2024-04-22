#pragma once
int SaveCard(const Card* pCard, const char* pPath);
int ReadCard(Card* pCard, const char* pPath);
Card praseCard(char* pBuf);
int getCardCount(const char* pPath);
int updateCard(const Card* pCard, const Card* pPath, int nIndex);