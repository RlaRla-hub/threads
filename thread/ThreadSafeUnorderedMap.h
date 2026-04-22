#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <memory>


template<typename Key, typename Value>
class UnorderedMap
{
private:

	struct Node
	{
	Key key;
	Value value;
	std::unique_ptr<Node> next;

	Node(Key key_,Value value_,std::unique_ptr<Node> next_) : key(key_), value(value_), next(std::move(next_)) {}
	};

	std::vector<std::unique_ptr<Node>> buckets;
	size_t numBuckets;

	size_t makeHash(const Key& key)
	{
		std::hash<Key> hasher;
		return hasher(key) % numBuckets;
	}


public:

	UnorderedMap(size_t size) :numBuckets(size)
	{
		buckets.resize(numBuckets);
	}

	template <typename V>
	void operator[](const V&& value)
	{

	}

	void insert(const Key& key, const Value& value)
	{
		if (this->find(key).has_value())
		{
			return;
		}

		size_t num = makeHash(key);
		std::unique_ptr<Node> node = std::make_unique<Node>(key, value, std::move(buckets[num]));
		buckets[num] = std::move(node);
	}

	void erase(const Key& key)
	{

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
				return;
			}
			prev = temp;
			temp = temp->next.get();
		}
	}

	std::optional<Value> find(const Key& key)
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

};

