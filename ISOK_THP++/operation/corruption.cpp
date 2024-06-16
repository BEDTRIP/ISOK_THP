#include "operation.h"
#include <bitset>
#include <ctime>
#include <omp.h>

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

bool error_check(int k, int n, vector<unsigned __int64> err)
{
	bool IsFound = false;

	vector<unsigned __int64> ccc = ChosenList(k, n);

	for (const auto& key : ccc)
	{
		if (!erased_check(err, key))
		{
			IsFound = true;
			break;
		}
	}

	return !IsFound;
}

int s2i(string val)
{
	int x = 0;
	for (auto el : val)
	{
		x += (int)el;
	}
	return x;
}

bool erased_check(vector<unsigned __int64> data, unsigned __int64 ChosenOne)
{
#pragma warning(push)
#pragma warning(disable:4267)
	int n = data.size();
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4244)
	if (ChosenOne == 0) ChosenOne = pow(2, n) - 1;
#pragma warning(pop)

	bool check = false;
	int i = 0;
	while (ChosenOne > 0 && i < n)
	{
		if (ChosenOne % 2 == 1 && data[i] == ((unsigned __int64)(0) - 1)) {
			check = true;
			break;
		}
		ChosenOne /= 2;
		i++;
	}

	return check;
}

void erasing_error(vector<vector<unsigned __int64>>& data, vector<unsigned __int64> P, double probability)
{
	srand(time(nullptr));

	long long p = ceil(1 / probability);

	int j_len = P.size();
	vector<unsigned int> p_size;
	for (size_t i = 0; i < j_len; i++)
	{
		p_size.push_back(count_bits(P[i] - 1));
	}

#pragma omp parallel for
	for (int i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < j_len; j++)
		{
			if (rand() % p == 0)
				data[i][j] = (unsigned __int64)(0) - 1;
		}
	}
}

void corrupt_error(vector<vector<unsigned __int64>>& data, vector<unsigned __int64> P, double probability)
{
	srand(time(nullptr));

	long long p = ceil(1 / probability);

	int j_len = P.size();
	vector<unsigned int> p_size;
	for (size_t i = 0; i < j_len; i++)
	{
		p_size.push_back(count_bits(P[i] - 1));
	}

#pragma omp parallel for
	for (int i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < j_len; j++)
		{
			for (size_t k = 0; k < p_size[j]; k++)
			{
				if (rand() % p == 0)
				{
					bitset<64> foo = data[i][j];
					foo.flip(k);
					data[i][j] = foo.to_ullong();
				}
			}
		}
	}
}

vector<vector<unsigned __int64>> erasing_error(int l, int n, double probability)
{
	srand(time(nullptr));

	vector<vector<unsigned __int64>> ER(l);
	for (auto &el : ER) el.resize(n);

	long long p = ceil(1 / probability);

#pragma omp parallel for
	for (int i = 0; i < l; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			ER[i][j] = 0;
			if (rand() % p == 0)
				ER[i][j] = (unsigned __int64)0-1;
		}
	}

	return ER;
}

vector<vector<unsigned __int64>> corrupt_error(int l, vector<unsigned __int64> P, double probability)
{
	srand(time(nullptr));

	int n = P.size();
	long long p = ceil(1 / probability);

	vector<unsigned int> p_size;
	for (size_t i = 0; i < n; i++)
	{
		p_size.push_back(count_bits(P[i] - 1));
	}

	vector<vector<unsigned __int64>> ER(l);
	for (auto& el : ER) el.resize(n);

#pragma omp parallel for
	for (int i = 0; i < l; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			ER[i][j] = 0;
			for (size_t k = 0; k < p_size[j]; k++)
			{
				if (rand() % p == 0)
				{
					ER[i][j] = (unsigned __int64)0 - 1;
				}
			}
		}
	}

	return ER;
}
