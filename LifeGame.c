#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#pragma warning(disable:4996)
int spd=100;
void clearscreen()
{
	HANDLE hOut;
	COORD Pos;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Pos.X = 0;
	Pos.Y = 0;
	SetConsoleCursorPosition(hOut, Pos);
}
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
			if (board[i][j].on2)
			{
				memset(board[i][j].c, 0, sizeof(board[i][j].c));
				board[i][j].on2 = 0;
			}

		}
	}
	int c = 0;
	for (i = 0; i <= (N >> 5); i++)
	{
		for (j = 0; j <= (M >> 5); j++)
		{
			if (board[i][j].on == 0)
			{
				continue;
			}
			c++;
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
	set(N, M, brd);
	int i;
	for (i = 0; i < c; i++)
	{
		proc(N, M);
		proc2(N, M);

		if (pr) {//출력 형식 수정하고 싶으면 여기를 수정
			upd(N, M, brd);
			clearscreen();
			int j;
			printf("%d\n", i);
			for (j = 0; j < N; j++)
			{
				printf("%3d %s\n", j, brd + (j * (M + 1)));
			}
			Sleep(spd);
		}
		else
		{
			if (i % 100 == 0)
			{
				printf("%d 턴 완료\n", i + 1);
			}
		}
		if (_kbhit())
		{
			int v = getch();
			if (v == 'q' || v=='Q')
				break;
		}
	}
	upd(N, M, brd);
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
	cvt(N, M, arr, x);
	rep(N, M, c, x, p);
	cvt2(N, M, arr, x);
}
char arr3[3010][3010];
void inst()
{
	printf("동작 선택\n");
	printf("1 : 새로 만들기\n");
	printf("2 : 불러오기\n");
	printf("3 : 저장하기\n");
	printf("4 : 한 칸 상태 바꾸기\n");
	printf("5 : 직사각형 부분 상태 바꾸기\n");
	printf("6 : 턴 진행하기\n");
	printf("7 : 크기 확장\n");
	printf("8 : 현재 상태 출력\n");
	printf("9 : 화면 클리어\n");
	printf("0 : 종료\n");
	printf("S : 속도 조절\n");
	printf("H : 도움말\n");
}
int store(int N, int M, char(*arr)[3010], char name[])
{
	FILE* fp = NULL;
	fp = fopen(name, "w");
	if (fp == NULL)
	{
		printf("파일 오픈 오류");
		return 1;
	}
	fprintf(fp, "%d %d\n", N, M);
	int i, j;
	static char brr[3010][510];
	memset(brr, 33, sizeof(brr));
	for (i = 0; i < N; i++)
	{
		int j;
		for (j = 0; j < M; j++)
		{
			brr[i][j / 6] += (arr[i][j] == 'O') << (j % 6);
		}
		brr[i][(M - 1) / 6+1] = '\0';
		fprintf(fp, "%s\n", brr[i]);
	}
	fclose(fp);
	return 0;
}
int load(int* N, int* M, char(*arr)[3010], char name[])
{
	FILE* fp = NULL;
	fp = fopen(name, "r");
	if (fp == NULL)
	{
		printf("파일 오픈 오류");
		return 1;
	}
	static char brr[3010][510];
	fscanf(fp, "%d %d", N, M);
	int i;
	for (i = 0; i < *N; i++)
	{
		fscanf(fp, "%s", brr[i]);
		int j;
		for (j = 0; j < (*M+5)/6; j++)
		{
			brr[i][j] -= 33;
			int k;
			for (k = 0; k < 6; k++)
			{
				if (j * 6 + k >= *M)
					break;
				arr[i][j * 6 + k] = ((brr[i][j]) & (1 << k)) ? 'O' : '.';
			}
		}
	}
	return 0;
}
int main()
{
	char filename[100] = "";
	int N, M;
	inst();
	while (1)
	{
		printf("명령을 선택하시오\n");
		char arr[100];
		scanf("%s", arr);
		int s = strlen(arr);
		if (s != 1)
		{
			printf("잘못된 명령입니다\n");
			continue;
		}
		else {
			switch (arr[0]) {
			case '1':
				printf("크기를 정하시오\n");
				printf("세로 크기 : ");
				scanf("%d", &N);
				printf("가로 크기 : ");
				scanf("%d", &M);
				printf("파일 이름을 정하시오\n");
				printf("파일 이름 : ");
				scanf("%s", filename);
				printf("텍스트를 불러올까요? (Y/N) ");
				char buff[100] = "";
				scanf("%s", buff);
				int inp = 0;
				if (buff[0] == 'Y' || buff[0] == 'y') {
					inp = 1;
				}
				memset(arr3, 0, sizeof(arr3));
				int i;
				for (i = 0; i < N; i++)
				{
					int j;
					for (j = 0; j < M; j++)
					{
						arr3[i][j] = '.';
					}
					if (inp)
					{
						scanf("%s", arr3[i]);
					}
				}
				if (store(N, M, arr3, filename))
					printf("파일 생성 실패\n");
				else
					printf("파일 생성 성공\n");
				break;
			case '2':
				printf("파일 이름 : ");
				scanf("%s", filename);
				memset(arr3, 0, sizeof(arr3));
				if (load(&N, &M, arr3, filename))
					printf("로드 실패\n");
				else
					printf("로드 성공\n");
				break;
			case '3':
				if (strlen(filename) == 0)
				{
					printf("로드된 파일이 없습니다\n");
					break;
				}
				printf("다른 이름으로 저장? (Y/N) :");
				scanf("%s", buff);
				if (buff[0] == 'Y' || buff[0] == 'y') {
					printf("파일 이름을 정하시오\n");
					printf("파일 이름 : ");
					scanf("%s", filename);
				}
				if (store(N, M, arr3, filename))
					printf("저장 실패\n");
				else
					printf("저장 성공\n");
				break;
			case '4':
				if (strlen(filename) == 0)
				{
					printf("로드된 파일이 없습니다\n");
					break;
				}
				printf("바꿀 위치의 행(0부터 시작) : ");
				int x, y;
				scanf("%d", &x);
				printf("바꿀 위치의 열(0부터 시작) : ");
				scanf("%d", &y);
				if (x < 0 || y < 0 || x >= N || y >= M) {
					printf("잘못된 위치입니다\n");
					break;
				}
				printf("현재 %d, %d는 %s 있습니다\n", x, y, (arr3[x][y] == '.') ? "죽어" : "살아");
				printf("바꾸시겠습니까? (Y/N) ");
				scanf("%s", buff);
				if (buff[0] == 'Y' || buff[0] == 'y') {
					printf("바꿨습니다\n");
					if (arr3[x][y] == '.')
						arr3[x][y] = 'O';
					else
						arr3[x][y] = '.';
				}
				else
				{
					printf("취소했습니다\n");
				}
				break;
			case '5':
				if (strlen(filename) == 0)
				{
					printf("로드된 파일이 없습니다\n");
					break;
				}
				printf("바꿀 직사각형 왼쪽 위 꼭짓점의 행(0부터 시작) : ");
				scanf("%d", &x);
				printf("바꿀 직사각형 왼쪽 위 꼭짓점의 열(0부터 시작) : ");
				scanf("%d", &y);
				int h, w;
				printf("바꿀 직사각형 세로 높이 : ");
				scanf("%d", &h);
				printf("바꿀 직사각형 가로 높이 : ");
				scanf("%d", &w);
				if (x < 0 || y < 0 || x + h - 1 >= N || y + w - 1 >= M) {
					printf("잘못된 위치입니다\n");
					break;
				}
				printf("이 부분을 대체할 모양을 입력하시오\n");
				static char arr[3010][3010];
				for (i = 0; i < h; i++)
				{
					scanf("%s", arr[i]);
				}
				for (i = 0; i < h; i++)
				{
					if (strlen(arr[i]) != w)
					{
						printf("크기가 잘못되었습니다\n");
						break;
					}
				}
				if (i == h)
				{
					for (i = 0; i < h; i++)
					{
						int j;
						for (j = 0; j < w; j++)
						{
							arr3[i + x][j + y] = arr[i][j];
						}
					}
					printf("바꿨습니다\n");
				}
				break;
			case '6':
				if (strlen(filename) == 0)
				{
					printf("로드된 파일이 없습니다\n");
					break;
				}
				printf("진행할 턴수 입력 : ");
				int c;
				scanf("%d", &c);
				int p = 0;
				if (N <= 100 && M <= 100) {
					printf("매 턴마다 콘솔에 출력할까요? (Y/N) ");
					char buff[100] = "";
					scanf("%s", buff);
					if (buff[0] == 'Y' || buff[0] == 'y') {
						p = 1;
					}
				}
				slp(N, M, c, p, arr3);
				break;
			case '7':
				printf("크기를 정하시오\n");
				printf("세로 크기 : ");
				scanf("%d", &N);
				printf("가로 크기 : ");
				scanf("%d", &M);
				for (i = 0; i < N; i++)
				{
					int j;
					for (j = 0; j < M; j++)
					{
						if (arr3[i][j] == '\0')
							arr3[i][j] = '.';
					}
				}
				break;
			case '8':
				if (strlen(filename) == 0)
				{
					printf("로드된 파일이 없습니다\n");
					break;
				}
				for (i = 0; i < N; i++)
				{
					printf("%3d %s\n", i, arr3[i]);
				}
				break;
			case '9':
				system("cls");
				break;
			case '0':
				return 0;
			case 'H':
			case 'h':
				inst();
				break;
			case 'S':
			case 's':
				printf("턴당 시간을 밀리초 단위로 입력 : ");
				scanf("%d", &spd);
				break;
			}
		}
	}
}
