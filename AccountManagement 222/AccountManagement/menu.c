#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include"service.h"
#include "model.h"
#include"global.h"
#include <string.h>
#include"card_service.h"
#include "card_file.h"
#include"billing_file.h"
#include"billing_service.h"
#include"tool.h"
void showMenu()
 {
	printf("-----------菜单----------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("9.积分兑换\n");
	printf("0.退出\n");
	printf("请选择菜单项编号（0~8）：");
}


int getSize(const char* pInfo)
{
	int nSize = 0;
	while (*(pInfo + nSize))
	{
		nSize++;
	}
	return nSize;
}


void add()
{
	addCardInfo();
}

void query()
{
	char aName[18] = { 0 };//保存输入的卡号
	Card* pCard = NULL;//保存查询到的信息
	char aTime[20] = { 0 };
	int nIndex = 0;

	printf("---------- 查询卡----------\n");
	printf("请输入查询的卡号(长度为1-18):");
	scanf("%s", aName);
	pCard = queryCards(aName,&nIndex);
	if (pCard == NULL || nIndex == 0)
	{
		printf("没有查询到卡的信息！\n");
	}
	else
	{
		printf("卡号\t状态\t余额\t使用次数\t上次使用时间\n");
		for (int i = 0; i < nIndex; i++)
		{
			TimeToString(pCard[i].tLastTime, aTime);//加【】相当于解引用了
			printf("%s\t%d\t%0.1f\t      %d\t         %s\n",pCard[i].aName,pCard[i].nStatus,pCard[i].fBalance,pCard[i].nUseCount,aTime);
		}
		
	}

	
}

void copy(const char aInput[], char aOutput[], int nSize)
{
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		aOutput[i] = aInput[i];
		if (aInput[i] == 0)
		{
			break;
		}
	}
}

void exitApp()
{
	releaseList();
}


void logon()
{
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;
	//接收用户输入的卡号和密码
	printf("---------- 上机----------\n");
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	//进行上机
	flag = doLogon(aName, aPwd);
	if (flag==TRUE)
	{
		printf("上机成功\n");
	}
	else if(flag==DEL)
	{
		printf("该卡已删除，上机失败\n");
	}
	else if (flag == Unuse)
	{
		printf("该卡正在上机或已被删除或失效\n");
	}
	else if (flag == EnoughMoney)
	{
		printf("余额不足，请充值\n");
	}
	else if(flag==FALSE)
	{
		printf("账号或密码错误\n");
	}
	
}

void settle()
{
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	Card* pCard = NULL;
	//接收用户输入的卡号和密码
	printf("---------- 下机----------\n");
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	//进行上机
	flag = doSettle(aName, aPwd);
	if (flag==TRUE)
	{
		printf("下机成功，感谢使用\n");
	}
	else if(flag==FALSE)
	{
		printf("账号或密码错误\n");
	}
	else if (flag=Unuse)
	{
		printf("该账户未上机\n");
	}
	//下机
}

void recharge()
{
	float fbalance = 0;
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	float money = 0;
	Card* pCard = NULL;
	//接收用户输入的卡号和密码
	printf("---------- 充值----------\n");
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	printf("请输入充值的金额：");
	scanf("%f", &money);
	flag = doRecharge(aName, aPwd, money,&fbalance);
	if (flag==TRUE)
	{
		printf("已经为您充值%.1f,现有余额%.1f\n",money,fbalance);
	}
	else if(flag==NameOrPwdWrong)
	{
		printf("账号或密码错误\n");
	}
	else if (flag = -2)
	{
		printf("该卡已注销，不能充值\n");
	}
	else
	{
		printf("系统发生错误\n");
	}
}

int refund()
{
	float refundMoney = 0.0;
	int flag = 0;
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	int index = 0;
	Card* pCard = NULL;
	printf("---------- 退费----------\n");
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	pCard = checkCard(aName, aPwd, &index);
	if (pCard != NULL)
	{
		printf("您现有余额：%.1f\n",pCard->fBalance);
	}
	printf("请输入要退费的金额：");
	scanf("%f", &refundMoney);
	if (refundMoney > pCard->fBalance)
	{
		printf("退费金额超过余额，请重试\n");
		return FALSE;
	}
	else
	{
		if (TRUE == doRefund(aName, aPwd, refundMoney))
		{
			int index2 = 0;
			printf("已经退费%.1f，当前余额%.1f\n", refundMoney, (pCard=checkCard(aName,aPwd,&index2))->fBalance);
		}
		else
		{
			return FALSE;
		}
	}
}

int logoff()
{
	char aName[20] = { 0 };
	char aPwd[8] = { 0 };
	int index1 = 0,index2=0;
	printf("---------- 注销----------\n");
	printf("请输入您的账号：");
	scanf("%s", aName);
	printf("请输入你的密码：");
	scanf("%s", aPwd);
	Card* pCard = checkCard(aName, aPwd, &index1);
	Billing* pBilling = checkBilling(aName, &index2);
	if (pCard != NULL&&pBilling!=NULL)
	{
		//判断是否能注销
		if (pCard->nStatus != 0)
		{
			printf("该账户正在上机或已经注销，不能注销\n");
			return FALSE;
		}
		pBilling->nDel = 1;
		pCard->nStatus = 2;
		pCard->nDel = 1;
		if (TRUE == updateCard(pCard, CardPath, index1)&&TRUE==updateBillingfile(pBilling,BillingPath,index2))
		{
			printf("注销成功！\n");
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		printf("账号或密码错误\n");
		return FALSE;
	}
}

int statistics()
{
	//统计办卡总人数，统计未注销卡人数，统计在上机人数，统计营收总额（消费金额）
	int mTotal = 0;
	int mPerson = 0;
	int mOnlinePerson = 0;
	float TotalAmount = 0.0;
	if (TRUE == doStatistics12(&mTotal, &mPerson, &mOnlinePerson) && TRUE == doStatistics3(&TotalAmount))
	{
		printf("当前办卡总人数：%d\n未注销卡总人数：%d\n当前上机人数：%d\n总营业额（消费金额）：%.1f\n", mTotal, mPerson, mOnlinePerson, TotalAmount);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}