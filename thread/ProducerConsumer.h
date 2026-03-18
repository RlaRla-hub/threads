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
			wt.wait(lock);
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
			wt.wait(lock);
		}
		else
		{
			values.pop();
			wt.notify_all();
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

