#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logServer.h"
#include "ticketOffice.h"

using namespace std;

ofstream slog;
vector<pthread_t> tid;

void sigalarm_handler(int signum)
{
    //enviar sinal as threads para terminarem
    exit(1);
}

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

    if (mkfifo(REQUESTS, 0660) != 0)
    {
        perror("requests fifo:");
        return -1;
    }

    struct sigaction alarme;
    alarme.sa_handler = sigalarm_handler;
    sigemptyset(&alarme.sa_mask);
    alarme.sa_flags = 0;

    if (sigaction(SIGALRM, &alarme, NULL) == -1)
    {
        perror("sigaction:");
        return -4;
    }

    alarm(open_time);
    tid = vector<pthread_t>(num_ticket_offices);
    for (int i = 0; i < num_ticket_offices; i++)
    {
        if (pthread_create(&tid.at(i), NULL, thread_func, NULL) != 0)
        {
            perror("pthread_create:");
            return -2;
        }
    }

    int fdRequests;
    if ((fdRequests = open(REQUESTS, O_RDONLY)) == -1)
    {
        perror("requests fifo:");
        return -1;
    }

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

        if (read(fdRequests, &clientPID, sizeof(int)) == -1)
        {
            perror("read:");
            return -3;
        }

        if (read(fdRequests, &num_seats, sizeof(int)) == -1)
        {
            perror("read:");
            return -3;
        }

        if (read(fdRequests, &size, sizeof(int)) == -1)
        {
            perror("read:");
            return -3;
        }

        for (int i = 0; i < size; i++)
        {
            if (read(fdRequests, &seat, sizeof(int)) == -1)
            {
                perror("read:");
                return -3;
            }
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