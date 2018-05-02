#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logServer.h"
#include "ticketOffice.h"

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

void *thread_func(void *arg)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int num_room_seats, num_ticket_offices, open_time;

    if (parseArguments(argv, num_room_seats, num_ticket_offices, open_time) == 1)
        return invalidArguments();

    mkfifo(REQUESTS, 0660);
    vector<pthread_t> tid(num_ticket_offices);

    for (int i = 0; i < num_ticket_offices; i++)
    {
        pthread_create(&tid.at(i), NULL, thread_func, NULL);
    }

    int fdRequests = open(REQUESTS, O_RDONLY);

    Request req;
    string fifo;
    int num_seats, size, seat, fdAns;
    pid_t clientPID;
    vector<int> prefSeats;
    Seat *seats = initSeats(num_room_seats);

    while (1)
    {
        fifo = FIFOname(clientPID);
        fdAns = open(fifo.c_str(), O_WRONLY);

        read(fdRequests, &clientPID, sizeof(int));
        read(fdRequests, &num_seats, sizeof(int));
        read(fdRequests, &size, sizeof(int));

        for (int i = 0; i < size; i++)
        {
            read(fdRequests, &seat, sizeof(int));
            prefSeats.push_back(seat);
        }

        req = Request(clientPID, num_seats, prefSeats);

        //direcionar para threads

        break;
    }

    delete[] seats;
    close(fdRequests);
    sleep(60);

    return 0;
}