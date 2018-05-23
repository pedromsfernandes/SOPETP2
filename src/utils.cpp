#include <iostream>
#include <sstream>
#include "utils.h"

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

string FIFOname(pid_t pid)
{
    return "ans" + to_string(pid);
}

string getRequestString(pid_t pid, int num_wanted_seats, const vector<int> &prefSeats)
{
    string request;
    char attrSep = '|', seatSep = '-';
    int size = prefSeats.size();

    request += to_string(pid) + attrSep + to_string(num_wanted_seats) + attrSep + to_string(size) + attrSep;

    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            request += seatSep;

        request += to_string(prefSeats.at(i));
    }

    return request;
}

Request *parseRequestString(const std::string request)
{
    istringstream str(request);
    int num_wanted_seats, size;
    pid_t clientPID;
    vector<int> prefSeats;
    int seat = 0;
    char sep;

    str >> clientPID >> sep >> num_wanted_seats >> sep >> size >> sep;

    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            str >> sep;
        str >> seat;

        prefSeats.push_back(seat);
    }

    return new Request(clientPID, num_wanted_seats, prefSeats);
}