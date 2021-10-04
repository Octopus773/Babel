

#include <catch2/catch.hpp>
#define protected public
#define private public
#include "Utilities/TSQueue.hpp"

TEST_CASE("pushFront", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushFront(1);
	CHECK(queue._queue.front() == 1);
}

TEST_CASE("pushBack", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushBack(1);
	CHECK(queue._queue.back() == 1);
}

TEST_CASE("front", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushFront(1);
	CHECK(queue._queue.front() == queue.front());
}

TEST_CASE("back", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushBack(1);
	CHECK(queue._queue.back() == queue.back());
}

TEST_CASE("popFront", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;
	queue.pushFront(1);

	int expected_popped = std::move(queue._queue.front());
	int actual_popped = queue.popFront();

	CHECK(expected_popped == actual_popped);
	CHECK(queue._queue.empty() == true);
}

TEST_CASE("popBack", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;
	queue.pushBack(1);

	int expected_popped = std::move(queue._queue.back());
	int actual_popped = queue.popBack();

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

	queue.pushBack(1);
	queue.pushBack(2);
	queue.pushBack(3);
	queue.clear();
	CHECK(queue._queue.empty() == true);
}

TEST_CASE("several_pushFront", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushFront(1);
	queue.pushFront(2);
	queue.pushFront(3);
	queue.pushFront(4);
	CHECK(queue._queue.front() == 4);
}

TEST_CASE("several_pushBack", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushBack(1);
	queue.pushBack(2);
	queue.pushBack(3);
	queue.pushBack(4);
	CHECK(queue._queue.back() == 4);
}

TEST_CASE("popBackFull_list", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushBack(1);
	queue.pushBack(2);
	queue.pushBack(3);
	queue.pushBack(4);
	int expected_value = std::move(queue._queue.back());
	int actual_value = queue.popBack();
	CHECK(expected_value == actual_value);
}

TEST_CASE("popFrontFull_list", "[Babel][TSQueue]")
{
	Babel::TSQueue<int> queue;

	queue.pushBack(1);
	queue.pushBack(2);
	queue.pushBack(3);
	queue.pushBack(4);
	int expected_value = std::move(queue._queue.front());
	int actual_value = queue.popFront();
	CHECK(expected_value == actual_value);
}