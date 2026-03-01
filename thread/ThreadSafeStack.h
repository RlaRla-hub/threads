/*Безопасный стек : Реализуйте класс ThreadSafeStack(на основе std::vector или std::stack)
с операциями push и pop.Защитите внутренние данные мьютексом.pop должен корректно обрабатывать случай пустого стека
*/

//std::optional<T> top() const должен делать возврат по ссылке, чтобы не было лишнего копирования
//реализовать метод emplace_back() 
//потокобезопасность 


#pragma once

#include <vector>
#include <optional>
#include <utility>

template <typename T>
class ThreadSafeStack
{
private:
	std::vector<T> values;

public:

	std::optional<T> pop() noexcept
	{
		if (values.empty()) {
			return std::nullopt;
		}

		T result = std::move(values.back());  
		values.pop_back();
		return result;
	}


	template <typename U>
	void push(U&& value)
	{
		try
		{
			values.push_back(std::forward<U>(value));
		}
		catch(std::bad_alloc& exp)
		{
			std::cerr << "Стек переполнен, недостаточно памяти " << exp.what() << "\n";
		}
	}


	template <typename...Args>
	void emplace(Args&&...args)
	{
		try
		{
			values.emplace_back(std::forward<Args>(args)...);
		}
		catch (std::bad_alloc& exp)
		{
			std::cerr << "Стек переполнен, недостаточно памяти " << exp.what() << "\n";
		}
	}

	std::optional<T> top() const 
	{
		return !values.empty() ? std::optional<T>(values.back()) : std::nullopt;
	}

	bool empty() const noexcept
	{
		return values.empty();
	}

	size_t size() const noexcept
	{
		return values.size();
	}

	void clear() noexcept
	{
		values.clear();
	}
};