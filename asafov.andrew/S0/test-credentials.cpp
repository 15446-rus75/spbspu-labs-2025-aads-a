#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  asafov::outCredentials(out);
  BOOST_TEST(out.str() == "asafov.andrew");
}
