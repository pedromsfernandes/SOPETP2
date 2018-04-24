#include <iostream>
#include "utils.h"

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << "<time_out> <num_wanted_seats> <pref_seat_list>" << endl;
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    if (!(isNumber(argv[1]) && isNumber(argv[2])))
        return invalidArguments();

    return 0;
}