#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>


#define WIDTH 100           //地图宽度
#define HEIGHT 40           //地图高度
#define SNAKEN 6            //蛇初始长度
#define LIFE 1              //生命次数
//#define SPEED 70           //游戏速度(游戏循环时间)
#define U 1                 
#define D 2                 
#define L 3                 //蛇的状态，U：上 ；D：下；L:左 R：右
#define R 4                 

int Mode;
char ModeName[20]="hello";
int food_x,food_y;
int if_game_again=1;           

void HideCursor()//隐藏光标
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y)//光标移动
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


int updatetime()   //获取一次电脑现在的时间
{
    int now;
    SYSTEMTIME system_time;             
    GetLocalTime(&system_time);
    now = system_time.wMinute * 60 + system_time.wSecond;   
    return now;
}

int time_0=updatetime();
int time_1;


void Gametime()                           
{
    time_1 = updatetime() - time_0;     //用现在的时间减去刚开始的时间得游戏时间
    gotoxy(116,8);
    printf("%d s", time_1);
}


//1：打印地图和随机食物

void Init_Map()              //打印地图函数
{

    for (int i = 1; i < WIDTH; i++)        //打印上下面墙
    {
        gotoxy(i, 38);
        printf("O");
        gotoxy(i, 0);
        printf("O");
    }
    for (int i = 0; i < HEIGHT-1; i++)    //打印左右两面墙
    {
        gotoxy(1, i);
        printf("O");
        gotoxy(99, i);
        printf("O");
    }
    gotoxy(108, 2);
    printf("score:  0        ");
    gotoxy(108, 5); 
    printf("life:   %d  ", LIFE);  
    gotoxy(108,8);
    printf("time:   0        ");
    gotoxy(101,12);
    printf("W->up     S->down");
    gotoxy(101,14);
    printf("A->left   D->right");
    gotoxy(101,18);
    printf("if you want to pause the game?");
    gotoxy(101,20);
    printf("please enter 'Space' ");
	gotoxy(101,22);
    printf("Current game mode:%s", ModeName);
    gotoxy(106,34);
    printf("ID:201910501019");    
	gotoxy(106,36);
    printf("Author:DengYang");
}

void Produce_Food()         //产生随机食物
{
    srand(time(NULL));      

    food_x = rand() % (WIDTH - 8) + 2;       

    while (food_x % 2)                       
    {
        food_x = rand() % (WIDTH - 8) + 2;   
    }

    if (food_x<3)
        food_x +=2;

    food_y = rand() % (HEIGHT - 8) + 3;      

    gotoxy(food_x, food_y);
    printf("o");
}




typedef struct snake
{
    int x;	//蛇身体位置坐标
    int y;
    struct snake* next;
}snake;

snake* head;      //蛇头指针  指向结构体head的地址

void Init_Snake() //初始化蛇
{   
    snake *tail;  //蛇尾指针
    int i;
	//蛇的初始位置
    tail = (snake*)malloc(sizeof(snake));
    tail->next = NULL;
    tail->x = HEIGHT-6;
    tail->y = 8;
				//SNAKEN 宏定义 蛇的初始长度
    for (i = 1; i <= SNAKEN; i++)     //在蛇尾处创建链表
    {
        head = (snake*)malloc(sizeof(snake));
        head->next = tail;

        head->x = 24 + i * 2;               
        head->y = 8;
        tail = head;         	     //此时蛇尾指针指向蛇头 
    }

    while (tail)		
    {
        gotoxy(tail->x, tail->y);
        printf("o");
        tail = tail->next;
    }
}




int status = R;         //蛇前进状态

snake* p = NULL;        //运行指针
snake* nexthead;        //下一个蛇头
int score = 0;          //得分

void Snake_Move()       
{
    nexthead = (snake*)malloc(sizeof(snake));

    if (status == U)
    {
        nexthead->y = head->y - 1;  //确定新蛇头的下一个坐标
        nexthead->x = head->x;
    }
    else if (status == D)                //下
    {
        nexthead->y = head->y + 1;
        nexthead->x = head->x;
    }
    else if (status == L)                //左
    {
        nexthead->x = head->x - 2;
        nexthead->y = head->y;
    }
    else if (status == R)                //右
    {
        nexthead->x = head->x + 2;
        nexthead->y = head->y;
    }
    nexthead->next = head;
    head = nexthead;
    p = head;

    if (p->x == food_x && p->y == food_y) //判断蛇头的位置是否和食物的位置重合
    {
        while (p)                       //输出尾结点
        {
            gotoxy(p->x, p->y);
            if (p == head)
                printf("o");
            else
                printf("o");
            p = p->next;                //创建头节点删除尾节点
        }                               //吃到食物就不删除尾节点
        
        Produce_Food();                   //碰到食物则再产生一个食物

        score+=10;;
        gotoxy(116, 2);
        printf("%d", score);
    }
    else
    {
        while (p->next->next)           //不输出尾结点
        {
            gotoxy(p->x, p->y);
            if (p == head)
                printf("o");
            else
                printf("o");
            p = p->next;
        }

        gotoxy(p->next->x, p->next->y);
        printf("  ");
        free(p->next);
        p->next = NULL;
    }

    p = head;
    while (p)                     //如果食物产生在蛇身上就再产生一个食物
    {
        if (p->x == food_x && p->y == food_y)
            Produce_Food();
        p = p->next;
    }
}






int ch;

void Control_Direction()//操作方向函数，接收从键盘输入的按键，控制蛇行进方向
{
    ch = _getch();
    switch (ch)		//up代表72，down代表80，left代表77，right代表75
    {
        case 72: 
            if(status != D)
                status = U; break;
        case 80: 
            if (status != U)
                status = D; break;
        case 75: 
            if (status != R)
                status = L; break;
        case 77: 
            if (status != L)
                status = R; break;
        case ' ': 
            _getch(); break;        
    }
}

int die_1()                  //死亡条件1咬自己
{
    snake* self = head->next;       //self为蛇身上的一个结点
    while (self)
    {
        if (self->x == head->x && self->y == head->y) 
        {
            return 1;
        }
        self = self->next;
    }
    return 0;
}

int die_2()                  //死亡条件2碰墙
{
    if (head-> x <= 1 || head->x >= 98 || head->y <= 0 || head->y >= 38)
        return 1;
    
    return 0;
}


void Quit_Game()        //退出游戏函数
{
    system("cls");
    gotoxy(55, 20);
    printf("gameover！");
    gotoxy(55, 22);
    printf("score：%d", score);
    gotoxy(55,24);
    printf("if you want to keep playing,please enter ‘tcs’ ");

    p = head;              //释放内存
    while (head)
    {
        head = head->next;
        free(p);
        p = head;
    }
    
}                 


void Gameover()                     //失败选择界面
{
    char key1;
    int sw1=0;
    gotoxy(101,24);
    printf("If you want to quit ?");
    gotoxy(101,26);
    printf("Please enter the 'q' ");
    gotoxy(101,28);
    printf("If you want to play again?");
    gotoxy(101,30);
    printf("Please enter the 'r' ");
    while (!sw1)
    {
        key1 = _getch();
        if (key1 == 'q')
        {
            if_game_again--;
            sw1=1;
        }
        else if (key1 == 'r')
        {
             break;
             sw1=1;
        }
    }    
}



void Game_Start()   //游戏开始提示界面   
{
    char key2;
    int sw2=0;
    gotoxy(38,12);
    printf("Press the space to start the game");
	gotoxy(38,14);
    printf("Choose game mode");
	gotoxy(38,16);
    printf("'E':    Easy'");
	gotoxy(38,18);
    printf("'C':    Common'");
	gotoxy(38,20);
    printf("'I':    Infernal'");
	
	
	gotoxy(38,24);
    printf("******201910501019*******");
	gotoxy(38,26);
    printf("*********DengYang*********");
	
    while (!sw2)		//游戏难度选择
    {
		Mode=1;
        key2=_getch();
        if (key2=='e')
		{
			Mode=1;	//Mode=1,通过后面函数实现蛇速度的控制
			strcpy(ModeName, "Easy");
			sw2=1;
		}
		if (key2=='c')
		{
			Mode=2;
			strcpy(ModeName, "Common");
			sw2=1;
		}
		if (key2=='i')
		{
			Mode=4;
			strcpy(ModeName, "Infernal");
			sw2=1;
		}
    }
}









int main()
{
    system("mode con cols=132 lines=40");   //初始化控制台大小
    system("title  Greedy_snake");          //标题
    HideCursor();                           //隐藏光标
    Init_Map();      //初始化地图
    Game_Start();   //等待游戏开始界面
    while (if_game_again) 
    {
		Game_Start();
        score=0;
        time_0= updatetime(); //更新游戏开始时间
        system("cls");      //清屏
        Produce_Food();                   //初始化食物
        Init_Map();                       //初始化地图
        Init_Snake();         			  //初始化蛇
        status = R;                       //初始化运动方向
        

        while (1)
        {
			
            Snake_Move();                    

            if (_kbhit())//接收键盘按键
            {
                Control_Direction(); //方向控制
            }
            
            if (die_1() || die_2())//死亡条件
                break;

            Gametime();   //更新游戏时间                

            Sleep(90-(score*Mode));//循环延时（宏定义可以改变蛇的速度）
        }
        Gameover();   //游戏结束                             
    }
    Quit_Game();    //退出游戏
    return 0;
}


