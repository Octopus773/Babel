

#include <catch2/catch.hpp>
#define protected public
#define private public
#include "Utilities/TSQueue.hpp"

TEST_CASE("push_front", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_front(1);
	CHECK(queue._queue.front() == 1);
}

TEST_CASE("push_back", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	CHECK(queue._queue.back() == 1);
}

TEST_CASE("front", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_front(1);
	CHECK(queue._queue.front() == queue.front());
}

TEST_CASE("back", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	CHECK(queue._queue.back() == queue.back());
}

TEST_CASE("pop_front", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;
	queue.push_front(1);

	int expected_popped = std::move(queue._queue.front());
	int actual_popped = queue.pop_front();

	CHECK(expected_popped == actual_popped);
	CHECK(queue._queue.empty() == true);
}

TEST_CASE("pop_back", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;
	queue.push_back(1);

	int expected_popped = std::move(queue._queue.back());
	int actual_popped = queue.pop_back();

	CHECK(expected_popped == actual_popped);
	CHECK(queue._queue.empty() == true);
}

TEST_CASE("empty", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	CHECK(queue._queue.empty() == queue.empty());

	queue._queue.push_back(1);
	CHECK(queue._queue.empty() == queue.empty());
}

TEST_CASE("clear", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	queue.push_back(2);
	queue.push_back(3);
	queue.clear();
	CHECK(queue._queue.empty() == true);
}

TEST_CASE("several_push_front", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_front(1);
	queue.push_front(2);
	queue.push_front(3);
	queue.push_front(4);
	CHECK(queue._queue.front() == 4);
}

TEST_CASE("several_push_back", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	queue.push_back(2);
	queue.push_back(3);
	queue.push_back(4);
	CHECK(queue._queue.back() == 4);
}

TEST_CASE("pop_back_full_list", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	queue.push_back(2);
	queue.push_back(3);
	queue.push_back(4);
	int expected_value = std::move(queue._queue.back());
	int actual_value = queue.pop_back();
	CHECK(expected_value == actual_value);
}

TEST_CASE("pop_front_full_list", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.push_back(1);
	queue.push_back(2);
	queue.push_back(3);
	queue.push_back(4);
	int expected_value = std::move(queue._queue.front());
	int actual_value = queue.pop_front();
	CHECK(expected_value == actual_value);
}