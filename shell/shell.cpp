
#include <omp.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <cstdlib>


using namespace std;
 int cTreads = -1;   // ���������� ������� (-1 - ���)
const int ArrLenght=1000;  //���������� ���������

int* createArray(int*);
void shellMulti(int*, int, int);
void shellSolo(int*, int);
void PrntArr(int*);
void shellSort(int*, int, int, int);

void PrintArr(int n[])   //����� �������
{
	cout << endl << endl;
	int num = 0;
	for (int r = 0; r < ArrLenght; r++)
	{
		cout << n[r] << " ";
	}
	cout << endl << endl;
}


int* createArray(int temp[])   // ��������� �������
{
	int r = 0;
	int arr[ArrLenght];
	srand(time(0));
	srand((unsigned)time(NULL));
	for (int i = 0; i < ArrLenght; i++)
	{
		r = rand() % ArrLenght;
		arr[i] = r;
		temp[i] = arr[i];
	}
	return temp;
}

void shellSort(int arr[], int i, int length, int half) {     //���������� ����� ��� OpenMP
	int temp = 0;
	int j = 0;

	for (int f = half + i; f < length; f = f + half)
	{
		j = f;
		while (j > i && arr[j - half] > arr[j])
		{
			temp = arr[j];
			arr[j] = arr[j - half];
			arr[j - half] = temp;
			j = j - half;
		}
	}
}

void shellMulti(int array[], int length, int thread)
{
	int m;
	int h;
	int j = 0;
	int temp = 0;
	int i = 0;
	if (thread != -1) { omp_set_num_threads(thread); }    //�������� �� ���������� ������� (-1-���)
	for (h = length / 2; h > 0; h = h / 2)
	{
		
#pragma omp parallel for shared( array, length, h, i)           //��������������� For � ��������� ����� ���������� Shared
		for (i = 0; i < h; i++)
		{

			shellSort(array, i, length, h);

			/*m = omp_get_thread_num();
			cout << "����� " << m << "\n";
			if (h % 100 == 0) {
				system("pause");
			}*/
		}
	}
}



void shellSolo(int array[], int length)   //���������� ����� ��� OpenMP
{

	int j = 0;
	int temp = 0;
	int h = 0;

	for (h = length / 2; h > 0; h = h / 2)
	{
		for (int i = 0; i < h; i++)
		{
			for (int f = h + i; f < length; f = f + h)
			{
				j = f;
				while (j > i && array[j - h] > array[j])
				{
					temp = array[j];
					array[j] = array[j - h];
					array[j - h] = temp;
					j = j - h;
				}
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	int num = 0;
	//�������� �������
	int* A;
	int length = ArrLenght;
	int temp[ArrLenght];
	A = createArray(temp);
	//PrintArr(A);   //����� ���������������� �������
	double start, end, instanceTotal, total = 0, endTime = 0, startTime = 0, totalTime = 0; //���������� ��� ������������� �������
tryAgain:
	cout <<"���������� ���������: "<< ArrLenght << "\n1 - ���� �����; 2 - ����������; 3 - ����� ���������� �������" << endl;
	cin >> num;

	switch (num)
	{
	case 1:
		
		startTime = time(NULL);
		for (int l = 0; l < 100; l++)
		{
			for (int i = 0; i < 100; i++)
			{
				
				
				
				start = omp_get_wtime();// ������ ����� �������� 
				shellSolo(A, length);//����������
				end = omp_get_wtime();// ����� ����� ��������
				instanceTotal = end - start;// ����� �� ���� ���������
				total += instanceTotal;// ����� �����
			}
		}
		endTime = time(NULL);
		break;
	case 2:
		
		startTime = time(NULL); 
		for (int l = 0; l < 100; l++)
		{
			for (int i = 0; i < 100; i++)
			{

				start = omp_get_wtime();// ������ ����� �������� 
				shellMulti(A, length, cTreads);//����������
				end = omp_get_wtime();// ����� ����� ��������
				instanceTotal = end - start;// ����� �� ���� ���������
				total += instanceTotal;// ����� �����
			}

		}
		endTime = time(NULL);
		break;
	case 3:
		cout << "���������� �������:" << endl;
		cin >> cTreads;
		startTime = time(NULL);
		for (int l = 0; l < 100; l++)
		{
			for (int i = 0; i < 100; i++)
			{

				start = omp_get_wtime();// ������ ����� �������� 
				shellMulti(A, length, cTreads);//����������
				end = omp_get_wtime();// ����� ����� ��������
				instanceTotal = end - start;// ����� �� ���� ���������
				total += instanceTotal;// ����� �����
			}

		}
		endTime = time(NULL);
		break;
	default:
		cout << "ERROR" << endl;
		goto tryAgain;
	}

	totalTime = endTime - startTime;
	cout << "����������� �����:  " << totalTime << " ������" << endl;
	cout << "������ �����  " << total << "." << endl;
	//����� ���������������� �������
	//PrintArr(A);
	system("pause");
	return 0;


}
