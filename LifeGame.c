#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

typedef struct arr {
    unsigned int val[32];//val[i]의 j번째 비트: 격자의 왼쪽 위를 (0,0)이라 할 때 (i,j)가 살아 있는지
    unsigned int c[32][4];//val[i]의 j*4~j*4+3번째 비트: 격자의 왼쪽 위를 (0,0)이라 할 때 (i,j) 주변에 몇 개가 켜저 있는지
    int on, on2;//이 구간을 볼 필요가 있는지
}arr;//격자의 32*32 구간을 저장하는 구조체
arr board[100][100];
inline void sv(int x, int y, int v)
{
    if (v)
    {
        board[x >> 5][y >> 5].val[x & 31] |= (1U << (y & 31));
    }
    else
    {
        board[x >> 5][y >> 5].val[x & 31] &= 4294967295U - (1U << (y & 31));
    }
}//x,y의 상태를 바꿈
inline void pc(int x, int y)
{
    board[x >> 5][y >> 5].c[x & 31][(y & 24) >> 3] += (1 << ((y & 7) * 4));
}//x,y의 주변에 살아 있는 세포의 개수 카운터를 증가시킴
inline int gv(int x, int y)
{
    return !!(board[x >> 5][y >> 5].val[x & 31] & ((1U << (y & 31))));
}//x,y의 상태를 가져욤
inline int gc(int x, int y)
{
    return (board[x >> 5][y >> 5].c[x & 31][(y & 24) >> 3] & (15 << ((y & 7) * 4))) >> (y * 4);
}//x,y의 주변에 살아 있는 세포의 개수 리턴
int dx[8] = { 1,1,1,0,-1,-1,-1,0 };
int dy[8] = { 1,0,-1,-1,-1,0,1,1 };
void set(int N, int M, char* brd)//[N][M+1] 크기의 2차원 배열이나 N*(M+1) 크기의 1차원 배열 받아서 보드 구조체 배열에 저장함(i*(M+1)+M 번째는 출력을 위한 널 문자)
{
    int i, j, k, l;
    for (i = 0; i <= (N >> 5); i++)
    {
        for (j = 0; j <= (M >> 5); j++)
        {
            int mk = (i << 5) + 32;
            int ml = (j << 5) + 32;
            if (mk > N)
                mk = N;
            if (ml > M)
                ml = M;
            for (k = (i << 5); k < mk; k++)
            {
                for (l = (j << 5); l < ml; l++)
                {
                    sv(k, l, (*(brd + k * (M + 1) + l)) == 'O');
                    board[i][j].on |= (*(brd + k * (M + 1) + l)) == 'O';
                }

            }
        }
    }
}
void proc(int N, int M)//주변에 살아 있는 세포 계수 계산
{
    int i, j, k, l;
    for (i = 0; i <= (N >> 5); i++)
    {
        for (j = 0; j <= (M >> 5); j++)
        {
            memset(board[i][j].c, 0, sizeof(board[i][j].c));
            board[i][j].on2 = 0;
        }
    }
    for (i = 0; i <= (N >> 5); i++)
    {
        for (j = 0; j <= (M >> 5); j++)
        {
            if (board[i][j].on == 0)
                continue;
            board[i][j].on2 = 1;
            int mk = (i << 5) + 32;
            int ml = (j << 5) + 32;
            if (mk > N)
                mk = N;
            if (ml > M)
                ml = M;
            for (k = (i << 5); k < mk; k++)
            {
                for (l = (j << 5); l < ml; l++)
                {
                    if (gv(k, l))
                    {
                        int m;
                        for (m = 0; m < 8; m++)
                        {
                            int nx = (k + N + dx[m]) % N;
                            int ny = (l + M + dy[m]) % M;
                            pc(nx, ny);
                            board[nx >> 5][ny >> 5].on2 = 1;
                        }
                    }
                }
            }
        }
    }
}
void proc2(int N, int M)//살아 있는 세포의 개수로 상태 업데이트
{
    int i, j, k, l;
    for (i = 0; i <= (N >> 5); i++)
    {
        for (j = 0; j <= (M >> 5); j++)
        {
            if (board[i][j].on2 == 0)
                continue;
            board[i][j].on = 0;
            int mk = (i << 5) + 32;
            int ml = (j << 5) + 32;
            if (mk > N)
                mk = N;
            if (ml > M)
                ml = M;
            for (k = (i << 5); k < mk; k++)
            {
                for (l = (j << 5); l < ml; l++)
                {
                    int r = gc(k, l);
                    int v = (r == 3) || (r == 2 && gv(k, l));
                    board[i][j].on |= v;
                    sv(k, l, v);
                }
            }
        }
    }
}
void upd(int N, int M, char* brd)//현재 상태를 char에 저장함
{
    int i, j, k, l;
    for (i = 0; i <= (N >> 5); i++)
    {

        for (j = 0; j <= (M >> 5); j++)
        {
            int mk = (i << 5) + 32;
            int ml = (j << 5) + 32;
            if (mk > N)
                mk = N;
            if (ml > M)
                ml = M;
            for (k = (i << 5); k < mk; k++)
            {
                brd[k * (M + 1) + M] = '\0';
                for (l = (j << 5); l < ml; l++)
                {
                    brd[k * (M + 1) + l] = (gv(k, l) ? 'O' : '.');
                }
            }
        }
    }
}
void rep(int N, int M, int c, char* brd, int pr) {//스텝을 K번 반복

    FILE* fp = NULL;
//    fp = fopen("/Users/Sunrin/Desktop/c_project/game_file", "r+");
    fp = fopen("/Users/moontaeseo/Desktop/codeing/single/practice/C/practice_01_c/game_file", "w");  // 새로 덮어쓰기
    if (fp == NULL)
        printf("파일오픈 오류");



    set(N, M, brd);
    int i;
    for (i = 0; i < c; i++)
    {
        proc(N, M);
        proc2(N, M);
        upd(N, M, brd);
        if (pr) {//출력 형식 수정하고 싶으면 여기를 수정
            int j;
            printf("%d\n", i);
            for (j = 0; j < N; j++)
            {
                printf("%3d %s\n", j, brd + (j * (M + 1)));
            }
        }
        Sleep(500);

        
        
        // 게임이 돌아가는 각 회차마다 상태가 on인 배열들을 파일에 출력
        
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (*(brd + i * (M + 1) + j) == 'O')
                {
                    // printf("%d %d = %d\n", i, j, *(brd + i * (M + 1) + j));
                    fprintf(fp, "%d %d\n", i, j);
                }
            }
//            printf("\n");
        }
        Sleep(3000);
    }
}
void cvt(int N, int M, char(*arr)[3010], char* brd) {//[3010][3010] 2차원 배열을 1차원 배열에 담음
    int i;
    for (i = 0; i < N; i++)
    {
        int j;
        for (j = 0; j <= M; j++) {
            brd[i * (M + 1) + j] = arr[i][j];
        }
    }
}
void cvt2(int N, int M, char(*arr)[3010], char* brd) {//[3010][3010] 2차원 배열에 1차원 배열을 저장
    int i;
    for (i = 0; i < N; i++)
    {
        int j;
        for (j = 0; j <= M; j++) {
            arr[i][j] = brd[i * (M + 1) + j];
        }
    }
}
void slp(int N, int M, int c, int p, char(*arr)[3010]) {//크기, 반복 횟수, 출력 유무 [3010][3010] 2차원 배열이 주어지면 2차원 배열에 연산을 n번 진행

    char* x = (char*)malloc(10000000);
    memset(x, '.', sizeof(x));
    int j;
    for (j = 0; j < N; j++)
    {
        x[j * (M + 1) + M] = '\0';


    }
    cvt(N, M, arr, x);  // 1차원 배열에 담기
    rep(N, M, c, x, p);
    cvt2(N, M, arr, x);  // 2차원 배열에 담기


}



char loadMap (void) {
    // 맴이 저장된 파일의 경로를 받고 이를 불러들인 뒤, 배열에 담고 map을 호출하여 맵을 완성
    
    
    char ch[1000];
    int i, j;
    while(getchar() != '\n');  //입력버퍼 초기화
    printf("맵이 저장된 파일 경로를 입력해주세요");
    scanf("%s", ch);
    
    
    FILE* fp = NULL;
    fp = fopen(ch, "r");
    if (fp == NULL)
        printf("파일오픈 오류");
    
    fscanf(fp, "%d %d\n", &i, &j);
}

char creatMap (void) {
    // 새로운 맵을 만들 수 있도록 입력을 받고 이를 배열에 담아 map을 호출하여 맴을 완성
}

void map (void) {
    // 배열을 풀어 각 위치에 표시를 하고 맵을 생성
}



char arr3[3010][3010];

int main()
{
    int input;
    printf("<게임의 시작 모드를 입력해주세요>\n\n");
    printf("    1. 게임파일 불러오기\n");
    printf("    2. 게임 시작지점 지정하기\n\n\n>> ");
    scanf("%d", &input);
    
    // 이 뒤에 사용자가 입력한 게임 모드에 따라 맵이 저장된 파일을 불러오거나, 새로운 맵을 지정해서 게임을 시작할 수 있도록 구현.
    
    system("mode con cols=100 lines=100");
    int N = 60;
    int M = 60;

    arr3[3][10] = 'O';
    arr3[3][11] = 'O';
    arr3[3][12] = 'O';
    arr3[2][11] = 'O';
    arr3[4][12] = 'O';

    


    /*for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if (arr3[i][j] == 'O')
                printf("%d %d = %d\n", i, j, arr3[i][j]);
        }
        printf("\n");
    }
    Sleep(100000);*/

    slp(N, M, 10000, 1, arr3);
}

