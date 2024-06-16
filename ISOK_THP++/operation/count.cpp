#include "operation.h"
#include <iostream>

int count_bits(unsigned __int64 x)
{
	int count = 0;
	while (x > 0) {
		count++;
		x /= 2;
	}
	return count;
}

int count_bytes(unsigned __int64 x)
{
	int count = count_bits(x);
	count = count / 8 + (count % 8 > 0);
	return count;
}

void print(const vector<unsigned __int64>& Vector)
{
	size_t n = Vector.size();
	cout << "[";
	for (int i = 0; i < n - 1; i++)
	{
		cout << Vector[i] << ", ";
	}
	cout << Vector[n - 1] << "]" << endl;
};