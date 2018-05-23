#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logClient.h"

using namespace std;

int fdAns;
string fifo;

void sigalarm_handler(int)
{
    ofstream clog(CLIENT_LOG, ios_base::app);

    logUnSuccessfulRequestResult(clog, getpid(), -7);

    close(fdAns);
    unlink(fifo.c_str());
    exit(1);
}

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

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    int time_out, num_wanted_seats;
    vector<int> prefList;

    if (parseArguments(argv, time_out, num_wanted_seats, prefList) == 1)
        return invalidArguments();

    pid_t pid = getpid();

    fifo = FIFOname(pid);
    if (mkfifo(fifo.c_str(), 0660) != 0)
    {
        perror("dedicated fifo:");
        return -1;
    }
    int fdRequest;
    if ((fdRequest = open(REQUESTS, O_WRONLY)) == -1)
    {
        perror("requests fifo:");
        return -2;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    fcntl(fdRequest, F_SETLKW, &lock);

    while (write(fdRequest, &pid, sizeof(int)) < 0)
    {
        perror("write:");
        return -3;
    }
    if (write(fdRequest, &num_wanted_seats, sizeof(int)) == -1)
    {
        perror("write:");
        return -3;
    }

    int size = prefList.size();
    if (write(fdRequest, &size, sizeof(int)) == -1)
    {
        perror("write:");
        return -3;
    }

    for (auto &x : prefList)
        if (write(fdRequest, &x, sizeof(int)) == -1)
        {
            perror("write:");
            return -3;
        }

    if ((fdAns = open(fifo.c_str(), O_RDONLY)) == -1)
    {
        perror("dedicated fifo:");
        return -1;
    }

    lock.l_type = F_UNLCK;
    fcntl(fdRequest, F_SETLKW, &lock);

    struct sigaction alarme;
    alarme.sa_handler = sigalarm_handler;
    sigemptyset(&alarme.sa_mask);
    alarme.sa_flags = 0;

    if (sigaction(SIGALRM, &alarme, NULL) == -1)
    {
        perror("sigaction:");
        return -4;
    }

    int numSeats;
    alarm(time_out);

    if (read(fdAns, &numSeats, sizeof(int)) == -1)
    {
        cout << "falhou read" << endl;
        perror("read:");
        return -3;
    }
    alarm(0);

    ofstream clog(CLIENT_LOG, ios_base::app);

    if (numSeats < 0)
        logUnSuccessfulRequestResult(clog, pid, numSeats);

    else
    {
        vector<int> seats;
        int seatNum;

        for (int i = 0; i < numSeats; i++)
        {
            if (read(fdAns, &seatNum, sizeof(int)) == -1)
            {
                perror("read:");
                return -3;
            }
            seats.push_back(seatNum);
        }

        logSuccessfulRequestResult(clog, pid, seats);

        ofstream cbook(CLIENT_BOOK, ios_base::app);

        saveClientBookings(cbook, seats);
    }

    close(fdAns);
    unlink(fifo.c_str());

    return 0;
}