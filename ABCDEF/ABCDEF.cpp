// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// ABCDEF - ABCDEF
// This problem's URL:
// http://www.spoj.com/problems/ABCDEF/
//
// Approach: simplify expression, count cases
// (a*b + c)/d - e = f  =>  a*b + c = d*(e + f)
//
// abcdef:
// Build LEFT and RIGHT value arrays (N^3), sort, walk through
//
// abcdef2b:
// Sort source
// Build nXn multiplication and sum arrays (N*logN)
// Sort multiplication array
// Build LEFT and RIGHT value arrays using pre-built MUL and SUM arrays
// Sort RIGHT array
// Walk through every thread in LEFT array (1..N) vs RIGHT, adding ranges multiplication to result
//
// abcdef_radix:
// The same as above, but using RADIX sort
//
// abcdef3:
// Use std::map to store/count values (vey slow on data with wide range)
//
// abcdef3a:
// Use std::unordered_map to store/count values (vey slow on data with wide range)


#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <memory>
using namespace std;

//#define DEVELOP

static int pdata[256];
static int v1[2000000];
static int v2[2000000];


int abcdef(int * a, int n)
{
    int i, j, k, cnt, p1, p2;
    cnt = p1 = p2 = 0;
    for(i = 0; i < n; i++) {
        if(a[i]) {
            for(j = 0; j < n; j++)
                for(k = 0; k < n; k++)
                    v1[p1++] = a[i] * (a[j] + a[k]);
        }
    }
    sort(v1, v1 + p1);
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            for(k = 0; k < n; k++) {
                v2[p2++] = a[i] * a[j] + a[k];
            }
        }
    }
    sort(v2, v2 + p2);
    int L1 = 0, L2 = 0, R1 = p1, R2 = p2;
    for(; L1 < p1 && L2 < p2;) {
        int c1, c2;
        //L1 += lb(v1 + L1, p1 - L1, v2[L2]);
        //R1 = L1 + ub(v1 + L1, p1 - L1, v2[L2]);
        L1 = lower_bound(v1 + L1, v1 + p1, v2[L2]) - v1;
        R1 = upper_bound(v1 + L1, v1 + p1, v2[L2]) - v1;

        if(R1 > L1) {
            c1 = R1 - L1;
            //L2 += lb(v2 + L2, p2 - L2, v1[L1]);
            //R2 = L2 + ub(v2 + L2, p2 - L2, v1[L1]);
            L2 = lower_bound(v2 + L2, v2 + p2, v1[L1]) - v2;
            R2 = upper_bound(v2 + L2, v2 + p2, v1[L1]) - v2;
            c2 = R2 - L2;
            cnt += c1 * c2;
            L1 = R1;
            L2 = R2;
        } else {
            L2++;
        }
    }
    return cnt;
}


#define MAXVALS 100
int abcdef2(int * a, int n)
{
    // find every multiplication
    int m = 0, s = 0;
    int ai, aj;
    int * mul = v1;
    int * sum = &v1[MAXVALS*MAXVALS];
    int * left = v2, * right = &v2[MAXVALS*MAXVALS*MAXVALS];

    sort(a, a + n);
#ifdef DEVELOP
    // Dump mul & sum
    for(int i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
    cout << endl << endl;
#endif
    // Build matrices (n*n)
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        mul[m++] = ai * ai;
        sum[s++] = ai + ai;
        for(int j = i + 1; j < n; ++j) {
            aj = a[j] * ai; mul[m++] = aj; mul[m++] = aj;
            aj = a[j] + ai; sum[s++] = aj; sum[s++] = aj;
        }
    }
    sort(mul, mul + m);
    sort(sum, sum + s);

    // Build left & right value arrays
    int lsize = 0, rsize = 0;
    for(int j = 0; j < n; ++j) {
        for(int i = 0; i < m; ++i) {
            aj = a[j];
            left[lsize++] = mul[i] + aj;
            if(aj) right[rsize++] = sum[i] * aj;
        }
    }

    sort(left, left + lsize);
    sort(right, right + rsize);

    // Now count
    int count = 0;
    int * llp = left, * rlp = right, * lup, * rup, * le = left + lsize, * re = right + rsize;
    for(; llp < le && rlp < re;) {
        int lval = * llp;
        int rval = * rlp;
        if(lval == rval) {
            lup = upper_bound(llp, le, lval);
            rup = upper_bound(rlp, re, rval);
            count += (lup - llp) * (rup - rlp);
            llp = lup;
            rlp = rup;
            continue;
        }
        if(lval < rval) {
            llp = lower_bound(llp, le, rval);
        } else {
            rlp = lower_bound(rlp, re, lval);
        }
    }

    return count;
}

int abcdef2a(int * a, int n)
{
    // find every multiplication
    int m = 0, s = 0;
    int ai, aj;
    int * mul = v1;
    int * sum = &v1[MAXVALS*MAXVALS];
    int * left = v2, *right = &v2[MAXVALS*MAXVALS*MAXVALS];

    sort(a, a + n);
    // Build matrices (n*n)
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        mul[m++] = ai * ai;
        sum[s++] = ai + ai;
        for(int j = i + 1; j < n; ++j) {
            aj = a[j] * ai; mul[m++] = aj; mul[m++] = aj;
            aj = a[j] + ai; sum[s++] = aj; sum[s++] = aj;
        }
    }
    sort(mul, mul + m);
    sort(sum, sum + s);

    // Build left & right value arrays
    int lsize = 0, rsize = 0, rn = 0;;
    for(int j = 0; j < n; ++j) {
        aj = a[j];
        if(aj) {
            rn++;
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
                right[rsize++] = sum[i] * aj;
            }
        } else {
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
            }
        }
    }

    int count = 0;
    sort(right, right + rsize);
    
    int * llp = left;
    for(int i = 0; i < n; ++i) {
        int *rlp = right, *re = rlp + rsize;
        int * le = llp + m;
        int *lup, *rup;
        int vc = 0;
        int lval = *llp;
        int rval = *rlp;
        for(;;) {
            if(lval == rval) {
                lup = upper_bound(llp, le, lval);
                rup = upper_bound(rlp, re, rval);
                count += (lup - llp) * (rup - rlp);
                llp = lup;
                rlp = rup;
                if(llp >= le)
                    break;
                lval = *llp;
                if(rlp >= re)
                    break;
                rval = *rlp;
                continue;
            }
            if(lval < rval) {
                llp = lower_bound(llp, le, rval);
                if(llp >= le)
                    break;
                lval = *llp;
            } else {
                rlp = lower_bound(rlp, re, lval);
                if(rlp >= re)
                    break;
                rval = *rlp;
            }
        }
        llp = le;
    }

    return count;
}

int abcdef2b(int * a, int n)
{
    // find every multiplication
    int m = 0, s = 0;
    int ai, aj;
    int * mul = v1;
    int * sum = &v1[MAXVALS*MAXVALS];
    int * left = v2, *right = &v2[MAXVALS*MAXVALS*MAXVALS];

    sort(a, a + n);
    // Build matrices (n*n)
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        mul[m++] = ai * ai;
        sum[s++] = ai + ai;
        for(int j = i + 1; j < n; ++j) {
            aj = a[j] * ai; mul[m++] = aj; mul[m++] = aj;
            aj = a[j] + ai; sum[s++] = aj; sum[s++] = aj;
        }
    }
    sort(mul, mul + m);

    // Build left & right value arrays
    int lsize = 0, rsize = 0, rn = 0;;
    for(int j = 0; j < n; ++j) {
        aj = a[j];
        if(aj) {
            rn++;
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
                right[rsize++] = sum[i] * aj;
            }
        } else {
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
            }
        }
    }

    int count = 0;
    sort(right, right + rsize);

    int * llp = left;
    for(int i = 0; i < n; ++i) {
        int *rlp = right, *re = rlp + rsize;
        int * le = llp + m;
        int *lup, *rup;
        int vc = 0;
        int lval = *llp;
        int rval = *rlp;
        for(;;) {
            if(lval == rval) {
                lup = upper_bound(llp, le, lval);
                rup = upper_bound(rlp, re, rval);
                count += (lup - llp) * (rup - rlp);
                llp = lup;
                rlp = rup;
                if(llp >= le)
                    break;
                lval = *llp;
                if(rlp >= re)
                    break;
                rval = *rlp;
                continue;
            }
            if(lval < rval) {
                llp = lower_bound(llp, le, rval);
                if(llp >= le)
                    break;
                lval = *llp;
            } else {
                rlp = lower_bound(rlp, re, lval);
                if(rlp >= re)
                    break;
                rval = *rlp;
            }
        }
        llp = le;
    }

    return count;
}


static int radix_counts[1024];
static uint32_t array_a[1000000];
static uint32_t array_b[1000000];
static uint32_t array_size;
inline uint32_t digit(uint32_t n, int p)
{
    return (n >> (p << 3) & 255);
}
inline void radix_init()
{
    memset(radix_counts, 0, 1024 * sizeof(uint32_t));
    array_size = 0;
}
inline void radix_append(int x)
{
    int64_t x64 = x;
    x += 0x7FFFFFFF;
    uint32_t x32 = x;
    array_a[array_size++] = x;
    radix_counts[x32 & 0xFF]++;
    x32 >>= 8;
    radix_counts[0x100 + (x32 & 0xFF)]++;
    x32 >>= 8;
    radix_counts[0x200 + (x32 & 0xFF)]++;
    x32 >>= 8;
    radix_counts[0x300 + (x32 & 0xFF)]++;
}
void rsort(int * data)
{
    int *c = radix_counts;

    uint32_t *a = array_a;
    uint32_t *b = array_b;

    for(int i = 0; i < 4; ++i) {

        for(int j = 1; j < 256; ++j) {
            c[j] += c[j - 1];
        }

        for(int j = array_size - 1; j >-1; j--) {
            b[--c[digit(a[j], i)]] = a[j];
        }
        // Swap
        uint32_t *tmp = a;
        a = b;
        b = tmp;

        c += 256;
    }
    for(uint32_t i = 0; i < array_size; ++i) {
        int64_t x = a[i];
        x -= 0x7FFFFFFF;
        data[i] = (int)x;
    }
}


int abcdef_radix(int * a, int n)
{
    // find every multiplication
    int m = 0, s = 0;
    int ai, aj;
    int * mul = v1;
    int * sum = &v1[MAXVALS*MAXVALS];
    int * left = v2, *right = &v2[MAXVALS*MAXVALS*MAXVALS];

    sort(a, a + n);
    // Build matrices (n*n)
    radix_init();
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        aj = ai*ai;
        radix_append(aj);
        mul[m++] = aj;
        sum[s++] = ai + ai;
        for(int j = i + 1; j < n; ++j) {
            aj = a[j] * ai; mul[m++] = aj; mul[m++] = aj; radix_append(aj); radix_append(aj);
            aj = a[j] + ai; sum[s++] = aj; sum[s++] = aj;
        }
    }
    rsort(mul);

    // Build left & right value arrays
    radix_init();
    int lsize = 0, rsize = 0, rn = 0;
    for(int j = 0; j < n; ++j) {
        aj = a[j];
        if(aj) {
            rn++;
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
                ai = sum[i] * aj;
                right[rsize++] = ai; radix_append(ai);
            }
        } else {
            for(int i = 0; i < m; ++i) {
                left[lsize++] = mul[i] + aj;
            }
        }
    }

    int count = 0;
    rsort(right);

    int * llp = left;
    for(int i = 0; i < n; ++i) {
        int *rlp = right, *re = rlp + rsize;
        int * le = llp + m;
        int *lup, *rup;
        int vc = 0;
        int lval = *llp;
        int rval = *rlp;
        for(;;) {
            if(lval == rval) {
                lup = upper_bound(llp, le, lval);
                rup = upper_bound(rlp, re, rval);
                count += (lup - llp) * (rup - rlp);
                llp = lup;
                rlp = rup;
                if(llp >= le)
                    break;
                lval = *llp;
                if(rlp >= re)
                    break;
                rval = *rlp;
                continue;
            }
            if(lval < rval) {
                llp = lower_bound(llp, le, rval);
                if(llp >= le)
                    break;
                lval = *llp;
            } else {
                rlp = lower_bound(rlp, re, lval);
                if(rlp >= re)
                    break;
                rval = *rlp;
            }
        }
        llp = le;
    }

    return count;
}

int abcdef3(int * a, int n)
{
    // a*b + c == d*(f + e)
    // To make map's life easier
    sort(a, a + n);
    int z = 0;
    int ai, aj;
    int * mult2add = v2;
    map<int, int> mul, sum;
    // Build multiplications matrix (n*n)
    for(int i = 0; i < n; ++i) {
        if((ai = a[i]) == 0) z++;
        int m = ai * ai;
        int s = ai + ai;
        mul[m]++;
        sum[s]++;
        for(int j = i + 1; j < n; ++j) {
            m = a[j] * ai;
            s = a[j] + ai;
            mul[m] += 2;
            sum[s] += 2;
        }
    }
    map<int, int> left, right;
    // Build A+c and F+E matrices (n*n*n)
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        for(auto m: mul) {
            aj = ai + m.first;
            left[aj] += m.second;
        }
        if(ai) {
            for(auto s : sum) {
                aj = ai * s.first;
                right[aj] += s.second;
            }
        }
    }

    int count = 0;
    map<int, int>::iterator LB = left.begin(), LE = left.end();
    map<int, int>::iterator RB = right.begin(), RE = right.end();
    for(; LB != LE && RB != RE;) {
        if(LB->first == RB->first) {
            count += LB->second * RB->second;
            LB++;
            RB++;
            continue;
        }
        if(LB->first < RB->first) {
            LB++;
        } else {
            RB++;
        }
    }
    return count;
}

int abcdef3a(int * a, int n)
{
    // a*b + c == d*(f + e)
    // To make map's life easier
    sort(a, a + n);
    int z = 0;
    int ai, aj;
    int * mult2add = v2;
    unordered_map<int, int> mul, sum;
    // Build multiplications matrix (n*n)
    for(int i = 0; i < n; ++i) {
        if((ai = a[i]) == 0) z++;
        int m = ai * ai;
        int s = ai + ai;
        mul[m]++;
        sum[s]++;
        for(int j = i + 1; j < n; ++j) {
            m = a[j] * ai;
            s = a[j] + ai;
            mul[m] += 2;
            sum[s] += 2;
        }
    }
    unordered_map<int, int> left, right;
    // Build A+c and F+E matrices (n*n*n)
    for(int i = 0; i < n; ++i) {
        ai = a[i];
        for(auto m : mul) {
            aj = ai + m.first;
            left[aj] += m.second;
        }
        if(ai)
            for(auto s : sum) {
                aj = ai * s.first;
                right[aj] += s.second;
            }
    }

    int count = 0;
    unordered_map<int, int>::iterator LB = left.begin(), LE = left.end();
    unordered_map<int, int>::iterator RB = right.begin(), RE = right.end();
    for(; LB != LE;) {
        if((RB = right.find(LB->first)) != RE) {
            count += LB->second * RB->second;
        }
        LB++;
    }
    return count;
}


int main()
{
    int n, val;

#ifdef DEVELOP
    n = 100;
    for(int i = 0; i < n; ++i) {
        //val = (n * 2 * rand())/RAND_MAX - n;
        val = rand();// -(RAND_MAX / 2);
        pdata[i] = val;
    }
    pdata[0] = 0;
    pdata[1] = 0;
    //pdata[2] = 0;
    //int d[] = {5,7,10};
    //val = abcdef3(d, 3);
#else
    scanf("%d", &n);
    for(int i = 0; i < n; ++i) {
        scanf("%d", &pdata[i]);
    }
#endif

#ifdef DEVELOP

    std::chrono::time_point<std::chrono::system_clock> time0, time1, time2;
    std::chrono::duration<double> elapsed;
    time0 = std::chrono::system_clock::now();

#endif

    val = abcdef_radix(pdata, n);

#ifdef DEVELOP

    time1 = std::chrono::system_clock::now();
    elapsed = time1 - time0;
    cout << "Answer: " << val << "  time: " << elapsed.count() << endl;

    int val0 = abcdef2b(pdata, n);
    time2 = std::chrono::system_clock::now();
    elapsed = time2 - time1;
    cout << "Answer: " << val0 << "  time: " << elapsed.count() << endl;

    cout << val - val0 << endl;

#else

    printf("%d\n", val);

#endif
    return 0;
}