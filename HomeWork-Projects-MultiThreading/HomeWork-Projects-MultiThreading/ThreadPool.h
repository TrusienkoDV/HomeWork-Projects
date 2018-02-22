#pragma once

#include "Promise.h"

template <typename T>
struct Impl;

class ThreadPool {
private:

	std::queue <std::function<void()> > functionQueue;

	std::vector <std::thread> threads;

	std::mutex lock;

	std::condition_variable check;

	void workerFunc() {
		thisThreadPool = this;
		while (true) {
			std::unique_lock<std::mutex> locker(lock);
			check.wait(locker, [&]() {return !functionQueue.empty() || isEnd; });
			if (isEnd) {
				break;
			}
			std::function<void()> process = functionQueue.front();
			functionQueue.pop();
			locker.unlock();
			process();
		}
	}


	bool isEnd;

	template<typename T, typename F>
	friend Future<typename std::result_of<F(typename Impl<Future<T>>::typeOfFuture)>::type> Map(Future<T> fut, F const & func);

	template <typename T>
	friend Future<typename Impl<Future<T> >::typeOfFuture> Flatten(Future<Future<T> > that);

	template <class Tuple, std::size_t... Ind>
	friend auto FlatTup(Tuple tup, std::index_sequence<Ind...>);

	static thread_local ThreadPool* thisThreadPool;

public:
	ThreadPool(size_t num_threads) {
		isEnd = false;
		while (num_threads--) {
			threads.push_back(std::thread(&ThreadPool::workerFunc, this));
		}
	}

	~ThreadPool() {
		lock.lock();
		isEnd = true;
		lock.unlock();
		check.notify_all();
		for (auto &t : threads)
			t.join();
	}

	void execute(std::function<void()> const & foo) {
		lock.lock();
		functionQueue.push(foo);
		lock.unlock();
		check.notify_one();
	};

	template <typename T, typename F>
	void Parallel(T first, T last, F task) {
		long long size = 0;
		size = std::distance(first, last);
		long long as = 0;
		std::vector<bool> used(size);
		long long k = 0;
		std::mutex mut;
		for (auto i = first; i != last; i++) {
			auto f = [i, &used, task, k, &as, &mut]() {
				mut.lock();
				if (used[k] == false) {
					used[k] = true;
					mut.unlock();
					task(*i);
					mut.lock();
					as++;
					mut.unlock();
				}
				else {
					mut.unlock();
				}
			};
			k++;
			execute(f);
		}
		last--;
		k--;
		for (auto i = last; i != first; i--) {
			mut.lock();
			if (used[k] == false) {
				used[k] = true;
				mut.unlock();
				task(*i);
				mut.lock();
				as++;
				mut.unlock();
			}
			else {
				mut.unlock();
			}
			k--;
		}
		while(as != size) {}
	}
};

thread_local ThreadPool* ThreadPool::thisThreadPool = nullptr;