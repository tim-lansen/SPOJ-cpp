// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// EBOXES - Empty Boxes
// This problem's URL:
// http://www.spoj.com/problems/EBOXES/
//
// The description is unclear
// N = initial number of boxes
// n0 = number of selected in first step
// K = number of smaller boxes placed at every step
// k(1), k(2), ... k(t) = number of selected *smallest empty* boxes
// Every i step adds K*k(i) empty boxes and fills k(i) boxes
// Number of empty boxes
// E = N - n0 + K*n0 + (K*k(1) - k(1)) + ... + (K*k(t) - k(t))
// E = N + K*(n0 + k(1) + ... + k(t)) - (n0 + k(1) + ... + k(t))
// (E - N)/(K - 1) = n0 + k(1) + ... + k(t)
// Total number of boxes is
// T = N + K*n0 + K*k(1) + ... + K*(t) = N + K*(n0 + k(1) + ... + k(t)) = N + K*((E - N)/(K - 1))

#include <cstdio>

int main()
{
    int cases, N, K, t, E;
    scanf("%d", &cases);
    for(; cases; --cases) {
        scanf("%d %d %d %d", &N, &K, &t, &E);
        printf("%d\n", (E - N) / (K - 1) + E);
    }
    return 0;
}
