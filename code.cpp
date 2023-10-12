//사용할 함수 Sleep(n): n초동안 멈춤 System("Cls") 콘솔 창 다 지우기 kbhit(): 입력 대기 상태에서도 프로그램이 실행되도록 해줌->getch와 씀(버퍼가 없기때문)
//방향키 위:224 72 아래:224 80 왼:224 75 오:224 77

#include <iostream>
#include <cstdio>
#include <Windows.h> //gotoxy()
#include <conio.h> //kbhit()
#include <time.h>
//#include "DoubleBuffering.h"
using namespace std;

void gotoxy(int x, int y);
void CursorView();
void setBackground();
void moveCur();
void showdeadblock();
////버퍼///////////////////////////////////////
//extern int g_nScreenIndex;
//extern HANDLE g_hScreen[2];
//extern int g_numofFPS;
//extern clock_t CurTime, OldTime;
//extern char* FPSTextInfo;
//void ScreenInit();
//void ScreenFlipping();
//void ScreenClear();
//void ScreenRelease();
//void ScreenPrint(int x, int y, char* string);
//void Render();
//void Release();
///////////////////////////////////////////////
int xlist[100]; //이것들을 큐로 처리해서 한 줄이 된 경우 pop처리?
int ylist[100];
int bc;
int isblock[30] = { 14,14, 14, 14, 14, 14, 14, 14, 14, 14,14,14, 14, 14, 14, 14, 14, 14, 14, 14,14,14, 14, 14, 14, 14, 14, 14, 14, 14 };


int x = 5, y = 0;
int t;
double save;
int main()
{
    CursorView();
    setBackground();
    while (true)
    {
        clock_t start = clock();
        gotoxy(x, y);
        cout << 'X';
        moveCur(); //커서이동
        clock_t end = clock();
        save += (double)(end - start) / CLOCKS_PER_SEC;
        if (save > 1)
        {
            save = 0;
            if (y == isblock[x]) //죽은 블럭처리는 여기서 이루어짐//방향키로 아래로 갔더라도 save>1 조건에 들어와서 처리됨.
            {
                xlist[bc] = x;
                ylist[bc] = y;
                bc++;
                isblock[x]--;
                x = 5, y = -1;
            }
            y++;
            gotoxy(x, y);
            system("cls");
            setBackground();
        }
    }
    return 0;
}


void gotoxy(int x, int y)
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setBackground()
{
    int x = 0, y = 0;
    for (int i = 0; i < 15; i++)
    {
        gotoxy(x, y);
        cout << 'W';
        y++;
    }
    gotoxy(x, y);
    cout << 'W';
    x++;
    for (int i = 0; i < 15; i++)
    {
        gotoxy(x, y);
        cout << 'W';
        x++;
    }
    for (int i = 0; i < 16; i++)
    {
        gotoxy(x, y);
        cout << 'W';
        y--;
    }
    showdeadblock();
}

void showdeadblock()
{
    for (int i = 0; i < bc; i++)
    {
        gotoxy(xlist[i], ylist[i]);
        cout << 'X';
    }
}

void moveCur()
{
    if (_kbhit())
    {
        t = _getch();
        if (t == 224)
        {
            t = _getch();
            switch (t)
            {
                //up 나중에 없애기
            case 72:
            {if (y == 0)break; }
            y--;
            system("cls");
            setBackground();
            break;

            //down
            case 80:
                if (y + 1 == 15 || isblock[x] == y) break;
                y++;
                system("cls");
                setBackground();
                break;

                //left
            case 75:
            {if (x - 1 == 1 || isblock[x - 1] < y)break; }
            x--;
            system("cls");
            setBackground();
            break;

            //right
            case 77:
            {if (x + 1 == 16 || isblock[x + 1] < y)break; }
            x++;
            system("cls");
            setBackground();
            break;
            }
        }
    }
}

int delete_block()
{
    for (int i = 1; i < 15; i++)
    {
        if (isblock[i] != 13)
        {
            return 0;
        }
    }
    for (int i = 1; i < 15; i++)
    {
        isblock[i] = 14;
    }
    //bc는 죽은블럭수 ->블럭 위치는스택으로 처리해야할듯???????? 각 x별 스택을 만들고 value가 y좌표인 스택;

}

//////////////////////////////////doublebuffering/////////////////////////////////////////////////////////////////////
//void ScreenInit()
//{
//    CONSOLE_CURSOR_INFO cci;
//
//    // 화면 버퍼 2개를 만든다.
//    g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//    g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//
//    // 커서를 숨긴다.
//    cci.dwSize = 1;
//    cci.bVisible = FALSE;
//    SetConsoleCursorInfo(g_hScreen[0], &cci);
//    SetConsoleCursorInfo(g_hScreen[1], &cci);
//}
//
//void ScreenFlipping()
//{
//    SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
//    g_nScreenIndex = !g_nScreenIndex;
//}
//
//void ScreenClear()
//{
//    COORD Coor = { 0, 0 };
//    DWORD dw;
//    FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 25, Coor, &dw);
//}
//
//void ScreenRelease()
//{
//    CloseHandle(g_hScreen[0]);
//    CloseHandle(g_hScreen[1]);
//}
//
//void ScreenPrint(int x, int y, char* string)
//{
//    DWORD dw;
//    COORD CursorPosition = { x, y };
//    SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
//    WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
//}
//
//void Render()
//{
//    ScreenClear();
//
//    if (CurTime - OldTime >= 1000) // 출력 코드
//    {
//        sprintf(FPSTextInfo, "FPS : %d", g_numofFPS);
//        OldTime = CurTime;
//        g_numofFPS = 0;
//    }
//
//    g_numofFPS++;
//    ScreenPrint(0, 0, FPSTextInfo);
//    ScreenFlipping();
//}
//
//void Release()
//{
//    delete[] FPSTextInfo;
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
