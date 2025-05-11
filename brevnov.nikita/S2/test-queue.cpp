#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(constructors)
{
  brevnov::Queue< int > test_queue;
  test_queue.push(6);
  brevnov::Queue< int > test_copy_queue(test_queue);
  BOOST_TEST(test_copy_queue.front() == 6);
  brevnov::Queue< int > test_move_queue(std::move(test_copy_queue));
  BOOST_TEST(test_move_queue.front() == 6);
}

BOOST_AUTO_TEST_CASE(front_back)
{
  brevnov::Queue< int > test_queue;
  test_queue.push(5);
  test_queue.push(2);
  const int i = test_queue.front();
  BOOST_TEST(i == 5);
  BOOST_TEST(test_queue.front() == 5);
  const int a = test_queue.back();
  BOOST_TEST(a == 2);
  BOOST_TEST(test_queue.back() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  brevnov::Queue< int > test_queue;
  brevnov::Queue< int > test_queue2;
  test_queue2.push(9);
  test_queue = test_queue2;
  BOOST_TEST(test_queue.front() == 9);
  test_queue.push(8);
  test_queue2 = test_queue;
  BOOST_TEST(test_queue2.back() == 8);
}

BOOST_AUTO_TEST_CASE(push_and_pop)
{
  brevnov::Queue< int > test_queue;
  int i = 2;
  test_queue.push(i);
  BOOST_TEST(test_queue.front() == 2);
  test_queue.push(4);
  test_queue.push(8);
  BOOST_TEST(test_queue.back() == 8);
  test_queue.popFront();
  BOOST_TEST(test_queue.front() == 4);
  test_queue.popBack();
  BOOST_TEST(test_queue.back() == 4);
}

BOOST_AUTO_TEST_CASE(empty_size_clear)
{
  brevnov::Queue< int > arr;
  for (int i = 0; i < 15; ++i)
  {
    test_queue.push(i);
  }
  BOOST_TEST(test_queue.size() == 15);
  test_queue.clear();
  BOOST_TEST(test_queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
