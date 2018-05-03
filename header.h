#pragma once

#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <ctime>
#include <string>
#include <set>
#include <limits>

using std::vector;
using std::set;
using std::cout;
using std::endl;

void generate_combinations_recursive(int num_workers, size_t num_tasks, set<vector<int>>& result_bank, vector<int> curr_stack);
set<vector<int>> generate_combinations(int num_workers, size_t num_tasks);
vector<vector<int>> opt(vector<int> tasks, int n);
int getRandomInt(int min, int max);