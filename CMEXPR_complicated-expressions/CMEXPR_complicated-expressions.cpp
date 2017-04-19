// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// CMEXPR - Complicated Expressions
// This problem's URL:
// http://www.spoj.com/problems/CMEXPR/
//
// Description from SPOJ:
//      The most important activity of ACM is the GSM network.
//      As the mobile phone operator, ACM must build its own transmitting stations.
//      It is very important to compute the exact behaviour of electro-magnetic waves.
//      Unfortunately, prediction of electro-magnetic fields is a very complex task and the formulas describing them are very long and hard-to-read.
//      For example, Maxwell's Equations describing the basic laws of electrical engineering are really tough.
//      ACM has designed its own computer system that can make some field computations and produce results in the form of mathematic expressions.
//      Unfortunately, by generating the expression in several steps, there are always some unneeded parentheses inside the expression.
//      Your task is to take these partial results and make them "nice" by removing all unnecessary parentheses.
//
// Possible approaches:
// 1. Dig: try to solve the problem recursively scanning input.
//      Seems to be a complex task for inserted brackets.
// 2. Expression tree: build an expression tree and then convert it to Infix format.
//      Okay, but i don't want to abuse execution with object creation/deletion.
//      However, the problem constraints let fixed set of nodes, and i can use static number of structures...
// 3. Use RPN: convert input expression to RPN, and then back to Infix.
//      So be it.


#include <stdio.h>
#include <cstring>
#include <stack>
#include <string>
#include <strstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;


#pragma optimize("ts", on)


#ifdef getchar_unlocked
#define gc getchar_unlocked
#else
#define gc getchar
#endif

// Convert string to integer
inline void scan_int(register int &x)
{
    register int c = gc();
    x = 0;
    for(; (c<48 || c>57); c = gc());
    for(; c>47 && c<58; c = gc()) {
        x = (x << 1) + (x << 3) + c - 48;
    }
}

// Read line from std-in
inline unsigned int scan_line(register char * p)
{
    register char c = gc();
    register int len = 0;
    while(c == '\r' || c == '\n')
        c = gc();
    while(c != '\r' && c != '\n') {
        len++;
        *p++ = c;
        c = gc();
    }
    *p = 0;
    return len;
}

struct OP_TRAITS
{
    char priority;
    char associative;
    const bool operator < (const OP_TRAITS & other)
    {
        if(priority == other.priority) {
            return !other.associative;
        }
        return priority < other.priority;
    }
    const bool operator > (const OP_TRAITS & other) { return priority > other.priority; }
    const bool operator <= (const OP_TRAITS & other) { return priority <= other.priority; }
};

unordered_map<char, OP_TRAITS> Operations = {
    {'+', {1, true}},
    {'-', {1, false}},
    {'*', {2, true}},
    {'/', {2, false}},
    {'^', {3, false}},
    {'(', {0, 0}},
    {')', {0, 0}}
};

string to_RPN(const char *s)
{
    stack<char> operations;
    string result;
    char c;
    for(; (c = *s++) != 0;) {
        auto f = Operations.find(c);
        if(f == Operations.end()) {
            result += c;
            continue;
        }
        if(c == '(') {
            operations.push(c);
            continue;
        }
        if(c == ')') {
            for(;;) {
                c = operations.top();
                operations.pop();
                if(c == '(') {
                    break;
                }
                result += c;
            }
            continue;
        }
        while(operations.size()) {
            char top = operations.top();
            if(f->second > Operations[top])
                break;
            result += top;
            operations.pop();
        }
        operations.push(c);
    }
    while(operations.size()) {
        result += operations.top();
        operations.pop();
    }
    return result;
}

struct Entry_
{
    string expr_;
    char op_;
    Entry_(string s): expr_(s), op_(0) {}
};

inline void AddToken(stack<Entry_>* stack, char token)
{
    if(token >= 'a' && token <= 'z') {
        string e; e += token;
        stack->push(Entry_(e));
    } else {
        //if(stack->size() < 2)
        //    throw exception("Stack underflow");
        auto rhs = stack->top();
        if(Operations.find(rhs.op_) != Operations.end() && Operations[rhs.op_] < Operations[token]) {
            rhs.expr_ = '(' + rhs.expr_ + ')';
        }
        stack->pop();
        auto lhs = stack->top();
        if(Operations.find(lhs.op_) != Operations.end() && Operations[token] > Operations[lhs.op_]) {
            lhs.expr_ = '(' + lhs.expr_ + ')';
        }
        stack->top().expr_ = lhs.expr_ +  token + rhs.expr_;
        stack->top().op_ = token;
    }
}

string ToInfix(string& src)
{
    stack<Entry_> stack;
    for(auto p = src.begin(); p != src.end(); ++p) {
        AddToken(&stack, *p);
    }
    return stack.top().expr_;
}

string CMEXPR_solver(string& str)
{
    string rpn = to_RPN(str.c_str());
    //cout << rpn << " ";
    string res = ToInfix(rpn);
    return res;
}

#undef DEVELOP

#ifdef DEVELOP
// Test correctness against working algorithm
int test(vector<string(*)(string&)> solvers, vector<string> test_data);
string CMEXPR_by_cacophonix(string & str);

#endif


int main(int argc, const char * argv[])
{
    char line[1024];
#ifdef DEVELOP
    vector<string(*)(string&)> solvers = {
        &CMEXPR_by_cacophonix,
        &CMEXPR_solver
    };
    int error_cases = test(solvers, vector<string>());  //{"a+(b/c*d)-e"}
    if(error_cases) {
        cout << "Test has errors!\n";
    }
#else
    // Read data from stdin
    int t = 0;
    scan_int(t);
    for(; t > 0; --t) {
        scan_line(line);
        string str;
        str = CMEXPR_solver(string(line));
        cout << str << endl;
    }
#endif
    return 0;
}

