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

	void produce(T value)
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (values.size() >= sizeQueue)
		{
			wt.wait(lock, [this, value]() { return values.size() < sizeQueue; });
		}
		else
		{
			values.push(value);
			wt.notify_one();
		}
	}

	T consume()
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (values.empty())
		{
			wt.wait(lock, [this]() { return !values.empty(); });
		}
		else
		{
			T result = values.front();
			values.pop();
			wt.notify_one();
			return result;
		}

	}


public:
	ProducerConsumer(size_t sizeQueue) : sizeQueue(sizeQueue) {};

	void producer(T& value)
	{
		produce(value);
	}

	template <typename U>
	void consumer(U&& handler)
	{
		handler(consume());
	}

};

