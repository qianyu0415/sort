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
		cerr << "�����޷������ļ�" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "��������Թ�ģ(0--100000)" << endl;
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
	// ʹ�����������������ͷֲ���ʵ���������������
	mt19937 gen(random_device{}()); // ʹ��Ӳ���������������
	uniform_int_distribution<int> dist(0, scope - 1); // ����һ�����ȷֲ�

	// �������data�����е�Ԫ��
	for (int i = scope - 1; i > 0; --i) {
		int randomIndex = dist(gen); // �����������
		swap(data[i], data[randomIndex]); // ����Ԫ��
	}

	// ������������Ԫ��д��inputdata2.txt�ļ�
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
		cerr << "�����޷����ļ���" << endl;
		exit(EXIT_FAILURE);
	}
	infile >> size;
	for (int i = 0; i < size; i++)
	{
		if (!(infile >> data[i]))
		{
			cerr << "�����ļ��е�������Ч��" << endl;
			exit(EXIT_FAILURE);
		}
	}
	infile.close();
}
//�������
void print_sorted(ElemType A[])
{
	for (int i = 0; i < NUM; i++)
		cout << A[i] << " ";
	cout << endl;
}
//A[low...mid]��A[mid+1...high]�������򣬰�����������鲢��һ��
void Merge(ElemType A[], int low, int mid, int high)
{
	ElemType* B = (ElemType*)malloc((NUM + 1) * sizeof(ElemType));
	for (int i = low; i <= high; i++)
		B[i] = A[i];			//��A��Ԫ�ظ��Ƶ�B��
	int i, j, k;
	for (i = low, j = mid + 1, k = i; i <= mid && j <= high; k++)
	{
		if (B[i] <= B[j])			//<=�������ȶ���
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
		MergeSort(A, low, mid);		//����������н��еݹ�����
		MergeSort(A, mid + 1, high);//���Ҳ������н��еݹ�����
		Merge(A, low, mid, high);
	}
}

int Partition(ElemType A[], int low, int high)
{
	ElemType pivot = A[low];//�����е�һ��Ԫ����Ϊ��Ŧ���л���
	while (low < high)  //��low==highʱ��ѭ������
	{
		while (low < high && A[high] >= pivot) high--;
		A[low] = A[high];		//��pivotС��Ԫ���Ƶ����
		while (low < high && A[low] < pivot) low++;
		A[high] = A[low];      //��pivot���Ԫ���Ƶ��Ҷ�
	}
	A[low] = pivot;		//��Ŧ�ŵ�����λ��
	return low;			//���ش����Ŧ��λ��
}
void QuickSort(ElemType A[], int low, int high)
{
	//�ݹ���ÿ���ӱ�ֻ��һ��Ԫ�ػ�Ϊ��
	if (low < high)
	{
		//Partition()���ǻ��ֲ���
		int pivotpos = Partition(A, low, high);
		QuickSort(A, low, pivotpos-1);
		QuickSort(A, pivotpos + 1, high);
	}
}

//ѡ������
void SelectSort(ElemType A[], int n)
{
	for (int i = 0; i < n - 1; i++)		//һ������n-1��
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

//������
//����kΪ�����������е���
void HeadAdjust(ElemType A[], int k, int len)
{
	A[0] = A[k];
	for (int i = 2 * k; i <= len; i *= 2)
	{
		if (i < len && A[i] < A[i + 1])
			i++;		//ȡk�ϴ��ӽڵ�����ɸѡ
		if (A[0] >= A[i]) break;
		else
		{
			A[k] = A[i];
			k = i;		//����kֵ���Ա��������ɸѡ
		}
	}
	A[k] = A[0];		//��ɸѡ����ֵ��������λ����
}
//���������
void BuildMaxHeap(ElemType A[], int len)
{
	for (int i = len / 2; i > 0; i--)		//���ϵ������ն˽ڵ�
	{
		HeadAdjust(A, i, len);
	}
}

void HeapSort(ElemType A[], int len)
{
	BuildMaxHeap(A, len);	//��ʼ�������
	for (int i = len; i > 1; i--)		//n-1�˵Ľ����ͽ��ѹ���
	{
		ElemType temp = A[i];    //�Ѷ�Ԫ�غͶѵ�Ԫ�ؽ���
		A[i] = A[1];
		A[1] = temp;
		HeadAdjust(A, 1, i - 1);//��ʣ�µ�n-1��Ԫ�ص����ɶ�
	}
}

int main()
{
	GenerateRandomData("inputdata1.txt");

	ElemType data[MAX];
	ReadDataFromFile("inputdata2.txt", data, NUM);
	cout << "��ȡ������Ϊ:" << endl;
	print_sorted(data);
	std::cout << "1. �鲢����  2. ��������  3. ѡ������   4.������" << std::endl;
	int ch;
	std::cin >> ch;

	clock_t start, end;

	switch (ch) {
	case 1:
		start = clock();
		MergeSort(data, 0, NUM - 1);
		end = clock();
		std::cout << "�鲢��������У�" << std::endl;
		print_sorted(data);
		break;
	case 2:
		start = clock();
		QuickSort(data, 0, NUM - 1);
		end = clock();
		std::cout << "������������У�" << std::endl;
		print_sorted(data);
		break;
	case 3:
		start = clock();
		SelectSort(data, NUM);
		end = clock();
		std::cout << "ѡ����������У�" << std::endl;
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
		std::cout << "����������У�" << std::endl;
		for (int i = 1; i < NUM + 1; i++)
			cout << heap[i] << " ";
		cout << endl;
		break;
	default:
		std::cerr << "������Ч��ѡ��" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "��ʱ��" << static_cast<double>(end - start) << "ms" << std::endl;
	return 0;
}
