#pragma once
#include <vector>
#include <map>
#include <utility>
#include "../Galois/include/galois++/field.h"
#include "../Galois/include/galois++/primes.h"
#include "../Galois/include/galois++/element.h"

using namespace std;

///////// matrix operations
// ����� ������� � �������
void print(vector<vector<Galois::Element>> data, string str = "");
// �������� ������ �������
vector<vector<Galois::Element>> create_matrix(Galois::Field& gf, int n, int m);
// ��������� ������
vector<vector<Galois::Element>> mul_matrix(vector<vector<Galois::Element>> a, vector<vector<Galois::Element>> b, Galois::Field& gf);
// ���������� ������
vector<vector<Galois::Element>> exclude_row(Galois::Field& gf, vector<vector<Galois::Element>> A, int pos);
// ���������� �������
vector<vector<Galois::Element>> exclude_column(Galois::Field& gf, vector<vector<Galois::Element>> A, int pos);
// ������ ������������
Galois::Element determinant(Galois::Field& gf, vector<vector<Galois::Element>> A);
// ���������������� �������
vector<vector<Galois::Element>> transpose(Galois::Field& gf, vector<vector<Galois::Element>> A);
// ������ �������� �������
vector<vector<Galois::Element>> reverse(Galois::Field& gf, vector<vector<Galois::Element>> A, int ChosenOne = 0);

///////// key creation
// ��������� ������� V
vector<vector<Galois::Element>> thp_key(Galois::Field& gf, int n, int k);

///////// coder
// ����������� ������
vector<vector<vector<Galois::Element>>> THP_code(vector<vector<vector<Galois::Element>>> M_vec, vector<vector<Galois::Element>> V, Galois::Field& gf);
// ������ � �������
pair<vector<vector<vector<Galois::Element>>>, int> read_file_thp(string name, Galois::Field& gf, const int n, const int k);
void write_coded_thp(string foldername, string matrixname, vector<vector<vector<Galois::Element>>> data);

///////// decoder
// ������ �������� ������ V`
map<unsigned __int64, vector<vector<Galois::Element>>> OrtogonalSystem(Galois::Field& gf, vector<vector<Galois::Element>> V);
// ������������� ������
pair<vector<vector<Galois::Element>>, bool> THP_decode(Galois::Field& gf, vector<vector<Galois::Element>> R,
	map<unsigned __int64, vector<vector<Galois::Element>>> AllVs,
	vector<unsigned __int64> ER, int k);
// ������ � �������
vector<vector<vector<Galois::Element>>> read_coded_thp(string foldername, string filename, int n, Galois::Field& gf);
void write_file(string name, vector<vector<vector<Galois::Element>>> data, int dumbbell);