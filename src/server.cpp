#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logServer.h"
#include "ticketOffice.h"

using namespace std;

Seat *seats;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Request *req = NULL;

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

int verifyRequest(Request *request)
{
    int seats_wanted = request->getNumWantedSeats();
    vector<int> prefSeats = request->getPrefSeats();

    if (seats_wanted > MAX_CLI_SEATS)
        return -1;

    if (prefSeats.size() > MAX_CLI_SEATS || prefSeats.size() < seats_wanted)
        return -2;

    //CHECKAR OUTROS ERROS
}

void *thread_func(void *arg)
{
    Request *a_tratar;
    pid_t clientPID;
    int fdAns, seats_wanted, seats_taken;
    vector<int> prefSeats, atrSeats;
    string fifo;

    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (req == NULL)
            pthread_cond_wait(&cond, &mutex);

        a_tratar = req;
        req = NULL;

        pthread_mutex_unlock(&mutex);

        fifo = FIFOname(clientPID);
        fdAns = open(fifo.c_str(), O_WRONLY);

        if (verifyRequest(a_tratar) < 0)
        {
            //DAR LOG ERRO E ENVIAR RESPOSTA
            delete a_tratar;
        }

        clientPID = a_tratar->getClientPID();
        seats_wanted = a_tratar->getNumWantedSeats();
        prefSeats = a_tratar->getPrefSeats();

        for (int i = 0; i < prefSeats.size() && seats_taken < seats_wanted; i++)
        {
            if (isSeatFree(seats, prefSeats[i]))
            {
                bookSeat(seats, prefSeats[i], clientPID);
                seats_taken++;
                atrSeats.push_back(prefSeats[i]);
            }
        }

        //FAIL
        if (seats_taken < seats_wanted)
        {
            for (int i = 0; i < atrSeats.size(); i++)
            {
                freeSeat(seats, atrSeats[i]);
            }

            //DAR LOG E ENVIAR RESPOSTA
        }

        //WIN
        else
        {
        }

        atrSeats.clear();
        delete a_tratar;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int num_room_seats, num_ticket_offices, open_time;

    if (parseArguments(argv, num_room_seats, num_ticket_offices, open_time) == 1)
        return invalidArguments();

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

    if (mkfifo(REQUESTS, 0660) != 0)
    {
        perror("requests fifo:");
        return -1;
    }

    seats = initSeats(num_room_seats);

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

    int num_seats, size, seat;
    pid_t clientPID;
    vector<int> prefSeats;

    while (1)
    {
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

        Request *temp_req = new Request(clientPID, num_seats, prefSeats);

        pthread_mutex_lock(&mutex);
        req = temp_req;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        break;
    }

    delete[] seats;
    close(fdRequests);
    sleep(60);

    return 0;
}