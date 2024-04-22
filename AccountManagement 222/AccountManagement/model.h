#pragma once
#include<time.h>
typedef struct Card
{
	char aName[18];//卡号
	char aPwd[8];//密码
	int nStatus;//卡状态（0-未上机;1-正在上机;2-已注销;3-失效)
	float fTotalUse;//累计消费金额
	int nUseCount;// 使用次数
	float fBalance;//余额
	int nDel;//删除标识（0-未删除;1-删除）
	time_t tStart;//开始上机时间
	time_t tEnd;//下机时间
	time_t tLastTime;//最后使用的时间
}Card;

typedef struct CardNode
{
	Card data;
	struct CardNode* next;
}CardNode,*lpCardNode;//lpCardNode p则直接创建指针，CardNode*p才是指针


typedef struct Billing
{
	char aCardName[18];//卡号
	time_t tStart;//开卡时间
	time_t tEnd;//卡的截止时间
	float fAmount;//消费金额
	int nStatus;//消费状态 0未结算 1已经结算
	int nDel;//删除标识 0未删除 1删除
}Billing;

typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode,*lpBillingNode;


typedef struct LogonInfo//上机信息
{
	char aCardName[18];//上机卡号
	time_t tLogon;//上机时间
	float fBalance;//上机时卡的余额
}LogonInfo;

typedef struct SettleInfo
{
	char aCardName[18];//上机卡号
	time_t tStart;//上机时间
	time_t tEnd;//上机时卡的余额
	float fAmount;//消费金额
	float fBalance;//余额
}SettleInfo;
