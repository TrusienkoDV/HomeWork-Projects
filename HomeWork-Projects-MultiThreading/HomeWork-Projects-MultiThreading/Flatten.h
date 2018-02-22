#pragma once
#include "ThreadPool.h"
#include <memory>
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>
#include <atomic>

template<typename T>
Future<T> Flatten(T const that) {
	Promise<T> p;
	auto f = p.getFuture();
	p.set(that);
	return std::move(f);
}

template<typename T>
Future<T> Flatten(Future<T> that) {
    return std::move(that);
}

template<typename T>
struct Impl{
	using typeOfFuture = T;
};

template<typename T>
struct TupleImpl {
	using ans = T;
};

template<typename T>
struct TupleImpl<Future<T> > {
	using ans = typename Future<T>;
};

template<typename T>
struct TupleImpl<Future<Future<T> > > {
	using ans = typename TupleImpl<Future<T> >::ans;
};

template <typename T>
Future<typename Impl<Future<T> >::typeOfFuture> Flatten(Future<Future<T> > that) {
    std::shared_ptr<Promise<typename Impl<Future<T> >::typeOfFuture>> promise(new Promise<typename Impl<Future<T> >::typeOfFuture>());
	Future<typename Impl<Future<T> >::typeOfFuture> future = promise->getFuture();
	ThreadPool* pool = ThreadPool::thisThreadPool;
	std::function<void()> lambda = 
		[promise, that]() -> void {
		typename Impl<Future<T>>::typeOfFuture value = FlattenImpl(std::move(that));
		promise->set(std::move(value));
	};
	if (pool) {
		pool->execute(lambda);
	} else {
		std::thread thr(lambda);
		thr.detach();
	}
    return std::move(future);
}

template<template<typename , typename...> class C, typename T, typename A>
Future <C<T>> Flatten(C<Future<T>, A> & that) {
    Promise<C<T>> promise;
	Future<C<T> > future = promise.getFuture();

    std::thread values_getter = std::thread([](Promise<C<T>> & promise, C<Future<T>,A> & that) {
        C<T> a(that.size());
        auto curr = a.begin();
        for (auto item = that.begin(); item != that.end(); ++item) {
            *curr = item->get();
        }
        promise.set(a);
    },std::move(promise), std::ref(that));

    values_getter.detach();
	
    return std::move(future);
}

template<class Tuple, std::size_t... Ind>
auto FlatTup(Tuple tup, std::index_sequence<Ind...>) {
	std::tuple <typename Impl<typename std::tuple_element < Ind, Tuple >::type>::typeOfFuture ...> t;
	std::shared_ptr<Promise<decltype(t)> > p(new Promise<decltype(t)>);
	auto f = p->getFuture();
	ThreadPool* pool = ThreadPool::thisThreadPool;
	std::function<void()> lambda = 
		[p, tup]() -> void {
		p->set(std::move(std::make_tuple(std::move(Flatten(std::move(std::get<Ind>(tup))).get())...)));
	};
	if (pool) {
		pool->execute(lambda);
	}
	else {
		std::thread thr(lambda);
		thr.detach();
	}
	return std::move(f);
}

template<class... Types1>
auto Flatten(std::tuple<Types1...> tup) { 
	return FlatTup(std::move(tup), std::make_index_sequence<std::tuple_size<std::tuple<Types1...> >::value>());
}

template <typename T>
struct Impl<Future<T> > {
	using typeOfFuture = T;
};

template <typename T>
struct Impl<const Future<T> &> {
	using typeOfFuture = typename Impl<T>::typeOfFuture;
};

template <typename T>
struct Impl<Future<Future<T> > > {
	using typeOfFuture = typename Impl<Future<T>>::typeOfFuture;
};

template <typename T>
typename Impl<Future<T> >::typeOfFuture FlattenImpl(Future<T> that) {
	return std::move(that.get());
}

template <typename T>
typename Impl<Future<T>>::typeOfFuture FlattenImpl(Future<Future<T>> that) {
	return std::move(FlattenImpl(std::move(that.get())));
}

