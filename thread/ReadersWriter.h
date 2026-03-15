#pragma once

#include <thread>
#include <string>
#include <shared_mutex>


class ReadersWriter
{
private:
	std::string tableOfRecords;
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
	void writer()
	{

	}

	void reader()
	{

	}
}
