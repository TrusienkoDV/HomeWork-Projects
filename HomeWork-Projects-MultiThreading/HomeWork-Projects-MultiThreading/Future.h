#pragma once

#include "SharedVariables.h"

//Future.h

template<typename T>
class Promise;

template <class T>
class Future {
public:

	Future<T> & operator=(Future<T> && that) {
		if (this != &that) {
			attributes = that.attributes;
			isGet = that.isGet;
			that.attributes = NULL;
		}
		return *this;
	}

	/*Future<T> & operator=(Future<T> const & that) {
		if (this != &that) {
			attributes = that.attributes;
		}
		return *this;
	}*/

	Future(Future const & that):attributes(that.attributes)
	{ }

	T get() {
		wait();
		if (isGet) {
			throw std::logic_error("recall future get");
		}
		isGet = true;
		(*attributes).lock.lock();
		if (!(*attributes).valid) {
			throw std::logic_error("future is invalidate");
		}
		if ((*attributes).isExceptionSet) {
			std::rethrow_exception((*attributes).exception);
		}
		(*attributes).lock.unlock();
		return (*attributes).value;
	}

	void wait() {
		std::unique_lock<std::mutex> locker((*attributes).lock);
		while (!(*attributes).isReady) {
			(*attributes).check.wait(locker);
		}
	}

	bool isReady() const {
		std::unique_lock<std::mutex> locker((*attributes).lock);
		return (*attributes).isReady;
	}

	friend Promise<T>;
	friend sharedVariables<T>;
	Future() {};
private:
	std::shared_ptr<sharedVariables<T> > attributes;
	bool isGet = false;
};

template <class T>
class Future<T&> {
public:
	Future(const Future<T>&) = delete;
	Future(Future<T&>&&) = default;

	T& get() {
		wait();
		(*attributes).lock.lock();
		if (!(*attributes).valid) {
			throw "ERROR";
		}
		if ((*attributes).isExceptionSet) {
			throw (*attributes).exception;
		}
		(*attributes).lock.unlock();
		return (*(*attributes).value);
	}

	void wait() {
		if (isWait) {
			throw "ERROR";
		}
		isWait = true;
		std::unique_lock<std::mutex> locker((*attributes).lock);
		while (!(*attributes).isReady) {
			(*attributes).check.wait(locker);
		}
	}

	bool isReady() const {
		std::unique_lock<std::mutex> locker((*attributes).lock);
		return (*attributes).isReady;
	}

	friend Future<T&> Promise<T&>::getFuture();
	Future() {};
private:
	std::shared_ptr<sharedVariables<T&> > attributes;
	bool isWait = false;
};

template <>
class Future<void> {
public:
	Future(const Future<void>&) = delete;
	Future(Future<void>&&) = default;
	void get() {
		wait();
		(*attributes).lock.lock();
		if (!(*attributes).valid) {
			throw "ERROR";
		}
		if ((*attributes).isExceptionSet) {
			throw (*attributes).exception;
		}
		(*attributes).lock.unlock();
	}
	void wait() {
		if (isWait) {
			throw "ERROR";
		}
		isWait = true;
		std::unique_lock<std::mutex> locker((*attributes).lock);
		while (!(*attributes).isReady) {
			(*attributes).check.wait(locker);
		}
	}
	bool isReady() const {
		std::unique_lock<std::mutex> locker((*attributes).lock);
		return (*attributes).isReady;
	}
	friend Promise<void>;
private:
	Future() {};
	std::shared_ptr<sharedVariables<void> > attributes;
	bool isWait = false;
};
