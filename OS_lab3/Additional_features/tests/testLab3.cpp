#define BOOST_TEST_MODULE testLab3
#include "../MarkerFunction.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(test_printArray)
{
    int test1[] = {1, 2, 3, 4, 5};
    BOOST_CHECK_EQUAL(printArray(test1, 5), "1 2 3 4 5");

    int test2[] = {1, 5, 5, 3, 1, 9, 9, 9};
    BOOST_CHECK_EQUAL(printArray(test2, 8), "1 5 5 3 1 9 9 9");

    int test3[] = {1, 0, 2, 4};
    BOOST_CHECK_EQUAL(printArray(test3, 4), "1 0 2 4");
}

BOOST_AUTO_TEST_CASE(test_isNaturalNumber)
{
    BOOST_CHECK_EQUAL(isNaturalNumber(-1), false);
    BOOST_CHECK_EQUAL(isNaturalNumber(INT_MAX), true);
    BOOST_CHECK_EQUAL(isNaturalNumber(19), true);
    BOOST_CHECK_EQUAL(isNaturalNumber(0), false);
}

BOOST_AUTO_TEST_CASE(test_removeMarksFromElements)
{
    int test_init1[] = {1, 2, 3, 4, 5};
    int test_result1[] = {1, 0, 3, 4, 5};
    removeMarksFromElements(test_init1, 5, 2);
    BOOST_CHECK_EQUAL_COLLECTIONS( test_init1, test_init1+5, test_result1, test_result1+5);

    int test_init2[] = {1, 5, 5, 3, 1, 9, 9, 9};
    int test_result2[] = {1, 5, 5, 3, 1, 0, 0, 0};
    removeMarksFromElements(test_init2, 8, 9);
    BOOST_CHECK_EQUAL_COLLECTIONS( test_init2, test_init2+8, test_result2, test_result2 + 8);

    int test_init3[] = {1, 0, 2, 4};
    int test_result3[] = {1, 0, 2, 4};
    removeMarksFromElements(test_init3, 4, 3);
    BOOST_CHECK_EQUAL_COLLECTIONS( test_init3, test_init3+4, test_result3, test_result3 + 4);
}
