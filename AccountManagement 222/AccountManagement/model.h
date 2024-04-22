#pragma once
#include<time.h>
typedef struct Card
{
	char aName[18];//����
	char aPwd[8];//����
	int nStatus;//��״̬��0-δ�ϻ�;1-�����ϻ�;2-��ע��;3-ʧЧ)
	float fTotalUse;//�ۼ����ѽ��
	int nUseCount;// ʹ�ô���
	float fBalance;//���
	int nDel;//ɾ����ʶ��0-δɾ��;1-ɾ����
	time_t tStart;//��ʼ�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	time_t tLastTime;//���ʹ�õ�ʱ��
}Card;

typedef struct CardNode
{
	Card data;
	struct CardNode* next;
}CardNode,*lpCardNode;//lpCardNode p��ֱ�Ӵ���ָ�룬CardNode*p����ָ��


typedef struct Billing
{
	char aCardName[18];//����
	time_t tStart;//����ʱ��
	time_t tEnd;//���Ľ�ֹʱ��
	float fAmount;//���ѽ��
	int nStatus;//����״̬ 0δ���� 1�Ѿ�����
	int nDel;//ɾ����ʶ 0δɾ�� 1ɾ��
}Billing;

typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode,*lpBillingNode;


typedef struct LogonInfo//�ϻ���Ϣ
{
	char aCardName[18];//�ϻ�����
	time_t tLogon;//�ϻ�ʱ��
	float fBalance;//�ϻ�ʱ�������
}LogonInfo;

typedef struct SettleInfo
{
	char aCardName[18];//�ϻ�����
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�ϻ�ʱ�������
	float fAmount;//���ѽ��
	float fBalance;//���
}SettleInfo;
