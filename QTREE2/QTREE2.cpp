// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// QTREE2 - Query on a tree II
// This problem's URL:
// http://www.spoj.com/problems/QTREE2/
//
// Approach: pre-build 'reverse to root' sequences and store levels and distances from root
// then for every DIST query find an LCA and calculate answer (d(a) + d(b) - 2*d(LCA))
//      for every KTH query: if level(a) - level(LCA) >= k-1 answer is reverse[a][k-1]
//                           else answer is reverse[b][(level(b)-level(LCA)) - (k-1-(level(a)-level(LCA)))] bcl - (k - acl)
// Optimized by using Sparse Table


#include <strstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define MAXNODES 10000

struct CONNECT
{
    int node;
    int cost;
};
struct NODE
{
    int level;
    int parent;
    int distance_from_root;
    bool seen;
    vector<CONNECT> connections;
    //NODE(): seen(false), level(0), parent(-1), distance_from_root(0) {}
    //bool operator< (NODE &b) { return level < b.level; }
};
static NODE Tree[MAXNODES + 1] = {
    {0, 0, 0, true},    // dummy
    {0, -1, 0, false},  // first node
};
//static vector<int> Reverse[MAXNODES + 1];
int Roots[16][MAXNODES + 1];

void clean(int n)
{
    if(Tree[1].seen) {
        for(int i = 1; i <= n; ++i) {
            //Reverse[i].clear();
            Tree[i].connections.clear();
            Tree[i].seen = false;
        }
    }
}

void traverse(int v, int level)
{
    NODE *current = &Tree[v];
    current->level = level;
    current->seen = true;
    level++;
    for(auto i : Tree[v].connections) {
        NODE *next = &Tree[i.node];
        if(!next->seen) {
            next->parent = v;
            next->distance_from_root = current->distance_from_root + i.cost;
            traverse(i.node, level);
        }
    }
}

/*void build_reverse(int n)
{
    for(int i = 1; i <= n; ++i) {
        NODE *node = &Tree[i];
        Reverse[i].push_back(i);
        for(;;) {
            Reverse[i].push_back(node->parent);
            if(node->parent == -1)
                break;
            node = &Tree[node->parent];
        }
    }
}*/

void build_roots(int n)
{
    memset(Roots, 0xFF, sizeof(Roots));
    for(int i = 1; i <= n; ++i) {
        Roots[0][i] = Tree[i].parent;
    }
    for(int j = 1; 1 << j < n; ++j) {
        
        int k = j - 1;
        for(int i = 1; i <= n; ++i) {
            if(Roots[k][i] != -1) {
                Roots[j][i] = Roots[k][Roots[k][i]];
            }
        }
    }
}

void preprocess(int n)
{
    Tree[1].parent = -1;
    Tree[1].level = 0;
    traverse(1, 0);
    //build_reverse(n);
    build_roots(n);
}

/*int lca_reverse(int p, int q)
{
    if(p == q)
        return p;
    int level;
    if(Tree[p].level < Tree[q].level)
        swap(p, q);
    level = Tree[q].level;
    // Align levels: level(p) >= level(q)
    p = Reverse[p][Tree[p].level - level];
    // Now levels are equal, quick check
    if(p == q)
        return p;
    // Perform binary search on reverse sequences
    int upper = level, lower = 0, i;
    for(;;) {
        i = lower + ((upper - lower) >> 1);
        if(Reverse[p][i] != Reverse[q][i]) {
            p = Reverse[p][i];
            q = Reverse[q][i];
            upper -= i;
            lower = 0;
            if(upper < 2) {
                i = 1;
                break;
            }
        } else {
            upper -= i;
            if(upper - lower == 1) {
                i = upper;
                break;
            }
        }
    }
    return Reverse[p][i];
}*/

inline int roots_limit(int level)
{
    int limit = 0, l = 2;
    for(; l <= level; l <<= 1, ++limit);
    return limit;
}

inline void get_ancestor_with_level(int &a, int &level_a, int level, int limit)
{
    for(int i = limit; i >= 0; --i) {
        if(level_a - (1 << i) >= level) {
            a = Roots[i][a];
            level_a = Tree[a].level;
        }
    }
}

int lca_roots(int a, int b)
{
    if(a == b)
        return a;
    int i, level_a = Tree[a].level, level_b = Tree[b].level;
    if(level_a < level_b) {
        swap(a, b);
        swap(level_a, level_b);
    }
    int limit = roots_limit(level_a);
    // Equalize levels
    if(level_a != level_b) {
        get_ancestor_with_level(a, level_a, level_b, limit);
        if(a == b)
            return a;
    }
    // Climb the tree
    for(i = limit; i >= 0; --i)
        if(Roots[i][a] != -1 && Roots[i][a] != Roots[i][b])
            a = Roots[i][a], b = Roots[i][b];
    return Roots[0][a];
}

int dist(int a, int b)
{
    int c = lca_roots(a, b);
    return Tree[a].distance_from_root + Tree[b].distance_from_root - (Tree[c].distance_from_root << 1);
}

int kth(int a, int b, int k)
{
    if(k == 1 || a == b)
        return a;
    k--;
    int aa = a, bb = b;
    int i, level_a = Tree[a].level, level_b = Tree[b].level;
    if(level_a < level_b) {
        swap(a, b);
        swap(level_a, level_b);
    }
    int limit = roots_limit(level_a);
    // Equalize levels
    if(level_a != level_b) {
        get_ancestor_with_level(a, level_a, level_b, limit);
    }
    if(a != b) {
        // Climb the tree
        for(i = limit; i >= 0; --i)
            if(Roots[i][a] != -1 && Roots[i][a] != Roots[i][b])
                a = Roots[i][a], b = Roots[i][b];
        a = Roots[0][a];
    }

    int cl = Tree[a].level;
    level_a = Tree[aa].level;
    level_b = Tree[bb].level;
    int acl = level_a - cl;
    int bcl = level_b - cl;
    if(acl >= k) {
        get_ancestor_with_level(aa, level_a, level_a - k, limit);
        return aa;
    }
    k -= acl;
    get_ancestor_with_level(bb, level_b, cl + k, limit);
    return bb;
}

int main()
{
    int cases, N;

    scanf("%d", &cases);
    for(; cases; --cases) {
        scanf("%d", &N);
        clean(N);
        int a, b, c, k;
        for(int i = 0; i < N - 1; ++i) {
            scanf("%d %d %d", &a, &b, &c);
            Tree[a].connections.push_back({b, c});
            Tree[b].connections.push_back({a, c});
        }
        preprocess(N);

        char token[8];
        for(;;) {
            scanf("%s", &token);

            if(token[1] == 'I') {
                scanf("%d %d", &a, &b);
                printf("%d\n", dist(a, b));
                continue;
            }
            if(token[1] == 'T') {
                scanf("%d %d %d", &a, &b, &k);
                printf("%d\n", kth(a, b, k));
                continue;
            }
            break;
        }
    }
    return 0;
}
