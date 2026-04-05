#pragma once

#include<iostream>
#include<thread>
#include<mutex>

class RecursiveMutex
{
private:
	std::recursive_mutex rMutex;

public:
	void A()
	{
		for (int i = 0; i < 100; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::lock_guard<std::recursive_mutex>lock(rMutex);
			std::cout << "methodA()" << std::this_thread::get_id() << "\n";
			B();
		}
	}

	void B()
	{
		std::lock_guard<std::recursive_mutex>lock(rMutex);
		std::cout << "methodB()" << std::this_thread::get_id() << "\n";
	}
};