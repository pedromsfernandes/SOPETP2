#include <iostream>
#include "utils.h"

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " <num_room_seats> <num_ticket_offices> <open_time> " << endl;
    return 1;
}

int parseArguments(char *argv[], int &num_room_seats, int &num_ticket_offices, int &open_time)
{
    if (!(isNumber(argv[1]) && isNumber(argv[2]) && isNumber(argv[3])))
        return 1;

    num_room_seats = stoi(argv[1]);
    num_ticket_offices = stoi(argv[2]);
    open_time = stoi(argv[3]);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int num_room_seats, num_ticket_offices, open_time;

    if (parseArguments(argv, num_room_seats, num_ticket_offices, open_time) == 1)
        return invalidArguments();

    return 0;
}