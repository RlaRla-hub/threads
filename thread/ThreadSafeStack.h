/*Ѕезопасный стек : –еализуйте класс ThreadSafeStack(на основе std::vector или std::stack)
с операци€ми push и pop.«ащитите внутренние данные мьютексом.pop должен корректно обрабатывать случай пустого стека
*/

//std::optional<T> top() const должен делать возврат по ссылке, чтобы не было лишнего копировани€
//реализовать метод emplace_back() 
//сделать возврат значени€ в операции pop() 
//потокобезопасность 


#pragma once

#include <vector>
#include <optional>

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

	void push(const T& value)
	{
		try
		{
			values.push_back(value);
		}
		catch(std::bad_alloc exp)
		{
			std::cerr << "—тек переполнен, недостаточно пам€ти " << exp.what() << "\n";
		}
	}

	void push(T&& value)
	{
		try
		{
			values.push_back(std::move(value));
		}
		catch (std::bad_alloc exp)
		{
			std::cerr << "—тек переполнен, недостаточно пам€ти " << exp.what() << "\n";
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