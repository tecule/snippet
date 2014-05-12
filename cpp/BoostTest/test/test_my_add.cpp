#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testBoost

#include "boost/test/unit_test.hpp"
#include "my_add.h"

BOOST_AUTO_TEST_SUITE(addTest)

BOOST_AUTO_TEST_CASE (test1)
{
    BOOST_CHECK(my_add(1, 1) == 2);
}

BOOST_AUTO_TEST_CASE (test2)
{
    BOOST_CHECK(my_add(1, 1) == 3);
}

BOOST_AUTO_TEST_SUITE_END()

