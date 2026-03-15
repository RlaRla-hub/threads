#pragma once

#include <thread>
#include <string>
#include <shared_mutex>
#include <future>


class ReadersWriter
{
private:
	std::string tableOfRecords = "record 0";
	mutable std::shared_mutex sharedMutex;

	std::string read() const
	{
		std::shared_lock<std::shared_mutex> lock(sharedMutex);
		return tableOfRecords;
	}

	void write(const std::string& tableOfRecordsNew)
	{
		std::unique_lock<std::shared_mutex> lock(sharedMutex);
		tableOfRecords = tableOfRecordsNew;
	}

public:
	void reader(std::promise<std::string>&& promise)
	{
		promise.set_value(read());
	}

	void writer(const std::string& tableOfRecordsNew)
	{
		write(tableOfRecordsNew);
	}
};
