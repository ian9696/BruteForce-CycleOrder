
#include "stdafx.h"

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<cassert>
#include<ctime>
#include<algorithm>
#include<vector>
#include<stack>
#include<map>
#include<set>
#include<unordered_set>

using namespace std;

#define x first
#define y second

const int INF = (1 << 30) - 1;
const int n = 5;
const int s = 4, t = 0;
int m, ncycleorder;
vector<pair<int, int>> edg;
vector<int> E[n], F;
vector<vector<int>> O[n];
vector<bool> C;
bool A[n][n];
map<int, int> M;
vector<vector<bool>> dp;

void print(pair<int, int> &p)
{
	printf("(%d,%d)\n", p.x, p.y);
}

void print(int b)
{
	for (int i = 0; i < 20; i++)
		printf("=");
	printf("\n");
	printf("b=%d\n", b);
	for (int i = 0; i < m; i++)
		if (b & 1 << i)
			printf("%d - %d\n", edg[i].x, edg[i].y);
}

pair<int, int> go(int u, vector<int> &V)
{
	for (int v : V)
		if (A[u][v])
			return pair<int, int>(u, v);
	return{ -1, -1 };
}

void sol()
{
	int cnt = 0;
	for (int b = (1 << m) - 1; b > 0; b--)
	{
		if (!C[b])
			continue;

		for (int i = 0; i < m; i++)
			A[edg[i].x][edg[i].y] = A[edg[i].y][edg[i].x] = b & 1 << i;

		pair<int, int> p = { s,s };
		for (unordered_set<int> S; p.y != t && S.find(p.x*n + p.y) == S.end();)
		{
			S.insert(p.x*n + p.y);
			int ep = 0;
			for (; E[p.y][ep] != p.x; ep++)
				;
			p = go(p.y, O[p.y][ep]);
			assert(p.x != -1);
		}

		if (p.y == t)
			cnt++;
		else
			print(b);
	}
	printf("cnt=%d\n", cnt);
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

bool check_order(bool restricted)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < E[i].size(); j++)
		{
			if (i == t || E[i][j] == t)
				continue;

			vector<int> vt = E[i], vt2 = O[i][j];
			sort(vt.begin(), vt.end());
			sort(vt2.begin(), vt2.end());
			if (vt != vt2)
				return 0;
			if (restricted && O[i][j].back() != E[i][j])
				return 0;
		}
	}
	return 1;
}

int main()
{
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (!(i == s && j == t) && !(i == t && j == s))
				edg.push_back({ i, j });
	edg.push_back({ s, s });

	m = edg.size();
	for (int i = 0; i < m; i++)
		A[edg[i].x][edg[i].y] = A[edg[i].y][edg[i].x] = 1;

	C.assign(1 << m, 0);
	int nconself = 0;
	for (int b = 0; b < 1 << m; b++)
	{
		if (!(b & 1 << m - 1))
			continue;
		DS ds(n);
		for (int i = 0; i < m; i++)
			if (b & 1 << i)
				ds.join(edg[i].x, edg[i].y);
		if (ds.find(s) == ds.find(t))
			C[b] = 1, nconself++;
	}
	printf("nconself/nsubgraph=%d/%d\n", nconself, 1 << m);

	for (auto e : edg)
	{
		E[e.x].push_back(e.y);
		if (e.x != e.y)
			E[e.y].push_back(e.x);
	}

	map<int, map<int, vector<int>>> M;

	M[s][s] = { 1, 2, 3, s };

	M[s][1] = { t, 2, 3, s };
	M[s][2] = { t, 1 ,3 ,s };
	M[s][3] = { t, 1, 2, s };

	M[1][s] = { 2, 3, s, 1 };
	M[2][s] = { 3, 1, s, 2 };
	M[3][s] = { 2, 1, s, 3 };

	M[1][2] = { t, 3, s, 1 };
	M[2][1] = { t, 3, s, 2 };
	M[1][3] = { t, 2, s, 1 };
	M[3][1] = { t, 2, s, 3 };
	M[2][3] = { t, 1, s, 2 };
	M[3][2] = { t, 1, s, 3 };

	for (int i = 0; i < n; i++)
	{
		O[i].assign(E[i].size(), {});
		for (int j = 0; j < E[i].size(); j++)
		{
			if (i == t || E[i][j] == t)
				continue;
			O[i][j] = M[E[i][j]][i];
			//	O[i][j].push_back(t);
			//	if (i <= E[i][j])
			//	{
			//		for (int k = 0; k < n; k++)
			//			if (k != t && k != E[i][j] && A[i][k])
			//				O[i][j].push_back(k);
			//	}
			//	else
			//	{
			//		for (int k = E[i][j] + 1; k < n; k++)
			//			if (k != t && k != E[i][j] && A[i][k])
			//				O[i][j].push_back(k);
			//		for (int k = 0; k < E[i][j]; k++)
			//			if (k != t && k != E[i][j] && A[i][k])
			//				O[i][j].push_back(k);
			//	}
			//	O[i][j].push_back(E[i][j]);
		}
	}

	if (!check_order(0))
	{
		printf("wrong grouptalbe order format!!!\n");
		return 0;
	}

	sol();
}

/*

*/
