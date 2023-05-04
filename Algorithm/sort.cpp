#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int partition(vector<int>&arr, int beg, int end) {
	int mid = arr[beg];
	int left = beg + 1;
	int right = end;
	while (left <= right) {//完成后right的位置就是arr[beg]的基准位，不能是left，left可能是基准位置+1（偶数个数字情况下）
		if (arr[left] < mid)
			++left;
		else if (arr[right] > mid)
			--right;
		else if (arr[left] >= mid && arr[right] <= mid)
			swap(arr[left++], arr[right--]);
	}
	swap(arr[beg], arr[right]);
	return right;
}

vector<int> QuickSort(vector<int>&arr, int beg, int end) {
	//平均时间复杂度O(nlogn)，最好情况所有基准点都是中位数，时间复杂度一样，最差情况有序数列O(n²)，空间复杂度O(n)，最优空间O(logn)。
	if (beg < end) {
		int divide = partition(arr, beg, end);
		QuickSort(arr, divide + 1, end);
		QuickSort(arr, beg, divide - 1);
	}
	return arr;
}
//冒泡排序，稳定
vector<int> BubbleSort(vector<int>&arr) {
	int size = arr.size();
	//两个循环，时间复杂度O(n²)，最好情况O(n)，空间复杂度O(1)。
	for (int round = 0; round < size - 1; ++round) {
		for (int start = 0; start < size - 1; ++start) {
			if (arr[start] > arr[start + 1])
				swap(arr[start], arr[start + 1]);
		}
	}
	return arr;
}

void merge(vector<int>&arr, int left, int right, int mid) {
	vector<int>res;
	int L_left = left, L_right = mid;//左半部分[left,mid]
	int R_left = mid + 1, R_right = right;//右半部分[mid+1,right]
	while (L_left <= L_right && R_left <= R_right) {
		if (arr[L_left] <= arr[R_left])
			res.push_back(arr[L_left++]);
		else
			res.push_back(arr[R_left++]);
	}
	while (L_left <= L_right) //如果左半边还有剩	
			res.push_back(arr[L_left++]);
	while (R_left <= R_right) //如果右半边还有剩
			res.push_back(arr[R_left++]);
	//将res赋值给arr
	int cur = left;
	for (const auto val : res) 
		arr[cur++] = val;
}
//归并排序，稳定
vector<int> MergeSort(vector<int>&arr, int beg, int end) {
	//平均时间+最好+最差时间复杂度都是O(nlogn)，空间复杂度O(n)。
	int mid = (end - beg) / 2 + beg;
	if (beg < end) {
		MergeSort(arr, beg, mid);
		MergeSort(arr, mid + 1, end);
		merge(arr, beg, end, mid);
	}
	return arr;
}
//插入排序，稳定
vector<int> InsertSort(vector<int>&arr) {
	//平均时间复杂度O(n²)，最好情况正序数列O(n)，最差情况逆序数列O(n²)，空间复杂度O(1)。
	for (unsigned int round = 1; round < arr.size(); ++round) {
		int curIndex = round - 1;
		int curVal = arr[round];
		while (curIndex >= 0 && arr[curIndex] > curVal) {
			arr[curIndex + 1] = arr[curIndex];
			curIndex--;
		}
		arr[curIndex + 1] = curVal;
	}
	return arr;
}
//希尔排序，不稳定
vector<int> ShellSort(vector<int>&arr, int gap) {
	//平均时间复杂度O(n²)，最好情况正序数列O(n)，最差情况逆序数列O(n²)，空间复杂度O(1)。
	while (gap > 0) {
		for (unsigned int round = gap; round < arr.size(); ++round) {
			int curIndex = round - gap;
			int curVal = arr[round];
			while (curIndex >= 0 && arr[curIndex] > curVal) {
				arr[curIndex + gap] = arr[curIndex];
				curIndex -= gap;
			}
			arr[curIndex + gap] = curVal;
		}
		gap--;
	}
	return arr;
}

vector<int> SelectionSort(vector<int>&arr) {
	//平均时间+最好+最差时间复杂度都是O(n²)，空间复杂度O(1)。
	for (unsigned int i = 0; i < arr.size(); ++i) {
		int min = arr[i];
		int minIndex = i;
		for (unsigned int cur = i + 1; cur < arr.size(); ++cur) {
			if (arr[cur] < min) {
				min = arr[cur];
				minIndex = cur;
			}
		}
		swap(arr[i], arr[minIndex]);
	}
	return arr;
}

void initHeap(vector<int>&arr,int parent,int size) {
	int child = parent * 2 + 1;//2n+1就是数组中子节点的位置
	while (child < size) {
		//找出较大的那个子节点
		if ((child + 1) < size && arr[child] < arr[child + 1])
			child++;
		//如果父节点小于这个子节点，就交换
		if (arr[parent] < arr[child]) {
			swap(arr[child], arr[parent]);
			//递归调整下面节点位置
			parent = child;
			child = parent * 2 + 1;
		}
		else break;
	}
}

vector<int> HeapSort(vector<int>&arr) {
	//平均时间O(n^1.3)最好O(n)最差时间复杂度O(n²)，空间复杂度O(1)。
	int size = arr.size() - 1;
	//构造大根堆
	for (int beg = (size >> 1) - 1; beg >= 0; --beg)
		initHeap(arr, beg, size);
	//交换根结点和最后一个叶子结点的值，重复构建大根堆
	while (size>0) {
		swap(arr[0], arr[size]);
		initHeap(arr, 0, size);
		size--;
	}
	return arr;
}

void reset(vector<int>&arr) {
	arr = { 4,2,1,6,5,6,2,3,10,1,4,5,8,7,9,2,9 };
}

void menu() {
	cout << "┌───────────┐" << endl;
	cout << "│ 排序算法：│" << endl;
	cout << "│1、冒泡排序│" << endl;
	cout << "│2、快速排序│" << endl;
	cout << "│3、归并排序│" << endl;
	cout << "│4、插入排序│" << endl;
	cout << "│5、希尔排序│" << endl;
	cout << "│6、选择排序│" << endl;
	cout << "│7、 堆排序 │" << endl;
	cout << "│8、  退出  │" << endl;
	cout << "└───────────┘" << endl;
}

void output(vector<int>&arr) {
	cout << "vector的内容是：";
	for (const auto v : arr)
		cout << v << ends;
	cout << endl;
}

int main()
{
	vector<int>arr{ 4,2,1,6,5,6,2,3,10,1,4,5,8,7,9,2,9 };
	int type;
	while (1) {
		menu();
		output(arr);
		cout << "请选择排序方式：";
		cin >> type;
		switch (type) {
		case 1:
			BubbleSort(arr);
			cout << "冒泡排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 2:
			QuickSort(arr, 0, arr.size() - 1);
			cout << "快速排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 3:
			MergeSort(arr, 0, arr.size() - 1);
			cout << "归并排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 4:
			InsertSort(arr);
			cout << "插入排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 5:
			int gap;
		loop:			cout << "请输入希尔排序的间隔值："; cin >> gap;
			if (gap >= arr.size()) {
				cout << "数值过大，请输入0至" << arr.size() - 1 << "的值" << endl;
				goto loop;
			}
			ShellSort(arr, gap);
			cout << "希尔排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 6:
			SelectionSort(arr);
			cout << "选择排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 7:
			HeapSort(arr);
			cout << "堆排序完成" << endl;
			output(arr);
			system("pause");
			system("cls");
			reset(arr);
			break;
		case 8:
			cout << "即将退出。" << endl;
			system("pause");
			return 0;
			break;
		default:
			cout << "错误输入" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
	return 0;
}
