#include "utils.h"
#include <iostream>

using namespace std;

bool isNumber(string str)
{
    for (char x : str)
        if (!isdigit(x))
            return false;

    return true;
}

int invalidArguments()
{
    cout << "Invalid arguments!" << endl;
    return 1;
}