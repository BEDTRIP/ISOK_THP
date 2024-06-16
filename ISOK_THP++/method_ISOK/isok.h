#pragma once
#include "../LongInt/LongInt.h"
#include <map>
#include <utility>
#include <vector>

using namespace std;

///////// key_gen
// ��������� ������ ������
vector<unsigned __int64> key_auto_creation(const int b, const int k, const int n);

///////// coder
// ����������� �����
vector<unsigned __int64> Code(const vector<unsigned __int64> P, const int n, const int b, const unsigned __int64 data);
// ����������� ������� �����
vector<vector<unsigned __int64>> Code_vec(const vector<unsigned __int64> P, const int n, const int b, vector<unsigned __int64> data);
// ������ � �������
vector<unsigned __int64> read_file(string name, const int bit_count);
void write_matrix_splitbycolumn(string foldername, string matrixname, vector<unsigned __int64> keys, string fileextension, vector<vector<unsigned __int64>> data);

///////// decoder
// ��������� ����������������� �������
map<unsigned __int64, vector<LongInt>> OrtogonalSystem(vector<unsigned __int64> P, int k, int n);
// �������� �� ������� ������
bool IsHereBeyondBorders(vector<unsigned __int64> data, vector<LongInt> ortogonal, vector<unsigned __int64> P, int k, const unsigned __int64 ChosenOne);
// ���������� �������� ��� ���� ��������
bool HammingDistance(vector<unsigned __int64> A, vector<unsigned __int64> B, int k);
// ����������� �� ���� ������� �������� (��� �� �������������)
pair <unsigned __int64, bool> Decode(vector<unsigned __int64> A, map<unsigned __int64, vector<LongInt>> B, vector<unsigned __int64> P, const int b, const int k, unsigned __int64 ChosenOne = 0, bool erased_only = false);
// ����������� �������
pair<vector<unsigned __int64>, bool> Decode_vec(vector<vector<unsigned __int64>> data, map<unsigned __int64, vector<LongInt>> AllBs, vector<unsigned __int64> P, const int b, const int k);
// ������ � �������
vector<vector<unsigned __int64>> read_matrix_splitbycolumn(string foldername, string matrixname, vector<unsigned __int64> keys, string fileextension);
void write_file(string name, const int bit_count, vector<unsigned __int64> data);