#include <vector>
#include <string>
#include <fstream>
#include "thp.h"
#include "../operation/operation.h"

using namespace std;
vector<vector<vector<Galois::Element>>> read_coded_thp(string foldername, string filename, int n, Galois::Field& gf)
{
	vector<vector<vector<Galois::Element>>> Readed_Files;

	string outputfilename = foldername + '/' + filename + "_" + to_string(1) + ".thp";
	ifstream fin(outputfilename, ios_base::binary);
	unsigned __int64 y;
	int k = 0; int i = 0;
	Readed_Files.push_back(create_matrix(gf, n, n));

	while (fin.read((char*)&y, 1))
	{
		Readed_Files[k][0][i] = (int)(y % ((unsigned __int64)1 << 8));
		if (i < n - 1)
		{
			i++;
		}
		else {
			k++;
			i = 0;
			Readed_Files.push_back(create_matrix(gf, n, n));
		}
	}
	fin.close();
	Readed_Files.pop_back();

	for (size_t ii = 1; ii < n; ii++)
	{
		string outputfilename = foldername + '/' + filename + "_" + to_string(ii + 1) + ".thp";
		ifstream fin(outputfilename, ios_base::binary);
		k = 0; i = 0;

		while (fin.read((char*)&y, 1))
		{
			Readed_Files[k][ii][i] = (int)(y % ((unsigned __int64)1 << 8));
			if (i < n - 1)
			{
				i++;
			}
			else {
				k++;
				i = 0;
			}
		}
		fin.close();
	}

	return Readed_Files;
}

static unsigned int count_bits_1(unsigned __int64 x)
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < sizeof(x) * CHAR_BIT; ++i)
	{
		count += (x >> i) & 1;
	}
	return count;
}

static vector<unsigned __int64> ChosenList(int k, int n)
{
	vector<unsigned __int64> List;

#pragma warning(push)
#pragma warning(disable:4244)
	unsigned __int64 m = pow(2, k) - 1;
	unsigned __int64 mmax = pow(2, n) - 1 - (pow(2, (n - k)) - 1);
#pragma warning(pop)

	List.push_back(m);

	while (m < mmax - 1) {
		m++;

		if (count_bits_1(m) == k)
			List.push_back(m);
	}

	List.push_back(mmax);

	return List;
}

static vector<vector<Galois::Element>> OrtogonalSystem_f(Galois::Field& gf, vector<vector<Galois::Element>> V, unsigned __int64 ChosenOne = 0)
{
#pragma warning(push)
#pragma warning(disable:4267)
	int n = V.size();
	int k = V[0].size();
#pragma warning(pop)
	if (ChosenOne == 0) ChosenOne = pow(2, n) - 1;

	vector<vector<Galois::Element>> V_1 = create_matrix(gf, k, k);

	unsigned __int64 ChosenOneI = ChosenOne;
	int i = 0; int ii = 0;
	while (ChosenOneI > 0 && i < n && ii < k)
	{
		if (ChosenOneI % 2 == 1) 
		{
			for (int j = 0; j < k; j++) 
			{
				V_1[ii][j] = V[i][j];
			}
			ii++;
		}

		ChosenOneI /= 2;
		i++;
	}

	vector<vector<Galois::Element>> V_2 = reverse(gf, V_1, ChosenOne);
	return V_2;
}
map<unsigned __int64, vector<vector<Galois::Element>>> OrtogonalSystem(Galois::Field& gf, vector<vector<Galois::Element>> V)
{
	vector<unsigned __int64> ChosenL = ChosenList(V[0].size(), V.size());
	map<unsigned __int64, vector<vector<Galois::Element>>> AllBs;

	for (size_t i = 0; i < ChosenL.size(); i++) {
		AllBs[ChosenL[i]] = OrtogonalSystem_f(gf, V, ChosenL[i]);
	}

	return AllBs;
}

pair<vector<vector<Galois::Element>>, bool> THP_decode(Galois::Field& gf, vector<vector<Galois::Element>> R,
	map<unsigned __int64, vector<vector<Galois::Element>>> AllVs,
	vector<unsigned __int64> ER, int k)
{
	vector<vector<Galois::Element>> M = create_matrix(gf, k, R.size());
	bool IsFound = false;
	for (const auto& [key, value] : AllVs)
	{
		if (!erased_check(ER, key))
		{
			IsFound = true;
			unsigned __int64 a = key;
			vector<vector<Galois::Element>> R_seized(k);
			int ii = 0;
			for (int i = 0; i < R.size(); i++)
			{
				if (a % 2) {
					R_seized[ii] = R[i];
					ii++;
				}
				a /= 2;
			}

			M = mul_matrix(value, R_seized, gf);
			return make_pair(M, IsFound);
		}
	}

	return make_pair(M, IsFound);
}

void write_file(string name, vector<vector<vector<Galois::Element>>> data, int dumbbell)
{
	ofstream fout(name, ios_base::binary);

	vector<unsigned __int64> A;

	for (int m = 0; m < data.size(); m++)
	{
		for (int i = 0; i < data[0].size(); i++)
		{
			for (int j = 0; j < data[0][0].size(); j++)
			{
				A.push_back(data[m][i][j].value());
			}
		}
	}

	for (size_t i = 0; i < dumbbell; i++)
	{
		A.pop_back();
	}

	for (auto el: A)
	{
		fout.write((char*)&(el), 1);
	}

	fout.close();
}