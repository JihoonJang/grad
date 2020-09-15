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

const int getRandInt()
{
    return dis(gen) * RANDOM_MAX;
}

const double getRandDouble()
{
    return dis(gen);
}

template <typename S>
auto select_random(const S &s, size_t n)
{
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it, n);
    return it;
}

int main()
{
    int total, delete_percent = 50;
    cout << "total elements : ";
    cin >> total;
    ofstream f("input.txt");

    f << total << "\n";

    vector<int> randomNumbers, searched, operations;
    for (int i = 0; i < RANDOM_MAX; i++)
    {
        int op = (int)(getRandDouble() * 100);
        int rand_num;
        if (op < delete_percent && randomNumbers.size() > 0)
        {
            op = 0;
            swap(randomNumbers[(int)(getRandDouble() * randomNumbers.size())], randomNumbers.back());
            rand_num = randomNumbers.back();
            randomNumbers.pop_back();
        }
        else
        {
            op = 1;
            rand_num = getRandInt();
            randomNumbers.push_back(rand_num);
        }
        f << op << " " << rand_num << "\n";
    }
    f.close();

    return 0;
}