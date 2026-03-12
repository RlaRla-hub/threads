/*Ѕезопасный стек : –еализуйте класс ThreadSafeStack(на основе std::vector или std::stack)
с операци€ми push и pop.«ащитите внутренние данные мьютексом.pop должен корректно обрабатывать случай пустого стека
*/

//std::optional<T> top() const должен делать возврат по ссылке, чтобы не было лишнего копировани€
//потокобезопасность 


#pragma once

#include <vector>
#include <optional>
#include <utility>
#include <mutex>
#include <functional>

template <typename T>
class ThreadSafeStack
{
private:
	std::vector<T> values;
	mutable std::mutex mutex;

public:

	std::optional<T> pop() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
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
		std::lock_guard<std::mutex> lock(mutex);
		values.push_back(std::forward<U>(value));
	}


	template <typename...Args>
	void emplace(Args&&...args)
	{
		std::lock_guard<std::mutex> lock(mutex);
		values.emplace_back(std::forward<Args>(args)...);
	}

	std::optional<T> top() const&
	{
		std::lock_guard<std::mutex> lock(mutex);
		return !values.empty() ? std::optional<T>(values.back()) : std::nullopt;
	}

	std::optional<std::reference_wrapper<const T>> top() const&& {
		std::lock_guard<std::mutex> lock(mutex);
		return !values.empty()
			? std::optional<std::reference_wrapper<const T>>(std::cref(values.back()))
			: std::nullopt;
	}

	bool empty() const noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return values.empty();
	}

	size_t size() const noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		return values.size();
	}

	void clear() noexcept
	{
		std::lock_guard<std::mutex> lock(mutex);
		values.clear();
	}
};