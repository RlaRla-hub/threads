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

	/*std::thread t1([&stack, &str1]() { for (int i = 0; i < 30; ++i) { stack.push("communist cat say mao mao"); } });
	std::thread t2([&stack, &str2]() { for (int i = 0; i < 30; ++i) { stack.push("fox say ???"); } });
	std::thread t3([&stack, &str3]() { for (int i = 0; i < 30; ++i) { stack.push("i say lalalalalalala"); } });*/

	
}

