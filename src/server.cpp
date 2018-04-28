#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logServer.h"

using namespace std;

ofstream slog;

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

int isRequestValid(int num_wanted_seats, int num_room_seats, vector<int> pref_seats)
{
    if (!(num_wanted_seats >= 1 && num_wanted_seats <= MAX_CLI_SEATS))
        return -1;

    int size = pref_seats.size();
    int value = 0;

    if (!(size >= num_wanted_seats && size <= MAX_CLI_SEATS))
        return -2;

    for (int i = 0; i < size; i++)
    {
        value = pref_seats.at(i);

        if (!(value >= 1 && value <= num_room_seats))
            return -3;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int num_room_seats, num_ticket_offices, open_time;

    if (parseArguments(argv, num_room_seats, num_ticket_offices, open_time) == 1)
        return invalidArguments();

    mkfifo(REQUESTS, 0660);
    int fdRequests = open(REQUESTS, O_RDONLY);

    int num_seats;
    int size;
    int seat;
    vector<int> seats;

    read(fdRequests, &num_seats, sizeof(int));
    read(fdRequests, &size, sizeof(int));

    for (int i = 0; i < size; i++)
    {
        read(fdRequests, &seat, sizeof(int));
        seats.push_back(seat);
    }

    close(fdRequests);
    return 0;
}