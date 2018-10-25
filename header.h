#pragma once

#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

using std::vector;
using std::set;
using std::string;

void generate_combinations_recursive(int num_workers, int num_tasks, set<vector<int>>& result_bank, vector<int> curr_stack);
set<vector<int>> generate_combinations(int num_workers, int num_tasks);
vector<vector<int>> opt(vector<int> tasks, vector<int> workerRelationship);