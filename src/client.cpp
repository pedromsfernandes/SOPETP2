#include <iostream>

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << "<time_out> <num_wanted_seats> <pref_seat_list>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    return 0;
}