#include "unity.h"

#include <stdio.h>
#include <string.h>

#include "utils/vfct.h"

/* Yes, we include the test objects from cpp file here. */
#include "utils/vfct.cpp"


static uint32_t testval;

static uint8_t get_val(uint32_t *val)
{
    if(testval == 666)
    {
        return VFCT_ERR_NO_INIT;
    }
    else
    {
        *val = testval;
    }

    return VFCT_ERR_OK;
}

static uint8_t set_val(uint32_t *val)
{
    if(*val > 5)
    {
        testval = *val;
        return VFCT_ERR_OK;
    }
    else
    {
        return VFCT_ERR_RANGE;
    }
}


static vfct_t const vfct_u32 = VFCT_DEF(u32, get_val, set_val);
static vfct_t const vfct_u32_no_set = VFCT_DEF(u32, get_val, NULL);


static void test_vfct_fmt_u32(void)
{
    char buf[100];

    /* 17 is a normal value without special effect. */
    testval = 17;
    
    int len = vfct_fmt(buf, 100, &vfct_u32);

    TEST_ASSERT_EQUAL_STRING("17", buf);
    TEST_ASSERT_EQUAL(2, len);
}


static void test_vfct_fmt_u32_no_init(void)
{
    char buf[100];

    /* 666 causes a NO_INIT return value. */
    testval = 666;
    
    int len = vfct_fmt(buf, 100, &vfct_u32);

    TEST_ASSERT_EQUAL_STRING("E:3", buf);
    TEST_ASSERT_EQUAL(3, len);
}


static void test_vfct_parse_u32(void)
{
    char const * val = "12";

    testval = 0;

    vfct_err_t err = vfct_parse(&vfct_u32, val);

    TEST_ASSERT_EQUAL(VFCT_ERR_OK, err);
    TEST_ASSERT_EQUAL(12, testval);
}


static void test_vfct_no_set_u32(void)
{
    char const * val = "12";

    testval = 0;

    vfct_err_t err = vfct_parse(&vfct_u32_no_set, val);

    TEST_ASSERT_EQUAL(VFCT_ERR_NO_OP, err);
    TEST_ASSERT_EQUAL(0, testval);
}


int main(int argc, char **argv)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_vfct_fmt_u32);
    RUN_TEST(test_vfct_fmt_u32_no_init);
    RUN_TEST(test_vfct_parse_u32);
    RUN_TEST(test_vfct_no_set_u32);
    
    UNITY_END();

    return 0;
}
        
