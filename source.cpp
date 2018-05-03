#include "header.h"

int main()
{
	cout.precision(3);
	srand((unsigned)time(0));
	while (true)
	{
		int n = getRandomInt(2, 8);
		int arrSize = getRandomInt(n + 1, 9);
		vector<int> a(arrSize);
		for (int i = 0; i < arrSize; ++i)
		{
			a[i] = getRandomInt(1, 1000);
		}
		cout << "Input array[" << a.size() << "]: ";
		for (unsigned int i = 0; i < a.size(); ++i)
		{
			cout << a[i] << ' ';
		}
		cout << endl << "Workers: " << n << endl;
		clock_t tStart = clock();
		auto optimal = opt(a, n);
		cout << "Time taken: " << std::fixed << (double)(clock() - tStart) / CLOCKS_PER_SEC << "s" << endl;
		system("pause");
		//system("cls");
		cout << "---------------------" << endl;
	}
	return 0;
}

void generate_combinations_recursive(int num_workers, size_t num_tasks, set<vector<int>>& result_bank, vector<int> curr_stack)
{
	if (num_tasks == 0 && num_workers == 0)
	{
		std::sort(curr_stack.begin(), curr_stack.end());
		result_bank.insert(curr_stack);
	}
	else
	{
		for (unsigned int i = 1; i < num_tasks + 1; ++i)
		{
			vector<int> new_stack = curr_stack;
			new_stack.push_back(i);
			generate_combinations_recursive(num_workers - 1, num_tasks - i, result_bank, new_stack);
		}
	}
}

set<vector<int>> generate_combinations(int num_workers, size_t num_tasks)
{
	set<vector<int>> result;
	generate_combinations_recursive(num_workers, num_tasks, result, vector<int>());
	return result;
}

vector<vector<int>> opt(vector<int> tasks, int n)
{
	vector<int> Mindeces(tasks.size());
	vector<vector<int>> N(n);
	vector<vector<int>> tempOpt(n);
	int X = (int)(std::accumulate(tasks.begin(), tasks.end(), 0) / n);//optimal time//fixme:linux round
	unsigned int error = std::numeric_limits<int>::max();;//infinity
	for (unsigned int i = 0; i < tasks.size(); ++i)
	{
		Mindeces[i] = i;
	}
	auto amountCombos = generate_combinations(n, tasks.size());
	do
	{
		for (auto amountCombo : amountCombos) //(1,1,1,7; 1,1,2,6; etc)
		{
			unsigned int thisComboError = 0;
			auto offset = Mindeces.begin();//offset for mission amount
			for (int i = 0; i < n; ++i) //for each worker
			{
				int companionSum = 0; //total working time for a worker
				N[i] = vector<int>(offset, offset + amountCombo[i]); //take amountCombo[+] next elements
				offset += amountCombo[i];
				for (int j = 0; j < amountCombo[i]; ++j)
				{
					companionSum += tasks[N[i][j]]; //increase total working time
				}
				thisComboError += abs(companionSum - X); //increase error by (total work time - optimal time)
			}
			if (thisComboError < error) //if current error is less than minimal
			{
				error = thisComboError;
				tempOpt = N; //current is optimal
				//cout << error << endl;
			}
		}
	} while (std::next_permutation(Mindeces.begin(), Mindeces.end()));
	cout << "Target time: " << X << endl << endl;
	cout << "Optimal distribution:" << endl;
	for (unsigned int i = 0; i < tempOpt.size(); ++i)
	{
		for (unsigned int j = 0; j < tempOpt[i].size(); ++j)
		{
			cout << tempOpt[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
	cout << "Error: " << error << endl;
	return tempOpt;
}

int getRandomInt(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}