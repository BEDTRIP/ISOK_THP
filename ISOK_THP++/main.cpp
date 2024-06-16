#include "Aspose.Cells.h"
#include "LongInt/LongInt.h"
#include "method_ISOK/isok.h"
#include "method_THP/thp.h"
#include "operation/operation.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <format>
#include <list>
#include <omp.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace Aspose::Cells;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	system("cls");
	cout << "ISOK-THP coding-decoding programm\n"
		<< "made by: BEDTRIP\n";
	for (;;)
	{
		cout << "-----------------------------------\n"
			<< "enter command:\n"
			<< "to   code file by ISOK enter '1'\n"
			<< "to decode file by ISOK enter '2'\n"
			<< "to   code file by THP  enter '3'\n"
			<< "to decode file by THP  enter '4'\n"
			<< "-----------------------------------\n"
			<< "to make statistics of decoding file by ISOK enter '5'\n"
			<< "to make statistics of decoding file by THP  enter '6'\n"
			<< "-----------------------------------\n"
			<< "enter 'q' to quit\n"
			<< "-----------------------------------\n"
			<< ">>> ";

		string str1;
		bool flag1 = false;
		cin >> str1;
		switch (s2i(str1))
		{
		case 113: // press 'q' to quit
		{
			flag1 = true;
			break;
		}
		case 49: // to code file by ISOK
		{
			system("cls");
			for (;;)
			{
				/////////////////////// Начальные данные
				cout << "place the file at the folder\n"
					<< "'../ISOK_THP++/File/InputFile'\n\n"
					<< "and enter the full name (ex: 'file.txt')\n"
					<< "-----------------------------------\n"
					<< "or enter 'q' to quit\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				string inputfilename;
				cin >> inputfilename;
				if (inputfilename == "q") break;

				int n;
				for (;;)
				{
					cout << "-----------------------------------\n"
						<< "enter n\n"
						<< "-----------------------------------\n"
						<< ">>> ";
					cin >> n;
					if (n > 0 && n <= 100) break;
					cout << "invalid value\n";
				}

				int k;
				for (;;)
				{
					cout << "-----------------------------------\n"
						<< "enter k\n"
						<< "-----------------------------------\n"
						<< ">>> ";
					cin >> k;
					if (k > 0 && k <= n) break;
					cout << "invalid value\n";
				}

				int b = 7 * 8;

				/////////////////////// Генерация ключей
				vector<unsigned __int64> P = key_auto_creation(b, k, n);

				/////////////////////// Чтение файла
				vector<unsigned __int64> A = read_file(format("../ISOK_THP++/File/InputFile/{}", inputfilename), b);

				/////////////////////// Кодирование
				vector<vector<unsigned __int64>> Clear_Result = Code_vec(P, n, b, A);

				/////////////////////// Запись остатков
				error_code e;
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\StoredFile\\isok_{}_k{}_n{}_b{}", inputfilename, k, n, b), e);
				write_matrix_splitbycolumn(format("../ISOK_THP++/File/StoredFile/isok_{}_k{}_n{}_b{}", inputfilename, k, n, b), inputfilename, P, "isok", Clear_Result);

				/////////////////////// Запись данных для восстановления
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\StoredFile\\LinkedFile"), e);
				ofstream fout(format("../ISOK_THP++/File/StoredFile/LinkedFile/{}.isok", inputfilename));

				fout << n << endl;
				fout << k << endl;
				fout << b << endl;

				fout.close();

				cout << "-----------------------------------\n"
					<< "File " << inputfilename << " stored\n"
					<< "===================================\n";
			}
			system("cls");
			break;
		}
		case 50: // to decode file by ISOK
		{
			system("cls");
			for (;;)
			{
				/////////////////////// Начальные данные
				cout << "pick the file from the folder\n"
					<< "'../ISOK_THP++/File/StoredFile/LinkedFile'\n\n"
					<< "and enter the full name without '.isok' (ex: 'file.txt')\n"
					<< "-----------------------------------\n"
					<< "or enter 'q' to quit\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				string inputfilename;
				cin >> inputfilename;
				if (inputfilename == "q") break;

				ifstream fin(format("../ISOK_THP++/File/StoredFile/LinkedFile/{}.isok", inputfilename));
				int n;
				fin >> n;
				int k;
				fin >> k;
				int b;
				fin >> b;
				fin.close();

				/////////////////////// Генерация ключей
				vector<unsigned __int64> P = key_auto_creation(b, k, n);

				////////////////////// Создание ортогональных базисов
				map<unsigned __int64, vector<LongInt>> AllBs = OrtogonalSystem(P, k, n);

				////////////////////// Чтение остатков
				vector<vector<unsigned __int64>> Read_Files = read_matrix_splitbycolumn(
					format("../ISOK_THP++/File/StoredFile/isok_{}_k{}_n{}_b{}", inputfilename, k, n, b),
					inputfilename, P, "isok");

				/////////////////////// Декодирование файла
				vector<unsigned __int64> Repaired_File(Read_Files.size());
				bool is_decoded = true;
#pragma omp parallel for
				for (int i = 0; i < Read_Files.size(); i++)
				{
					pair<unsigned __int64, bool> Ai = Decode(Read_Files[i], AllBs, P, b, k);
					if (!Ai.second) {
						is_decoded = false;
						break;
					}
					Repaired_File[i] = Ai.first;
				}

				/////////////////////// Запись файла
				if (is_decoded)
				{
					error_code e;
					filesystem::create_directories(format("..\\ISOK_THP++\\File\\Restored"), e);
					write_file(format("../ISOK_THP++/File/Restored/{}", inputfilename), b, Repaired_File);

					cout << "-----------------------------------\n"
						<< "File " << inputfilename << " decoded\n"
						<< "and stored at the folder\n"
						<< "'../ISOK_THP++/File/Restored'\n"
						<< "===================================\n";
				}
				else
				{
					cout << "-----------------------------------\n"
						<< "File did not decoded\n"
						<< "===================================\n";
				}
			}
			system("cls");
			break;
		}
		case 51: // to code file by THP
		{
			system("cls");
			for (;;)
			{
				/////////////////////// Начальные данные
				cout << "place the file at the folder\n"
					<< "'../ISOK_THP++/File/InputFile'\n\n"
					<< "and enter the full name (ex: 'file.txt')\n"
					<< "-----------------------------------\n"
					<< "or enter 'q' to quit\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				string inputfilename;
				cin >> inputfilename;
				if (inputfilename == "q") break;

				int n;
				for (;;)
				{
					cout << "-----------------------------------\n"
						<< "enter n\n"
						<< "-----------------------------------\n"
						<< ">>> ";
					cin >> n;
					if (n > 0 && n <= 100) break;
					cout << "invalid value\n";
				}

				int k;
				for (;;)
				{
					cout << "-----------------------------------\n"
						<< "enter k\n"
						<< "-----------------------------------\n"
						<< ">>> ";
					cin >> k;
					if (k > 0 && k <= n) break;
					cout << "invalid value\n";
				}

				/////////////////////// Генерация V
				Galois::Field gf(256);
				vector<vector<Galois::Element>> V = thp_key(gf, n, k);

				/////////////////////// Чтение файла
				pair<vector<vector<vector<Galois::Element>>>, int> Res = read_file_thp(format("../ISOK_THP++/File/InputFile/{}", inputfilename), gf, n, k);

				vector<vector<vector<Galois::Element>>> M_vec = Res.first;
				int dumbbell = Res.second;

				/////////////////////// Кодирование
				vector<vector<vector<Galois::Element>>> Clear_Result = THP_code(M_vec, V, gf);

				/////////////////////// Запись остатков
				error_code e;
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\StoredFile\\thp_{}_k{}_n{}", inputfilename, k, n), e);
				write_coded_thp(format("../ISOK_THP++/File/StoredFile/thp_{}_k{}_n{}", inputfilename, k, n), inputfilename, Clear_Result);

				/////////////////////// Запись данных для восстановления
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\StoredFile\\LinkedFile"), e);
				ofstream fout(format("../ISOK_THP++/File/StoredFile/LinkedFile/{}.thp", inputfilename));

				fout << n << ' ' << k << endl;
				fout << dumbbell << endl;

				for (auto row : V)
				{
					for (auto el : row)
						fout << setw(3) << setfill(' ') << el.value() << ' ';
					fout << endl;
				}

				fout.close();

				cout << "-----------------------------------\n"
					<< "File " << inputfilename << " stored\n"
					<< "===================================\n";
			}
			system("cls");
			break;
		}
		case 52: // to decode file by THP
		{
			system("cls");
			for (;;)
			{
				/////////////////////// Начальные данные
				cout << "pick the file from the folder\n"
					<< "'../ISOK_THP++/File/StoredFile/LinkedFile'\n\n"
					<< "and enter the full name without '.thp' (ex: 'file.txt')\n"
					<< "-----------------------------------\n"
					<< "or enter 'q' to quit\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				string inputfilename;
				cin >> inputfilename;
				if (inputfilename == "q") break;

				Galois::Field gf(256);

				ifstream fin(format("../ISOK_THP++/File/StoredFile/LinkedFile/{}.thp", inputfilename));
				int n;
				fin >> n;
				int k;
				fin >> k;
				int dumbbell;
				fin >> dumbbell;

				vector<vector<Galois::Element>> V = create_matrix(gf, n, k);

				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < k; j++)
					{
						int x;
						fin >> x;
						Galois::Element xx(&gf, x);
						V[i][j] = xx;
					}
				}

				fin.close();

				////////////////////// Создание обратных матриц для всех случаев
				map<unsigned __int64, vector<vector<Galois::Element>>> AllVs = OrtogonalSystem(gf, V);

				////////////////////// Чтение остатков
				vector<vector<vector<Galois::Element>>> Read_Files = read_coded_thp(format("../ISOK_THP++/File/StoredFile/thp_{}_k{}_n{}", inputfilename, k, n), inputfilename, n, gf);

				/////////////////////// Декодирование файла
				vector<vector<vector<Galois::Element>>> Repaired_File(Read_Files.size());
				bool is_decoded = true;
				for (int i = 0; i < Read_Files.size(); i++)
				{
					vector<unsigned __int64> ER;
					for (int j = 0; j < n; j++) ER.push_back(0);

					pair<vector<vector<Galois::Element>>, bool> Ai = THP_decode(gf, Read_Files[i], AllVs, ER, k);
					if (!Ai.second) {
						cout << i << endl;
						is_decoded = false;
						break;
					}
					Repaired_File[i] = Ai.first;

				}

				/////////////////////// Запись файла
				if (is_decoded)
				{
					error_code e;
					filesystem::create_directories(format("..\\ISOK_THP++\\File\\Restored"), e);
					write_file(format("../ISOK_THP++/File/Restored/{}", inputfilename), Repaired_File, dumbbell);

					cout << "-----------------------------------\n"
						<< "File " << inputfilename << " decoded\n"
						<< "and stored at the folder\n"
						<< "'../ISOK_THP++/File/Restored'\n"
						<< "===================================\n";
				}
				else
				{
					cout << "-----------------------------------\n"
						<< "File did not decoded\n"
						<< "===================================\n";
				}
			}
			system("cls");
			break;
		}
		case 53: // to make statistics of decoding file by ISOK
		{
			system("cls");
			cout << "place the file at the folder\n"
				<< "'../ISOK_THP++/File/InputFile'\n\n"
				<< "and enter the full name (ex: 'file.txt')\n"
				<< "-----------------------------------\n"
				<< "or enter 'q' to quit\n"
				<< "-----------------------------------\n"
				<< ">>> ";
			string inputfilename;
			cin >> inputfilename;
			if (inputfilename == "q") break;

			int steps = 0;
			int attempts = 1;

			list<double> erase_prob;
			double erase_prob_min = -1, erase_prob_max = -1;

			list<double> corrupt_prob;
			double corrupt_prob_min = -1, corrupt_prob_max = -1;
			
			bool flag3 = false;
			for (;;) 
			{
				cout << "-----------------------------------\n" 
					<< "       statistics for ISOK\n"
					<< "-----------------------------------\n"
					<< "to check erase errors enter    '1'\n"
					<< "to check corrupt errors enter  '2'\n"
					<< "to check both errors enter     '3'\n"
					<< "-----------------------------------\n"
					<< "or enter 'q' to quit\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				string str2;
				bool flag2 = false;
				cin >> str2;

				switch (s2i(str2))
				{
				case 113:// press 'q' to quit
				{
					flag2 = true;
					flag3 = true;
					break;
				}
				case 49: //erase errors
				{
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter probability for erasing like that\n"
							<< "Pmin Pmax\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> erase_prob_min;
						cin >> erase_prob_max;
						if (erase_prob_min > 0 && erase_prob_max >= erase_prob_min) 
							break;
						cout << "invalid value, the segment must be above zero\n";
					}
					flag2 = true;
					break;
				}
				case 50: //corrupt errors
				{
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter probability for corruption like that\n"
							<< "Pmin Pmax\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> corrupt_prob_min;
						cin >> corrupt_prob_max;
						if (corrupt_prob_min > 0 && corrupt_prob_max >= corrupt_prob_min)
							break;
						cout << "invalid value, the segment must be above zero\n";
					}
					flag2 = true;
					break;
				}
				case 51: //both errors
				{
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter probability for corruption like that\n"
							<< "Pmin Pmax count\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> corrupt_prob_min;
						cin >> corrupt_prob_max;
						cin >> steps;
						if (corrupt_prob_min > 0 && corrupt_prob_max >= corrupt_prob_min)
							break;
						cout << "invalid value, the segment must be above zero\n";
					}
					if (steps == 1)
					{
						corrupt_prob.push_back(corrupt_prob_min);
					}
					else
					{
						double step = (corrupt_prob_max - corrupt_prob_min) / (steps - 1);
						for (size_t i = 0; i <= steps - 1; i++)
						{
							corrupt_prob.push_back(corrupt_prob_min + step * i);
						}
					}
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter probability for erasing like that\n"
							<< "Pmin Pmax count\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> erase_prob_min;
						cin >> erase_prob_max;
						cin >> steps;
						if (erase_prob_min > 0 && erase_prob_max >= erase_prob_min)
							break;
						cout << "invalid value, the segment must be above zero\n";
					}
					if (steps == 1)
					{
						erase_prob.push_back(erase_prob_min);
					}
					else
					{
						double step = (erase_prob_max - erase_prob_min) / (steps - 1);
						for (size_t i = 0; i <= steps - 1; i++)
						{
							erase_prob.push_back(erase_prob_min + step * i);
						}
					}
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter count of attempts\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> attempts;
						if (attempts > 0)
							break;
						cout << "invalid value, count of attempts must be above zero\n";
					}

					int n;
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter n\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> n;
						if (n > 0 && n <= 100) break;
						cout << "invalid value\n";
					}

					int k1;
					int k2;
					for (;;)
					{
						cout << "-----------------------------------\n"
							<< "enter k\n"
							<< "-----------------------------------\n"
							<< ">>> ";
						cin >> k1;
						k2 = k1;
						if (k1 > 0 && k2 >= k1 && n >= k2) break;
						cout << "invalid value\n";
					}
					int K = k2;

					int b = 8 * 7; // b - кратное 8

					/////////////////////// Генерация ключей
					vector<unsigned __int64> P = key_auto_creation(b, K, n);

					/////////////////////// Чтение файла
					vector<unsigned __int64> A = read_file(format("../ISOK_THP++/File/InputFile/{}", inputfilename), b);

					/////////////////////// Кодирование
					vector<vector<unsigned __int64>> Clear_Result = Code_vec(P, n, b, A);

					/////////////////////// Запись остатков
					error_code e;
					filesystem::create_directories(format("..\\ISOK_THP++\\File\\RecoveryStatistics\\isok_{}_n{}\\isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K, n, b), e);
					write_matrix_splitbycolumn(format("../ISOK_THP++/File/RecoveryStatistics/isok_{}_n{}/isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K, n, b), inputfilename, P, "isok", Clear_Result);

					////////////////////// Чтение остатков
					vector<vector<unsigned __int64>> Read_Files = read_matrix_splitbycolumn(
						format("../ISOK_THP++/File/RecoveryStatistics/isok_{}_n{}/isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K, n, b),
						inputfilename, P, "isok");

					////////////////////// Создание ортогональных базисов
					map<unsigned __int64, vector<LongInt>> AllBs = OrtogonalSystem(P, K, n);

					Aspose::Cells::Startup();
					License lic;
					lic.SetLicense(u"Aspose.Cells.lic");

					Workbook workbook(FileFormatType::Xlsx);
					Worksheet sheet = workbook.GetWorksheets().Get(0);
					Cells cells = sheet.GetCells();

					cells.Get(0, 0).PutValue(u"is decoded");

					cells.Get(steps + 5, 0).PutValue(u"mid time");

					int o = 0;
					vector <double> prob_e;
					vector <double> prob_c;
					for (auto el : corrupt_prob) {
						cells.Get(0, o + 1).PutValue(el);
						cells.Get(steps + 5, o + 1).PutValue(el);
						o++;
						prob_c.push_back(el);
					}
					cells.Get(0, o + 1).PutValue(u"corrupt");
					o = 0;
					for (auto el : erase_prob) {
						cells.Get(o + 1, 0).PutValue(el);
						cells.Get(steps + 5 + o + 1, 0).PutValue(el);
						o++;
						prob_e.push_back(el);
					}
					cells.Get(o + 1, 0).PutValue(u"erasing");
					
					int counter_excel = 0;

					for (double probability_c : prob_c)
					{
						for (double probability_e : prob_e)
						{
							cout << "corrupt = " << probability_c << " | erasing = " << probability_e << endl;
							cells.Get((attempts + 2) * counter_excel, 0 + 15 - 1).PutValue(probability_c);
							cells.Get((attempts + 2) * counter_excel + 1, 0 + 15 - 1).PutValue(probability_e);

							cells.Get((attempts + 2) * counter_excel, 0 + 15).PutValue(u"N");
							cells.Get((attempts + 2) * counter_excel, 1 + 15).PutValue(u"is decoded");
							cells.Get((attempts + 2) * counter_excel, 2 + 15).PutValue(u"time, ms");
							cells.Get((attempts + 2) * counter_excel, 3 + 15).PutValue(u"broken parts");
							cells.Get((attempts + 2) * counter_excel, 4 + 15).PutValue(u"corrupt parts");
							cells.Get((attempts + 2) * counter_excel, 5 + 15).PutValue(u"erased parts");
							cells.Get((attempts + 2) * counter_excel, 6 + 15).PutValue(u"all parts");

							for (int ii = 0; ii < attempts; ii++)
							{
								auto begin = chrono::high_resolution_clock::now();

								vector<vector<unsigned __int64>> erre = erasing_error(Read_Files.size(), n, probability_e);
								vector<vector<unsigned __int64>> errc = corrupt_error(Read_Files.size(), P, probability_c);

								int x_b = 0, x_c = 0, x_e = 0;

								for (size_t i = 0; i < erre.size(); i++)
								{
									for (size_t j = 0; j < erre[i].size(); j++)
									{
										if (erre[i][j] || errc[i][j]) x_b++;
										if (erre[i][j]) x_e++;
										if (errc[i][j]) x_c++;
									}
								}

								vector<vector<unsigned __int64>> errb(erre.size());
								for (int i = 0; i < errb.size(); i++) {
									errb[i].resize(n);
									for (int j = 0; j < n; j++) {
										errb[i][j] = errc[i][j] | erre[i][j];
									}
								}

								bool q = true;
#pragma omp parallel for
								for (int i = 0; i < erre.size(); i++)
								{
									if (error_check(K, n, errb[i])) {
										q = false;
										break;
									}
								}

								auto end = chrono::high_resolution_clock::now();
								int qq = chrono::duration_cast<chrono::milliseconds> (end - begin).count();
								cout << ii << ' ';

								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 0).PutValue((int)(ii + 1));
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 1).PutValue((int)q);
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 2).PutValue(qq);
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 3).PutValue(x_b);
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 4).PutValue(x_c);
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 5).PutValue(x_e);
								cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 6).PutValue((int)(Read_Files.size() * Read_Files[0].size()));
							}

							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 0).PutValue(u"TOTAL");
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 1).SetFormula((format("=SUM(Q{}:Q{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 2).SetFormula((format("=AVERAGE(R{}:R{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 3).SetFormula((format("=AVERAGE(S{}:S{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 4).SetFormula((format("=AVERAGE(T{}:T{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 5).SetFormula((format("=AVERAGE(U{}:U{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
							cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 6).SetFormula((format("=V{}", (attempts + 2) * counter_excel + 2).c_str()));

							cells.Get(counter_excel % (steps) + 1, (int)(find(prob_c.begin(), prob_c.end(), probability_c) - prob_c.begin()) + 1).SetFormula((format("=Q{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));
							cells.Get(steps + 5 + counter_excel % (steps) + 1, (int)(find(prob_c.begin(), prob_c.end(), probability_c) - prob_c.begin()) + 1).SetFormula((format("=R{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));

							Style st = cells.GetStyle();
							st.SetPattern(BackgroundType::Solid);
							st.SetForegroundColor(Color{ 0xff, 0xff, 0xff, 0 });//Yellow
							for (size_t i = 0; i < 7; i++)
							{
								Cell cell = sheet.GetCells().Get((attempts + 2) * counter_excel + attempts + 1, 15 + i);
								cell.SetStyle(st);
							}

							counter_excel++;
							cout << endl;
						}
					}
					

					workbook.Save(format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_k{}_count{}_both.xlsx",
						inputfilename, n, inputfilename,
						n, K,
						attempts
					).c_str());
					cout << "-----------------------------------\n"
						<< "Finished, excel file writed in:\n"
						<< "../" << format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_k{}_count{}_both.xlsx",
							inputfilename, n, inputfilename,
							n, K,
							attempts)
						<< "\n===================================\n";

					flag2 = true;
					flag3 = true;
					break;
				}
				default:
				{
					system("cls");
					cout << "invalid input\n"
						<< "===================================\n";
				}
				}
				if (flag2)
					break;
			}
			if (flag3)
				break;

			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter count of dots between Pmin & Pmax (including them)\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> steps;
				if (steps > 0)
					break;
				cout << "invalid value, count of dots must be above zero\n";
			}
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter count of attempts\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> attempts;
				if (attempts > 0)
					break;
				cout << "invalid value, count of attempts must be above zero\n";
			}

			if (steps == 1) 
			{
				corrupt_prob.push_back(corrupt_prob_min);
				erase_prob.push_back(erase_prob_min);
			}
			else 
			{
				double step = (corrupt_prob_max - corrupt_prob_min) / (steps - 1);
				for (size_t i = 0; i <= steps - 1; i++)
				{
					corrupt_prob.push_back(corrupt_prob_min + step * i);
				}
				step = (erase_prob_max - erase_prob_min) / (steps - 1);
				for (size_t i = 0; i <= steps - 1; i++)
				{
					erase_prob.push_back(erase_prob_min + step * i);
				}
			}

			int n;
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter n\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> n;
				if (n > 0 && n <= 100) break;
				cout << "invalid value\n";
			}

			int k1;
			int k2;
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter k like that\n"
					<< "Kmin Kmax\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> k1;
				cin >> k2;
				if (k1 > 0 && k2 >= k1 && n >= k2) break;
				cout << "invalid value\n";
			}
			vector<int> K(k2-k1+1);
			for (int i = 0; i < K.size(); i++)
			{
				K[i] = k1 + i;
			}

			int b = 8 * 7; // b - кратное 8

			/////////////////////// Генерация ключей
			vector<vector<unsigned __int64>> P_i(K.size());
			for (int k = 0; k < K.size(); k++)
			{
				P_i[k] = key_auto_creation(b, K[k], n);
			}

			/////////////////////// Чтение файла
			vector<unsigned __int64> A = read_file(format("../ISOK_THP++/File/InputFile/{}", inputfilename), b);

			for (int k = 0; k < K.size(); k++)
			{
				/////////////////////// Кодирование
				vector<vector<unsigned __int64>> Clear_Result = Code_vec(P_i[k], n, b, A);

				/////////////////////// Запись остатков
				error_code e;
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\RecoveryStatistics\\isok_{}_n{}\\isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K[k], n, b), e);
				write_matrix_splitbycolumn(format("../ISOK_THP++/File/RecoveryStatistics/isok_{}_n{}/isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K[k], n, b), inputfilename, P_i[k], "isok", Clear_Result);
			}

			Aspose::Cells::Startup();
			License lic;
			lic.SetLicense(u"Aspose.Cells.lic");

			Workbook workbook(FileFormatType::Xlsx);
			Worksheet sheet = workbook.GetWorksheets().Get(0);
			Cells cells = sheet.GetCells();

			cells.Get(0, 0).PutValue(u"is decoded");

			cells.Get(steps + 5, 0).PutValue(u"mid time");

			int o = 0;

			vector <double> prob;

			if (erase_prob_min > 0)
			{
				for (auto el : erase_prob) {
					cells.Get(o + 1, 0).PutValue(el);
					cells.Get(steps + 5 + o + 1, 0).PutValue(el);
					o++;
					prob.push_back(el);
				}
			}
			else 
			{
				for (auto el : corrupt_prob) {
					cells.Get(o + 1, 0).PutValue(el);
					cells.Get(steps + 5 + o + 1, 0).PutValue(el);
					o++;
					prob.push_back(el);
				}
			}

			int counter_excel = 0;

			for (int k = 0; k < K.size(); k++)
			{
				////////////////////// Чтение остатков
				vector<vector<unsigned __int64>> Read_Files = read_matrix_splitbycolumn(
					format("../ISOK_THP++/File/RecoveryStatistics/isok_{}_n{}/isok_{}_k{}_n{}_b{}", inputfilename, n, inputfilename, K[k], n, b),
					inputfilename, P_i[k], "isok");

				cells.Get(0, k + 1).PutValue(K[k]);
				cells.Get(steps + 5, k+1).PutValue(K[k]);

				////////////////////// Создание ортогональных базисов
				map<unsigned __int64, vector<LongInt>> AllBs = OrtogonalSystem(P_i[k], K[k], n);

				for (double probability : prob)
				{
					cout << "k = " << K[k] << " | prob = " << probability << endl;
					cells.Get((attempts + 2) * counter_excel, 0 + 15 - 1).PutValue(K[k]);
					cells.Get((attempts + 2) * counter_excel + 1, 0 + 15 - 1).PutValue(probability);

					cells.Get((attempts + 2) * counter_excel, 0 + 15).PutValue(u"N");
					cells.Get((attempts + 2) * counter_excel, 1 + 15).PutValue(u"is decoded");
					cells.Get((attempts + 2) * counter_excel, 2 + 15).PutValue(u"time, ms");
					cells.Get((attempts + 2) * counter_excel, 3 + 15).PutValue(u"broken parts");
					cells.Get((attempts + 2) * counter_excel, 4 + 15).PutValue(u"all parts");

					for (int ii = 0; ii < attempts; ii++)
					{
						auto begin = chrono::high_resolution_clock::now();

						vector<vector<unsigned __int64>> err;

						if (erase_prob_min > 0)
						{
							vector<vector<unsigned __int64>> err1 = erasing_error(Read_Files.size(), n, probability);
							err = err1;
						}
						else
						{
							vector<vector<unsigned __int64>> err1 = corrupt_error(Read_Files.size(), P_i[k], probability);
							err = err1;
						}

						int x = 0;
						for (size_t i = 0; i < Read_Files.size(); i++)
						{
							for (size_t j = 0; j < Read_Files[i].size(); j++)
							{
								if (err[i][j]) x++;
							}
						}

						bool q = true;
#pragma omp parallel for
						for (int i = 0; i < err.size(); i++)
						{
							if (error_check(K[k], n, err[i])) {
								q = false;
								break;
							}
						}

						auto end = chrono::high_resolution_clock::now();
						int qq = chrono::duration_cast<chrono::milliseconds> (end - begin).count();
						cout << ii << ' ';

						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 0).PutValue((int)(ii + 1));
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 1).PutValue((int)q);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 2).PutValue(qq);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 3).PutValue(x);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 4).PutValue((int)(Read_Files.size() * Read_Files[0].size()));
					}

					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 0).PutValue(u"TOTAL");
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 1).SetFormula((format("=SUM(Q{}:Q{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 2).SetFormula((format("=AVERAGE(R{}:R{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 3).SetFormula((format("=AVERAGE(S{}:S{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 4).SetFormula((format("=T{}", (attempts + 2) * counter_excel + 2).c_str()));

					cells.Get(counter_excel % (steps) + 1, k + 1).SetFormula((format("=Q{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));
					cells.Get(steps + 5 + counter_excel % (steps) + 1, k + 1).SetFormula((format("=R{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));

					Style st = cells.GetStyle();
					st.SetPattern(BackgroundType::Solid);
					st.SetForegroundColor(Color{ 0xff, 0xff, 0xff, 0 });//Yellow
					for (size_t i = 0; i < 5; i++)
					{
						Cell cell = sheet.GetCells().Get((attempts + 2) * counter_excel + attempts + 1, 15 + i);
						cell.SetStyle(st);
					}

					counter_excel++;
					cout << endl;
				}
			}

			if (erase_prob_min > 0)
			{
				workbook.Save(format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_b{}_count{}_erase.xlsx",
					inputfilename, n, inputfilename,
					n, b,
					attempts
				).c_str());
				cout << "-----------------------------------\n"
					<< "Finished, excel file writed in:\n"
					<< "../" << format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_b{}_count{}_erase.xlsx",
						inputfilename, n, inputfilename,
						n, b,
						attempts)
					<< "\n===================================\n";
			}
			else
			{
				workbook.Save(format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_b{}_count{}_corrupt.xlsx",
					inputfilename, n, inputfilename,
					n, b,
					attempts
				).c_str());
				cout << "-----------------------------------\n"
					<< "Finished, excel file writed in:\n"
					<< "../" << format("File/RecoveryStatistics/isok_{}_n{}/{}_n{}_b{}_count{}_corrupt.xlsx",
						inputfilename, n, inputfilename,
						n, b,
						attempts)
					<< "\n===================================\n";
			}
			break;
		}
		case 54: // to make statistics of decoding file by THP
		{
			system("cls");
			cout << "place the file at the folder\n"
				<< "'../ISOK_THP++/File/InputFile'\n\n"
				<< "and enter the full name (ex: 'file.txt')\n"
				<< "-----------------------------------\n"
				<< "or enter 'q' to quit\n"
				<< "-----------------------------------\n"
				<< ">>> ";
			string inputfilename;
			cin >> inputfilename;
			if (inputfilename == "q") break;

			int steps = 0;
			int attempts = 1;

			list<double> erase_prob;
			double erase_prob_min = -1, erase_prob_max = -1;

			cout << "-----------------------------------\n"
				<< "       statistics for THP\n";

			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter probability for erasing like that\n"
					<< "Pmin Pmax\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> erase_prob_min;
				cin >> erase_prob_max;
				if (erase_prob_min > 0 && erase_prob_max >= erase_prob_min)
					break;
				cout << "invalid value, the segment must be above zero\n";
			}
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter count of dots between Pmin & Pmax (including them)\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> steps;
				if (steps > 0)
					break;
				cout << "invalid value, count of dots must be above zero\n";
			}
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter count of attempts\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> attempts;
				if (attempts > 0)
					break;
				cout << "invalid value, count of attempts must be above zero\n";
			}

			if (steps == 1)
			{
				erase_prob.push_back(erase_prob_min);
			}
			else
			{
				double step = (erase_prob_max - erase_prob_min) / (steps - 1);
				for (size_t i = 0; i <= steps - 1; i++)
				{
					erase_prob.push_back(erase_prob_min + step * i);
				}
			}

			int n;
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter n\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> n;
				if (n > 0 && n <= 100) break;
				cout << "invalid value\n";
			}

			int k1;
			int k2;
			for (;;)
			{
				cout << "-----------------------------------\n"
					<< "enter k like that\n"
					<< "Kmin Kmax\n"
					<< "-----------------------------------\n"
					<< ">>> ";
				cin >> k1;
				cin >> k2;
				if (k1 > 0 && k2 >= k1 && n >= k2) break;
				cout << "invalid value\n";
			}
			vector<int> K(k2 - k1 + 1);
			for (int i = 0; i < K.size(); i++)
			{
				K[i] = k1 + i;
			}

			/////////////////////// Генерация ключей
			Galois::Field gf(256);
			vector<vector<vector<Galois::Element>>> V(K.size());
			vector<int> Read_Files_size;
			for (int k = 0; k < K.size(); k++)
			{
				V[k] = thp_key(gf, n, K[k]);

				/////////////////////// Чтение файла
				pair<vector<vector<vector<Galois::Element>>>, int> Res = read_file_thp(format("../ISOK_THP++/File/InputFile/{}", inputfilename), gf, n, K[k]);
				vector<vector<vector<Galois::Element>>> M_vec = Res.first;
				int dumbbell = Res.second;

				/////////////////////// Кодирование
				vector<vector<vector<Galois::Element>>> Clear_Result = THP_code(M_vec, V[k], gf);

				/////////////////////// Запись остатков
				error_code e;
				filesystem::create_directories(format("..\\ISOK_THP++\\File\\RecoveryStatistics\\thp_{}_n{}\\thp_{}_k{}_n{}", inputfilename, n, inputfilename, K[k], n), e);
				write_coded_thp(format("../ISOK_THP++/File/RecoveryStatistics/thp_{}_n{}/thp_{}_k{}_n{}", inputfilename, n, inputfilename, K[k], n), inputfilename, Clear_Result);

				////////////////////// Чтение остатков
				vector<vector<vector<Galois::Element>>> Read_Files = read_coded_thp(format("../ISOK_THP++/File/RecoveryStatistics/thp_{}_n{}/thp_{}_k{}_n{}", inputfilename, n, inputfilename, K[k], n), inputfilename, n, gf);
				Read_Files_size.push_back(Read_Files.size());
			}

			Aspose::Cells::Startup();
			License lic;
			lic.SetLicense(u"Aspose.Cells.lic");

			Workbook workbook(FileFormatType::Xlsx);
			Worksheet sheet = workbook.GetWorksheets().Get(0);
			Cells cells = sheet.GetCells();

			cells.Get(0, 0).PutValue(u"is decoded");

			cells.Get(steps + 5, 0).PutValue(u"mid time");

			int o = 0;

			vector <double> prob;

			for (auto el : erase_prob) {
				cells.Get(o + 1, 0).PutValue(el);
				cells.Get(steps + 5 + o + 1, 0).PutValue(el);
				o++;
				prob.push_back(el);
			}

			int counter_excel = 0;

			for (int k = 0; k < K.size(); k++)
			{
				cells.Get(0, k + 1).PutValue(K[k]);
				cells.Get(steps + 5, k + 1).PutValue(K[k]);

				for (double probability : prob)
				{
					cout << "k = " << K[k] << " | prob = " << probability << endl;
					cells.Get((attempts + 2) * counter_excel, 0 + 15 - 1).PutValue(K[k]);
					cells.Get((attempts + 2) * counter_excel + 1, 0 + 15 - 1).PutValue(probability);

					cells.Get((attempts + 2) * counter_excel, 0 + 15).PutValue(u"N");
					cells.Get((attempts + 2) * counter_excel, 1 + 15).PutValue(u"is decoded");
					cells.Get((attempts + 2) * counter_excel, 2 + 15).PutValue(u"time, ms");
					cells.Get((attempts + 2) * counter_excel, 3 + 15).PutValue(u"broken parts");
					cells.Get((attempts + 2) * counter_excel, 4 + 15).PutValue(u"all parts");

					for (int ii = 0; ii < attempts; ii++)
					{
						auto begin = chrono::high_resolution_clock::now();
						vector<vector<unsigned __int64>> err = erasing_error(Read_Files_size[k], n, probability);

						int x = 0;

						for (size_t i = 0; i < err.size(); i++)
						{
							for (size_t j = 0; j < err[i].size(); j++)
							{
								if (err[i][j]) x++;
							}
						}

						//vector<unsigned __int64> Repaired_File(Row.size());
						bool q = true;
#pragma omp parallel for
						for (int i = 0; i < err.size(); i++)
						{
							if (error_check(K[k], n, err[i])) {
								q = false;
								break;
							}
						}

						auto end = chrono::high_resolution_clock::now();
						int qq = chrono::duration_cast<chrono::milliseconds> (end - begin).count();
						cout << ii << ' ';

						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 0).PutValue((int)(ii + 1));
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 1).PutValue((int)q);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 2).PutValue(qq);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 3).PutValue(x);
						cells.Get((attempts + 2) * counter_excel + 1 + ii, 15 + 4).PutValue((int)(Read_Files_size[k]* n));
					}

					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 0).PutValue(u"TOTAL");
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 1).SetFormula((format("=SUM(Q{}:Q{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 2).SetFormula((format("=AVERAGE(R{}:R{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 3).SetFormula((format("=AVERAGE(S{}:S{})", (attempts + 2) * counter_excel + 2, (attempts + 2) * counter_excel + attempts + 1).c_str()));
					cells.Get((attempts + 2) * counter_excel + attempts + 1, 15 + 4).SetFormula((format("=T{}", (attempts + 2) * counter_excel + 2).c_str()));

					cells.Get(counter_excel % (steps)+1, k + 1).SetFormula((format("=Q{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));
					cells.Get(steps + 5 + counter_excel % (steps)+1, k + 1).SetFormula((format("=R{}", (attempts + 2) * counter_excel + attempts + 1 + 1).c_str()));

					Style st = cells.GetStyle();
					st.SetPattern(BackgroundType::Solid);
					st.SetForegroundColor(Color{ 0xff, 0xff, 0xff, 0 });//Yellow
					for (size_t i = 0; i < 5; i++)
					{
						Cell cell = sheet.GetCells().Get((attempts + 2) * counter_excel + attempts + 1, 15 + i);
						cell.SetStyle(st);
					}

					counter_excel++;
					cout << endl;
				}
			}

			workbook.Save(format("File/RecoveryStatistics/thp_{}_n{}/{}_n{}_count{}_erase.xlsx",
				inputfilename, n, inputfilename,
				n,
				attempts
			).c_str());

			cout << "-----------------------------------\n"
				<< "Finished, excel file writed in:\n"
				<< "../" << format("File/RecoveryStatistics/thp_{}_n{}/{}_n{}_count{}_erase.xlsx",
					inputfilename, n, inputfilename,
					n,
					attempts)
				<< "\n===================================\n";
			break;
		}
		default:
		{
			system("cls");
			cout << "invalid input\n"
				<< "===================================\n";
		}
		}
		if (flag1)
			break;
	}
	return 0;
}
