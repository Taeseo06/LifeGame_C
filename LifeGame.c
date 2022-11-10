#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

typedef struct arr {
	unsigned int val[32];//val[i]�� j��° ��Ʈ: ������ ���� ���� (0,0)�̶� �� �� (i,j)�� ��� �ִ���
	unsigned int c[32][4];//val[i]�� j*4~j*4+3��° ��Ʈ: ������ ���� ���� (0,0)�̶� �� �� (i,j) �ֺ��� �� ���� ���� �ִ���
	int on, on2;//�� ������ �� �ʿ䰡 �ִ���
}arr;//������ 32*32 ������ �����ϴ� ����ü
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
}//x,y�� ���¸� �ٲ�
inline void pc(int x, int y)
{
	board[x >> 5][y >> 5].c[x & 31][(y & 24) >> 3] += (1 << ((y & 7) * 4));
}//x,y�� �ֺ��� ��� �ִ� ������ ���� ī���͸� ������Ŵ
inline int gv(int x, int y)
{
	return !!(board[x >> 5][y >> 5].val[x & 31] & ((1U << (y & 31))));
}//x,y�� ���¸� ������
inline int gc(int x, int y)
{
	return (board[x >> 5][y >> 5].c[x & 31][(y & 24) >> 3] & (15 << ((y & 7) * 4))) >> (y * 4);
}//x,y�� �ֺ��� ��� �ִ� ������ ���� ����
int dx[8] = { 1,1,1,0,-1,-1,-1,0 };
int dy[8] = { 1,0,-1,-1,-1,0,1,1 };
void set(int N, int M, char* brd)//[N][M+1] ũ���� 2���� �迭�̳� N*(M+1) ũ���� 1���� �迭 �޾Ƽ� ���� ����ü �迭�� ������(i*(M+1)+M ��°�� ����� ���� �� ����)
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
void proc(int N, int M)//�ֺ��� ��� �ִ� ���� ��� ���
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
void proc2(int N, int M)//��� �ִ� ������ ������ ���� ������Ʈ
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
void upd(int N, int M, char* brd)//���� ���¸� char�� ������
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
void rep(int N, int M, int c, char* brd, int pr) {//������ K�� �ݺ�

	FILE* fp = NULL;
	fp = fopen("/Users/Sunrin/Desktop/c_project/game_file", "r+");
	if (fp == NULL)
		printf("���Ͽ��� ����");



	set(N, M, brd);
	int i;
	for (i = 0; i < c; i++)
	{
		proc(N, M);
		proc2(N, M);
		upd(N, M, brd);
		if (pr) {//��� ���� �����ϰ� ������ ���⸦ ����
			int j;
			printf("%d\n", i);
			for (j = 0; j < N; j++)
			{
				printf("%3d %s\n", j, brd + (j * (M + 1)));
			}
		}
		Sleep(300);

		
		


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
			printf("\n");
		}
		Sleep(3000000);
	}
}
void cvt(int N, int M, char(*arr)[3010], char* brd) {//[3010][3010] 2���� �迭�� 1���� �迭�� ����
	int i;
	for (i = 0; i < N; i++)
	{
		int j;
		for (j = 0; j <= M; j++) {
			brd[i * (M + 1) + j] = arr[i][j];
		}
	}
}
void cvt2(int N, int M, char(*arr)[3010], char* brd) {//[3010][3010] 2���� �迭�� 1���� �迭�� ����
	int i;
	for (i = 0; i < N; i++)
	{
		int j;
		for (j = 0; j <= M; j++) {
			arr[i][j] = brd[i * (M + 1) + j];
		}
	}
}
void slp(int N, int M, int c, int p, char(*arr)[3010]) {//ũ��, �ݺ� Ƚ��, ��� ���� [3010][3010] 2���� �迭�� �־����� 2���� �迭�� ������ n�� ����

	char* x = (char*)malloc(10000000);
	memset(x, '.', sizeof(x));
	int j;
	for (j = 0; j < N; j++)
	{
		x[j * (M + 1) + M] = '\0';


	}
	cvt(N, M, arr, x);  // 1���� �迭�� ���
	rep(N, M, c, x, p);
	cvt2(N, M, arr, x);  // 2���� �迭�� ���


}

char arr3[3010][3010];

int main()
{
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

