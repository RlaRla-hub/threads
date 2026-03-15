#include <iostream>
#include <windows.h>

#include "ThreadSafeStack.h"
#include <thread>

#include <mutex>
#include <chrono>

#include "ReadersWriter.h"
#include <future>


std::mutex mutex;
int counterMutex = 0;

std::atomic<int> counterAtomic = 0;


int main()
{
	SetConsoleOutputCP(1251);  
	SetConsoleCP(1251);        

	/*Задача 1.	Безопасный стек : 
	 Реализуйте класс ThreadSafeStack
     (на основе std::vector или std::stack) с операциями push и pop.
     Защитите внутренние данные мьютексом.
     pop должен корректно обрабатывать случай пустого стека */

	std::cout << "********** Задача 1 ********** \n";

	ThreadSafeStack<std::string> stack;

	stack.push("communist cat say mao mao");
	stack.push("fox say ???");
	stack.push("i say lalalalalalala");

	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	if (!stack.pop()) std::cout << "Стек пуст\n";
	
	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	if (!stack.pop()) std::cout << "Стек пуст\n";

	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	if (!stack.pop()) std::cout << "Стек пуст\n";

	ThreadSafeStack<std::pair<float, std::string>> stackPair;

	stackPair.push(std::make_pair(3.14f, "PI" ));
	std::optional<std::pair<float, std::string>> result = stackPair.top();
	result ? std::cout << result.value().first << " " << result.value().second << "\n" : std::cout << "Элемент не найден \n";
	if (!stackPair.pop()) std::cout << "Стек пуст\n";

	stackPair.emplace(5.45, "kalibr");
	result = stackPair.top();
	result ? std::cout << result.value().first << " " << result.value().second << "\n" : std::cout << "Элемент не найден \n";
	if (!stackPair.pop()) std::cout << "Стек пуст\n";

	std::thread t1([&stack]() { for (int i = 0; i < 30; ++i) { stack.push("communist cat say mao mao"); } });
	std::thread t2([&stack]() { for (int i = 0; i < 30; ++i) { stack.push("fox say ???"); } });
	std::thread t3([&stack]() { for (int i = 0; i < 30; ++i) { stack.push("i say lalalalalalala"); } });

	t1.join();
	t2.join();
	t3.join();

	std::cout << "size: " << stack.size() << "\n";

	/*Задача 2.	
	  Создайте глобальный целочисленный счетчик, инициализированный нулем.
      Запустите 100 потоков, каждый из которых увеличивает счетчик на 1 миллион раз.
      Используйте std::mutex для защиты. Проверьте итоговое значение.
      Решите задачу эту же задачу, но используйте std::atomic<int>.
      Сравните производительность с мьютекс - версией.*/

	std::cout << "********** Задача 2 ********** \n";

	std::chrono::steady_clock::time_point startMutex = std::chrono::steady_clock::now();
	
	std::vector<std::thread> threadPullMutex;
	for (int i = 0; i < 100; ++i)
	{
		threadPullMutex.emplace_back([]() {std::lock_guard<std::mutex> lock(mutex); 
		                                   std::this_thread::sleep_for(std::chrono::milliseconds(10));
										   ++counterMutex; });
	}

	for (std::thread& t : threadPullMutex)
	{
		t.join();
	}

	std::cout << "counterMutex: " << counterMutex << "\n";

	std::chrono::steady_clock::time_point endMutex = std::chrono::steady_clock::now();
	std::cout << "Time Mutex: " << std::chrono::duration_cast<std::chrono::milliseconds>(endMutex - startMutex).count() << "\n";


	std::chrono::steady_clock::time_point startAtomic = std::chrono::steady_clock::now();

	std::vector<std::thread> threadPullAtomic;
	for (int i = 0; i < 100; ++i)
	{
		threadPullAtomic.emplace_back([]() {std::this_thread::sleep_for(std::chrono::milliseconds(10));
		                                    ++counterAtomic; });
	}

	for (std::thread& t : threadPullAtomic)
	{
		t.join();
	}

	std::cout << "counterAtomic: " << counterAtomic << "\n";

	std::chrono::steady_clock::time_point endAtomic = std::chrono::steady_clock::now();
	std::cout << "Time Atomic: " << std::chrono::duration_cast<std::chrono::milliseconds>(endAtomic - startAtomic).count() << "\n";


	/*Задача 3.	Читатели и писатели(упрощенный) : Создайте общий ресурс(например, строку).
		Несколько потоков - «читателей» должны одновременно читать строку, 
		а один поток - «писатель» — периодически ее менять.
		Используйте std::shared_mutex(shared_lock для чтения, unique_lock для записи).*/ 

	std::cout << "********** Задача 3 ********** \n";

	ReadersWriter rw;
	std::vector<std::string> dataRecords = { "record 3", "record 7", "record 2", "record 9", "record 5" };
	std::vector<std::thread> threadPullReaders;
	std::vector<std::future<std::string>> futuresRW;

	
	std::thread threadWriter([&dataRecords, &rw]() {
	for (size_t i = 0; i < 20; ++i)
	{
		for (size_t j = 0; j < 5; ++j)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			rw.writer(dataRecords[j]);
		}
	}
		});

	
	size_t countUser = 0;
	for (size_t i = 0; i < 20; ++i)
	{
		for (size_t j = 0; j < 5; ++j)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			std::promise<std::string> promiseRW;
			std::future<std::string> futureRW = promiseRW.get_future();
			futuresRW.push_back(std::move(futureRW));
			threadPullReaders.emplace_back([&rw, promiseRW = std::move(promiseRW)]() mutable { rw.reader(std::move(promiseRW)); });
		}
	}


	threadWriter.join();

	for (std::future<std::string>& future : futuresRW)
	{
		std::cout << "Отправляем данные пользователю " << ++countUser << ": " << future.get() << "\n";
	}

	for (std::thread &t : threadPullReaders)
	{
		t.join();
	}
}

