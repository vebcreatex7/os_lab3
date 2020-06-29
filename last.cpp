#include <iostream>
#include <pthread.h>
#include <string>
#include <cmath>


int NumberOfThreads;
pthread_mutex_t mutex;
int Length;

struct arguments {
	std::string* array;
	int left;
	int right;
	std::string* prev;
};

void Merge(std::string* origin, int l, int m, int r, std::string* prev) {
	int left = l;
	int right = m;
	for (int i = l; i < r; i++) {
		if (left < m && (right >= r || origin[l] <= origin[r])) {
			prev[i] = origin[left++];
		} else {
			prev[i] = origin[right++];
		}
	}
	for (int i = l; i < r; i++) {
		origin[i] = prev[i];
	}
}

void* Split(void* arg) {
	arguments* tmp = new arguments;
	int tmp_right, tmp_left;
	tmp = (arguments*)arg;
	if (tmp->right - tmp->left < 2) {
		return nullptr;
	}
	tmp_right = tmp->right;
	tmp_left = tmp->left;
	tmp->right = (tmp_right + tmp_left) / 2;
	Split((void*)tmp);
	tmp->right = tmp_right;
	tmp->left = (tmp_right + tmp_right) / 2;
	Split((void*)tmp);
	tmp->left = tmp_left;
	Merge(tmp->array, tmp->left, (tmp->left + tmp->right) / 2, tmp->right, tmp->prev);

}	

void MergeSort(std::string* arr) {
	arguments* arg = new arguments;
	std::string temp[Length];
	pthread_t threads[NumberOfThreads];
	for (int i = 0; i < NumberOfThreads; i++) {
		int left = i * Length / NumberOfThreads;
		int right = (i + 1) * Length / NumberOfThreads;
		arg->prev = temp;
		arg->array = arr;
		arg->left = left;
		arg->right = right;
		Split((void*)arg);
	}
	for (int i = NumberOfThreads / 2;i > 0; i /= 2) {
		for (int j = 0; j < i; j++) {
			int left = j * Length / i;
			int right = (j + 1) * Length / i;
			Merge(arr, left, (left + right) / 2, right, temp);
		}
	}
}

int main(int argc, char* argv[]) {
	std::cin >> Length;
	std::string arr[Length];
	for (int i = 0; i < Length; i++) {
		std::cin >> arr[i];
	}
	NumberOfThreads = atoi(argv[1]);
	int pow = 0;
	while (NumberOfThreads > 0) {
		NumberOfThreads = NumberOfThreads / 2;
		pow++;
	}
	NumberOfThreads = (int)std::pow(2, pow);
	std::cout << NumberOfThreads;
	MergeSort(arr);
} 