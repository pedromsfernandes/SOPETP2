#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
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
int num_room_seats;
int num_seats_taken = 0;

pthread_cond_t request_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t request_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t logfile_mutex = PTHREAD_MUTEX_INITIALIZER;

Request *req = NULL;

ofstream slog;
ofstream sbook;

bool timeout = false;

void sigalarm_handler(int signum)
{
    timeout = true;
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

int verifyRequest(Request *request)
{
    unsigned int seats_wanted = request->getNumWantedSeats();
    vector<int> prefSeats = request->getPrefSeats();

    if (seats_wanted < 1 || seats_wanted > MAX_CLI_SEATS)
        return -1;

    if (prefSeats.size() < seats_wanted || prefSeats.size() > MAX_CLI_SEATS)
        return -2;

    for (unsigned int i = 0; i < prefSeats.size(); i++)
    {
        if (prefSeats[i] < 1 || prefSeats[i] > num_room_seats)
            return -3;
    }

    if (num_room_seats == num_seats_taken)
        return -6;

    return 0;
}

void *thread_func(void *arg)
{
    pthread_mutex_lock(&logfile_mutex);
    int bilhID = *((int *)arg);
    logTicketOfficeOpen(bilhID);
    pthread_mutex_unlock(&logfile_mutex);
    Request *a_tratar;
    pid_t clientPID;
    int fdAns, seats_wanted, seats_taken;
    vector<int> prefSeats, atrSeats;
    string fifo;

    while (/*!timeout*/ 1)
    {
        pthread_mutex_lock(&request_mutex);
        while (req == NULL) //&& !timeout)
            pthread_cond_wait(&request_cond, &request_mutex);

        /*if (timeout)
        {
            pthread_mutex_unlock(&request_mutex);
            break;
        }*/

        a_tratar = req;
        req = NULL;

        pthread_mutex_unlock(&request_mutex);

        cout << bilhID << "woke\n";

        clientPID = a_tratar->getClientPID();
        seats_wanted = a_tratar->getNumWantedSeats();
        prefSeats = a_tratar->getPrefSeats();

        fifo = FIFOname(clientPID);
        fdAns = open(fifo.c_str(), O_WRONLY);

        int error;

        if ((error = verifyRequest(a_tratar)) < 0)
        {
            pthread_mutex_lock(&logfile_mutex);
            logUnSuccessfulRequest(bilhID, clientPID, prefSeats, error);
            pthread_mutex_unlock(&logfile_mutex);
            write(fdAns, &error, sizeof(int));

            close(fdAns);
            delete a_tratar;
            continue;
        }

        for (unsigned int i = 0; i < prefSeats.size() && seats_taken < seats_wanted; i++)
        {
            if (isSeatFree(seats, prefSeats[i]))
            {
                bookSeat(seats, prefSeats[i], clientPID);
                seats_taken++;
                num_seats_taken++;
                atrSeats.push_back(prefSeats[i]);
            }
        }

        if (seats_taken < seats_wanted)
        {
            for (unsigned int i = 0; i < atrSeats.size(); i++)
            {
                freeSeat(seats, atrSeats[i]);
                num_seats_taken--;
            }

            error = -5;

            pthread_mutex_lock(&logfile_mutex);
            logUnSuccessfulRequest(bilhID, clientPID, prefSeats, error);
            pthread_mutex_unlock(&logfile_mutex);

            write(fdAns, &error, sizeof(int));
        }

        else
        {
            pthread_mutex_lock(&logfile_mutex);
            logSuccessfulRequest(bilhID, clientPID, prefSeats, atrSeats);
            pthread_mutex_unlock(&logfile_mutex);

            write(fdAns, &seats_wanted, sizeof(int));

            for (unsigned int i = 0; i < atrSeats.size(); i++)
                write(fdAns, &atrSeats[i], sizeof(int));
        }

        close(fdAns);
        atrSeats.clear();
        delete a_tratar;
    }

    logTicketOfficeClose(bilhID);
    delete (int *)arg;
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int num_ticket_offices, open_time;

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

    mkfifo(REQUESTS, 0660);

    slog.open(SERVER_LOG, ios::trunc);

    seats = initSeats(num_room_seats);

    vector<pthread_t> tid = vector<pthread_t>(num_ticket_offices);
    for (int i = 0; i < num_ticket_offices; i++)
    {
        int *arg = new int;
        *arg = (i + 1);
        if (pthread_create(&tid.at(i), NULL, thread_func, arg) != 0)
        {
            perror("pthread_create:");
            return -2;
        }
    }

    int fdRequests;
    fdRequests = open(REQUESTS, O_RDONLY);

    int num_seats, size, seat;
    pid_t clientPID;
    vector<int> prefSeats;

    while (/*!timeout*/ 1)
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

        pthread_mutex_lock(&request_mutex);
        req = temp_req;

        pthread_cond_signal(&request_cond);
        pthread_mutex_unlock(&request_mutex);
    }

    pthread_cond_broadcast(&request_cond);

    for (int i = 0; i < num_ticket_offices; i++)
    {
        pthread_join(tid[i], NULL);
    }

    if (req != NULL)
        delete req;
    close(fdRequests);
    unlink(REQUESTS);

    slog << "SERVER CLOSED\n";
    slog.close();

    sbook.open(SERVER_BOOK, ios::trunc);
    logBookedSeats(seats, num_room_seats);
    sbook.close();

    delete[] seats;

    return 0;
}