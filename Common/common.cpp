#include "common.h"

int limit_int(int value, int low, int high)
{
    if (value < low)
        return low;
    else if (value > high)
        return high;
    else
        return value;
}
