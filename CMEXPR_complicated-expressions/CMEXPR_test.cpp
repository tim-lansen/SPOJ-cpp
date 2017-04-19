#include <string>
#include <vector>
#include <iostream>
#include <strstream>
#include <algorithm>
using namespace std;

typedef vector<char> SEQ;
typedef vector<SEQ> SEQQ;

typedef pair<char, char> BRAC;
typedef vector<BRAC> BRACC;

bool operator < (BRAC & a, BRAC & b)
{
    if(a.first == b.first) {
        return a.second < b.second;
    }
    return a.first < b.first;
};


vector<string> generator()
{
    SEQ a = {0, 1, 2, 3};
    char * opsymbols = "+-*/";
    char far_bracket = a.size();
    //SEQQ ops, skeleton;
    vector<string> skeleton;
    vector<string> result;
    char operands[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    // Generate all variants of operations order
    do {
        int i = 0;
        string seq;
        for(auto op_idx : a) {
            seq += operands[i++];
            seq += opsymbols[op_idx];
        }
        seq += operands[i];
        skeleton.push_back(seq);
    } while(next_permutation(a.begin(), a.end()));
    
    vector<BRACC> bbraces;
    for(int k = 0; k < 5; ++k) {
        BRACC bset;
        for(int i = 0; i < 5; ++i) {
            char vL = rand() % far_bracket;
            char vR = far_bracket - (rand() % (far_bracket - vL));
            bset.push_back({vL, vR});
        }
        bbraces.push_back(bset);
    }
    for(auto seq : skeleton) {
        for(auto braces : bbraces) {
            string s = seq;
            vector<BRAC> br;
            for(auto b : braces) {
                br.push_back(make_pair(b.first, '('));
                br.push_back(make_pair(b.second, ')'));
            }
            sort(br.rbegin(), br.rend());
            for(auto b : br) {
                int pos = b.first * 2 + (b.second == ')' ? 1 : 0);
                s.insert(s.begin() + pos, b.second);
            }
            strstream strs;
            string str;
            for(auto b : s) strs << b;
            strs >> str;
            result.push_back(str);
        }
    }
    return result;
}


int test(vector<string(*)(string&)> solvers, vector<string> test_data)
{
    if(test_data.size() == 0)
        test_data = generator();
    int error_cases = 0;
    for(string ts : test_data) {
        cout << ts << "  ";
        string last_result;
        int ec = 0;
        for(auto solver : solvers) {
            string res = solver(ts);
            cout << res;
            if(last_result.size() && res != last_result) {
                cout << "!!!";
                ec = 1;
            }
            cout << "  ";
            last_result = res;
        }
        error_cases += ec;
        cout << endl;
    }
    return error_cases;
}

