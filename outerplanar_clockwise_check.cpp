
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<vector>
#include<stack>
#include<map>
#include<set>

using namespace std;

#define x first
#define y second

const int INF = (1 << 30) - 1;
const int n = 15;
const int s = 3, t = 5;
int m, T[n][n], f;
bool A[n][n];
vector<pair<int, int>> E;
vector<int> O[n];
vector<bool> C, dp;

void print(pair<int, int> &p)
{
	printf("(%d,%d)\n", p.x, p.y);
}

void print(int b)
{
	for (int i = 0; i < m; i++)
		if (b & 1 << i)
			printf("%d - %d\n", E[i].x, E[i].y);
}

void go(pair<int, int> &p)
{
	p = A[p.x][p.y] ? pair<int, int>(p.y, T[p.y][p.x]) : pair<int, int>(p.x, T[p.x][p.y]);
}

void sol()
{
	//printf("u=%d n=%d m=%d\n", u, n, m);
		//printf("s=%d f=%d\n", s, f);
	int cnt = 0;
	int okmin = INF, nokmin = INF;
	int okmax = -INF, nokmax = -INF;
	int ok = INF, nok = INF;
	dp.assign(1 << m, 0);
	for (int b = (1 << m) - 1; b > 0; b--)
	{
		if (!C[b])
			continue;
		//printf("b=%d\n", b);
		int ms = 0;
		for (int i = 0; i < m; i++)
		{
			A[E[i].x][E[i].y] = A[E[i].y][E[i].x] = b & 1 << i;
			if (b & 1 << i)
				ms++;
			//	printf("%d - %d\n", E[i].x, E[i].y);
		}

		pair<int, int> p(s, f), start = p;
		for (go(p); p != start && p.x != t; go(p))
			;// print(p), printf("t=%d\n", t);

		if (p.x == t)
		{
			cnt++;
			okmin = min(okmin, ms);
			okmax = max(okmax, ms);
			ok = b;
			dp[b] = 1;
		}
		else
		{
			nokmin = min(nokmin, ms);
			nokmax = max(nokmax, ms);
			nok = b;
		}
	}
	//printf("cnt=%d\n", cnt);
	//if (nokmax < minnokmax)
	//{
	//	minnokmax = nokmax;
	//	printf("update minnokmax=%d\n", minnokmax);
	//}
	printf("cnt=%d\n", cnt);
	printf("~~%d %d   %d %d\n", okmin, okmax, nokmin, nokmax);
}

struct DS
{
	vector<int> a;
	DS(int n) :a(n, -1) {}
	int find(int x)
	{
		return a[x] == -1 ? x : a[x] = find(a[x]);
	}
	void join(int x, int y)
	{
		x = find(x);
		y = find(y);
		if (x != y)
			a[x] = y;
	}
};

int main()
{
	/*
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			E.push_back({ i, j });
	for (int i = 0; i < n / 2; i++)
	for (int j = n / 2; j < n; j++)
	E.push_back({ i, j });
	*/
	vector<int> tmp = { 0, 1, 0, 4, 0, 5, 1, 2, 1, 4, 1, 3, 2, 3, 3, 4, 3, 13, 3, 6, 3, 12, 3, 7, 3, 8, 3, 9, 4, 5, 6, 11, 7, 12, 8, 9, 8, 10, 9, 14, 9, 10, 10, 14, 11, 12 };
	for (int i = 0; i < tmp.size(); i += 2)
		E.push_back({ tmp[i], tmp[i + 1] });
	O[0] = { 1, 4, 5 };
	O[1] = { 2, 3, 4, 0 };
	O[2] = { 3, 1 };
	O[3] = { 13, 6, 12, 7, 9, 8, 4, 1, 2 };
	O[4] = { 5, 0, 1, 3 };
	O[5] = { 0, 4 };
	O[6] = { 11, 3 };
	O[7] = { 3, 12 };
	O[8] = { 3, 9, 10 };
	O[9] = { 14, 10, 8, 3 };
	O[10] = { 8, 9, 14 };
	O[11] = { 12, 6 };
	O[12] = { 7, 3, 11 };
	O[13] = { 3 };
	O[14] = { 10, 9 };
	f = 2;

	m = E.size();
	C.assign(1 << m, 0);

	int ncon = 0;
	for (int b = 0; b < 1 << m; b++)
	{
		DS ds(n);
		for (int i = 0; i < m; i++)
			if (b & 1 << i)
				ds.join(E[i].x, E[i].y);
		if (ds.find(s) == ds.find(t))
			C[b] = 1, ncon++;
	}
	printf("ncongraph/ngraph=%d/%d\n", ncon, 1 << m);

	for (int u = 0; u < n; u++)
		for (int i = 0; i < O[u].size(); i++)
			T[u][O[u][i]] = O[u][(i + 1) % O[u].size()];

	sol();

	int minms = INF;
	for (int b = 0; b < 1 << m; b++)
	{
		if (!C[b])
			continue;
		for (int i = 0; i < m; i++)
			if (b & 1 << i && C[b ^ 1 << i])
				dp[b] = dp[b] && dp[b ^ 1 << i];
		if (dp[b])
			continue;
		int ms = 0;
		for (int i = 0; i < m; i++)
			if (b & 1 << i)
				ms++;
		if (ms < minms)
		{
			minms = ms;
			printf("update minms=%d\n", minms);
			print(b);
		}
	}
}
