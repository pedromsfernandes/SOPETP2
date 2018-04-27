#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "utils.h"
#include "macros.h"
#include <string.h>

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

    for (auto x : split)
    {  
        if (!isNumber(x))
            return vector<int>();

        parsed.push_back(stoi(x));
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

string FIFOname()
{
    return "ans" + to_string(getpid());
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int time_out, num_wanted_seats;
    vector<int> prefList;

    if (parseArguments(argv, time_out, num_wanted_seats, prefList) == 1)
        return invalidArguments();

    //string fifo = FIFOname();
    //mkfifo(fifo.c_str(), 0660);
    //int fdAns = open(fifo.c_str(), O_RDONLY);
    int fdRequest = open(REQUESTS, O_WRONLY);
    write(fdRequest, &num_wanted_seats, sizeof(int));
    int size = prefList.size();
    write(fdRequest, &size, sizeof(int));
    for(auto &x : prefList)
        write(fdRequest, &x, sizeof(int));


    return 0;
}