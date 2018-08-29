#include "unity.h"

#include <stdio.h>
#include <string.h>

#define UPTIME_COUNTER test_millis

static uint32_t test_cnt = 0;

static uint32_t test_millis(void)
{
    return test_cnt;
}


/* Yes, we include the test objects from cpp file here. */
#include "uptime.cpp"


/* Test normal time handling */
static void test_uptime_start(void)
{
    uint32_t secs;
    uint8_t ret;

    test_cnt = 0;
    uptime_main();
        
    ret = uptime_get_seconds(&secs);
    
    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(0, secs);
}


/* Test time handling with millisecond counter overflow. */
static void test_uptime_count(void)
{
    uint32_t secs;
    uint8_t ret;

    test_cnt = 0;
    uptime_main();
    
    test_cnt = 1000;
    uptime_main();

    ret = uptime_get_seconds(&secs);
    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(1, secs);

    test_cnt = 1999;
    uptime_main();
    ret = uptime_get_seconds(&secs);
    
    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(1, secs);
    
    test_cnt = 2000;
    uptime_main();
    ret = uptime_get_seconds(&secs);
    
    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(2, secs);

    /* test overflow behavior of counter */
    test_cnt = UINT32_MAX;
    uptime_main();
    ret = uptime_get_seconds(&secs);

    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(4294967, secs);

    /* this overflows uint32. Still the uptime counter must work correctly. */
    test_cnt += 1000;
    uptime_main();
    ret = uptime_get_seconds(&secs);

    TEST_ASSERT_EQUAL(VFCT_ERR_OK, ret);
    TEST_ASSERT_EQUAL(4294967 + 1, secs);
}


int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_uptime_start);
    RUN_TEST(test_uptime_count);
    
    UNITY_END();

    return 0;
}
