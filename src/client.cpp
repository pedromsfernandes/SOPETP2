#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include "utils.h"
#include "macros.h"
#include "logClient.h"
#include <string.h>

using namespace std;

void sigalarm_handler(int signum)
{
    cout << "Server took too long to respond\n";
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

    string fifo = FIFOname(pid);
    mkfifo(fifo.c_str(), 0660);

    int fdRequest = open(REQUESTS, O_WRONLY);

    write(fdRequest, &pid, sizeof(int));
    write(fdRequest, &num_wanted_seats, sizeof(int));

    int size = prefList.size();
    write(fdRequest, &size, sizeof(int));

    for (auto &x : prefList)
        write(fdRequest, &x, sizeof(int));

    int fdAns = open(fifo.c_str(), O_RDONLY);

    int numSeats;

    struct sigaction alarme;
    alarme.sa_handler = sigalarm_handler;
    sigemptyset(&alarme.sa_mask);
    alarme.sa_flags = 0;

    sigaction(SIGALRM, &alarme, NULL);

    alarm(time_out);
    read(fdAns, &numSeats, sizeof(int));
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
            read(fdAns, &seatNum, sizeof(int));
            seats.push_back(seatNum);
        }

        logSuccessfulRequestResult(clog, numSeats, seats);

        ofstream cbook(CLIENT_BOOK, ios_base::app);

        saveClientBookings(cbook, seats);
    }

    return 0;
}