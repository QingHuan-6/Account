#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include<time.h>
#include "billing_file.h"
#include<stdlib.h>//malloc
#include "global.h"
#include<string.h>//strcpy
#include"card_file.h"
#include"billing_service.h"
Card aCard[50];//ȫ�ֿ���Ϣ�ṹ������
int nCount = 0;//��ʵ�ʸ���

lpCardNode CardList= NULL;


int getCard();


int initCardList()//��ʼ������
{
	lpCardNode head = NULL;//�ڳ�ʼ�������ڣ����ⴴ��һ��head�����ո���CardList��ȫ�֣�
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

Card* queryCard(const char* pName)//���忨�Ų�ѯ
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

//������ģ����ѯ
//���ܣ�ʵ��ģ����ѯ�����ݴ�ȡ
//����������������
//����ֵ���ṹ�������׵�ַ
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
	if (FALSE == ReadCard(pCard, CardPath))//��������뵽��pCard
	{
		free(pCard);//��ֹ�ڴ�й©
		pCard = NULL;
		return FALSE;
	}
	if (CardList != NULL)
	{
		node = CardList->next;
		while (node != NULL)
		{
			if (strstr(node->data.aName, pName) != NULL)//strstrʵ���Ͼ�����cde ������acdef����
			{
				pCard[*pIndex] = node->data;//Ҳ�൱��pCardָ�벻������
				(*pIndex)++;
				pCard = (Card*)realloc(pCard, (((*pIndex)+1)*sizeof(Card)));//��������Ϊpindex+1
			}
			node = node->next;
		}
	}
	return pCard;
}

int getCard()//���ļ��еĿ���Ϣ������������..�ȶ��뵽���飬���������
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
	//��ȡ����Ϣ������
	nCount = getCardCount(CardPath);
	//��ȡ����Ϣ
	pCard = (Card*)(malloc)(sizeof(Card) * nCount);
	if (pCard == NULL)//��û�п��ٳɹ�
	{
		return FALSE;
	}
	if (FALSE == ReadCard(pCard, CardPath))//��������뵽��pCard
	{
		free(pCard);//��ֹ�ڴ�й©
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
		node = cur;//β�壬node
	}
	free(pCard);
	pCard = NULL;
	return TRUE;
}


Card* checkCard(const char* pName, const char* pPwd,int* index)
{
	lpCardNode CardNode = NULL;
	//��ȡ�ļ��еĿ���Ϣ
	if (FALSE == getCard())
	{
		return NULL;
	}
	CardNode = CardList->next;
	//���������ж��Ƿ�����ϻ�
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
	//���ؿ���Ϣ
}



int doStatistics12(int*mt,int* mp, int* mop)
{
	lpCardNode CardNode = NULL;
	//��ȡ�ļ��еĿ���Ϣ
	if (FALSE == getCard())
	{
		return FALSE;
	}
	CardNode = CardList->next;
	//���������ж��Ƿ�����ϻ�
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






