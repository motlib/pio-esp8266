#include "../unity/src/unity.h"

#include <stdio.h>

#include <string.h>

#include "../../src/utils/vfct.h"

#include "../../src/utils/vfct.cpp"


static uint32_t testval;

static uint8_t get_val(uint32_t *val)
{
    if(testval == 666)
    {
        return VFCT_STAT_NO_INIT;
    }
    else
    {
        *val = testval;
    }

    return VFCT_STAT_OK;
}

static uint8_t set_val(uint32_t *val)
{
    if(*val > 5)
    {
        testval = *val;
    }
    else
    {
        return VFCT_STAT_RANGE;
    }
}


static vfct_t const get_vfct = { .type = vfct_type_get_u32,  .fct = { .get_u32 = get_val } };
static vfct_t const set_vfct = { .type = vfct_type_set_u32,  .fct = { .set_u32 = set_val } };


#define ASSERT_STRING_EQ(A, B) if(strcmp((A), (B)) != 0) { printf("Error!\n"); } else { printf("Pass!\n"); }

static void test_u32_get(void)
{
    char buf[100];

    /* 17 is a normal value without special effect. */
    testval = 17;
    
    int len = vfct_fmt(buf, 100, &get_vfct);

    TEST_ASSERT_EQUAL_STRING("17", buf);
    TEST_ASSERT_EQUAL(2, len);
}



static void test_u32_get_no_init(void)
{
    char buf[100];

    /* 666 causes a NO_INIT return value. */
    testval = 666;
    
    int len = vfct_fmt(buf, 100, &get_vfct);

    TEST_ASSERT_EQUAL_STRING("E:3", buf);
    TEST_ASSERT_EQUAL(2, len);
}


int main(int argc, char **argv)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_u32_get);
    RUN_TEST(test_u32_get_no_init);

    UNITY_END();

    return 0;
}
        
