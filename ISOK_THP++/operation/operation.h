#pragma once
#include <string>
#include <vector>
#include "../Galois/include/galois++/field.h"
#include "../Galois/include/galois++/primes.h"
#include "../Galois/include/galois++/element.h"

using namespace std;

void print(const vector<unsigned __int64>& Vector);
int count_bits(unsigned __int64 x);
int count_bytes(unsigned __int64 x);

// �������� �� ����������� �������������� ����� �� ����� �� ����� ������
bool error_check(int k, int n, vector<unsigned __int64> err);
int s2i(string val);

// �������� �� ������� ������� ���� � ��������� ������ �����
bool erased_check(vector<unsigned __int64> data, unsigned __int64 ChosenOne = 0);
// ��������� ������ �� �������� ��� ������ ����
void erasing_error(vector<vector<unsigned __int64>>& A, vector<unsigned __int64> P, double probability);
// ��������� ������ �� ��������� ��� ������ ����
void corrupt_error(vector<vector<unsigned __int64>>& A, vector<unsigned __int64> P, double probability);
// ��������� ������ �� �������� ��� ������ ���
vector<vector<unsigned __int64>> erasing_error(int l, int n, double probability);
// ���������� ����� ��������� ������ �� ���������
vector<vector<unsigned __int64>> corrupt_error(int l, vector<unsigned __int64> P, double probability);