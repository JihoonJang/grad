#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <cstdio>
#include "RBTree_kColor.h"

using namespace std;

int main()
{
    int k, total;
    cout << "Number of colors : ";
    cin >> k;

    vector<int> randomNumbers, operations;

    ifstream f("input.txt");
    f >> total;
    randomNumbers.resize(total);
    operations.resize(total);
    for (int i = 0; i < total; i++)
    {
        f >> operations[i] >> randomNumbers[i];
    }
    f.close();

    RBTree_kColor myset_kcolor(k);
    for (int i = 0; i < total; i++)
    {
        switch (operations[i])
        {
        case 0:
            myset_kcolor.erase(randomNumbers[i]);
            break;
        default:
            myset_kcolor.insert(randomNumbers[i]);
        }
    }
    cout << "Pure Execution time: " << myset_kcolor.getExecTime() << "\n";
    cout << "Height: " << myset_kcolor.getHeight() << "\n";
    cout << "Black Height: " << myset_kcolor.getBlackHeight() << "\n";
    cout << "Total Elements Count: " << myset_kcolor.getSize() << "\n";
    cout << "Black Count: " << myset_kcolor.getBlackSize() << "\n";
    cout << "Total Rotate: " << myset_kcolor.getTotalRotateCount() << ", insert: " << myset_kcolor.getInsertRotateCount() << ", delete: " << myset_kcolor.getDeleteRotateCount() << "\n";
    cout << "is Red black tree: " << myset_kcolor.isRBTree() << "\n";
    cout << endl;

    cout << "end" << endl;

    return 0;
}