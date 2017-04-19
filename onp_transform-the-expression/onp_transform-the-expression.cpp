// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// ONP - Transform the Expression
// This problem's URL:
// http://www.spoj.com/problems/ONP/


#include <iostream>
#include <stack>
#include <unordered_map>
using namespace std;


static unordered_map<char, int> ops = {
    {'+', 1},
    {'-', 2},
    {'*', 3},
    {'/', 4},
    {'^', 5},
    {'(', 0},
    {')', 0}
};

void transform(char *s)
{
    stack<char> operations;
    char c;
    for(; (c = *s++) != 0;) {
        auto f = ops.find(c);
        if(f == ops.end()) {
            cout << c;
            continue;
        }
        if(c == '(') {
            operations.push(c);
            continue;
        } 
        if(c == ')') {
            for(;;){
                c = operations.top();
                operations.pop();
                if(c == '(')
                    break;
                cout << c;
            }
            continue;
        }
        int p = f->second;
        while(operations.size()) {
            char top = operations.top();
            if(ops[top] <= p)
                break;
            cout << top;
            operations.pop();
        }
        operations.push(c);
    }
    while(operations.size()) {
        cout << operations.top();
        operations.pop();
    }
    cout << endl;
}

int main(int argc, char * argv[])
{
    int t;
    cin >> t;
    while(t--) {
        char s[401];
        cin >> s;
        transform(s);
    }
    return 0;
}