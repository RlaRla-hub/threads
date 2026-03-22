#pragma once

#include<iostream>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<functional>


template <typename T>
class ProducerConsumer
{
private:
	std::queue<T> values;
	size_t sizeQueue;
	std::condition_variable wt;
	std::mutex mutex;

	template <typename U>
	void produce(U&& value, std::unique_lock<std::mutex>& lock)
	{
		wt.wait(lock, [this]() { return values.size() < sizeQueue; });
		values.push(std::forward<U>(value));
		wt.notify_one();
	}

	T consume(std::unique_lock<std::mutex>& lock)
	{
		wt.wait(lock, [this]() { return !values.empty(); });
		T result = std::move(values.front());
		values.pop();
		wt.notify_one();
		return result;
	}


public:
	explicit ProducerConsumer(size_t sizeQueue) : sizeQueue(sizeQueue) {};

	template <typename U>
	void producerValue(U&& value)
	{
		std::unique_lock<std::mutex> lock(mutex);
		produce(std::forward<U>(value), lock);
	}

	template <typename H>
	void producerFunc(H&& handler)
	{
		std::unique_lock<std::mutex> lock(mutex);
		produce(std::forward<H>(handler)(), lock);
	}

	template <typename H>
	void consumer(H&& handler)
	{
		std::unique_lock<std::mutex> lock(mutex);
		handler(consume(lock));
	}

};

