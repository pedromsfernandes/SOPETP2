#include "utils.h"
#include <iostream>

using namespace std;

int invalidArguments()
{
    cout << "Invalid arguments!" << endl;
    return 1;
}

bool isNumber(const string &str)
{
    for (auto x : str)
    {
        if (!isdigit(x))
            return false;
    }

    try
    {
        stoi(str);
    }
    catch (invalid_argument)
    {
        return false;
    }
    catch (out_of_range)
    {
        return false;
    }

    return true;
}

vector<string> splitList(const string &list)
{
    vector<string> split;
    const string delim = " ";
    size_t pos1 = 0, pos2 = 0;

    while (true)
    {
        if ((pos2 = list.find(delim, pos1)) == string::npos)
        {
            split.push_back(list.substr(pos1, list.size() - pos1));
            break;
        }
        else
            split.push_back(list.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 1;
    }
    return split;
}

string getCode(int motive)
{
    string code;

    switch (motive)
    {
    case -1:
        code = "MAX";
        break;
    case -2:
        code = "NST";
        break;
    case -3:
        code = "IID";
        break;
    case -4:
        code = "ERR";
        break;
    case -5:
        code = "NAV";
        break;
    case -6:
        code = "FUL";
        break;
    case -7:
        code = "OUT";
    default:
        break;
    }

    return code;
}