// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// PALIN - The Next Palindrome
// This problem's URL:
// http://www.spoj.com/problems/PALIN/
//
// Description from SPOJ:
//      A positive integer is called a palindrome if its representation in the decimal system is the same when read from left to right and from right to left.
//      For a given positive integer K of not more than 1000000 digits, write the value of the smallest palindrome larger than K to output.
//      Numbers are always displayed without leading zeros.


#include <iostream>
#include <memory.h>
using namespace std;


inline size_t inc_from(char *s, size_t from)
{
    while(from) {
        from--;
        s[from]++;
        if(s[from] <= '9')
            break;
        s[from] = '0';
    }
    return from;
}

inline void palin(char *s)
{
    char b[2] = {0,0};
    size_t size = strlen(s);
    // Increment s
    size_t current = inc_from(s, size);
    if(s[0] == '0') {
        // Add '1' at beginning
        memset(s, '0', size);
        s[0] = '1';
        s[size] = '1';
        s[size + 1] = 0;
        cout << s << endl;
        return;
    }
    size_t i = size, j = 0;
    for(;; ++j) {
        if(j == size >> 1)
            break;
        i--;
        if(j > current) {
            s[i] = s[j] = '0';
            continue;
        }
        if(s[i] != s[j]) {
            if(i > current) {
                s[i] = s[j];
                continue;
            }
            if(s[i] < s[j]) {
                s[i] = s[j];
            } else {
                current = inc_from(s, i);
                s[i] = s[j];
            }
        }
    }
    cout << s << endl;
}

int main()
{
    int t;
    cin >> t;
    char *s = new char[1000002];
    while(t--) {
        cin >> s;
        palin(s);
    }
    delete []s;
    return 0;
}