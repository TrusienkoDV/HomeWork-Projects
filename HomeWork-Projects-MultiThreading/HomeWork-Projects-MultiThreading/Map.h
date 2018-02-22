#pragma once
#include "Flatten.h"


template<typename T, typename F>
Future<typename std::result_of<F(typename Impl<Future<T>>::typeOfFuture)>::type> Map(Future<T> fut, F const & func) {
	std::shared_ptr<Promise<typename std::result_of<F(typename Impl<Future<T>>::typeOfFuture)>::type>> 
		promise(new Promise<typename std::result_of<F(typename Impl<Future<T>>::typeOfFuture)>::type>());
	Future<typename std::result_of<F(typename Impl<Future<T>>::typeOfFuture)>::type> future = promise->getFuture();
	ThreadPool* pool = ThreadPool::thisThreadPool;
	std::function<void()> lambda =
		[promise, fut, &func]() -> void {
		auto f = Flatten(std::move(fut));
		auto value = func(f.get());
		promise->set(value);
	};
	if (pool) {
		pool->execute(lambda);
	} else {
		std::thread thr(lambda);
		thr.detach();
	}
	return std::move(future);
}