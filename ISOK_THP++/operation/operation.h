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

// проверка на возможность восстановления числа из долей по карте ошибок
bool error_check(int k, int n, vector<unsigned __int64> err);
int s2i(string val);

// проверка на наличие стертой доли в выбранном наборе долей
bool erased_check(vector<unsigned __int64> data, unsigned __int64 ChosenOne = 0);
// генерация ошибок на стирание для метода ИСОК
void erasing_error(vector<vector<unsigned __int64>>& A, vector<unsigned __int64> P, double probability);
// генерация ошибок на искажение для метода ИСОК
void corrupt_error(vector<vector<unsigned __int64>>& A, vector<unsigned __int64> P, double probability);
// генерация ошибок на стирание для метода ТХП
vector<vector<unsigned __int64>> erasing_error(int l, int n, double probability);
// построение карты генерации ошибок на искажение
vector<vector<unsigned __int64>> corrupt_error(int l, vector<unsigned __int64> P, double probability);