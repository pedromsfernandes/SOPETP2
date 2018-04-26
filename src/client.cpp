#include <iostream>
#include <sstream>
#include "utils.h"
#include <vector>

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " <time_out> <num_wanted_seats> <pref_seat_list>" << endl;
    return 1;
}

vector<int> parseList(string list)
{
    vector<string> split = splitList(list);
    vector<int> parsed;
    int value;

    for (auto x : split)
    {
        if (!isNumber(x))
            return vector<int>();

        parsed.push_back(value);
    }

    return parsed;
}

int parseArguments(char *argv[], int &time_out, int &num_wanted_seats, vector<int> &pref_seat_list)
{
    if (!(isNumber(argv[1]) && isNumber(argv[2])))
        return 1;

    time_out = stoi(argv[1]);
    num_wanted_seats = stoi(argv[2]);
    pref_seat_list = parseList(argv[3]);

    if (pref_seat_list.size() == 0)
        return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int time_out, num_wanted_seats;
    vector<int> prefList;

    if (parseArguments(argv, time_out, num_wanted_seats, prefList) == 1)
        return invalidArguments();

    return 0;
}