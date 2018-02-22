#include <memory>
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>
#include <atomic>
#include <string>

template<class T>
class Future;

template<class T>
class Promise;

template<class T>
class sharedVariables {
public:
	T value;
	std::condition_variable check;
	bool valid = false, isReady = false, isExceptionSet = false;
	std::exception_ptr exception;
	std::mutex lock;
	friend Promise<T>;
	friend Future<T>;
private:
	sharedVariables() {}
};

template<class T>
class sharedVariables<T&> {
public:
	T *value;
	std::condition_variable check;
	bool valid = false, isReady = false, isExceptionSet = false;
	std::exception_ptr exception;
	std::mutex lock;
	friend Promise<T&>;
	friend Future<T&>;
private:
	sharedVariables() {}
};

template<>
class sharedVariables<void> {
public:
	std::condition_variable check;
	bool valid = false, isReady = false, isExceptionSet = false;
	std::exception_ptr exception;
	std::mutex lock;
	friend Promise<void>;
	friend Future<void>;
private:
	sharedVariables() {}
};
