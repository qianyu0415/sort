#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<cstring>
#include<time.h>
#include <random>
#include<vector>
const int MAX = 100010;
typedef int ElemType;
using namespace std;
int NUM = 0;

void GenerateRandomData(const string& filename)
{
	ofstream file(filename);
	if (!file)
	{
		cerr << "错误：无法创建文件" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "请输入测试规模(0--100000)" << endl;
	int scope;
	cin >> scope;
	file << scope << endl;
	vector<int> data(scope);
	for (int i = 0; i < scope; i++)
	{
		data[i] = i + 1;
		file << data[i] << endl;
	}
	file.close();
	// 使用随机数生成器引擎和分布来实现真正的随机交换
	mt19937 gen(random_device{}()); // 使用硬件产生的随机种子
	uniform_int_distribution<int> dist(0, scope - 1); // 定义一个均匀分布

	// 随机交换data数组中的元素
	for (int i = scope - 1; i > 0; --i) {
		int randomIndex = dist(gen); // 生成随机索引
		swap(data[i], data[randomIndex]); // 交换元素
	}

	// 将随机交换后的元素写入inputdata2.txt文件
	ofstream outfile1("inputdata2.txt");
	outfile1 << scope<<endl;
	for (int i = 0; i < scope; ++i) {
		outfile1 << data[i] << endl;
	}
	outfile1.close();
	
}
void ReadDataFromFile(const string& filename, ElemType data[], int& size)
{
	ifstream infile(filename);
	if (!infile)
	{
		cerr << "错误：无法打开文件。" << endl;
		exit(EXIT_FAILURE);
	}
	infile >> size;
	for (int i = 0; i < size; i++)
	{
		if (!(infile >> data[i]))
		{
			cerr << "错误：文件中的数据无效。" << endl;
			exit(EXIT_FAILURE);
		}
	}
	infile.close();
}
//输出序列
void print_sorted(ElemType A[])
{
	for (int i = 0; i < NUM; i++)
		cout << A[i] << " ";
	cout << endl;
}
//A[low...mid]和A[mid+1...high]各自有序，把这两个数组归并在一起
void Merge(ElemType A[], int low, int mid, int high)
{
	ElemType* B = (ElemType*)malloc((NUM + 1) * sizeof(ElemType));
	for (int i = low; i <= high; i++)
		B[i] = A[i];			//将A中元素复制到B中
	int i, j, k;
	for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++)
	{
		if (B[i] <= B[j])			//<=保持了稳定性
			A[k] = B[i++];
		else
			A[k] = B[j++];
	}
	while (i <= mid)
		A[k++] = B[i++];
	while (j <= high)
		A[k++] = B[j++];
	delete B;
}
void MergeSort(ElemType A[], int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;
		MergeSort(A, low, mid);		//对左侧子序列进行递归排序
		MergeSort(A, mid + 1, high);//对右侧子序列进行递归排序
		Merge(A, low, mid, high);
	}
}

int Partition(ElemType A[], int low, int high)
{
	ElemType pivot = A[low];//将表中第一个元素作为枢纽进行划分
	while (low < high)  //当low==high时，循环结束
	{
		while (low < high && A[high] >= pivot) high--;
		A[low] = A[high];		//比pivot小的元素移到左端
		while (low < high && A[low] < pivot) low++;
		A[high] = A[low];      //比pivot大的元素移到右端
	}
	A[low] = pivot;		//枢纽放到最终位置
	return low;			//返回存放枢纽的位置
}
void QuickSort(ElemType A[], int low, int high)
{
	//递归至每个子表只有一个元素或为空
	if (low < high)
	{
		//Partition()就是划分操作
		int pivotpos = Partition(A, low, high);
		QuickSort(A, low, pivotpos-1);
		QuickSort(A, pivotpos + 1, high);
	}
}

//选择排序
void SelectSort(ElemType A[], int n)
{
	for (int i = 0; i < n - 1; i++)		//一共进行n-1趟
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (A[j] < A[min]) min = j;
		}
		if(min != i)
		{
			ElemType temp = A[i];
			A[i] = A[min];
			A[min] = temp;
		}
	}
}

//堆排序
//将以k为根的子树进行调整
void HeadAdjust(ElemType A[], int k, int len)
{
	A[0] = A[k];
	for (int i = 2 * k; i <= len; i *= 2)
	{
		if (i < len && A[i] < A[i + 1])
			i++;		//取k较大子节点向下筛选
		if (A[0] >= A[i]) break;
		else
		{
			A[k] = A[i];
			k = i;		//更新k值，以便继续向下筛选
		}
	}
	A[k] = A[0];		//被筛选结点的值放在最终位置上
}
//建立大根堆
void BuildMaxHeap(ElemType A[], int len)
{
	for (int i = len / 2; i > 0; i--)		//不断调整非终端节点
	{
		HeadAdjust(A, i, len);
	}
}

void HeapSort(ElemType A[], int len)
{
	BuildMaxHeap(A, len);	//初始化大根堆
	for (int i = len; i > 1; i--)		//n-1趟的交换和建堆过程
	{
		ElemType temp = A[i];    //堆顶元素和堆底元素交换
		A[i] = A[1];
		A[1] = temp;
		HeadAdjust(A, 1, i - 1);//把剩下的n-1个元素调整成堆
	}
}

int main()
{
	GenerateRandomData("inputdata1.txt");

	ElemType data[MAX];
	ReadDataFromFile("inputdata2.txt", data, NUM);
	cout << "读取的序列为:" << endl;
	print_sorted(data);
	std::cout << "1. 归并排序  2. 快速排序  3. 选择排序   4.堆排序" << std::endl;
	int ch;
	std::cin >> ch;

	clock_t start, end;

	switch (ch) {
	case 1:
		start = clock();
		MergeSort(data, 0, NUM - 1);
		end = clock();
		std::cout << "归并排序后序列：" << std::endl;
		print_sorted(data);
		break;
	case 2:
		start = clock();
		QuickSort(data, 0, NUM - 1);
		end = clock();
		std::cout << "快速排序后序列：" << std::endl;
		print_sorted(data);
		break;
	case 3:
		start = clock();
		SelectSort(data, NUM);
		end = clock();
		std::cout << "选择排序后序列：" << std::endl;
		print_sorted(data);
		break;
	case 4:
		start = clock();
		int heap[MAX];
		heap[0] = 0;
		for (int i = 0; i < NUM; i++)
		{
			heap[i + 1] = data[i];
		}
		HeapSort(heap, NUM);
		end = clock();
		std::cout << "堆排序后序列：" << std::endl;
		for (int i = 1; i < NUM + 1; i++)
			cout << heap[i] << " ";
		cout << endl;
		break;
	default:
		std::cerr << "错误：无效的选择。" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "耗时：" << static_cast<double>(end - start) << "ms" << std::endl;
	return 0;
}
