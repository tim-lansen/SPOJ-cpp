// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// QTREE2 - Query on a tree II
// This problem's URL:
// http://www.spoj.com/problems/QMAX3VN/
//
// Approach #1: update linear array of heights on every appear event, scan range linearly for maximum
// Approach #2: the same as above, but using vector to store data
// Approach #3: use modified treap structure: the order of every soldier will be a key, and the soldier's height will be a priority
//              the goal is to create a method to insert new node with minimal treap update (no need to increment every left-hand-node's order)

//#define DEVELOP

#define MAX_EVENTS 100001
static int TotalEvents;

#include <cstdio>
#include <memory.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <strstream>
#include <algorithm>
using namespace std;

class CMusterLinear
{
    struct Soldier
    {
        int right, left, height;
    };
private:
    int First, Last;
    Soldier *soldiers;
    int Fill;
    int find_soldier(int p)
    {
        int i = 1, ps = First;
        for(; i != p;) {
            ps = soldiers[ps].left;      // Index of the next soldier on the left
            ++i;
        }
        return ps;
    }
public:
    CMusterLinear(int total_events)
        : First(0)
        , Last(0)
        , Fill(0)
    {
        soldiers = new Soldier[total_events];
    }
    ~CMusterLinear(){
        delete[] soldiers;
    }

    void dump()
    {
        if(First) {
            for(int si = First, i = 1; si != 0; si = soldiers[si].left, ++i) {
                printf("%d:%d ", i, soldiers[si].height);
            }
            printf("\n");
        }
    }

    void insert(int height, int position)
    {
        Soldier * new_soldier = &soldiers[++Fill];
        new_soldier->height = height;
        if(Fill == 1) {
            new_soldier->left = 0;
            new_soldier->right = 0;
            First = Fill;
            Last = Fill;
            return;
        }
        if(position == 1) {
            // First in line
            new_soldier->right = 0;
            new_soldier->left = First;
            soldiers[First].right = Fill;
            First = Fill;
            return;
        }
        if(Fill == position) {
            // Last int line
            new_soldier->left = 0;
            new_soldier->right = Last;
            soldiers[Last].left = Fill;
            Last = Fill;
            return;
        }
        // In the middle
        int ps = find_soldier(position);
        new_soldier->left = ps;
        new_soldier->right = soldiers[ps].right;
        soldiers[ps].right = Fill;
        soldiers[new_soldier->right].left = Fill;
    }

    int height(int a, int b)
    {
        if(a > b)
            swap(a, b);
        int ps = find_soldier(a++);
        int h = soldiers[ps].height;
        for(; a <= b; ++a) {
            ps = soldiers[ps].left;
            if(h < soldiers[ps].height)
                h = soldiers[ps].height;
        }
        return h;
    }
};

class CMusterVector
{
    struct Soldier
    {
        int height;
    };
private:
    int First, Last;
    vector<int> Line;
public:
    CMusterVector(...)
        : First(0)
        , Last(0)
    {
        Line.push_back(0);
    }
    ~CMusterVector()
    {}

    void dump()
    {
        for(int i = 1; i < Line.size(); ++i) {
            printf("%d:%d ", i, Line[i]);
        }
        printf("\n");
    }

    void insert(int height, int position)
    {
        //int p0 = position - 1;
        if(Line.size() == position)
            Line.push_back(height);
        else {
            Line.insert(Line.begin() + position, height);
        }
    }

    int height(int a, int b)
    {
        if(a > b)
            swap(a, b);
        int h = Line[a++];
        for(; a <= b; ++a) {
            int hx = Line[a];
            if(h < hx)
                h = hx;
        }
        return h;
    }
};

class CTreap
{
private:
    class node
    {
    public:
        int height, order;  // Original height, relative order
        int abs_order;
        node *l, *r;
    };

    node *root;
    int peak;
    node * pool;

    void split(node *n, node *&l, node *&r, int order)
    {
        if(!n)
            l = r = NULL;
        else if(order > n->order) {
            split(n->l, l, n->l, order);
            r = n;
        } else {
            split(n->r, n->r, r, order);
            l = n;
        }
    }

    void insert(node *&cn, node *new_node)
    {
        if(!cn)
            cn = new_node;
        else if(new_node->height >= cn->height) {
            split(cn, new_node->l, new_node->r, new_node->order);
            cn = new_node;
        } else {
            if(new_node->order < cn->order)
                insert(cn->r, new_node);
            else
                insert(cn->l, new_node);
        }
    }


    int m_new_node_order;

    void splitx(node *n, node *&l, node *&r, int offset)
    {
        if(!n) {
            l = r = NULL;
            return;
        }
        int order = offset + n->order;
        n->abs_order = order;
        if(m_new_node_order > order) {
            splitx(n->l, l, n->l, order);
            r = n;
            return;
        }
        splitx(n->r, n->r, r, offset);
        l = n;
    }

    void insertx(node *&cn, node *new_node)
    {
        if(!cn) {
            cn = new_node;
            return;
        }
        if(new_node->height >= cn->height) {
            int offset = m_new_node_order - new_node->order;
            splitx(cn, new_node->l, new_node->r, offset);
            cn = new_node;
            offset += new_node->order;
            for(node *rn = new_node->l; rn; rn = rn->r) {
                rn->order = rn->abs_order + 1 - m_new_node_order;
            }
            /*if(new_node->l) {
                new_node->l->order = new_node->l->abs_order + 1 - m_new_node_order;
                
            }*/
            return;
        }
        if(new_node->order <= cn->order) {
            // Go right, increment passed node's order
            cn->order++;
            insertx(cn->r, new_node);
            return;
        }
        // Go left, decrease new node's order
        new_node->order -= cn->order;
        insertx(cn->l, new_node);
    }

    int maxh(node *n, int offset, int a, int b)
    {
        int order = offset + n->order;
        if(order >= a && order <= b)
            return n->height;
        if(a > order) {
            // Go left
            return maxh(n->l, order, a, b);
        }
        return maxh(n->r, offset, a, b);
    }
#ifdef DEVELOP
    void dump(node *n, int offset) {
        if(n->l)
            dump(n->l, offset + n->order);
        printf("O:%d (%d)  H:%d  l:%d r:%d %s\n",
            offset + n->order, n->order, n->height,
            n->l? n->l->order + offset + n->order : 0,
            n->r? n->r->order + offset : 0,
            n == root? "(root)" : ""
        );
        if(n->r)
            dump(n->r, offset);
    }
    void dump2(node *n, int offset, int level, vector<vector<int>> &store)
    {
        if(store.size() <= level)
            store.push_back({});
        if(n->l)
            dump2(n->l, offset + n->order, level + 1, store);
        store[level].push_back(offset + n->order);
        if(n->r)
            dump2(n->r, offset, level + 1, store);
    }
#endif
public:
    CTreap(...)
    {
        pool = (node*)malloc(MAX_EVENTS * sizeof(node));
        memset(pool, 0, MAX_EVENTS * sizeof(node));
        root = 0;
        peak = 0;
    }
    void insert(int h, int o)
    {
        node *new_node = &pool[peak];
        new_node->order = o;
        new_node->height = h;
        //new_node->index = peak;
        if(!peak) {
            root = pool;
            peak = 1;
            return;
        }
        m_new_node_order = o;
        insertx(root, new_node);
        peak++;
    }
    int height(int a, int b)
    {
        if(a < b)
            return maxh(root, 0, a, b);
        return maxh(root, 0, b, a);
    }
#ifdef DEVELOP
    void dump() {
        dump(root, 0);
        printf("=================\n");
    }
    void dump2()
    {
        vector<vector<int>> dr;
        dump2(root, 0, 0, dr);
        for(auto x : dr) {
            int last_offset = 0;
            for(auto a = x.rbegin(); a != x.rend(); a++) {
                for(; last_offset < *a; ++last_offset)
                    printf(" ");
                ++last_offset;
                printf("*");
            }
            printf("\n");
        }
        printf("=================\n");
    }
#endif
};


#ifdef DEVELOP

void do_test()
{
    char strline[16];
    int size = 0;
    const int maxH = 256;
    CMusterVector sampler;
    CTreap treap;
    for(int i = 0; i < 100; ++i) {
        /*if(size > 5 && rand() > (RAND_MAX >> 3)) {
            // Generate request
            int a = 1 + (rand() % size), b = 1 + (rand() % size);
            int m1 = sampler.height(a, b);
            int m2 = treap.max_height(a, b);
            printf("Q %d %d  %d:%d\n", a, b, m1, m2);
            if(m1 != m2) {
                printf("Error!\n");
            }
            //sprintf(strline, "Q %d %d \n",1 + (rand() % size), 1 + (rand() % size));
        } else*/ {
            // Generate append
            size++;
            int h = 1 + (rand() % maxH), o = 1 + (rand() % size);
            sampler.insert(h, o);
            treap.insert(h, o);
            printf("O: %d  H: %d\n", o, h);
            treap.dump2();
            //sprintf(strline, "A %d %d\n", 1 + (rand() % maxH), 1 + (rand() % size));
        }
    }
}

#endif

int main()
{
    int n, x, y, e;
    char q[8];
#ifndef DEVELOP
    scanf("%d", &e);
    CTreap muster(e);
    for(n = 0; n < e; ++n) {
        scanf("%s %d %d", &q, &x, &y);
        if(*q == 'A') {
            muster.insert(x, y);
            continue;
        }
        if(*q == 'Q') {
            int maxh = muster.height(x, y);
            printf("%d\n", maxh);
        }
    }
#else
    do_test();
    ifstream fstr("C:\\git\\edu.cpp\\SPOJ-cpp\\Debug\\QMAX3VN.tc2.txt");
    ifstream fsta("C:\\git\\edu.cpp\\SPOJ-cpp\\Debug\\QMAX3VN.tc2a.txt");
    if(fstr.bad() || fsta.bad())
        return -1;
    fstr >> e;
    int answer;
    CTreap treap(e);
    for(n = 0; n < e; ++n) {
        fstr >> q >> x >> y;
        if(*q == 'A') {
            printf("insert O:%d H:%d\n", y, x);
            treap.insert(x, y);
            treap.dump();
        } else if(*q == 'Q') {
            int maxh = treap.height(x, y);
            fsta >> answer;
            
            if(maxh != answer) {
                printf("Wrong answer! Q %d %d = %d, must be %d\n", x, y, maxh, answer);
            } else {
                printf("%d\n", maxh);
            }
        }
        //muster.dump();
        
    }
    fstr.close();
    fsta.close();
#endif
    return 0;
}

