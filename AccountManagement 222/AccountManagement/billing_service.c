#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"tool.h"
#include "model.h"
#include"global.h"
#include"billing_file.h"
#define BillingCharLen 256
lpBillingNode BillingList = NULL;
void releaseBillingList();

int initBillingList()
{
	if (BillingList == NULL)
	{
		BillingNode* head = (BillingNode*)malloc(sizeof(BillingNode));
		if (head != NULL)
		{
			head->next = NULL;
			BillingList = head;
			return TRUE;
		}
	return FALSE;
	}
	
}

int getBillingCount(const char*pPath)
{
	int nCount = 0;
	char buf[BillingCharLen] = { 0 };
	FILE* fp = NULL;
	//打开文件
	if ((fp = fopen(pPath, "r")) == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		memset(buf,0,BillingCharLen);//清空数组 都初始化为0
		if (fgets(buf, BillingCharLen, fp) != NULL)
		{
			if (strlen(buf) > 0)
			{
				nCount++;
			}
		}
	}
	//关闭文件
	fclose(fp);
	return nCount;
}

int getBilling()//将消费信息从文件存入链表当中
{
	int count = 0;
	lpBillingNode node = NULL;
	Billing* pBilling = NULL;
	FILE* fp = NULL;
	if ((fp=fopen(BillingPath, "r")==NULL))
	{
		return FALSE;
	}
	if (BillingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();
	node = BillingList;
	count = getBillingCount(BillingPath);
	pBilling = (Billing*)malloc(sizeof(Billing) * count);
	if (FALSE == readBilling(pBilling, BillingPath))
	{
		return FALSE;
	}
	for (int i = 0; i < count; i++)
	{
		BillingNode* cur = (BillingNode*)malloc(sizeof(BillingNode));
		if (cur == NULL)
		{
			free(pBilling);
			return FALSE;
		}
		cur->data = pBilling[i];
		cur->next = NULL;
		node->next = cur;
		node = node->next;
	}

}

Billing* queryBillings(char* pName,int* position)
{
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));
	nCount = getBillingCount(BillingPath);
	lpBillingNode node = NULL;

	if (FALSE == getBilling())
	{
		return FALSE;
	}
	if (FALSE == readBilling(pBilling, BillingPath))//在这里读入到了pCard
	{
		free(pBilling);//防止内存泄漏
		pBilling = NULL;
		return FALSE;
	}
	if (BillingList != NULL)
	{
		node = BillingList->next;
		while (node != NULL)
		{
			if (strstr(node->data.aCardName, pName) != NULL)//strstr实际上就是找cde 包含在acdef里面
			{
				pBilling[*position] = node->data;//也相当于pCard指针不断右移
				(*position)++;
				pBilling = (Billing*)realloc(pBilling, (((*position) + 1) * sizeof(Billing)));//扩容数组为pindex+1
			}
			node = node->next;
		}
	}
	return pBilling;
}

void releaseBillingList()
{
	BillingNode* cur = BillingList;
	BillingNode* next = NULL;
	while (cur != NULL)
	{
		next = cur->next;//在cur被销毁之前，保存其后继
		free(cur);
		cur = next;
	}
	BillingList = NULL;
}


Billing* checkBilling(const char* pName,int* index)
{
	lpBillingNode node = NULL;
	//获取文件中的卡信息
	if (FALSE == getBilling())
	{
		return NULL;
	}
	node = BillingList->next;
	//遍历链表判断是否进行上机
	while (node != NULL)
	{
		if (strcmp(node->data.aCardName, pName)==0)
		{
			return &node->data;
		}
		node = node->next;
		(*index)++;
	}
	return NULL;
}

int doStatistics3(float* ta)
{
	lpBillingNode node = NULL;
	//获取文件中的卡信息
	if (FALSE == getBilling())
	{
		return FALSE;
	}
	node = BillingList->next;
	//遍历链表判断是否进行上机
	while (node != NULL)
	{
		(*ta) = (*ta) + node->data.fAmount;
		node = node->next;
	}
	return TRUE;
}