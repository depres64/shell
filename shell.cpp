
#include <omp.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <cstdlib>


using namespace std;
 int cTreads = -1;   // количество потоков (-1 - все)
const int ArrLenght=1000;  //количество элементов

int* createArray(int*);
void shellMulti(int*, int, int);
void shellSolo(int*, int);
void PrntArr(int*);
void shellSort(int*, int, int, int);

void PrintArr(int n[])   //Вывод массива
{
	cout << endl << endl;
	int num = 0;
	for (int r = 0; r < ArrLenght; r++)
	{
		cout << n[r] << " ";
	}
	cout << endl << endl;
}


int* createArray(int temp[])   // Генерация массива
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

void shellSort(int arr[], int i, int length, int half) {     //Сортировка Шелла для OpenMP
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
	if (thread != -1) { omp_set_num_threads(thread); }    //Проверка на количество потоков (-1-все)
	for (h = length / 2; h > 0; h = h / 2)
	{
		
#pragma omp parallel for shared( array, length, h, i)           //Распаралеливаем For с передачей общих переменных Shared
		for (i = 0; i < h; i++)
		{

			shellSort(array, i, length, h);

			/*m = omp_get_thread_num();
			cout << "Поток " << m << "\n";
			if (h % 100 == 0) {
				system("pause");
			}*/
		}
	}
}



void shellSolo(int array[], int length)   //Сортировка Шелла без OpenMP
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
	//Создание массива
	int* A;
	int length = ArrLenght;
	int temp[ArrLenght];
	A = createArray(temp);
	//PrintArr(A);   //Вывод сгенерированного массива
	double start, end, instanceTotal, total = 0, endTime = 0, startTime = 0, totalTime = 0; //переменные для подсчитывания времени
tryAgain:
	cout <<"Количество элементов: "<< ArrLenght << "\n1 - один поток; 2 - Многопоток; 3 - Выбор количества потоков" << endl;
	cin >> num;

	switch (num)
	{
	case 1:
		
		startTime = time(NULL);
		for (int l = 0; l < 100; l++)
		{
			for (int i = 0; i < 100; i++)
			{
				
				
				
				start = omp_get_wtime();// начало одной итерации 
				shellSolo(A, length);//Сортировка
				end = omp_get_wtime();// Конец одной итерации
				instanceTotal = end - start;// Время за одну итеррацию
				total += instanceTotal;// Общее время
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

				start = omp_get_wtime();// начало одной итерации 
				shellMulti(A, length, cTreads);//Сортировка
				end = omp_get_wtime();// Конец одной итерации
				instanceTotal = end - start;// Время за одну итеррацию
				total += instanceTotal;// Общее время
			}

		}
		endTime = time(NULL);
		break;
	case 3:
		cout << "Количества потоков:" << endl;
		cin >> cTreads;
		startTime = time(NULL);
		for (int l = 0; l < 100; l++)
		{
			for (int i = 0; i < 100; i++)
			{

				start = omp_get_wtime();// начало одной итерации 
				shellMulti(A, length, cTreads);//Сортировка
				end = omp_get_wtime();// Конец одной итерации
				instanceTotal = end - start;// Время за одну итеррацию
				total += instanceTotal;// Общее время
			}

		}
		endTime = time(NULL);
		break;
	default:
		cout << "ERROR" << endl;
		goto tryAgain;
	}

	totalTime = endTime - startTime;
	cout << "Потраченное время:  " << totalTime << " секунд" << endl;
	cout << "Точное время  " << total << "." << endl;
	//Вывод отсортированного массива
	//PrintArr(A);
	system("pause");
	return 0;


}
