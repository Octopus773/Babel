//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <mutex>
#include <deque>

namespace Babel
{
	//! @brief Thread Safe queue
	//! @note Should be the same functionality as std::queue
	template<typename T>
	class TSQueue
	{
	public:
		TSQueue() = default;
		TSQueue(const TSQueue<T>&) = delete;
		~TSQueue() { this->clear(); };

		//!	@brief Returns a constant reference to the first element of the queue
		const T& front()
		{
			std::scoped_lock lock(_mutex);
			return _queue.front();
		}

		//!	@brief Returns a constant reference to the last element of the queue
		const T& back()
		{
			std::scoped_lock lock(_mutex);
			return _queue.back();
		}

		//!	@brief Removes and returns the first element of the queue
		T popFront()
		{
			std::scoped_lock lock(_mutex);
			auto t = std::move(_queue.front());
			_queue.pop_front();
			return t;
		}

		//!	@brief Removes and returns the last element of the queue
		T popBack()
		{
			std::scoped_lock lock(_mutex);
			auto t = std::move(_queue.back());
			_queue.pop_back();
			return t;
		}

		//!	@brief Adds an element to the beginning of the queue
		void pushFront(const T& t)
		{
			std::scoped_lock lock(_mutex);
			_queue.push_front(t);
		}

		//!	@brief Adds an element to the beginning of the queue
		void pushBack(const T& t)
		{
			std::scoped_lock lock(_mutex);
			_queue.push_back(t);
		}

		//!	@brief Returns true if the queue is empty, false otherwise
		bool empty()
		{
			std::scoped_lock lock(_mutex);
			return _queue.empty();
		}

		//!	@brief Returns the number of elements in the queue
		size_t count()
		{
			std::scoped_lock lock(_mutex);
			return _queue.size();
		}

		//!	@brief Removes every elements in the queue
		void clear()
		{
			std::scoped_lock lock(_mutex);
			_queue.clear();
		}

	protected:
		std::mutex _mutex;
		std::deque<T> _queue;
	};
}