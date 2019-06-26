#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <cstdlib>			//标准库
#include <stdio.h>			//getchar	
#include <ctime>		
#include <conio.h>

#define MAX_X 20
#define MAX_Y 30
bool flag = false;
bool slow = false;
bool autogame = true;

using namespace std;

int maze[MAX_X][MAX_Y];		//迷宫

//路线栈
class stack_of_maze {
private:
	//记录迷宫坐标
	struct node
	{
		int x;
		int y;
		char direction;  	//上一步路径（如何来的）
		node* next;
	};
	node* head;
public:
	stack_of_maze() {
		head = NULL;
	}

	~stack_of_maze() {
		node* p = head;
		while (head != NULL) {
			head = head->next;
			delete p;
			p = head;
		}
	}

	//压栈
	void push(int xx, int yy, char ddirection) {
		node* new_node = new node;
		if (new_node != NULL) {
			new_node->x = xx;
			new_node->y = yy;
			new_node->direction = ddirection;
			new_node->next = NULL;

			if (head == NULL)
				head = new_node;
			else {
				new_node->next = head;
				head = new_node;
			}
		}
		else
			cout << "内存分配失败" << endl;

	}

	//出栈
	node* pop(int& xx, int& yy) {
		if (head != NULL) {
			node* p = head;
			head = head->next;
			xx = p->x;
			yy = p->y;
			delete p;
		}
		return head;
	}

	void print() {
		if (head != NULL) {
			node* p = head;
			while (p != NULL) {
				cout << " " << p->x << " " << p->y << " " << p->direction << endl;
				p = p->next;
			}
		}
		else
			cout << "栈为空，打印失败" << endl;
	}
};

//创建迷宫
void createMaze() {
	int maxway = MAX_X * MAX_Y;		//最大通路
	int x, y;

	for (x = 0; x < MAX_X; x++)
		for (y = 0; y < MAX_Y; y++)
			maze[x][y] = 1;			//先填充迷宫

	srand((unsigned)time(NULL));	//随机函数种子，以时间为参数
	for (int i = 0; i < maxway; i++)		//随机构建迷宫通路
	{
		x = rand() % (MAX_X - 2) + 1;
		y = rand() % (MAX_Y - 2) + 1;
		maze[x][y] = 0;
	}

	maze[1][1] = 0; 				//入口
	maze[MAX_X - 2][MAX_Y - 2] = 0;		//出口

	maze[0][1] = 3;
	maze[MAX_X - 1][MAX_Y - 2] = 0;
}

//输出迷宫
void printMaze() {
	int x, y;
	system("cls");				//windows下使用system("cls")
	//cout<<endl;
	for (x = 0; x < MAX_X; x++)
	{
		for (y = 0; y < MAX_Y; y++)
		{
			if (maze[x][y] == 0) { cout << "  "; continue; }		//通路
			if (maze[x][y] == 1) { cout << "■"; continue; }		//墙
			if (maze[x][y] == 2) { cout << "×"; continue; }		//死胡同
			if (maze[x][y] == 3) { cout << "↓"; continue; }		//向下走
			if (maze[x][y] == 4) { cout << "→"; continue; }
			if (maze[x][y] == 5) { cout << "←"; continue; }
			if (maze[x][y] == 6) { cout << "↑"; continue; }
			if (maze[x][y] == 7) { cout << "※"; continue; }		//当前站立位置
		}
		cout << endl;
	}
	if (slow) {
		Sleep(1);											//延时函数
	}
}

void check(stack_of_maze& s) {
	int temp[MAX_X][MAX_Y];

	for (int x = 0; x < MAX_X; x++)
		for (int y = 0; y < MAX_Y; y++)
			temp[x][y] = maze[x][y];

	int x = 1, y = 1;					//出发点	
	while (1) {
		temp[x][y] = 2;

		//向下
		if (temp[x + 1][y] == 0) {
			s.push(x, y, 'D');
			temp[x][y] = 3;			//在当前位置做一个向下的标志
			x = x + 1;
			temp[x][y] = 7;			//当前位置
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向右
		if (temp[x][y + 1] == 0) {
			s.push(x, y, 'R');
			temp[x][y] = 4;			//在当前位置做一个向右的标志
			y = y + 1;
			temp[x][y] = 7;
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向上
		if (temp[x - 1][y] == 0) {
			s.push(x, y, 'U');
			temp[x][y] = 6;			//在当前位置做一个向上的标志
			x = x - 1;
			temp[x][y] = 7;
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向左
		if (temp[x][y - 1] == 0) {
			s.push(x, y, 'L');
			temp[x][y] = 5;			//在当前位置做一个向右的标志
			y = y - 1;
			temp[x][y] = 7;
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//上下左右不通，则回退
		if (s.pop(x, y) == NULL && temp[x - 1][y] != 0 && temp[x][y - 1] != 0 && temp[x][y + 1] != 0 && temp[x + 1][y] != 0) {
			temp[0][1] = 7;
			if (temp[1][1] != 1)
				temp[1][1] = 2;
			return;
		}
	}
}

//输入,windows下可以使用#incldue<conio.h>替代此函数
void move() {
	int x = 1, y = 1;					//出发点	
	while (1) {
		switch (_getch()) {
		case 's':
			if (maze[x + 1][y] == 0) {
				maze[x][y] = 0;
				x = x + 1;
				maze[x][y] = 7;			//当前位置
				printMaze();
				if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
					cout << "\n\n              成功走出" << endl;
					return;
				}
			}
			break;
		case 'd':
			if (maze[x][y + 1] == 0) {
				if (maze[x][y + 1] == 0) {
					maze[x][y] = 0;
					y = y + 1;
					maze[x][y] = 7;
					printMaze();
					if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
						cout << "\n\n              成功走出" << endl;
						return;
					}
				}
			}

			break;
		case 'w':
			if (maze[x - 1][y] == 0) {
				maze[x][y] = 0;
				x = x - 1;
				maze[x][y] = 7;
				printMaze();
				if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
					cout << "\n\n              成功走出" << endl;
					return;
				}
			}
			break;
		case 'a':
			if (maze[x][y - 1] == 0) {
				maze[x][y] = 0;
				y = y - 1;
				maze[x][y] = 7;
				printMaze();
				if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
					cout << "\n\n              成功走出" << endl;
					return;
				}
			}
			break;
		}
	}
}

void autoMove(stack_of_maze& s) {
	int x = 1, y = 1;					//出发点	
	while (1) {
		maze[x][y] = 2;

		//向下
		if (maze[x + 1][y] == 0) {
			s.push(x, y, 'D');
			maze[x][y] = 3;			//在当前位置做一个向下的标志
			x = x + 1;
			maze[x][y] = 7;			//当前位置
			if (slow)
				printMaze();
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				s.push(x, y, '*');
				cout << "\n\n              成功走出" << endl;
				return;
			}
			else
				continue;
		}

		//向右
		if (maze[x][y + 1] == 0) {
			s.push(x, y, 'R');
			maze[x][y] = 4;			//在当前位置做一个向右的标志
			y = y + 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				s.push(x, y, '*');
				cout << "\n\n              成功走出" << endl;
				return;
			}
			else
				continue;
		}

		//向上
		if (maze[x - 1][y] == 0) {
			s.push(x, y, 'U');
			maze[x][y] = 6;			//在当前位置做一个向上的标志
			x = x - 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				s.push(x, y, '*');
				cout << "\n\n              成功走出" << endl;
				return;
			}
			else
				continue;
		}

		//向左
		if (maze[x][y - 1] == 0) {
			s.push(x, y, 'L');
			maze[x][y] = 5;			//在当前位置做一个向右的标志
			y = y - 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == MAX_X - 1) && (y == MAX_Y - 2)) {
				s.push(x, y, '*');
				cout << "\n\n              成功走出" << endl;
				return;
			}
			else
				continue;
		}

		//上下左右不通，则回退
		if (s.pop(x, y) == NULL && maze[x - 1][y] != 0 && maze[x][y - 1] != 0 && maze[x][y + 1] != 0 && maze[x + 1][y] != 0) {
			cout << "\n\n              没有找到合适的路径" << endl;
			maze[0][1] = 7;
			if (maze[1][1] != 1)
				maze[1][1] = 2;
			return;
		}
	}
}

void menu();

void gamestart() {
	flag = false;
	while (!flag) {
		stack_of_maze stack;			//定义一个栈的对象，用来记录行走路线	
		createMaze();
		check(stack);
		system("cls");
		cout << "\t*                loading.              *" << endl;
		system("cls");
		cout << "\t*                loading..             *" << endl;
		system("cls");
		cout << "\t*                loading...            *" << endl;
	}
	printMaze();						//输出当前迷宫的初始状态
	cout << "\n\n              输入enter键继续" << endl;
	getchar();
	if (!autogame) {
		move();
		cout << "\n\n              输入enter键继续" << endl;
		getchar();
		menu();
	}
	else {
		stack_of_maze stack1;
		autoMove(stack1);					//行走中……
	}
	printMaze();							//输出迷宫的最终状态
	cout << "\n\n              输入enter键继续" << endl;
	getchar();
	menu();
}

void menu() {
	system("cls");
	int num;
	cout << "\t****************************************" << endl;
	cout << "\t*                                      *" << endl;
	cout << "\t*               1.查看路径             *" << endl;
	cout << "\t*                                      *" << endl;
	cout << "\t*               2.自动进行             *" << endl;
	cout << "\t*                                      *" << endl;
	cout << "\t*               3.自行游戏             *" << endl;
	cout << "\t*                                      *" << endl;
	cout << "\t*               4.退出游戏             *" << endl;
	cout << "\t*                                      *" << endl;
	cout << "\t****************************************" << endl;
	slow = false;
	switch (_getch()) {
	case '1':
		autogame = true;
		gamestart(); break;
	case '2':
		autogame = true;
		slow = true;
		gamestart();
		break;
	case '3':
		autogame = false;
		gamestart();
		break;
	case '4':
		exit(1); break;
	default:
		cout << "\n\n              错误操作，输入enter返回！" << endl;
		getchar();
		menu();
	}
	getchar();
}

int main(int argc, char** argv) {
	menu();
	return 0;
}
