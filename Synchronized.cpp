#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include "folly/Synchronized.h"


void worker_thread()
{

}

int main()
{
	folly::Synchronized<std::vector<int>> fib;

	fib.wlock()->push_back(1);
	fib.wlock()->push_back(1);

	auto calculate_next_fib_member = [&fib] () {
			auto locked = fib.wlock();
			auto rit = locked->rbegin();
			locked->push_back(*rit + *(++rit));
		};

	auto writeLock = fib.wlock();
	std::thread worker1(calculate_next_fib_member);
	std::thread worker2(calculate_next_fib_member);
	std::thread worker3(calculate_next_fib_member);
	std::thread worker4(calculate_next_fib_member);
	writeLock.unlock();

	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();

	auto readLock = fib.rlock();
	for (int i : *readLock)
		std::cout << i << ", ";
	std::cout << "\n";
}
