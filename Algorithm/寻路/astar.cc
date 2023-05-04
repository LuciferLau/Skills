#ifndef __ASTAR_H
#define __ASTAR_H

#include "astar.h"

void setBlock(int sx, int ex, int sy, int ey)
{
	for(int x = sx; x<=ex; x++)
	{
		for(int y = sy; y<=ey; y++)
		{
			BigMap[x][y].setInValid();
		}
	}
}

void initMap()
{
	for(auto i=0; i<MAX_X; ++i)
	{
		for(auto j=0; j<MAX_Y; ++j)
		{
			BigMap[i][j].x = i;
			BigMap[i][j].y = j;
		}
	}
	setBlock(4, 8, 0, 10);
	setBlock(10, 20, 12, 20);
	setBlock(23, 23, 23, 24);
//	setBlock(23, 24, 23, 23);
}

void showMap()
{
	for(auto i=0; i<MAX_Y; ++i)
	{
		for(auto j=0; j<MAX_X; ++j)
		{
			auto b = BigMap[j][i];
			if (b.isValid())
			{
				if (b.path > 0)
				{
					printf("%2d ", b.path);
				}
				else
				{
					printf("%2s ", "O");
				}
			}
			else
			{
				printf("%2s ", "X");
			}
		}
		printf("\n");
	}
}

void Block::tryAdd(std::vector<Block*> &v, int ox, int oy, int ex, int ey)
{
	if (!isValid())
	{
		return;
	}
	if(prev == nullptr)
	{
		prev = &BigMap[ox][oy];
	}
	auto p = BigMap[ox][oy];
	auto sdx = pow(x-ox, 2);
	auto sdy = pow(y-oy, 2);
	auto sadd = sqrt(sdx + sdy);

	auto edx = pow(x-ex, 2);
	auto edy = pow(y-ey, 2);
	auto eadd = sqrt(edx + edy);
	prior = p.prior + sadd + eadd;
	v.push_back(this);
//	printf("tryAdd,now=[%d,%d],prev=[%d,%d],sadd=%.3f,eadd=%.3f,prior=%.3f\n",
//		x, y, ox, oy, sadd, eadd, prior);
}

void bfs(std::vector<Block*> &v, int sx, int sy, int ex, int ey)
{
	for(auto x=sx-1; x<=sx+1; x++)
	{
		for(auto y=sy-1; y<=sy+1; y++)
		{
			if(!closed[x][y] && x >= 0 && y >= 0 && x < MAX_X && y < MAX_Y)
			{
				BigMap[x][y].tryAdd(v, sx, sy, ex, ey);
			}
		}
	}
}

Block* findMax(std::vector<Block*> &v)
{
	double min = -1.0;
	Block* ret;
	for(auto i=0; i<v.size(); ++i)
	{
		auto b = v[i];
		if(!closed[b->x][b->y])
		{
			if(min < 0)
			{
				ret = b;
				min = b->prior;
			}
			else if(min > b->prior)
			{
				ret = b;
				min = b->prior;
			}
		}
	}
//	printf("findMax,min=[%d,%d]\n", ret->x, ret->y);
	return ret;
}

void recordPath(Block *dest, int sx, int sy, int cur)
{
	cur = cur + 1;
	int nx = dest->x;
	int ny = dest->y;
	BigMap[nx][ny].path = cur;
	if(nx == sx && ny == sy)
	{
		return;
	}
	auto prev = dest->prev;
	int x = prev->x;
	int y = prev->y;
//	printf("recordPath,now=[%d,%d],prev=[%d,%d],src=[%d,%d]\n", nx, ny, x, y, sx, sy);
	recordPath(prev, sx, sy, cur);
}

void Astar(int sx, int sy, int ex, int ey, int depth, int bx, int by)
{
	//首轮参数校验
	if(depth == 0)
	{
		auto src = &BigMap[sx][sy];
		auto tar = &BigMap[ex][ey];
		if(!src->isValid() || !tar->isValid())
		{
			printf("args_fail\n");
			return;
		}
	}
	depth = depth + 1;
	//递归层数限制
	if(depth > 100)
	{
		printf("fail_too_deep,cur=[%d,%d],depth=%d\n", sx, sy, depth);
		return;
	}
	std::vector<Block*> open;
	auto now = BigMap[sx][sy];
	closed[sx][sy] = true;
	//递归结束
	if(sx == ex && sy == ey)
	{
		printf("ok,dest=[%d,%d],depth=%d\n", sx, sy, depth);
		recordPath(&now, bx, by, 0);
		showMap();
		return;
	}
	bfs(open, sx, sy, ex, ey);
	auto nb = findMax(open);
	Astar(nb->x, nb->y, ex, ey, depth, bx, by);
}

int main(int argc, char** argv)
{
	initMap();
	showMap();
	int sx, sy, ex, ey = -1;
	for(auto i = 1; i < argc; i++)
	{
		if((2 == strlen(argv[i])) && ('-' == argv[i][0]))
		{
			switch(argv[i][1])
			{
				case 's':
				case 'S':
					sx = strtol(argv[++i], NULL, 10);
					sy = strtol(argv[++i], NULL, 10);
					assert(sx >= 0 && sy >= 0 && sx < MAX_X && sy < MAX_Y);
					continue;
				case 'e':
				case 'E':
					ex = strtol(argv[++i], NULL, 10);
					ey = strtol(argv[++i], NULL, 10);
					assert(ex >= 0 && ey >= 0 && ex < MAX_X && ey < MAX_Y);
					continue;
				default:
					return -1;
			}
		}
	}
	printf("A* start,sx=%d,sy=%d,ex=%d,ey=%d\n", sx,sy,ex,ey);
	Astar(sx, sy, ex, ey, 0, sx, sy);

#ifdef __WIN32
	std::cout << "\nall test running under __WIN32 os\n________________________________\n\n" << std::endl;
	system("pause");
#endif
}

#endif //__ASTAR_H
