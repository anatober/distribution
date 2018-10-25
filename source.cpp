#include "header.h"


int
main(int argc, char* argv[])
{
    std::ifstream inFile;
    std::ofstream outFile;
    std::string data;
    vector<int> tasksTimes;
    vector<int> workerRelationship;
    inFile.open(argv[1]);
    if (!inFile)
    {
        exit(1);
    }
    std::getline(inFile, data);
	std::istringstream ss(data);
	std::string token;
	while(std::getline(ss, token, ' '))
    {
		tasksTimes.push_back(atoi(token.c_str()));
	}

    std::getline(inFile, data);
    int basedOn = atoi(data.c_str());
    for (int i = 0; i < tasksTimes.size(); ++i)
    {
        tasksTimes[i] = tasksTimes[i] * 100 / (100 - (basedOn * 1.5));//normalize for relationship 0
    }
    std::getline(inFile, data);
    inFile.close();
	std::istringstream ss2(data);
    while(std::getline(ss2, token, ' '))
    {
		workerRelationship.push_back(atoi(token.c_str()));
	}
	auto optimal = opt(tasksTimes, workerRelationship);
    outFile.open("out");
    for (int i = 0; i < workerRelationship.size(); ++i)
    {
        outFile << workerRelationship[i] << ": ";
        for (int j = 0; j < optimal[i].size(); ++j)
        {
            outFile << optimal[i][j] + 1 << " ";
        }
        outFile << std::endl;
    }
    outFile.close();
    return 0;
}

void
generate_combinations_recursive(int num_workers, int num_tasks,
    set<vector<int> >& result_bank,
    vector<int> curr_stack)
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
            generate_combinations_recursive(num_workers - 1, num_tasks - i,
                result_bank, new_stack);
        }
    }
}

set<vector<int> >
generate_combinations(int num_workers, int num_tasks)
{
    set<vector<int> > result;
    generate_combinations_recursive(num_workers, num_tasks, result,
        vector<int>());
    return result;
}

vector<vector<int> >
opt(vector<int> tasks, vector<int> workerRelationship)
{
    vector<int> Mindeces(tasks.size());
    vector<vector<int> > N(workerRelationship.size());
    vector<vector<int> > tempOpt(workerRelationship.size());
    int X = 0;
	for (int i = 0; i < workerRelationship.size(); ++i)
	{
		int workerMean = 0;
		for (auto taskTime : tasks)
		{
			workerMean += (taskTime * (100 - (workerRelationship[i] * 1.5)) / 100);
		}
		X += (workerMean/workerRelationship.size());
	}
	X /= workerRelationship.size();// mean for all relationships
    unsigned int error = std::numeric_limits<int>::max(); // infinity
    for (unsigned int i = 0; i < tasks.size(); ++i)
    {
        Mindeces[i] = i;
    }
    auto amountCombos = generate_combinations(workerRelationship.size(), tasks.size());
    do
    {
        for (auto amountCombo : amountCombos) // (1,1,1,7; 1,1,2,6; etc)
        {
            unsigned int thisComboError = 0;
            auto offset = Mindeces.begin(); // offset for mission amount
            for (int i = 0; i < workerRelationship.size(); ++i) // for each worker
            {
                int companionSum = 0; // total working time for a worker
                N[i] = vector<int>(
                    offset,
                    offset + amountCombo[i]); // take amountCombo[i] next elements
                offset += amountCombo[i];
                for (int j = 0; j < amountCombo[i]; ++j)
                {
                    companionSum += (tasks[N[i][j]] * (100 - (workerRelationship[i] * 1.5)) / 100); // increase total working time (calculating for relationship)
                }
                thisComboError += abs(companionSum - X); // increase error by (total work time - optimal time)
            }
            if (thisComboError < error) // if current error is less than minimal
            {
                error = thisComboError;
                tempOpt = N; // current is better
            }
        }
    } while (std::next_permutation(Mindeces.begin(), Mindeces.end()));
    return tempOpt;
}