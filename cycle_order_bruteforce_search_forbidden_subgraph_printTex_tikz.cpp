
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
#include<bitset>

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
string K5node;
string K33s1t2node =
"            \\node[mn] (sp) {$s^{\'}$};\n"
"            \\node[mn] (s) [below right=\\d*1cm and\\d*1cm of sp] {$s$};\n"
"            \\node[mn] (t) [below=\\d*2cm of s] {$t$};\n"
"            \\node[mn] (A) [below=\\d*2cm of t] {$A$};\n"
"            \\node[mn] (B) [right=\\d*2cm of s] {$B$};\n"
"            \\node[mn] (C) [below=\\d*2cm of B] {$C$};\n"
"            \\node[mn] (D) [below=\\d*2cm of C] {$D$};\n";
string K33s1t4node =
"            \\node[mn] (sp) {$s^{\'}$};\n"
"            \\node[mn] (s) [below right=\\d*1cm and\\d*1cm of sp] {$s$};\n"
"            \\node[mn] (A) [below=\\d*2cm of s] {$A$};\n"
"            \\node[mn] (B) [below=\\d*2cm of A] {$B$};\n"
"            \\node[mn] (t) [right=\\d*2cm of s] {$t$};\n"
"            \\node[mn] (C) [below=\\d*2cm of t] {$C$};\n"
"            \\node[mn] (D) [below=\\d*2cm of C] {$D$};\n";
map<int, string> K5map = { { 0,"s" },{ 1,"t" },{ 2,"A" },{ 3,"B" },{ 4,"C" } };
map<int, string> K33s1t2map = { { 0,"s" },{ 1,"t" },{ 2,"A" },{ 3,"B" },{ 4,"C" },{ 5,"D" } };
map<int, string> K33s1t4map = { { 0,"s" },{ 1,"A" },{ 2,"B" },{ 3,"t" },{ 4,"C" },{ 5,"D" } };


void print(pair<int, int> &p)
{
	printf("(%d,%d)\n", p.x, p.y);
}

void print(int b)
{
	if (b == INF || b == -INF)
	{
		printf("n/a\n");
		return;
	}
	for (int i = 0; i < m; i++)
		if (b & 1 << i)
			printf("%d - %d\n", E[i].x, E[i].y);
}

void printTex(vector<int> v)
{
	if (K5node == "")
	{
		for (int i = 0; i < 5; i++)
		{
			string label = string(1, i == 0 ? 's' : i == 1 ? 't' : 'A' + i - 2);
			K5node +=
				"            \\node[mn] (" + label + ") at ("
				+ to_string(90 + 72 - i * 72) + ":\\d*3 cm) {$" + label + "$};\n";
		}
		K5node += "            \\node[mn] (sp) [above left=\\d*0.8cm and \\d*0.8cm of s] {$s^{\'}$};\n";
	}
	string str =
		"\\begin{figure}[ht!]\n"
		"    \\centering\n";
	printf("%s", str.c_str());

	for (int j = 0; j < v.size(); j++)
	{
		printf("%s", j == 0 ? "" : "    \\hfill\n");
		str =
			"    \\begin{subfigure}[b]{0.3\\textwidth}\n"
			"        \\centering\n"
			"        \\begin{tikzpicture}\n"
			+ K5node +
			"            \\path[mp]\n";
		printf("%s", str.c_str());
		printf("            (sp) edge node {} (s)\n");
		for (int i = 0, p = 0; i < m; i++)
			if (v[j] & 1 << i)
				printf("            (%s) edge node {} (%s)\n", K5map[E[i].x].c_str(), K5map[E[i].y].c_str());

		str =
			"            ;\n"
			"        \\end{tikzpicture}\n"
			"        \\captionsetup{justification=centering}\n"
			"        \\caption{$G_" + string(1, 'a' + j) + "$}\n"
			"        \\label{fig:K5G" + string(1, 'a' + j) + "}\n"
			"    \\end{subfigure}\n";
		printf("%s", str.c_str());
	}
	str =
		"    \\captionsetup{justification=centering}\n"
		"    \\caption{Three $s$-$t$ connected subgraphs of $K_5$}\n"
		"    \\label{fig:K5}\n"
		"\\end{figure}\n";
	printf("%s", str.c_str());
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
					dp.back()[b] = 1;
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
				if (maxcnt < 10000)
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
	freopen("out.txt", "w", stdout);

	for (int i = 0; i < 5; i++)
		for (int j = i + 1; j < 5; j++)
			E.push_back({ i, j });

	//for (int i = 0; i < 3; i++)
	//	for (int j = 3; j < 6; j++)
	//		E.push_back({ i, j });


	m = E.size();
	assert(m <= 30);
	C.assign(1 << m, 0);
	printf("n=%d m=%d\n", n, m);

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
	ncycleorder = dp.size();
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

	for (int a = 0; a < 1 << m; a++)
		for (int b = a + 1; b < 1 << m; b++)
			for (int c = b + 1; c < 1 << m; c++)
				//for (int c = b; c <= b; c++)
			{
				if (!C[a] || !C[b] || !C[c])
					continue;
				bool counter = true;
				for (int i = 0; i < ncycleorder; i++)
					if (dp[i][a] && dp[i][b] && dp[i][c])
					{
						counter = false;
						break;
					}
				if (!counter)
					continue;
				//printTex({ a, b, c });
				//printf("a:\n");
				//print(a);
				//printf("b:\n");
				//print(b);
				//printf("c:\n");
				//print(c);
				int cnta = 0, cntb = 0, cntc = 0;
				for (int i = 0; i < m; i++)
				{
					cnta += a >> i & 1;
					cntb += b >> i & 1;
					cntc += c >> i & 1;
				}
				if (cnta + cntb + cntc > 18)
					continue;
				//printf("\n\n\n\n\n\nzzzzzzzzzzzzz============================\n\n\n");
				printf("a b c: %d %d %d\n", a, b, c);
				printf("~~~~~~~~~~~~~~~~~~~~%d      %d %d %d\n\n\n", cnta + cntb + cntc, cnta, cntb, cntc);
				printTex({ a, b, c });
				//printTex({ a, b });
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
