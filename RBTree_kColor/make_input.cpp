#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include "RBTree_kColor.h"

using namespace std;

int RANDOM_MAX = 100000000;
std::mt19937 gen(time(NULL));
std::uniform_real_distribution<double> dis(0, 1); //uniform distribution between 0 and 1

const int getRandInt(const int _max)
{
    return dis(gen) * _max;
}

int main()
{
    int total, delete_percent = 50;
    cout << "total elements : ";
    cin >> total;
    ofstream f("input.txt");

    f << total << "\n";

    vector<int> randomNumbers, searched, operations;
    for (int i = 0; i < total; i++)
    {
        int op = getRandInt(100);
        int rand_num;
        if (op < delete_percent && randomNumbers.size() > 0)
        {
            op = 0;
            swap(randomNumbers[getRandInt(randomNumbers.size())], randomNumbers.back());
            rand_num = randomNumbers.back();
            randomNumbers.pop_back();
        }
        else
        {
            op = 1;
            rand_num = getRandInt(RANDOM_MAX);
            randomNumbers.push_back(rand_num);
        }
        f << op << " " << rand_num << "\n";
    }
    f.close();

    return 0;
}