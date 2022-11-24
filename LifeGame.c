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
	set(N, M, brd);
	int i;
	for (i = 0; i < c; i++)
	{
		proc(N, M);
		proc2(N, M);

		if (pr) {//��� ���� �����ϰ� ������ ���⸦ ����
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
				printf("%d �� �Ϸ�\n", i + 1);
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
	cvt(N, M, arr, x);
	rep(N, M, c, x, p);
	cvt2(N, M, arr, x);
}
char arr3[3010][3010];
void inst()
{
	printf("���� ����\n");
	printf("1 : ���� �����\n");
	printf("2 : �ҷ�����\n");
	printf("3 : �����ϱ�\n");
	printf("4 : �� ĭ ���� �ٲٱ�\n");
	printf("5 : ���簢�� �κ� ���� �ٲٱ�\n");
	printf("6 : �� �����ϱ�\n");
	printf("7 : ũ�� Ȯ��\n");
	printf("8 : ���� ���� ���\n");
	printf("9 : ȭ�� Ŭ����\n");
	printf("0 : ����\n");
	printf("S : �ӵ� ����\n");
	printf("H : ����\n");
}
int store(int N, int M, char(*arr)[3010], char name[])
{
	FILE* fp = NULL;
	fp = fopen(name, "w");
	if (fp == NULL)
	{
		printf("���� ���� ����");
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
		printf("���� ���� ����");
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
		printf("������ �����Ͻÿ�\n");
		char arr[100];
		scanf("%s", arr);
		int s = strlen(arr);
		if (s != 1)
		{
			printf("�߸��� �����Դϴ�\n");
			continue;
		}
		else {
			switch (arr[0]) {
			case '1':
				printf("ũ�⸦ ���Ͻÿ�\n");
				printf("���� ũ�� : ");
				scanf("%d", &N);
				printf("���� ũ�� : ");
				scanf("%d", &M);
				printf("���� �̸��� ���Ͻÿ�\n");
				printf("���� �̸� : ");
				scanf("%s", filename);
				printf("�ؽ�Ʈ�� �ҷ��ñ��? (Y/N) ");
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
					printf("���� ���� ����\n");
				else
					printf("���� ���� ����\n");
				break;
			case '2':
				printf("���� �̸� : ");
				scanf("%s", filename);
				memset(arr3, 0, sizeof(arr3));
				if (load(&N, &M, arr3, filename))
					printf("�ε� ����\n");
				else
					printf("�ε� ����\n");
				break;
			case '3':
				if (strlen(filename) == 0)
				{
					printf("�ε�� ������ �����ϴ�\n");
					break;
				}
				printf("�ٸ� �̸����� ����? (Y/N) :");
				scanf("%s", buff);
				if (buff[0] == 'Y' || buff[0] == 'y') {
					printf("���� �̸��� ���Ͻÿ�\n");
					printf("���� �̸� : ");
					scanf("%s", filename);
				}
				if (store(N, M, arr3, filename))
					printf("���� ����\n");
				else
					printf("���� ����\n");
				break;
			case '4':
				if (strlen(filename) == 0)
				{
					printf("�ε�� ������ �����ϴ�\n");
					break;
				}
				printf("�ٲ� ��ġ�� ��(0���� ����) : ");
				int x, y;
				scanf("%d", &x);
				printf("�ٲ� ��ġ�� ��(0���� ����) : ");
				scanf("%d", &y);
				if (x < 0 || y < 0 || x >= N || y >= M) {
					printf("�߸��� ��ġ�Դϴ�\n");
					break;
				}
				printf("���� %d, %d�� %s �ֽ��ϴ�\n", x, y, (arr3[x][y] == '.') ? "�׾�" : "���");
				printf("�ٲٽðڽ��ϱ�? (Y/N) ");
				scanf("%s", buff);
				if (buff[0] == 'Y' || buff[0] == 'y') {
					printf("�ٲ���ϴ�\n");
					if (arr3[x][y] == '.')
						arr3[x][y] = 'O';
					else
						arr3[x][y] = '.';
				}
				else
				{
					printf("����߽��ϴ�\n");
				}
				break;
			case '5':
				if (strlen(filename) == 0)
				{
					printf("�ε�� ������ �����ϴ�\n");
					break;
				}
				printf("�ٲ� ���簢�� ���� �� �������� ��(0���� ����) : ");
				scanf("%d", &x);
				printf("�ٲ� ���簢�� ���� �� �������� ��(0���� ����) : ");
				scanf("%d", &y);
				int h, w;
				printf("�ٲ� ���簢�� ���� ���� : ");
				scanf("%d", &h);
				printf("�ٲ� ���簢�� ���� ���� : ");
				scanf("%d", &w);
				if (x < 0 || y < 0 || x + h - 1 >= N || y + w - 1 >= M) {
					printf("�߸��� ��ġ�Դϴ�\n");
					break;
				}
				printf("�� �κ��� ��ü�� ����� �Է��Ͻÿ�\n");
				static char arr[3010][3010];
				for (i = 0; i < h; i++)
				{
					scanf("%s", arr[i]);
				}
				for (i = 0; i < h; i++)
				{
					if (strlen(arr[i]) != w)
					{
						printf("ũ�Ⱑ �߸��Ǿ����ϴ�\n");
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
					printf("�ٲ���ϴ�\n");
				}
				break;
			case '6':
				if (strlen(filename) == 0)
				{
					printf("�ε�� ������ �����ϴ�\n");
					break;
				}
				printf("������ �ϼ� �Է� : ");
				int c;
				scanf("%d", &c);
				int p = 0;
				if (N <= 100 && M <= 100) {
					printf("�� �ϸ��� �ֿܼ� ����ұ��? (Y/N) ");
					char buff[100] = "";
					scanf("%s", buff);
					if (buff[0] == 'Y' || buff[0] == 'y') {
						p = 1;
					}
				}
				slp(N, M, c, p, arr3);
				break;
			case '7':
				printf("ũ�⸦ ���Ͻÿ�\n");
				printf("���� ũ�� : ");
				scanf("%d", &N);
				printf("���� ũ�� : ");
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
					printf("�ε�� ������ �����ϴ�\n");
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
				printf("�ϴ� �ð��� �и��� ������ �Է� : ");
				scanf("%d", &spd);
				break;
			}
		}
	}
}
