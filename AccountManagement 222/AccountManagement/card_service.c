#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include<time.h>
#include "billing_file.h"
#include<stdlib.h>//malloc
#include "global.h"
#include<string.h>//strcpy
#include"card_file.h"
#include"billing_service.h"
Card aCard[50];//全局卡信息结构体数组
int nCount = 0;//卡实际个数

lpCardNode CardList= NULL;


int getCard();


int initCardList()//初始化链表
{
	lpCardNode head = NULL;//在初始化函数内，另外创建一个head，最终赋给CardList（全局）
	head = (CardNode*)malloc(sizeof(CardNode));
	if (head != NULL)
	{
		head->next = NULL;
		CardList= head;
		return TRUE;
	}
	return FALSE;
}


void releaseCardList()
{
	lpCardNode next = NULL;
	lpCardNode DeleteNode =CardList;
		while(DeleteNode!=NULL)
			{
				next = DeleteNode->next;
				free(DeleteNode);
				DeleteNode = next;
			}
	CardList = NULL;
}

Card* queryCard(const char* pName)//具体卡号查询
{
	lpCardNode cur = NULL;
	if (CardList != NULL)
	{
		cur = CardList->next;
		while (cur != NULL)
		{
			if (strcmp(cur->data.aName, pName) == 0)
			{
				return &cur->data;
			}
			cur = cur->next;
		}
		return NULL;
	}
}

//函数：模糊查询
//功能：实现模糊查询的数据存取
//参数：姓名，密码
//返回值：结构体数组首地址
Card* queryCards(const char* pName, int* pIndex)
{	
	int nCount = 0;
	Card* pCard = NULL;
	nCount = getCardCount(CardPath);
	pCard = (Card*)(malloc)(sizeof(Card) * nCount);
	lpCardNode node = NULL;
	
	if (FALSE == getCard())
	{
		return FALSE;
	}
	if (FALSE == ReadCard(pCard, CardPath))//在这里读入到了pCard
	{
		free(pCard);//防止内存泄漏
		pCard = NULL;
		return FALSE;
	}
	if (CardList != NULL)
	{
		node = CardList->next;
		while (node != NULL)
		{
			if (strstr(node->data.aName, pName) != NULL)//strstr实际上就是找cde 包含在acdef里面
			{
				pCard[*pIndex] = node->data;//也相当于pCard指针不断右移
				(*pIndex)++;
				pCard = (Card*)realloc(pCard, (((*pIndex)+1)*sizeof(Card)));//扩容数组为pindex+1
			}
			node = node->next;
		}
	}
	return pCard;
}

int getCard()//将文件中的卡信息保存在链表中..先读入到数组，再往链表存
{
	int nCount = 0,i=0;
	lpCardNode node = NULL;
	lpCardNode cur = NULL;
	Card* pCard = NULL;
	if (CardList != NULL)
	{
		releaseCardList();
	}
	initCardList();
	node = CardList;
	//获取卡信息的数量
	nCount = getCardCount(CardPath);
	//获取卡信息
	pCard = (Card*)(malloc)(sizeof(Card) * nCount);
	if (pCard == NULL)//若没有开辟成功
	{
		return FALSE;
	}
	if (FALSE == ReadCard(pCard, CardPath))//在这里读入到了pCard
	{
		free(pCard);//防止内存泄漏
		pCard = NULL;
		return FALSE;
	}
	for (i = 0; i < nCount; i++)
	{
		cur = (CardNode*)malloc(sizeof(CardNode));
		if (cur == NULL)
		{
			free(pCard);
			return FALSE;
		}
		memset(cur, 0, sizeof(CardNode));
		cur->data = pCard[i];
		cur->next = NULL;
		node->next = cur;
		node = cur;//尾插，node
	}
	free(pCard);
	pCard = NULL;
	return TRUE;
}


Card* checkCard(const char* pName, const char* pPwd,int* index)
{
	lpCardNode CardNode = NULL;
	//获取文件中的卡信息
	if (FALSE == getCard())
	{
		return NULL;
	}
	CardNode = CardList->next;
	//遍历链表判断是否进行上机
	while (CardNode != NULL)
	{
		if (strcmp(CardNode->data.aName, pName)==0&&strcmp(CardNode->data.aPwd,pPwd)==0)
		{
			return &CardNode->data;
		}
		CardNode = CardNode->next;
		(*index)++;
	}
	return NULL;
	//返回卡信息
}



int doStatistics12(int*mt,int* mp, int* mop)
{
	lpCardNode CardNode = NULL;
	//获取文件中的卡信息
	if (FALSE == getCard())
	{
		return FALSE;
	}
	CardNode = CardList->next;
	//遍历链表判断是否进行上机
	while (CardNode != NULL)
	{
		if (CardNode->data.nDel!=1&&CardNode->data.nStatus!=2&& CardNode->data.nStatus!=3)
		{
			(*mp)++;
		}
		if (CardNode->data.nStatus == 1)
		{
			(*mop)++;
		}
		(*mt)++;
		CardNode = CardNode->next;
	}
	return TRUE;
}






