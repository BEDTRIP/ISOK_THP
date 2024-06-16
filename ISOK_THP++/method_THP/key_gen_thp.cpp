#include <vector>
#include <ctime>
#include <iostream>
#include "thp.h"

using namespace std;

static Galois::Element poww2(Galois::Element x, int y, Galois::Field& gf) 
{
    Galois::Element c(&gf, x.value());
    for (size_t i = 1; i < y; i++)
    {
        c *= x;
    }
    return c;
}
static vector<int> find_primitive_256(Galois::Field &gf, int n)
{

    vector<int> a(n);

    srand(time(NULL));

    Galois::Element p(&gf, 1);
    vector<int> res;
    int i = 1;
    while (i <= 255) {
	    p = i;
	    bool q = true;
	    Galois::Element qq(&gf, 1);
        if (poww2(p, 15, gf) != qq and poww2(p, 51, gf) != qq and poww2(p, 85, gf) != qq and poww2(p, 255, gf) == qq) 
        {
            res.push_back(p.value());
        }
	    i++;
    }

    for (int i = 0; i < n; i++)
    {
        int x = rand() % res.size();
        a[i] = res[x];
        res.erase(res.begin() + x);
    }


    return a;
}

vector<vector<Galois::Element>> thp_key(Galois::Field& gf, int n, int k) 
{
    vector<vector<Galois::Element>> V = create_matrix(gf, n, k);

    vector<int> a = find_primitive_256(gf, n);

    if (k > 1) 
    {

        for (int i = 0; i < n; i++)
        {
            Galois::Element qq(&gf, a[i]);
            V[i][1] = qq;
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 2; j < k; j++) {
                V[i][j] = poww2(V[i][1], j, gf);
            }
        }
    }
    return V;
}
