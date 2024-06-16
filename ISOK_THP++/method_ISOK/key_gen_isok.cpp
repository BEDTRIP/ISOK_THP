#include "../LongInt/LongInt.h"
#include "isok.h"
#include <iostream>
#include <numeric>

using namespace std;

static int trunc_log2(const LongInt& y)
{
	LongInt x = y;
	int log2x = 0;

	while (x / 2 > 0)
	{
		log2x++;
		x = x / 2;
	}

	return log2x;
}

static int ceil_log2(unsigned __int64 x)
{
	int log2x = 0;
	while (x % 2 != 1 && x > 0)
	{
		log2x++;
		x = x / 2;
	}

	if (x > 1) log2x++;

	while (x > 0)
	{
		log2x++;
		x = x / 2;
	}

	return log2x - 1;
}

static unsigned __int64 NODv(const vector<unsigned __int64>& a, const unsigned __int64& b)
{
	unsigned __int64 k = 1;
	for (const auto& i : a)
	{
		k *= gcd(i, b);
	}
	return k;
}

static bool NOKcheck(const vector<unsigned __int64>& a, unsigned __int64 m, int b)
{
	b -= ceil_log2(std::move(m));
	for (const auto& i : a)
	{
		b -= ceil_log2(i);
	}
	return b > 0;
}

vector<unsigned __int64> key_auto_creation(int b, int k, int n)
{
	auto* m = new unsigned __int64[n];
	///////////////////////////////НАБОР БЕЗ СТЕПЕНИ ДВОЙКИ/////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4244)
	unsigned __int64 m_mid = ceil(powf(2, double(b) / k));
#pragma warning(pop)
	int ceilk2 = int(ceil(double(k) / 2)) - 1; ///примерная середина списка рабочих оснований

	m[ceilk2] = m_mid;
	vector<unsigned __int64> M_temp; M_temp.push_back(m[ceilk2]);
	int i = 1;
	unsigned __int64 m_temp = m[ceilk2] - 1;

	while (i < ceilk2 and m_temp > 1)
	{
		if (NODv(M_temp, m_temp) == 1)
		{
			m[ceilk2 - i] = m_temp;
			M_temp.push_back(m[ceilk2 - i]);
			m_temp = m[ceilk2 + i - 1] + 1;

			while (NODv(M_temp, m_temp) != 1 or
				NOKcheck(M_temp, m_temp, int(ceil(double(b) / k) * (2 * i + 1)))
				)
			{
				m_temp = m_temp + 1;
			}

			m[ceilk2 + i] = m_temp;
			M_temp.push_back(m[ceilk2 + i]);
			m_temp = m[ceilk2 - i] - 1;

			i++;
		}
		else
		{
			m_temp = m_temp - 1;
		}
	}

#pragma warning(push)
#pragma warning(disable:6385)
	for (int j = 0; j < 2 * i - 1; j++)
	{
		m[j] = m[j + ceilk2 - i + 1];
	}
#pragma warning(pop)


	for (int j = 2 * i - 1; j < n; j++)
	{
		m_temp = m[2 * i - 1 - 1] + 1;

		while (NODv(M_temp, m_temp) != 1)
		{
			m_temp = m_temp + 1;
		}

		m[j] = m_temp;
		M_temp.push_back(m[j]);
	}

	///////////////////////////////ПРОВЕРКА/////////////////////////////////

	LongInt M_din = 1;
	for (int t = 0; t < k; ++t)
	{
		M_din = M_din * m[t];
	}
	if (M_din < LongInt::pow(2, b))
	{
		cout << "ОШИБКА!!! " << trunc_log2(M_din) << " < "
			<< trunc_log2(LongInt::pow(2, b)) << endl;
	}
	int b_m = trunc_log2(m[n - 1]);

	///////////////////////////////ВЫВОД////////////////////////////////////
	vector<unsigned __int64> P(n);

	for (size_t j = 0; j < n; j++)
	{
		P[j] = m[j];
	}

	return P;
}
