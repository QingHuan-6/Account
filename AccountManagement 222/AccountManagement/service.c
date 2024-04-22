#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include"billing_service.h"
#include "card_service.h"	//卡业务逻辑处理
#include"model.h"
#include "global.h"		
#include "card_file.h"		//卡文件处理
#include"billing_service.h"//计费信息逻辑处理
#include"billing_file.h"	//计费信息文件处理

void ShowInfo(Card* pCard);
double getAmount(time_t t);



int addCard(Card card)
{

	Billing* pBilling = (Billing*)malloc(sizeof(Billing));
	strcpy(pBilling->aCardName, card.aName);
	pBilling->nStatus = 0;
	pBilling->nDel = 0;
	pBilling->fAmount = 0;//消费金额 
	pBilling->tStart = time(NULL);
	pBilling->tEnd = pBilling->tStart + 31536000;
	if (FALSE == saveBilling(pBilling, BillingPath))//开卡同时开通账户
	{
		return FALSE;
	}
	if (FALSE == SaveCard(&card, CardPath))
	{
		return FALSE;
	}
	return TRUE;
}


void addCardInfo()
{

	Card card;
	char aName[32] = { '\0' };
	char aPwd[20] = { '\0' };
	int nNameSize = 0;
	int nPwdSize = 0;
	printf("-------添加卡-------\n");
	printf("请输入卡号：");
	scanf("%s", aName);
	int index = 0;
	Billing* pBilling = checkBilling(aName, &index);
	if (pBilling != NULL)
	{
		printf("该姓名已经存在,请重试\n");
		return;
	}
	printf("请输入密码：");
	scanf("%s", aPwd);
	nNameSize = getSize(aName);
	nPwdSize = getSize(aPwd);

	//判断卡号和密码是否合法
	if (nNameSize > 18 || nPwdSize > 8)
	{
		printf("输入的卡号或密码长度不合法");
		return;
	}
	else
	{
		strcpy(card.aName, aName);
		strcpy(card.aPwd, aPwd);
	}

	printf("请输入开卡金额：");
	scanf("%f", &card.fBalance);
	card.fTotalUse = 0;
	card.nUseCount = 0;
	card.nStatus = 0;
	card.nDel = 0;
	card.tStart = card.tEnd = card.tLastTime = time(NULL);
	addCard(card);
	printf("\n");
	printf("-------添加卡的信息如下-------\n");
	printf("卡号\t密码\t卡状态\t余额\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
}


int doLogon(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int index1 = 0;//卡信息在链表中的索引
	int index2 = 0;//在billing中的索引
	Billing* pBilling=NULL;// 计费信息
	//卡号密码是否正确
	//判断能否进行上机
	pCard=checkCard(pName,pPwd,&index1);//找到卡了就返回来
	if (pCard == NULL)
	{
		return FALSE;
	}
	//只有没有被删除的卡才能上机
	if (pCard->nDel != 0)
	{
		return DEL;
	}
	//只有未在使用的,没有注销的，卡才能上机
	if (pCard->nStatus == 1||pCard->nStatus==2||pCard->nStatus==3)
	{
		return Unuse;
	}
	//只有余额大于0才能上机
	if (pCard->fBalance <= 0)
	{
		return EnoughMoney;
	}
	//如果可以上机，那么更新卡信息
	pCard->nStatus = 1;
	pCard->nUseCount++;
	pCard->tStart = time(NULL);
	ShowInfo(pCard);
	if (FALSE == updateCard(pCard, CardPath, index1))
	{
		return FALSE;
	}
	//添加消费信息
	pBilling = checkBilling(pName, &index2);
	//更新billing链表中的信息
	strcpy(pBilling->aCardName, pName);
	pBilling->nStatus = 0;	//消费状态 改为未结算
	//消费金额
	pBilling->nDel = 0;
	updateBillingfile(pBilling, BillingPath, index2);
	return TRUE;
}

int doSettle(const char*pName,const char* pPwd)
{
	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int index1 = 0;//在card链表中的索引
	int index2 = 0;//在billing链表中的索引
	double dbAmount = 0.0;
	float fBalance = 0.0;
	//获取卡信息，只有正在上机的卡才能进行下机操作
	pCard = checkCard(pName, pPwd, &index1);
	pBilling = checkBilling(pName, &index2);
	//获取计费信息
	//上机已经检索过是否删除或者注销，无需再检索
	if (pCard == NULL)
	{
		return FALSE;
	}
	if (pCard->nStatus != 1)
	{
		return Unuse;
	}
	//计算消费金额
	dbAmount = getAmount(pCard->tStart);
	//判断卡余额与消费金额
	//更新卡余额
	if (dbAmount > pCard->fBalance)
	{
		printf("余额不足，请充值\n");
	}
	pCard->nStatus = 0;
	pCard->fBalance = pCard->fBalance - dbAmount;
	pCard->fTotalUse = pCard->fTotalUse + dbAmount;
	pCard->tLastTime = time(NULL);
	//更新卡余额
	pBilling->fAmount = pBilling->fAmount + dbAmount;
	pBilling->nStatus = 1;
	printf("此次消费%.1f元\n", dbAmount);
	ShowInfo(pCard);
	updateCard(pCard, CardPath, index1);
	updateBillingfile(pBilling, BillingPath, index2);
	return TRUE;
}

double getAmount(time_t t)
{
	int nSec;
	int nMin;
	int nCount;
	double dbAmount;
	time_t tEnd = time(NULL);
	nSec = tEnd - t;
	/*nMin = nSec / 60;*/
	//if (nMin / unit == 0)
	//{
	//	nCount = nMin / unit;//不超出一个15分钟就不收费
	//}
	//else
	//{
	//	nCount = nMin / unit + 1;//超出一个15分钟就+1
	//}
	/*dbAmount = nCount * charge2;*/
	dbAmount = nSec * charge2;
	return dbAmount;
}

int doRecharge(const char* pName, const char* pPwd, float money,float *fbalance)
{
	int index1 = 0;
	Card* pCard = NULL;

	if ((pCard = checkCard(pName, pPwd, &index1)) == NULL)
	{
		return NameOrPwdWrong;
	}
	if (pCard->nDel != 0)
	{
		return -2;
	}
	pCard->fBalance = pCard->fBalance + money;
	*fbalance = pCard->fBalance;
	if (TRUE == updateCard(pCard, CardPath, index1))
	{
	
		return TRUE;
	}
	return FALSE;
}


int doRefund(const char* pName, const char* pPwd, float refundMoney)
{
	int index1 = 0;
	Card* pCard = NULL;
	pCard = checkCard(pName, pPwd, &index1);
	if (pCard != NULL)
	{
		pCard->fBalance = pCard->fBalance - refundMoney;
		updateCard(pCard, CardPath, index1);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void releaseList()
{
	releaseCardList();
	releaseBillingList();//释放计费信息链表内存
}

void ShowInfo(Card*pCard)
{
	printf("当前账号为：%s\n", pCard->aName);
	printf("当前余额为：%.1f\n", pCard->fBalance);
	printf("当前状态为：");
	if (pCard->nStatus == 1)
	{
		printf("上机\n");
	}
	else if (pCard->nStatus == 0)
	{
		printf("下机\n");
	}
}

void present()
{
	char aName[32] = { '\0' };
	char aPwd[20] = { '\0' };
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	int index1 = 0;
	Card* pCard = checkCard(aName, aPwd, &index1);
	float score = pCard->fTotalUse * 2;
	int flag = 0;
	printf("20积分兑换茶杯一个,40积分兑换键盘一个,80积分兑换平板一个\n");
	printf("您当前有积分%.1f\n",score);
	if (score < 10.0)
	{
		printf("您目前的积分没有可以兑换的奖品\n");
		return;
	}
	else
	{
		printf("0.不兑换 1.茶杯 2.键盘 3.平板\n");
		printf("请输入您的选择：");
		scanf("%d", &flag);
		switch (flag)
		{
			case 0:break;
			case 1:
				if (pCard->fTotalUse < 10)
					{
					printf("积分不足");
					return;
					}
					pCard->fTotalUse -= 10.0;
					updateCard(pCard, CardPath, index1);
					printf("感谢兑换！\n"); break;
			case 2:
				if (pCard->fTotalUse < 20)
				{
					printf("积分不足");
					return;
				}pCard->fTotalUse -= 20.0;
					updateCard(pCard, CardPath, index1);
					printf("感谢兑换！\n"); break;
			case 3:
				if (pCard->fTotalUse < 30)
				{
					printf("积分不足");
					return;
				}pCard->fTotalUse -= 40.0;
					updateCard(pCard, CardPath, index1);
					printf("感谢兑换！\n"); break;
			default:printf("输入不合法\n"); break;
		}
	}
	return;
}