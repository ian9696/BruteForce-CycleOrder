
#include "stdafx.h"

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
#include<unordered_set>

using namespace std;

#define x first
#define y second

const int INF = (1 << 30) - 1;
const int n = 5;
const int s = 0, t = 1;
int m, maxcnt = -1, minnokmax = INF, ncycleorder;
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

void sol(int u, int ep)
{
	if (u == n + 1)
	{
		if (ncycleorder % 50000 == 0)
			printf("!@#ncycleorder=%d maxcnt=%d\n", ncycleorder, maxcnt);
		//printf("u=%d n=%d m=%d\n", u, n, m);
			//printf("s=%d f=%d\n", s, f);
		int cnt = 0;
		int okmin = INF, nokmin = INF;
		int okmax = -1, nokmax = -1;
		int ok = -1, nok = -1;
		dp.push_back(vector<bool>(1 << m));
		for (int b = (1 << m) - 1; b > 0; b--)
		{
			if (!C[b])
				continue;
			//printf("b=%d\n", b);
			int ms = 0;
			for (int i = 0; i < m; i++)
			{
				A[edg[i].x][edg[i].y] = A[edg[i].y][edg[i].x] = b & 1 << i;
				if (b & 1 << i)
					ms++;
				//	printf("%d - %d\n", E[i].x, E[i].y);
			}

			pair<int, int> p = go(s, F);
			assert(p.x != -1);
			//printf("=========\n");
			//print(p);
			for (unordered_set<int> S; p.y != t && S.find(p.x*n + p.y) == S.end();)
			{
				S.insert(p.x*n + p.y);
				for (ep = 0; E[p.y][ep] != p.x; ep++)
					;
				p = go(p.y, O[p.y][ep]);
				assert(p.x != -1);
				//print(p);
			}

			if (p.y == t)
			{
				cnt++;
				if (ms < okmin)
					ok = b;
				okmin = min(okmin, ms);
				okmax = max(okmax, ms);
				dp[ncycleorder][b] = 1;
			}
			else
			{
				if (ms < nokmin)
					nok = b;
				nokmin = min(nokmin, ms);
				nokmax = max(nokmax, ms);
			}
		}
		M[cnt]++;
		//printf("cnt=%d\n", cnt);
		//if (nokmax < minnokmax)
		//{
		//	minnokmax = nokmax;
		//	printf("update minnokmax=%d\n", minnokmax);
		//}
		if (cnt > maxcnt)
		{
			maxcnt = cnt;
			printf("update maxcnt=%d\n", maxcnt);
			printf("~ok~ %d %d   ~nok~ %d %d\n", okmin, okmax, nokmin, nokmax);

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < E[i].size(); j++)
				{
					printf("i=%d   from %d      ", i, E[i][j]);
					for (int v : O[i][j])
						printf("%d ", v);
					printf("\n");
				}
			}
			printf("F      ");
			for (int v : F)
				printf("%d ", v);
			printf("\n");

			if (ok != -1)
			{
				printf("===========ok\n");
				print(ok);
			}
			if (nok != -1)
			{
				printf("===========not ok\n");
				print(nok);
			}
		}
		ncycleorder++;
	}
	else if (u == n)
	{
		do
		{
			sol(u + 1, -1);
		} while (next_permutation(F.begin(), F.end()));
	}
	else if (ep >= E[u].size())
		sol(u + 1, 0);
	else if (O[u][ep].size() <= 1)
		sol(u, ep + 1);
	else
	{
		do
		{
			//random_shuffle(O[u][ep].begin(), O[u][ep].end());
			sol(u, ep + 1);
		} while (next_permutation(O[u][ep].begin(), O[u][ep].end()));
	}
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
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			edg.push_back({ i, j });
	/*
	for (int i = 0; i < n / 2; i++)
		for (int j = n / 2; j < n; j++)
			E.push_back({ i, j });
	*/

	m = edg.size();
	C.assign(1 << m, 0);

	int ncon = 0;
	for (int b = 0; b < 1 << m; b++)
	{
		DS ds(n);
		for (int i = 0; i < m; i++)
			if (b & 1 << i)
				ds.join(edg[i].x, edg[i].y);
		if (ds.find(s) == ds.find(t))
			C[b] = 1, ncon++;
	}
	printf("ncongraph/ngraph=%d/%d\n", ncon, 1 << m);

	for (auto e : edg)
	{
		E[e.x].push_back(e.y);
		E[e.y].push_back(e.x);
	}
	for (int i = 0; i < n; i++)
	{
		sort(E[i].begin(), E[i].end());
		O[i].assign(E[i].size(), E[i]);
		for (int j = 0; j < E[i].size(); j++)
		{
			if (i == t || E[i][j] == t)
				O[i][j].clear();
			//else
			//{
			//	for (int k = 0; k < O[i][j].size(); k++)
			//		if (O[i][j][k] == E[i][j])
			//		{
			//			swap(O[i][j][k], O[i][j].back());
			//			break;
			//		}
			//	sort(O[i][j].begin(), O[i][j].end() - 1);
			//}
		}
	}
	F = E[s];

	sol(0, 0);
	printf("ncycleorder=%d\n", ncycleorder);

	for (int p = 0, block = max(1, ncycleorder / 10); !M.empty(); )
	{
		int sum = 0, cnt = 0;
		while (!M.empty() && cnt < block)
		{
			auto it = M.begin();
			int t = min(block - cnt, it->second);
			sum += t * it->first;
			cnt += t;
			it->second -= t;
			if (M.begin()->second == 0)
				M.erase(M.begin());
		}
		printf("cycleorder %5d-%5d   total=%9d   average=%.3f\n", p + 1, p + cnt, sum, (double)sum / cnt);
		p += cnt;
	}

	int minms = INF;
	for (int b = 0; b < 1 << m; b++)
	{
		if (!C[b])
			continue;
		for (int j = 0; j < ncycleorder; j++)
			for (int i = 0; i < m; i++)
				if (b & 1 << i && C[b ^ 1 << i])
					dp[j][b] = dp[j][b] && dp[j][b ^ 1 << i];
		int cnt = 0;
		for (int j = 0; j < ncycleorder; j++)
			cnt += dp[j][b];
		if (cnt)
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

/*

*/
