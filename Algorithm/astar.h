#include <assert.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <vector>

#define MAX_X 25
#define MAX_Y 25

struct Block
{
	Block *prev;
	int x;
	int y;
	double prior = 0.0;
	bool valid = true;
	bool path = false;
	bool isValid(){return valid == 1;}
	void setInValid(){valid = 0;}
	void tryAdd(std::vector<Block*>&, int, int, int, int);
};

//大地图
static Block BigMap[MAX_X][MAX_Y];
//已遍历点
static bool closed[MAX_X][MAX_Y];