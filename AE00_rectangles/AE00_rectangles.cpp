// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// AE00 - rectangles
// This problem's URL:
// http://www.spoj.com/problems/AE00/
//
// Descriotion from SPOJ:
//      Byteman has a collection of N squares with side 1.
//      How many different rectangles can he form using these squares?
//      Two rectangles are considered different if none of them can be rotated and moved to obtain the second one.
//      During rectangle construction, Byteman can neither deform the squares nor put any squares upon any other ones.
//
// Approach: straightforward.


#include <stdio.h>


#pragma optimize("ts", on)


#ifdef getchar_unlocked
#define gc getchar_unlocked
#else
#define gc getchar
#endif


inline void scanint(register unsigned int &x)
{
    register int c = gc();
    x = 0;
    for(; (c<48 || c>57); c = gc());
    for(; c>47 && c<58; c = gc())
    {
        x = (x << 1) + (x << 3) + c - 48;
    }
}

inline unsigned int calculate_rectangles(register unsigned int num)
{
    register unsigned int res = num;
    for(register unsigned int side_a = 2; ; ++side_a) {
        unsigned int used = side_a*side_a;
        if(used > num)
            break;
        res += 1 + (num - used) / side_a;
    }
    return res;
}

int main()
{
    unsigned int num;
    scanint(num);
    printf("%d", calculate_rectangles(num));
    return 0;
}


