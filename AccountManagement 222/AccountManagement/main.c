#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<Windows.h>
#include "menu.h"
#
int main()
{
	char ch;
	int nSelection = -1;
	while (1)
	{
		showMenu();
		scanf("%d", &nSelection);
		while ((ch = getchar()) != '\n')//清理缓冲区
		{

		}
		switch (nSelection)
		{
		case 1: add(); break;
		case 2:	query(); break;
		case 3: logon(); break;
		case 4:settle(); break;
		case 5: recharge(); break;
		case 6: refund(); break;
		case 7: statistics(); break;
		case 8: logoff(); break;
		case 9: present();
		case 0: exitApp(); break;
		default: printf("wrong!\n"); break;
		}
		if (nSelection == 0)
		{
			printf("感谢使用\n");
			system("pause");
			return 0;
		}
		else
		{
			continue;
		}
	}
	system("pause");
	return 0;
}