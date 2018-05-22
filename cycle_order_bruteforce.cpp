
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
const int n = 5;
const int s = 0, t = 1;
int m, T[n][n], maxcnt = -INF, minnokmax = INF, ncycleorder;
bool A[n][n];
vector<pair<int, int>> E;
vector<int> O[n];
vector<bool> C;
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
			printf("%d - %d\n", E[i].x, E[i].y);
}

void go(pair<int, int> &p)
{
	p = A[p.x][p.y] ? pair<int, int>(p.y, T[p.y][p.x]) : pair<int, int>(p.x, T[p.x][p.y]);
}

void sol(int u)
{
	if (u == n)
	{
		//printf("u=%d n=%d m=%d\n", u, n, m);
		for (int f : O[s])
		{
			//printf("s=%d f=%d\n", s, f);
			int cnt = 0;
			int okmin = INF, nokmin = INF;
			int okmax = -INF, nokmax = -INF;
			int ok = INF, nok = INF;
			dp.push_back(vector<bool>(1 << m));
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
					dp[ncycleorder][b] = 1;
				}
				else
				{
					nokmin = min(nokmin, ms);
					nokmax = max(nokmax, ms);
					nok = b;
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
				printf("~~%d %d   %d %d\n", okmin, okmax, nokmin, nokmax);
				if (maxcnt < 319)
					continue;
				for (int i = 0; i < n; i++)
				{
					printf("i=%d     ", i);
					for (int j = 0; j < O[i].size(); j++)
						printf("%d ", O[i][j]);
					printf("\n");
				}
				printf("f=%d\n", f);
				printf("===========ok\n");
				print(ok);
				printf("===========not ok\n");
				print(nok);
			}
			ncycleorder++;
		}
	}
	else
	{
		do
		{
			for (int i = 0; i < O[u].size(); i++)
				T[u][O[u][i]] = O[u][(i + 1) % O[u].size()];
			sol(u + 1);
		} while (next_permutation(O[u].begin() + 1, O[u].end()));
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
			E.push_back({ i, j });
	/*
	for (int i = 0; i < n / 2; i++)
		for (int j = n / 2; j < n; j++)
			E.push_back({ i, j });
	*/

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

	for (auto e : E)
	{
		O[e.x].push_back(e.y);
		O[e.y].push_back(e.x);
	}
	for (int i = 0; i < n; i++)
		sort(O[i].begin(), O[i].end());

	sol(0);
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
