// SPOJ exersises
// Tim Lansen tim.lansen@gmail.com
// QTREE2 - Query on a tree II
// This problem's URL:
// http://www.spoj.com/problems/QMAX3VN/
//
// Approach: Every new element inserted as a node

#define DEVELOP

#define MAX_EVENTS 100001
static int TotalEvents;

#include <cstdio>
#include <memory>
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

#define oo 0x3c3c3c3c

class CMusterTree1
{
public:
    struct Soldier
    {
        int Max, Size, Height, Left, Right;
    };
    
private:
    static Soldier *soldiers;
    static int peak;
    struct node
    {
        int ll, rr, id;
        node(int L, int X)
        {
            ll = L, id = X;
            rr = ll + CMusterTree1::soldiers[id].Size - 1;
        }
        node left()
        {
            return node(ll, CMusterTree1::soldiers[id].Left);
        }
        node right()
        {
            return node(ll + CMusterTree1::soldiers[CMusterTree1::soldiers[id].Left].Size, CMusterTree1::soldiers[id].Right);
        }

        void insert(int Value, int u)
        {
            if(ll > u || u > rr)
                return;
            if(ll == rr) {
                CMusterTree1::soldiers[id].Left = CMusterTree1::create(Value);
                CMusterTree1::soldiers[id].Right = CMusterTree1::create(CMusterTree1::soldiers[id].Max);
                CMusterTree1::update(id);
                return;
            }
            left().insert(Value, u);
            right().insert(Value, u);
            CMusterTree1::update(id);
        }

        int max_range(int L, int R)
        {
            if(L>rr || ll>R || L>R)
                return -oo;
            if(L <= ll && rr <= R)
                return CMusterTree1::soldiers[id].Max;
            int Max1 = left().max_range(L, R);
            int Max2 = right().max_range(L, R);
            return max(Max1, Max2);
        }
    };

    static void update(int id)
    {
        int ll = soldiers[id].Left, rr = soldiers[id].Right;
        soldiers[id].Max = max(soldiers[ll].Max, soldiers[rr].Max);
        soldiers[id].Size = soldiers[ll].Size + soldiers[rr].Size;
        soldiers[id].Height = max(soldiers[ll].Height, soldiers[rr].Height) + 1;
    }

    static int create(int Value)
    {
        int id = ++peak;
        soldiers[id].Max = Value;
        soldiers[id].Size = 1;
        return id;
    }


public:
    CMusterTree1(int total_events)
    {
        int ss = 1 + 4 * (total_events + 1);
        soldiers = new Soldier[ss];
        memset(soldiers, 0, ss * sizeof(Soldier));
        create(-oo);
    }
    ~CMusterTree1()
    {
        delete[] soldiers;
    }

    static void dump()
    {
        for(int i = 1; i < peak; ++i) {
            Soldier *s = &soldiers[i];
            printf("id:%d M:%d S:%d H:%d L:%d R:%d\n", i, s->Max, s->Size, s->Height, s->Left, s->Right);
        }
        printf("\n");
    }

    void insert(int height, int position)
    {
        node(1, 1).insert(height, position);
    }

    int height(int a, int b)
    {
        node n = node(1, 1);
        int h = n.max_range(a, b);
        return h;
    }
};
CMusterTree1::Soldier * CMusterTree1::soldiers = 0;
int CMusterTree1::peak = 0;

const int INF = 200000000;

class CTreap
{
public:
    class node
    {
    public:
        int x, y, c;
        node *l, *r;
        /*node(int data)
            : x(data)
            , c(0), l(0), r(0)
        {
            y = ((rand() << 15) + rand()) % INF;
        }*/
        node(int ord, int data)
            : x(data)
            , c(0), l(0), r(0)
        {
            //y = ((rand() << 15) + rand()) % INF;
            y = ord;
        }
    };
    node *root, *null;
public:
    CTreap(...)
    {
        null = new node(0, 0);
        null->l = null->r = null;
        null->y = INF;
        root = null;
    }
    void update(node*&p)
    {
        if(p != null) {
            p->c = p->l->c + p->r->c + 1;
        }
    }
    void lr(node *&p)
    {
        node *q = p->l;
        p->l = q->r;
        q->r = p;
        update(p);
        update(q);
        p = q;
    }
    void rr(node *&p)
    {
        node *q = p->r;
        p->r = q->l;
        q->l = p;
        update(p);
        update(q);
        p = q;
    }
    void ins(node *&p, int x, int ord)
    {
        if(p == null) {
            p = new node(x, ord);
            p->l = p->r = null;
            p->c = 1;
        } else
            if(x < p->x) {
                ins(p->l, x, ord);
                if(p->l->y < p->y)
                    lr(p);
            } else {
                ins(p->r, x, ord);
                if(p->r->y < p->y)
                    rr(p);
            }
        update(p);
    }
    void del(node *&p, int x)
    {
        if(p == null) return;
        if(p->x == x)
            del(p);
        else
            if(x<p->x)
                del(p->l, x);
            else
                del(p->r, x);
        if(p != null) update(p);
    }
    void del(node *&p)
    {
        if(p->l == null&&p->r == null) {
            delete p;
            p = null;
            return;
        }
        if(p->l->y<p->r->y) {
            lr(p);
            del(p->r);
        } else {
            rr(p);
            del(p->l);
        }
        update(p);
    }
    bool find(node *&p, int x)
    {
        if(p == null) return false;
        if(x == p->x) return true;
        if(x<p->x)
            return find(p->l, x);
        else
            return find(p->r, x);
    }
    int rfs(node *&p, int k)
    {
        if(k <= p->l->c)
            return rfs(p->l, k);
        else
            if(k == p->l->c + 1)
                return p->x;
            else
                return rfs(p->r, k - p->l->c - 1);
    }

    void ins(int x, int ord)
    {
        ins(root, x, ord);
    }
    void del(int x)
    {
        del(root, x);
    }
    bool find(int x)
    {
        return find(root, x);
    }
    int rfs(int k)
    {
        if(k >= 1 && k <= root->c)
            return rfs(root, k);
        else
            return -1;
    }
};

class CMusterKienCoi1997
{

#define N 400005

    static int Max[N], Size[N], Height[N], Left[N], Right[N], Peak;

    static void update(int id)
    {
        int ll = Left[id], rr = Right[id];
        Max[id] = max(Max[ll], Max[rr]);
        Size[id] = Size[ll] + Size[rr];
        Height[id] = max(Height[ll], Height[rr]) + 1;
    }

    static int create(int Value)
    {
        int id = ++Peak;
        Max[id] = Value;
        Size[id] = 1;
        return id;
    }

    struct node
    {
        int ll, rr, id;
        node(int L, int X) {
            ll = L, id = X;
            rr = ll + CMusterKienCoi1997::Size[id] - 1;
        }
        node left() {
            return node(ll, CMusterKienCoi1997::Left[id]);
        }
        node right() {
            return node(ll + CMusterKienCoi1997::Size[Left[id]], CMusterKienCoi1997::Right[id]);
        }

        void insert(int Value, int u)
        {
            if(ll > u || u > rr)
                return;
            if(ll == rr) {
                CMusterKienCoi1997::Left[id] = CMusterKienCoi1997::create(Value);
                CMusterKienCoi1997::Right[id] = CMusterKienCoi1997::create(CMusterKienCoi1997::Max[id]);
                update(id);
                return;
            }
            left().insert(Value, u);
            right().insert(Value, u);
            CMusterKienCoi1997::update(id);
        }

        int max_range(int L, int R)
        {
            if(L>rr || ll>R || L>R)
                return -oo;
            if(L <= ll && rr <= R)
                return CMusterKienCoi1997::Max[id];
            int Max1 = left().max_range(L, R);
            int Max2 = right().max_range(L, R);
            return max(Max1, Max2);
        }
    };
public:
    CMusterKienCoi1997(...)
    {
        memset(Max, 0, sizeof(Max));
        memset(Size, 0, sizeof(Size));
        memset(Height, 0, sizeof(Height));
        memset(Left, 0, sizeof(Left));
        memset(Right, 0, sizeof(Right));
        create(-oo);
    }
    /*ostream& operator << (ostream& cout, node a)
    {
        if(a.ll == a.rr) return cout << Max[a.id];
        return cout << "(" << a.left() << " " << a.right() << ")";
        //return cout << a.left() << " " << a.right();
    }*/

    void dump()
    {
        for(int i = 1; i < Peak; ++i) {
            printf("id:%d M:%d S:%d H:%d L:%d R:%d\n", i, Max[i], Size[i], Height[i], Left[i], Right[i]);
        }
        printf("\n");
    }

    void insert(int height, int position)
    {
        node(1, 1).insert(height, position);
    }

    int height(int a, int b)
    {
        node n = node(1, 1);
        int h = n.max_range(a, b);
        return h;
    }
};
int CMusterKienCoi1997::Max[N], CMusterKienCoi1997::Size[N], CMusterKienCoi1997::Height[N];
int CMusterKienCoi1997::Left[N], CMusterKienCoi1997::Right[N], CMusterKienCoi1997::Peak = 0;


//typedef CMusterLinear CMuster;
//typedef CMusterVector CMuster;
typedef CMusterTree1 CMuster;
//typedef CMusterKienCoi1997 CMuster;

int main()
{
    int n, x, y, e;
    char q[8];
#ifndef DEVELOP
    scanf("%d", &e);
    CMuster muster(e);
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
    //strstream str;
    ifstream fstr("C:\\git\\edu.cpp\\SPOJ-cpp\\Debug\\QMAX3VN.tc1.txt");
    ifstream fsta("C:\\git\\edu.cpp\\SPOJ-cpp\\Debug\\QMAX3VN.tc1a.txt");
    if(fstr.bad() || fsta.bad())
        return -1;
    fstr >> e;
    int answer;
    CMuster muster(e);
    CTreap treap(e);
    for(n = 0; n < e; ++n) {
        fstr >> q >> x >> y;
        if(*q == 'A') {
            muster.insert(x, y);
            treap.ins(x, y);
        } else if(*q == 'Q') {
            int maxh = muster.height(x, y);
            fsta >> answer;
            
            if(maxh != answer) {
                printf("Wrong answer! Q %d %d = %d, must be %d\n", x, y, maxh, answer);
                muster.dump();
            } else {
                printf("%d\n", maxh);
            }
        }
        muster.dump();
        //treap.dump();
    }
    fstr.close();
    fsta.close();

#endif
    return 0;
}

