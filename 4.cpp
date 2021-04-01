#include<boost\lockfree\stack.hpp>
#include "threadsafe_stack.cpp"
#include "timer.h"
#include<iostream>
#include<vector>
#include<thread>
#include<future>
using namespace std;
void push_back(boost::lockfree::stack <int> s, int value, int M) {
	for(int i=0; i<M; i++)
		s.push(value);
}
void pop_back(boost::lockfree::stack <int> s, int value, int M) {
	for (int i = 0; i < M; i++)
		s.pop(value);
}

int main() {
	cout << "Enter the number of threads:";
	int N, M;
	cin >> N;
	cout << "Enter the number of elements per one thread: ";
	cin >> M;
	vector<thread> inTr(N);
	int value;
	vector<std::future<void>> infutures(N);
	vector<std::future<void>> outfutures(N);
	vector<thread> outTr(N);
	boost::lockfree::stack <int> s(M);
	Timer boost_stack;
	for (int j = 0; j < N; j++) {
		packaged_task<void(boost::lockfree::stack<int>, int, int)> task(push_back);
		infutures[j] = task.get_future();
		inTr[j] = thread(move(task), s, 1, M);
	}
	for (int j = 0; j < N; j++) {
		packaged_task<void(boost::lockfree::stack<int>, int, int)> task(pop_back);
		outfutures[j] = task.get_future();
		outTr[j] = thread(move(task), s, value, M);
	}
	
	
}
