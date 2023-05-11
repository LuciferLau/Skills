#include <cmath>
#include <ctime>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

#define VEC_SIZE 10
#define INC_FUNC inc
#define DEC_FUNC dec
#define CHAR_PIX 2

typedef std::function<bool(int,int)> SORT_FUNC;
typedef std::vector<int> VEC_INT;
typedef const std::vector<int> CVEC_INT;

static int steps = 0;

static bool inc(int a, int b)
{
	return a > b;
}

static bool dec(int a, int b)
{
	return a < b;
}

namespace heap {
using std::cout;
using std::swap;
void down(VEC_INT &v, SORT_FUNC f, int beg, int end)
{
	int parent = beg;
	int child = beg * 2 + 1;
	while (child <= end)
	{
		steps++;
		if (child + 1 <= end && f(v[child + 1], v[child]))
		{
			++child; // pick the other child
		}
		if (f(v[parent], v[child]))
		{
			return; // parent is the best
		}
		else // child is better
		{
			swap(v[parent], v[child]);
			parent = child;
			child = parent * 2 + 1;
		}
	}
}

void sort(VEC_INT &v, SORT_FUNC f)
{
	auto sz = v.size();
	int max = sz - 1;
	int first = max * 0.5 - 1;
	// 从第一个非叶子结点到顶点建堆
	for (int i = first; i >= 0; --i)
	{
		down(v, f, i, max);
	}
	printf("heapify cost steps=%d\n", steps);
	steps = 0;
	// 接下来调整叶子结点，使其完全有序
	for (int i = max; i > 0; --i)
	{
		swap(v[0], v[i]);
		down(v, f, 0, i-1);
	}
	printf("adjust cost steps=%d\n", steps);
	steps = 0;
}

void add(VEC_INT &v, SORT_FUNC f, int val)
{
	v.push_back(val);
	sort(v, f);
}

void del(VEC_INT &v, SORT_FUNC f, int idx)
{
	auto sz = v.size();
	if (idx < 0 || idx >= sz)
	{
		return;
	}
	if (idx == sz - 1)
	{
		v.pop_back();
	}
	else
	{
		swap(v[idx], v[sz - 1]);
		v.pop_back();
		sort(v, f);
	}
}

void update(VEC_INT &v, SORT_FUNC f, int idx, int val)
{
	auto sz = v.size();
	if (idx < 0 || idx >= sz)
	{
		return;
	}
	v[idx] = val;
	swap(v[idx], v[sz - 1]);
	sort(v, f);
}

void dump(CVEC_INT &v, int depth, int beg)
{
	auto sz = v.size();
	int maxdepth = (int)log2(sz);
	if (depth > maxdepth) return;
	int tabsize = maxdepth - depth;
	int scnt = CHAR_PIX * (pow(2, tabsize) - 1);
	if (scnt > 0) printf("%*s", scnt, " ");
	int nums = pow(2, depth);
	int max = beg + nums;
	for (int i=beg; i<max && i<sz; ++i) printf("%2d%*s", v[i], (scnt+1)*CHAR_PIX, " ");
	cout<< '\n';
	dump(v, depth + 1, max);
}

} //end of namespace heap

int randGen(int max, int min, int addseed)
{
	assert(max > min);
	srand(time(0) + addseed);
	int modbase = max - min; // [min,max]就+1,[min,max)就不加
	return rand() % modbase + min;
}

void output(CVEC_INT &v, const char *str)
{
	std::cout << str;
	for (auto n : v) std::cout << n << ' ';
	std::cout << '\n';
	heap::dump(v, 0, 0);
	std::cout << '\n';
}

void heapTest()
{
	VEC_INT v(VEC_SIZE);
	for (int i=0; i<VEC_SIZE; ++i)
	{
		v[i] = randGen(100, 10, i);
	}
	output(v, "init:\n");
	// sort
	heap::sort(v, INC_FUNC);
	output(v, "inc:\n");

	heap::sort(v, DEC_FUNC);
	output(v, "dec:\n");
	// alter
	int seed = VEC_SIZE;
	char tips[30];
	int randAdd = randGen(100, 50, seed++);
	sprintf(tips, "val=%d,add:\n", randAdd);
	heap::add(v, DEC_FUNC, randAdd);
	output(v, tips);

	int randDelIdx = randGen(v.size()-1, 0, seed++);
	int randDelVal = v[randDelIdx];
	sprintf(tips, "idx=%d,val=%d,del:\n", randDelIdx, randDelVal);
	heap::del(v, DEC_FUNC, randDelIdx);
	output(v, tips);

	int randUpIdx = randGen(v.size()-1, 0, seed++);
	int randUpVal = randGen(100, 50, seed++);
	sprintf(tips, "idx=%d,oval=%d,val=%d,update:\n", randUpIdx, v[randUpIdx], randUpVal);
	heap::update(v, DEC_FUNC, randUpIdx, randUpVal);
	output(v, tips);
}

int main (int argc, char **argv)
{
	heapTest();
#ifdef __WIN32
	system("pause");
#endif //__WIN32
	return 1;
}
