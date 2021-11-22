#include <thread>
#include <vector>
#include <iostream>
using namespace std;

/**
 * Author: Chris Haidas
 * Date: 11/21/2021
 * 
 * A program that generates a vector, then partitions the vector which is then split into a number of vectors
 * equal to that of the user's cpu's thread count. Each new vector will be ran concurrently to get the total sum
 * of each individual vector subtask.
 * 
 * I created this as practice for concurrent execution. 
 * This program may be unoptimized, but I created it for practice.
 */

//unsigned long long
typedef uint_fast64_t ull;

//Sum and output the contents of the vector
void sum_arr(vector<int>& vec, size_t start_pos, size_t end_pos) {
	ull sum = 0;

	for (size_t i = start_pos; i < end_pos; ++i) {
		sum += vec[i];
	}

	cout << sum << "\n";
}

//Generate random numbers into a vector MAX amount of times
vector<int> gen_vec(const ull MAX) {
	vector<int> vec;

	for (size_t i = 0; i < MAX; ++i) {
		vec.push_back(rand() % 100000 + 1);
	}

	return vec;
}

template<typename T>
class Con {
public:
	Con(vector<T>& vec = vector<T>(), size_t slices = 1) {
		this->vec = vec;
		this->slices = slices;
	}

	void run_arr() {
		vector<thread> threads;

		for (size_t i = 1; i <= slices; ++i) {
			threads.push_back(
				thread(sum_arr, ref(this->vec), (this->vec.size() / slices) * (i - 1), (this->vec.size() / slices) * i)
			);
		}

		for (auto& x : threads) {
			x.join();
		}
	}

private:
	vector<T> vec;
	size_t slices;
};


int main() {
	//How many numbers to generate for the vector
	const ull numbers = 10000000;
	vector<int> vec = gen_vec(numbers);

	Con<int> concurrentArray(
		vec, thread::hardware_concurrency() //The vector and the number of threads the computer has for concurrency, in my case 16
	);

	concurrentArray.run_arr();
}