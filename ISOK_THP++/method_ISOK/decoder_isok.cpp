#include "../LongInt/LongInt.h"
#include "../operation/operation.h"
#include "isok.h"
#include <omp.h>
#include <iostream>
#include <fstream>

using namespace std;

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

static LongInt MinusMOD(const LongInt& a, const LongInt& n) {
	LongInt t = 0, newt = 1, r = n, newr = a, q, debugT, debugR;
	//// скорее всего возникнет проблема на 64м бите ////

	while (newr != 0) {
		q = r / newr;
		debugT = newt; newt = t - q * newt; t = debugT;
		debugR = newr; newr = r - q * newr; r = debugR;
	}
	if (r > 1) {
		cout << "////////////////// Ошибка, " << a
			<< " не обратимо /////////////////////" << endl;
		return 0;
	}
	if (t < 0) { t += n; }

	return t;
}

static vector<LongInt> OrtogonalSystem_f(vector<unsigned __int64> P, unsigned __int64 ChosenOne)
{
#pragma warning(push)
#pragma warning(disable:4267)
	int n = P.size();
#pragma warning(pop)

	vector<LongInt> B(n);
	int i = 0, j;

#pragma warning(push)
#pragma warning(disable:4244)
	if (ChosenOne == 0) ChosenOne = pow(2, n) - 1;
#pragma warning(pop)

	unsigned __int64 ChosenOneI = ChosenOne;
	unsigned __int64 ChosenOneJ;

	while (ChosenOneI > 0 && i < n)
	{
		if (ChosenOneI % 2 == 1)
		{
			B[i] = 1;
			ChosenOneJ = ChosenOne; j = 0;

			while (ChosenOneJ > 0 && j < n)
			{
				if (ChosenOneJ % 2 == 1) B[i] *= P[j];
				ChosenOneJ /= 2;
				j++;
			}
			B[i] /= P[i];
			B[i] *= MinusMOD(B[i], P[i]);
		}

		ChosenOneI /= 2;
		i++;
	}

	return B;
}

map<unsigned __int64, vector<LongInt>> OrtogonalSystem(vector<unsigned __int64> P, int k, int n)
{
	vector<unsigned __int64> ChosenL = ChosenList(k, n);
	map<unsigned __int64, vector<LongInt>> AllBs;
	for (size_t i = 0; i < ChosenL.size(); i++) {
		AllBs[ChosenL[i]] = OrtogonalSystem_f(P, ChosenL[i]);
	}

	return AllBs;
}

static unsigned __int64 Decode_f(vector<unsigned __int64> data, vector<LongInt> B,
	vector<unsigned __int64> P, const unsigned __int64 ChosenOne)
{
#pragma warning(push)
#pragma warning(disable:4267)
	int n = P.size();
#pragma warning(pop)

	unsigned __int64 ChosenOneI = ChosenOne;

#pragma warning(push)
#pragma warning(disable:4244)
	if (ChosenOneI == 0) ChosenOneI = pow(2, n) - 1;
#pragma warning(pop)

	LongInt P_n = 1;
	int i = 0;
	while (ChosenOneI > 0 && i < n)
	{
		if (ChosenOneI % 2 == 1) {
			P_n *= P[i];
		}
		ChosenOneI /= 2;
		i++;
	}

	LongInt A = 0;

	ChosenOneI = ChosenOne; i = 0;

#pragma warning(push)
#pragma warning(disable:4244)
	if (ChosenOneI == 0) ChosenOneI = pow(2, n) - 1;
#pragma warning(pop)

	while (ChosenOneI > 0 && i < n)
	{
		if (ChosenOneI % 2 == 1) { A = (A + (B[i] * data[i]) % P_n) % P_n; }
		ChosenOneI /= 2;
		i++;
	}
	unsigned __int64 Al = stoll(LongInt::to_string(A));
	return Al;
}

bool IsHereBeyondBorders(vector<unsigned __int64> data, vector<LongInt> ortogonal,
	vector<unsigned __int64> P, int k, const unsigned __int64 ChosenOne)
{
	LongInt P_k = 1;
	for (int i = 0; i < k; i++) P_k *= P[i];

	return Decode_f(data, ortogonal, P, ChosenOne) >= P_k;
}

bool HammingDistance(vector<unsigned __int64> A, vector<unsigned __int64> B, int k) {
#pragma warning(push)
#pragma warning(disable:4267)
	int n = A.size();
#pragma warning(pop)

	int q = (n - k) / 2;
	int count = 0;
	for (int i = 0; i < n; i++) if (A[i] != B[i]) count++;

	return count <= q;
}


pair <unsigned __int64, bool> Decode(vector<unsigned __int64> data,
	map<unsigned __int64, vector<LongInt>> B,
	vector<unsigned __int64> P,
	const int b, const int k, unsigned __int64 ChosenOne, bool erased_only)
{

#pragma warning(push)
#pragma warning(disable:4267)
	const int N = P.size();
#pragma warning(pop)

	unsigned __int64 intA;
	vector<unsigned __int64> ClearA(N);
	int q = (N - k) / 2;
	bool IsFound = false;

	if (ChosenOne == 0)
	{
		for (const auto& [key, value] : B)
		{
			if (!erased_check(data, key))
			{
				intA = Decode_f(data, B[key], P, key);

				if (erased_only) 
				{
					IsFound = true;
					break;
				}

				ClearA = Code(P, N, b, intA);
				if (HammingDistance(data, ClearA, k))
				{
					IsFound = true;
					break;
				}
			}
		}
	}
	else
	{
		intA = Decode_f(data, B[ChosenOne], P, ChosenOne);

		if (erased_only)
		{
			IsFound = true;
		}
		else
		{
			ClearA = Code(P, N, b, intA);
			if (HammingDistance(data, ClearA, k))
			{
				IsFound = true;
			}
		}
	}

	return make_pair(intA, IsFound);
}

pair<vector<unsigned __int64>, bool> Decode_vec(vector<vector<unsigned __int64>> data,
	map<unsigned __int64, vector<LongInt>> AllBs,
	vector<unsigned __int64> P,
	const int b, const int k)
{
	vector<unsigned __int64> Repaired_File(data.size());
	bool is_ok = true;
#pragma omp parallel for
	for (int i = 0; i < data.size(); i++)
	{
		pair<unsigned __int64, bool> Ai = Decode(data[i], AllBs, P, b, k);
		if (!Ai.second) {
			is_ok = false;
			break;
		}
		Repaired_File[i] = Ai.first;
	}

	return make_pair(Repaired_File, is_ok);
}

vector<vector<unsigned __int64>> read_matrix_splitbycolumn(string foldername, string matrixname, vector<unsigned __int64> keys, string fileextension) 
{
	vector<vector<unsigned __int64>> Readed_Files;

	string outputfilename = foldername + '/' + matrixname + "_" + to_string(keys[0]) + '.' + fileextension;
	ifstream fin(outputfilename, ios_base::binary);
	unsigned int count = count_bytes(keys[0]);
	unsigned __int64 y;
	while (fin.read((char*)&y, count))
	{
		vector<unsigned __int64> Ai;
		Ai.push_back(y % ((unsigned __int64)1 << (8 * count)));
		Readed_Files.push_back(Ai);
	}
	fin.close();

	for (size_t i = 1; i < keys.size(); i++)
	{
		string outputfilename = foldername + '/' + matrixname + "_" + to_string(keys[i]) + '.' + fileextension;
		ifstream fin1(outputfilename, ios_base::binary);
		count = count_bytes(keys[i]);
		int j = 0;
		while (fin1.read((char*)&y, count))
		{
			Readed_Files[j].push_back(y % ((unsigned __int64)1 << (8 * count)));
			j++;
		}
		fin1.close();
	}

	return Readed_Files;
}

void write_file(string name, const int bit_count, vector<unsigned __int64> data)
{
	ofstream fout1(name, ios_base::binary);
	for (size_t i = 0; i < data.size(); i++)
	{
		fout1.write((char*)&(data[i]), bit_count / 8);
	}
	fout1.close();
}