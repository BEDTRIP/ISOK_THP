#include "isok.h"
#include "../operation/operation.h"
#include <fstream>

using namespace std;

vector<unsigned __int64> Code(const vector<unsigned __int64> P,
	const int n, const int b, const unsigned __int64 data) {

	vector<unsigned __int64> Result;
	for (int i = 0; i < n; i++)
	{
		Result.push_back(data % P[i]);
	}

	return Result;
}

vector<vector<unsigned __int64>> Code_vec(const vector<unsigned __int64> P,
	const int n, const int b, vector<unsigned __int64> data) {

	vector<vector<unsigned __int64>> Result;

	for (size_t i = 0; i < data.size(); i++)
	{
		vector<unsigned __int64> Result_i = Code(P, n, b, data[i]);
		Result.push_back(Result_i);
	}

	return Result;
}

vector<unsigned __int64> read_file(string name, const int bit_count)
{
	vector<unsigned __int64> A;
	unsigned __int64 y;
	ifstream fin(name, ios_base::binary);

	while (fin.read((char*)&y, bit_count / 8))
	{

		A.push_back(y % ((unsigned __int64)1 << bit_count));
	}

	fin.close();

	return A;
}

void write_matrix_splitbycolumn(string foldername, string matrixname, vector<unsigned __int64> keys, string fileextension, vector<vector<unsigned __int64>> data) 
{
	for (int i = 0; i < keys.size(); i++)
	{
		string outputfilename = foldername + '/' + matrixname + "_" + to_string(keys[i]) + '.' + fileextension;
		ofstream fout(outputfilename, ios_base::binary);

		unsigned int count = count_bytes(keys[i]);
		for (size_t j = 0; j < data.size(); j++)
		{
			fout.write((char*)&data[j][i], count);
		}

		fout.close();
	}
}