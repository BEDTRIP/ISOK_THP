#include <vector>
#include <filesystem>
#include <fstream>
#include "thp.h"

using namespace std;

pair<vector<vector<vector<Galois::Element>>>, int> read_file_thp(string name, Galois::Field& gf, const int n, const int k)
{
	unsigned __int64 y;
	vector<int> A;
	ifstream fin(name, ios_base::binary);
	while (fin.read((char*)&y, 1))
	{
		A.push_back(y % ((unsigned __int64)1 << 8));
	}

	fin.close();

	int dumbbell = (n * k) - (A.size() % (n * k));
	for (size_t i = 0; i < dumbbell; i++)
	{
		A.push_back(1);
	}

	vector<vector<vector<Galois::Element>>> M(A.size()/ (n * k));
	for (int i = 0; i < M.size(); i++)
	{
		M[i] = create_matrix(gf, k, n);
	}

	for (int g = 0; g < A.size(); g++) 
	{
		M[g/(n*k)][(g/n)%k][g % n] = A[g];
	}

	return make_pair(M, dumbbell);
}

vector<vector<vector<Galois::Element>>> THP_code(vector<vector<vector<Galois::Element>>> M_vec, vector<vector<Galois::Element>> V, Galois::Field& gf)
{
	vector<vector<vector<Galois::Element>>> Clear_Result;

	for (size_t i = 0; i < M_vec.size(); i++)
	{
		Clear_Result.push_back(mul_matrix(V, M_vec[i], gf));
	}

	return Clear_Result;
};

void write_coded_thp(string foldername, string matrixname, vector<vector<vector<Galois::Element>>> data)
{
	for (int i = 0; i < data[0].size(); i++)
	{
		string outputfilename = foldername + '/' + matrixname + "_" + to_string(i + 1) + ".thp";

		ofstream fout(outputfilename, ios_base::binary);

		for (size_t k = 0; k < data.size(); k++)
		{
			for (size_t j = 0; j < data[0][0].size(); j++)
			{
				int y = data[k][i][j].value();
				fout.write((char*)&y, 1);
			}
		}

		fout.close();
	}
}