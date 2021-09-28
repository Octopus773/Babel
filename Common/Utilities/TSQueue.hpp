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
		~TSQueue() { clear(); };

		const T& front()
		{
			std::scoped_lock lock(_mutex);
			return _queue.front();
		}

		const T& back()
		{
			std::scoped_lock lock(_mutex);
			return _queue.back();
		}

		T pop_front()
		{
			std::scoped_lock lock(_mutex);
			auto t = std::move(_queue.front());
			_queue.pop_front();
			return t;
		}

		T pop_back()
		{
			std::scoped_lock lock(_mutex);
			auto t = std::move(_queue.back());
			_queue.pop_back();
			return t;
		}

		void push_front(const T& t)
		{
			std::scoped_lock lock(_mutex);
			_queue.push_front(t);
		}

		void push_back(const T& t)
		{
			std::scoped_lock lock(_mutex);
			_queue.push_back(t);
		}

		bool empty()
		{
			std::scoped_lock lock(_mutex);
			return _queue.empty();
		}

		size_t count()
		{
			std::scoped_lock lock(_mutex);
			return _queue.size();
		}

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