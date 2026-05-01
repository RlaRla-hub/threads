#include<iostream>
#include<vector>
#include<optional>
#include<memory>
#include<thread>
#include<mutex>
#include<shared_mutex>
#include<unordered_map>


template <typename Key, typename Value>
class ThreadSafeUnorderedMap
{
private:

	std::shared_mutex mutex;

	struct Node
	{
		Key key;
		Value value;
		std::unique_ptr<Node> next;

		Node(Key key_, Value value_, std::unique_ptr<Node> next_) : key(key_), value(value_), next(std::move(next_)) {}
	};

	std::vector<std::unique_ptr<Node>> buckets;
	size_t bucketCount;
	size_t size = 0;

	size_t makeHash(const Key& key)
	{
		std::hash<Key> hasher;
		return hasher(key) % bucketCount;
	}

	std::optional<Value> findKey(const Key& key)
	{
		size_t num = makeHash(key);
		Node* temp = buckets[num].get();

		while (temp != nullptr)
		{
			if (temp->key == key)
			{
				return temp->value;
			}
			temp = temp->next.get();
		}

		return std::nullopt;
	}


public:

	ThreadSafeUnorderedMap(size_t bucketCount_)
	{
		(bucketCount_ == 0) ? (bucketCount = 1) : (bucketCount = bucketCount_);
		buckets.resize(bucketCount);
	}

	template <typename V>
	void operator[](const Key& key)
	{

	}


	void insert(const Key& key, const Value& value)
	{
		std::unique_lock<std::shared_mutex> lock(mutex);

		if (this->findKey(key).has_value())
		{
			return;
		}

		size_t num = makeHash(key);
		std::unique_ptr<Node> node = std::make_unique<Node>(key, value, std::move(buckets[num]));
		buckets[num] = std::move(node);
		++size;
	}

	void erase(const Key& key)
	{
		std::unique_lock<std::shared_mutex> lock(mutex);

		size_t num = makeHash(key);
		Node* temp = buckets[num].get();
		Node* prev = nullptr;

		while (temp != nullptr)
		{
			if (temp->key == key)
			{
				if (prev)
				{
					prev->next = std::move(temp->next);
				}
				else
				{
					buckets[num] = std::move(temp->next);
				}
				break;
			}
			prev = temp;
			temp = temp->next.get();
		}
	}

	std::optional<Value> find(const Key& key)
	{
		std::lock_guard<std::shared_mutex> lock(mutex);

		return findKey(key);
	}

	size_t getBucketCount()
	{
		std::lock_guard<std::shared_mutex> lock(mutex);
		return bucketCount;
	}

	size_t getSize()
	{
		std::lock_guard<std::shared_mutex> lock(mutex);
		return size;
	}

	void clear()
	{
		std::unique_lock<std::shared_lock> lock(mutex);
		for (size_t i = 0; i < buckets.size(); ++i)
		{
			if (buckets[i] == nullptr)
			{
				continue;
			}

			std::unique_ptr<Node> oldTemp = std::move(buckets[i]);
			std::unique_ptr<Node> temp = std::move(oldTemp->next);

			do
			{
				oldTemp.reset();
				oldTemp = std::move(temp);
				(oldTemp != nullptr) ? (temp = std::move(oldTemp->next)) : (temp = nullptr);

			} while (temp != nullptr);

		}
	}

	bool empty()
	{
		std::lock_guard<std::shared_mutex> lock(mutex);
		if (size == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

