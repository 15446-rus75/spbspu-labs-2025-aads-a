#define BOOST_TEST_MODULE test credentials
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_test)
{
  int i = 1;
  BOOST_TEST(i);
  BOOST_TEST(i == 2);
}
