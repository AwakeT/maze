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

//����ͣ��ʱ���복�� 
typedef struct
{
	int time1;//��ջʱ�� 
	int time2;//��ջʱ�� 
	char plate[100];//�洢������Ϣ 
}Car;

//ͣ�ſռ�ջ
typedef struct
{
	Car Stop[MAX_STOP - 1];//����ͣ�����Ϣ�洢�ռ� 
	int top;//����ָʾջ��ָ�� 
} Stopping;

//ͣ������Ⱥ����
typedef struct
{
	int count;//���ڴ洢�Ⱥ������� 
	Car Pave[TEMP_STOP - 1];//��ʱͣ���ĳ�����Ϣ 
	int front, rear;//��ָ�� 
} Temporary;

//��·�ռ�ջ
typedef struct
{
	Car Away[MAX_STOP - 1];//��Ҫ��·������Ϣ 
	int top; //ջ��ָ�� 
} Away;

Car c;
Stopping s;
Temporary p;
Away b;
char C[10];

void welcome();  //������ 
void stop_tem(); //�������� 
void tem_stop(); //������ͣ��λ 
void stop_away();//��������· 
void car_leave();//���뿪 
void Display();  //��ʾ������Ϣ 

void stop_tem() // ��ͣ����
{
	// �ж϶���
	if (p.count > 0 && (p.front == (p.rear + 1) % TEMP_STOP))
	{
		printf("������������´�����\n");
	}
	else
	{
		strcpy(p.Pave[p.rear].plate, C);
		p.rear = (p.rear + 1) % TEMP_STOP;// ��βָʾ����1
		p.count++;// ��������1
		printf("����Ϊ%s������ͣ�����ϵ�%d��λ��\n", C, p.rear);
	}
}

void temp_stop()// ��ͣ��ͣ��λ
{
	printf("�����뼴��ͣ���ĳ��ƺ�:");// ���복�ƺ�
	scanf("%s", &C);
	if (s.top >= MAX_STOP - 1)// ���ͣ��λ������ͣ����
	{
		stop_tem();// ͣ��λ->���  ����
	}
	else
	{
		s.top++;// ͣ��λջ��ָ���1
		time_t t1;
		long int t = time(&t1);// ��ǽ���ͣ������ʱ��
		char* t2;
		t2 = ctime(&t1);// ��ȡ��ǰʱ��
		c.time1 = t;
		strcpy(s.Stop[s.top].plate, C);// ��¼���ƺ�                   
		printf("����Ϊ%s������ͣ��ͣ��λ��%d��λ, ��ǰʱ��:%s\n", C, s.top + 1, t2);
	}
	return;
}

void stop_away()// ��������·ջ
{
	// ͣ��λջת����ʱջ��Ϊ��Ҫ��ջ�ĳ�����· 
	while (s.top >= 0)
	{
		if (0 == strcmp(s.Stop[s.top--].plate, C))
		{
			break;
		}
		// �ó��ĳ�������·ջ
		strcpy(b.Away[b.top++].plate, s.Stop[s.top + 1].plate);
		printf("����Ϊ%s��������ʱ�˳�ͣ��λ\n", s.Stop[s.top + 1].plate);
	}
	b.top--;
	// ���ͣ��λ�еĳ������˵���˵��ͣ��λ���޳�����Ҫ����
	if (s.top < -1)
	{
		printf("ͣ��λ���޴˳���Ϣ\n");
	}
	else
	{
		printf("����Ϊ%s��������ͣ��������\n", s.Stop[s.top + 1].plate);
	}
	// ����·ջ�еĳ���ѹ��ͣ��λջ
	while (b.top >= 0)
	{
		strcpy(s.Stop[++s.top].plate, b.Away[b.top--].plate);
		printf("����Ϊ%s������ͣ��ͣ��λ%d��λ\n", b.Away[b.top + 1].plate, s.top + 1);
	}
	// �ӱ���� -> ͣ��λ
	while (s.top < MAX_STOP - 1)
	{
		if (0 == p.count)// �ж϶����Ƿ�Ϊ��
		{
			break;
		}
		// ��Ϊ�գ�����������ȼ��ߵĳ�ͣ��ͣ��λ
		else
		{
			strcpy(s.Stop[++s.top].plate, p.Pave[p.front].plate);
			printf("����Ϊ%s�������ӱ���н���ͣ��λ��%d��λ\n", p.Pave[p.front].plate, s.top + 1);
			p.front = (p.front + 1) % TEMP_STOP;
			p.count--;
		}
	}
}

void car_leave()// ���뿪ͣ���� 
{
	printf("�����뼴���뿪�ĳ��ƺ�:\n");
	scanf("%s", &C);
	if (s.top < 0)// �ж�ͣ��λ�Ƿ��г���
	{
		printf("��λ�ѿգ��޳�����Ϣ��\n");
	}
	else
	{
		stop_away();
	}
	time_t t1;
	long int t = time(&t1);
	c.time2 = t;// ��¼�뿪ͣ������ʱ��
	char* t2;
	t2 = ctime(&t1);// ��ȡ��ǰʱ��
	printf("�뿪ʱ��%s\n��Ҫ֧��%ldԪ\n", t2, MONEY * (c.time2 - c.time1) / 10);
}

void Display()
{
	int i = s.top;
	if (-1 == i)
	{
		printf("ͣ����Ϊ��\n");
	}
	time_t t1;
	long int t = time(&t1);// �����ʾʱ��ʱ��
	printf("\t���ƺ�\t\t\tͣ��ʱ��\t\t��ǰ����֧�����\n");
	while (i != -1)
	{
		printf("\t%s\t\t%d��\t\t\t%dԪ\n", s.Stop[i].plate, t - c.time1, MONEY * (t - c.time1) / 10);
		i--;
	}
}

void welcome()
{
	printf("\t**********************ͣ������Ϣ***********************\n");
	printf("\tͣ��������%d����λ,��ǰͣ��������%d����,�Ⱥ�������%d����\n", MAX_STOP, s.top + 1, (p.rear + TEMP_STOP - p.front)
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
	// ��ʼ��
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

		printf("����������0\n");
		scanf("%d", &ch);
		if (0 == ch)
		{
			continue;
		}
		else
		{
			printf("����������������������\n");
			scanf("%d", &ch);
			continue;
		}
	}
	return 0;
}

