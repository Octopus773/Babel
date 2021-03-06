//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>

namespace Babel
{
	//! @brief Thread Safe queue
	//! @note Should be the same functionality as std::queue
	template<typename T>
	class TSQueue
	{
	public:
		TSQueue() = default;

		TSQueue(const TSQueue<T> &) = delete;

		~TSQueue()
		{
			this->clear();
		};

		//!	@brief Returns a constant reference to the first element of the queue
		const T &front()
		{
			std::scoped_lock lock(this->_mutex);
			return this->_queue.front();
		}

		//!	@brief Returns a constant reference to the last element of the queue
		const T &back()
		{
			std::scoped_lock lock(this->_mutex);
			return this->_queue.back();
		}

		//!	@brief Removes and returns the first element of the queue
		T popFront()
		{
			std::scoped_lock lock(this->_mutex);
			auto t = std::move(this->_queue.front());
			this->_queue.pop_front();
			return t;
		}

		//!	@brief Removes and returns the last element of the queue
		T popBack()
		{
			std::scoped_lock lock(this->_mutex);
			auto t = std::move(this->_queue.back());
			this->_queue.pop_back();
			return t;
		}

		//!	@brief Adds an element to the beginning of the queue
		void pushFront(const T &t)
		{
			std::scoped_lock lock(this->_mutex);
			this->_queue.push_front(t);
			this->_blocker.notify_one();
		}

		//!	@brief Adds an element to the beginning of the queue
		void pushBack(const T &t)
		{
			std::scoped_lock lock(this->_mutex);
			this->_queue.push_back(t);
			this->_blocker.notify_one();
		}

		//!	@brief Returns true if the queue is empty, false otherwise
		bool empty()
		{
			std::scoped_lock lock(this->_mutex);
			return this->_queue.empty();
		}

		//!	@brief Returns the number of elements in the queue
		size_t count()
		{
			std::scoped_lock lock(this->_mutex);
			return this->_queue.size();
		}

		//!	@brief Removes every elements in the queue
		void clear()
		{
			std::scoped_lock lock(this->_mutex);
			this->_queue.clear();
		}

		//! @brief Block a thread until the queue is non empty or the timeout is reached
		//! @param time The number time before release
		template<typename TimeType>
		void waitFor(std::chrono::duration<TimeType> time)
		{
			using namespace std::chrono_literals;
			std::unique_lock<std::mutex> ul(this->_waitMutex);
			this->_blocker.wait_for(ul, time);
		}

	protected:
		std::mutex _mutex;
		std::deque<T> _queue;
		std::condition_variable _blocker;
		std::mutex _waitMutex;
	};
}