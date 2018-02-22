#include"Future.h"

//Promise.h

template <class T>
class Promise {
public:

	Promise() {
		attributes.reset(new sharedVariables<T>);
	}

	~Promise() {
		if (attributes) {
			(*attributes).lock.lock();
			(*attributes).isReady = true;
			(*attributes).check.notify_one();
			(*attributes).lock.unlock();
		}
	}

	Promise(const Promise<T>&) = delete;
	Promise(Promise<T>&&) = default;

	void set(const T & a) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set");
		}
		(*attributes).lock.lock();
		(*attributes).value = a;
		(*attributes).valid = true;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}

	void set(T&& a) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set");
		}
		(*attributes).lock.lock();
		(*attributes).value = std::move(a);
		(*attributes).valid = true;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}

	Future<T> getFuture() {
		checkState();
		if (isFutureGet) {
			throw std::logic_error("re-creation future");
		}
		Future<T> f;
		f.attributes = attributes;
		isFutureGet = true;
		return std::move(f);
	}

	void setException(std::exception_ptr & exception) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set exception");
		}
		(*attributes).lock.lock();
		(*attributes).isExceptionSet = true;
		(*attributes).exception = exception;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}
	friend sharedVariables<T>;
private:
	std::shared_ptr<sharedVariables<T> > attributes;
	bool isAdd = false;
	bool isFutureGet = false;
	void checkState() {
		if (!attributes) {
			throw std::runtime_error("Promise is invalidate");
		}
	}
};

template <class T>
class Promise<T&> {
public:

	Promise() {
		attributes.reset(new sharedVariables<T&>);
	}

	~Promise() {
		if (attributes) {
			(*attributes).lock.lock();
			(*attributes).isReady = true;
			(*attributes).check.notify_one();
			(*attributes).lock.unlock();
		}
	}

	Promise(const Promise<T&>&) = delete;
	Promise(Promise<T&>&&) = default;

	void set(T& a) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set");
		}
		(*attributes).lock.lock();
		(*attributes).value = &a;
		(*attributes).valid = true;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}

	Future<T&> getFuture() {
		checkState();
		if (isFutureGet) {
			throw std::logic_error("re-creation future");
		}
		Future<T&> f;
		f.attributes = attributes;
		isFutureGet = true;
		return f;
	}

	void setException(std::exception_ptr & exception) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set exception");
		}
		(*attributes).lock.lock();
		(*attributes).isExceptionSet = true;
		(*attributes).exception = exception;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}

private:
	std::shared_ptr<sharedVariables<T&> > attributes;
	bool isAdd = false;
	bool isFutureGet = false;
	void checkState() {
		if (!attributes) {
			throw std::runtime_error("Promise is invalidate");
		}
	}
};

template <>
class Promise<void> {
public:
	Promise() {
		attributes.reset(new sharedVariables<void>);
	}
	~Promise() {
		if (attributes) {
			(*attributes).lock.lock();
			(*attributes).isReady = true;
			(*attributes).check.notify_one();
			(*attributes).lock.unlock();
		}
	}

	Promise(const Promise<void>&) = delete;
	Promise(Promise<void>&&) = default;

	void set() {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set");
		}
		(*attributes).lock.lock();
		(*attributes).valid = true;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}
	void setException(std::exception_ptr & exception) {
		checkState();
		if (isAdd) {
			throw std::logic_error("recall set exception");
		}
		(*attributes).lock.lock();
		(*attributes).isExceptionSet = true;
		(*attributes).exception = exception;
		(*attributes).check.notify_one();
		(*attributes).isReady = true;
		(*attributes).lock.unlock();
		isAdd = true;
	}
	Future<void> getFuture() {
		checkState();
		if (isFutureGet) {
			throw std::logic_error("re-creation future");
		}
		Future<void> f;
		f.attributes = attributes;
		isFutureGet = true;
		return f;
	}
private:
	std::shared_ptr<sharedVariables<void> > attributes;
	bool isAdd = false;
	bool isFutureGet = false;
	void checkState() {
		if (!attributes) {
			throw std::runtime_error("Promise is invalidate");
		}
	}
};