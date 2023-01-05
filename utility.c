#include "utility.h"

/* 大きい方の値を返す */
int maxU(int a, int b)
{
    if( a >= b ) {
        return a;
    }
    return b;
}

/* 小さい方の値を返す */
int minU(int a, int b)
{
    if( a <= b ) {
        return a;
    }
    return b;
}