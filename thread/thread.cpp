#include <iostream>
#include "ThreadSafeStack.h"
#include <thread>


int main()
{
	ThreadSafeStack<std::string> stack;

	stack.push("communist cat say mao mao");
	stack.push("fox say ???");
	stack.push("i say lalalalalalala");

	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	stack.pop();

	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	stack.pop();

	stack.top() ? std::cout << *stack.top() << "\n" : std::cout << "Элемент не найден \n";
	stack.pop();

	/*std::thread t1([&stack, &str1]() { for (int i = 0; i < 30; ++i) { stack.push("communist cat say mao mao"); } });
	std::thread t2([&stack, &str2]() { for (int i = 0; i < 30; ++i) { stack.push("fox say ???"); } });
	std::thread t3([&stack, &str3]() { for (int i = 0; i < 30; ++i) { stack.push("i say lalalalalalala"); } });*/

	
}

