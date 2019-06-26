#include<stdio.h>
#include<time.h>
#include<string.h> 
#include<stdlib.h>
#ifndef _PARKING_H_
#define _PARKING_H_
#endif
#define FALSE 0
#define TRUTH 1

#define MONEY 1
#define MAX_STOP 100
#define TEMP_STOP 10

//汽车停放时间与车牌 
typedef struct
{
	int time1;//入栈时间 
	int time2;//出栈时间 
	char plate[100];//存储车牌信息 
}Car;

//停放空间栈
typedef struct
{
	Car Stop[MAX_STOP - 1];//车辆停入后信息存储空间 
	int top;//用来指示栈顶指针 
} Stopping;

//停车场外等候队列
typedef struct
{
	int count;//用于存储等候车辆个数 
	Car Pave[TEMP_STOP - 1];//暂时停车的车辆信息 
	int front, rear;//队指针 
} Temporary;

//让路空间栈
typedef struct
{
	Car Away[MAX_STOP - 1];//需要让路车辆信息 
	int top; //栈顶指针 
} Away;

Car c;
Stopping s;
Temporary p;
Away b;
char C[10];

void welcome();  //主界面 
void stop_tem(); //车进入便道 
void tem_stop(); //车进入停车位 
void stop_away();//车进入让路 
void car_leave();//车离开 
void Display();  //显示车辆信息 

void stop_tem() // 车停入便道
{
	// 判断队满
	if (p.count > 0 && (p.front == (p.rear + 1) % TEMP_STOP))
	{
		printf("便道已满，请下次再来\n");
	}
	else
	{
		strcpy(p.Pave[p.rear].plate, C);
		p.rear = (p.rear + 1) % TEMP_STOP;// 队尾指示器加1
		p.count++;// 计数器加1
		printf("牌照为%s的汽车停入便道上的%d的位置\n", C, p.rear);
	}
}

void temp_stop()// 车停入停车位
{
	printf("请输入即将停车的车牌号:");// 输入车牌号
	scanf("%s", &C);
	if (s.top >= MAX_STOP - 1)// 如果停车位已满，停入便道
	{
		stop_tem();// 停车位->便道  函数
	}
	else
	{
		s.top++;// 停车位栈顶指针加1
		time_t t1;
		long int t = time(&t1);// 标记进入停车场的时间
		char* t2;
		t2 = ctime(&t1);// 获取当前时间
		c.time1 = t;
		strcpy(s.Stop[s.top].plate, C);// 记录车牌号                   
		printf("牌照为%s的汽车停入停车位的%d车位, 当前时间:%s\n", C, s.top + 1, t2);
	}
	return;
}

void stop_away()// 车进入让路栈
{
	// 停车位栈转入临时栈，为需要出栈的车辆让路 
	while (s.top >= 0)
	{
		if (0 == strcmp(s.Stop[s.top--].plate, C))
		{
			break;
		}
		// 让出的车进入让路栈
		strcpy(b.Away[b.top++].plate, s.Stop[s.top + 1].plate);
		printf("牌照为%s的汽车暂时退出停车位\n", s.Stop[s.top + 1].plate);
	}
	b.top--;
	// 如果停车位中的车都让了道，说明停车位中无车辆需要出行
	if (s.top < -1)
	{
		printf("停车位上无此车消息\n");
	}
	else
	{
		printf("牌照为%s的汽车从停车场开走\n", s.Stop[s.top + 1].plate);
	}
	// 将让路栈中的车辆压入停车位栈
	while (b.top >= 0)
	{
		strcpy(s.Stop[++s.top].plate, b.Away[b.top--].plate);
		printf("牌照为%s的汽车停回停车位%d车位\n", b.Away[b.top + 1].plate, s.top + 1);
	}
	// 从便道中 -> 停车位
	while (s.top < MAX_STOP - 1)
	{
		if (0 == p.count)// 判断队列是否为空
		{
			break;
		}
		// 不为空，将便道中优先级高的车停入停车位
		else
		{
			strcpy(s.Stop[++s.top].plate, p.Pave[p.front].plate);
			printf("牌照为%s的汽车从便道中进入停车位的%d车位\n", p.Pave[p.front].plate, s.top + 1);
			p.front = (p.front + 1) % TEMP_STOP;
			p.count--;
		}
	}
}

void car_leave()// 车离开停车场 
{
	printf("请输入即将离开的车牌号:\n");
	scanf("%s", &C);
	if (s.top < 0)// 判断停车位是否有车辆
	{
		printf("车位已空，无车辆信息！\n");
	}
	else
	{
		stop_away();
	}
	time_t t1;
	long int t = time(&t1);
	c.time2 = t;// 记录离开停车场的时间
	char* t2;
	t2 = ctime(&t1);// 获取当前时间
	printf("离开时间%s\n需要支付%ld元\n", t2, MONEY * (c.time2 - c.time1) / 10);
}

void Display()
{
	int i = s.top;
	if (-1 == i)
	{
		printf("停车场为空\n");
	}
	time_t t1;
	long int t = time(&t1);// 标记显示时的时间
	printf("\t车牌号\t\t\t停放时间\t\t当前所需支付金额\n");
	while (i != -1)
	{
		printf("\t%s\t\t%d秒\t\t\t%d元\n", s.Stop[i].plate, t - c.time1, MONEY * (t - c.time1) / 10);
		i--;
	}
}

void welcome()
{
	printf("\t**********************停车场信息***********************\n");
	printf("\t停车场共有%d个车位,当前停车场共有%d辆车,等候区共有%d辆车\n", MAX_STOP, s.top + 1, (p.rear + TEMP_STOP - p.front)
		% TEMP_STOP);
	printf("\t********************************************************\n");
	printf("\t*                     1.Parking                        *\n");
	printf("\t*                     2.leaving                        *\n");
	printf("\t*                     3.situation                      *\n");
	printf("\t*                     4.exit                           *\n");
	printf("\t********************************************************\n");
}
int main()
{
	// 初始化
	s.top = -1;
	b.top = 0;
	p.rear = 0;
	p.count = 0;
	p.front = 0;

	while (1)
	{
		system("cls");
		welcome();
		int i, ch;
		scanf("%d", &i);

		if (1 == i) temp_stop();
		if (2 == i) car_leave();
		if (3 == i) Display();
		if (4 == i) break;

		printf("返回请输入0\n");
		scanf("%d", &ch);
		if (0 == ch)
		{
			continue;
		}
		else
		{
			printf("您的输入有误，请重新输入\n");
			scanf("%d", &ch);
			continue;
		}
	}
	return 0;
}

