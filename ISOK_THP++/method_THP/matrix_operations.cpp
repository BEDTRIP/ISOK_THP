#include <vector>
#include <iostream>
#include <iomanip>
#include "thp.h"

using namespace std;

void print(vector<vector<Galois::Element>> data, string str)
{
    if (str.size())
    {
        cout << "---------------------------------------------" << endl;
        cout << str << endl;
        cout << "---------------------------------------------" << endl;
    }

    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t j = 0; j < data[i].size(); j++)
        {
            cout << setw(3)
                << setfill(' ')
                << data[i][j] << ' ';
        }
        cout << endl;
    }

    if (str.size())
    {
        cout << "---------------------------------------------" << endl;
    }
}

vector<vector<Galois::Element>> create_matrix(Galois::Field& gf, int n, int m)
{
    vector<vector<Galois::Element>> c;
    for (int i = 0; i < n; i++) {
        vector<Galois::Element> el;
        for (int j = 0; j < m; j++) {
            Galois::Element x(&gf, 1);
            el.push_back(x);
        }
        c.push_back(el);
    }

    return c;
}

vector<vector<Galois::Element>> copy_matrix(Galois::Field& gf, vector<vector<Galois::Element>> A)
{
    vector<vector<Galois::Element>> c;
    for (int i = 0; i < A.size(); i++) {
        vector<Galois::Element> el;
        for (int j = 0; j < A[0].size(); j++) {
            el.push_back(A[i][j]);
        }
        c.push_back(el);
    }

    return c;
}

vector<vector<Galois::Element>> mul_matrix(vector<vector<Galois::Element>> a, vector<vector<Galois::Element>> b, Galois::Field& gf)
{
    int l = a.size();
    int m = a[0].size();
//  int m = b.size();
    int n = b[0].size();

    vector<vector<Galois::Element>> c = create_matrix(gf, l, n);

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Galois::Element q(&gf, 0);
            c[i][j] = q;
            for (int r = 0; r < m; r++)
            {
                c[i][j] += (a[i][r] * b[r][j]);
            }
        }
    }
    return c;
}

bool sravn_matr(vector<vector<int>> a, vector<vector<int>> b) {
    bool flag = false;
    for (int i = 0; i < (int)a.size(); i++) {
        for (int j = 0; j < (int)a.size(); j++) {
            if (a[i][j] == b[i][j]) {
                flag = true;
            }
            else {
                flag = false;
                break;
            }
        }
    }
    return flag;

}
/*
vector<int> vibor_dolei(int n, int k) {
    vector<int> a;
    a.resize(k);
    a[0] = 0 + rand() % n;
    for (int i = 1; i < k; i++) {
        a[i] = 0 + rand() % n;
        while (povtor(a, k, a[i], i) == 1) {
            a[i] = 0 + rand() % n;
        }
    }
    int temp;
    for (int i = 0; i < k - 1; i++) {
        for (int j = 0; j < k - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
    return a;
}
*/

vector<vector<Galois::Element>> exclude_row(Galois::Field& gf, vector<vector<Galois::Element>> A, int pos)
{
    vector<vector<Galois::Element>> B = create_matrix(gf, A.size()-1, A[0].size());
    int i = 0; int ii = 0;
    while (ii < A.size() - 1)
    {
        if (i == pos) i++;

        for (int j = 0; j < A[0].size(); j++)
        {
            B[ii][j] = A[i][j];
        }

        ii++;
        i++;
    }

    return B;
}
vector<vector<Galois::Element>> exclude_column(Galois::Field& gf, vector<vector<Galois::Element>> A, int pos)
{
    vector<vector<Galois::Element>> B = create_matrix(gf, A.size(), A[0].size() - 1);
    int i = 0; int ii = 0;
    while (ii < A[0].size() - 1)
    {
        if (i == pos) i++;

        for (int j = 0; j < A.size(); j++)
        {
            B[j][ii] = A[j][i];
        }

        ii++;
        i++;
    }

    return B;
}

Galois::Element determinant(Galois::Field& gf, vector<vector<Galois::Element>> A) 
{
    Galois::Element det(&gf, 0);
    int n = A.size();

    if (n == 1)
    {
        det = A[0][0];
    }
    else if (n == 2)
    {
        det = A[0][0] * A[1][1] + A[0][1] * A[1][0];
    }
    else if (n == 3)
    {
        det = A[0][0] * A[1][1] * A[2][2] 
            + A[0][1] * A[1][2] * A[2][0] 
            + A[0][2] * A[1][0] * A[2][1] 
            + A[0][2] * A[1][1] * A[2][0] 
            + A[0][0] * A[1][2] * A[2][1] 
            + A[0][1] * A[1][0] * A[2][2];
    }
    else 
    {
        for (size_t i = 0; i < A[0].size(); i++)
        {
            det += A[0][i] * determinant(gf, exclude_row(gf, exclude_column(gf, A, i), 0));
        }
    }
    
    return det;
}

vector<vector<Galois::Element>> transpose(Galois::Field& gf, vector<vector<Galois::Element>> A) 
{
    vector<vector<Galois::Element>> c;
    for (int i = 0; i < A[0].size(); i++) {
        vector<Galois::Element> el;
        for (int j = 0; j < A.size(); j++) {
            Galois::Element x(&gf, A[j][i].value());
            el.push_back(x);
        }
        c.push_back(el);
    }

    return c;
}

static vector<vector<Galois::Element>> minor_matrix(Galois::Field& gf, vector<vector<Galois::Element>> A) 
{
    vector<vector<Galois::Element>> c;
    for (int i = 0; i < A.size(); i++) {
        vector<Galois::Element> el;
        for (int j = 0; j < A.size(); j++) {
            el.push_back(determinant(gf, exclude_row(gf, exclude_column(gf, A, j), i)));
        }
        c.push_back(el);
    }
    //print(c, "minor");
    return c;
}

static vector<vector<Galois::Element>> add_alg_matrix(Galois::Field& gf, vector<vector<Galois::Element>> A)
{
    vector<vector<Galois::Element>> c;
    for (int i = 0; i < A.size(); i++) {
        vector<Galois::Element> el;
        for (int j = 0; j < A.size(); j++) {
            Galois::Element x(&gf, 0);
            if ((i+j) % 2 == 0)
            {
                x += A[i][j];
            }
            else
            {
                x -= A[i][j];
            }
            el.push_back(x);
        }
        c.push_back(el);
    }
    return c;
}

vector<vector<Galois::Element>> reverse(Galois::Field& gf, vector<vector<Galois::Element>> A, int ChosenOne)
{
    vector<vector<Galois::Element>> B = minor_matrix(gf, A);

    B = transpose(gf, B);
    Galois::Element det = determinant(gf, A);
    if (det.value() == 0) cout << ChosenOne << endl;

    for (size_t i = 0; i < B.size(); i++)
    {
        for (size_t j = 0; j < B[0].size(); j++)
        {
            B[i][j] = B[i][j] / det;
        }
    }

    return B;
}