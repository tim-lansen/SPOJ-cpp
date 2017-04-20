// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// ABCDEF - ABCDEF
// This problem's URL:
// http://www.spoj.com/problems/ABCDEF/


#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
using namespace std;


static int pdata[256];
static int v1[2000000];
static int v2[2000000];


int abcdef(int * a, int n)
{
    int i, j, k, cnt, p1, p2, val;
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


#define DEVELOP

int main()
{
    int n, val;

#ifdef DEVELOP
    n = 20;
    for(int i = 0; i < n; ++i) {
        val = rand() / 256;
        pdata[i] = val;
    }
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

    val = abcdef(pdata, n);

#ifdef DEVELOP
    time1 = std::chrono::system_clock::now();
    elapsed = time1 - time0;
    cout << "Answer: " << val << "  time: " << elapsed.count() << endl;
#else

    printf("%d\n", val);

#endif
    return 0;
}