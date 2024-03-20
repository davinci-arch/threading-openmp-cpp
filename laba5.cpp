#include <iostream>
#include <omp.h>
#include <limits.h>
#include <time.h>
#include <ctime>
#include <cstdlib>

using namespace std;


struct Data{
	int idxRow;
	long sumOfRow;
};

long long get_part_sum(int);
Data get_min_sum(int);
void init_arr();
void print_arr();

const int row = 10000;
const int column = 10000;

int arr[row][column];

int main(){
	
	init_arr();
	
	omp_set_nested(1);
	
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			long long sum = get_part_sum(5);
			cout << "Sum of all elements: " << sum << endl;
		}
		#pragma omp section
		{
			Data data = get_min_sum(5);
			cout << "Min row idx: " << data.idxRow << endl << "Min sum: " << data.sumOfRow << endl; 
	
		}
	}
	
	
	
	//print_arr();
	
	return 0;
}

void print_arr() {
	
	for(int i = 0; i < row; i++){
		int localSum = 0;
		for(int j = 0; j < column; j++){
			localSum += arr[i][j];
			cout << arr[i][j] << " ";
		}
		cout << " sum: " << localSum << endl;
	}
	
}

void init_arr(){
	srand(time(0));
	
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			arr[i][j] = (rand() % 100);
		}
	}
}

long long get_part_sum(int amount_of_threads){
	long long sum = 0;
	
	double t1 = omp_get_wtime();
	#pragma omp parallel for reduction(+:sum) num_threads(amount_of_threads) collapse(2)
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			sum += arr[i][j];
		}
	}
	double t2 = omp_get_wtime();
	
	cout << "part_sum with threads: " << amount_of_threads << " working time: " << t2 - t1 << " seconds" << endl;
	
	return sum;
}

Data get_min_sum(int amount_of_threads){
	Data data;
	
	data.sumOfRow = INT_MAX;
	
	double t1 = omp_get_wtime();
	
	#pragma omp parallel for num_threads(amount_of_threads)
	for(int i = 0; i < row; i++) {
		int local_sum = 0;
		for(int j = 0; j < column; j++){
			local_sum += arr[i][j];
		}
		
		if (data.sumOfRow > local_sum)
		{
			#pragma omp critical
			if (data.sumOfRow > local_sum) {
				data.sumOfRow = local_sum;
				data.idxRow = i;
			}
		}
	}
	double t2 = omp_get_wtime();
	
	cout << "min_sum with threads: " << amount_of_threads << " working time: " << t2 - t1 << " seconds" << endl;
	
	return data;
}

