#include "logClient.h"
#include "macros.h"
#include "utils.h"
#include <iomanip>
#include <string>

using namespace std;

void logSuccessfulRequestResult(ostream &clog, pid_t clientPID, const std::vector<int> &bookSeats)
{
    int size = bookSeats.size();

    for (int i = 0; i < size; i++)
    {
        clog << setw(WIDTH_PID) << setfill('0') << clientPID << " ";
        clog << setw(WIDTH_XXNN / 2) << setfill('0') << i << ".";
        clog << setw(WIDTH_XXNN / 2) << setfill('0') << size << " ";
        clog << setw(WIDTH_SEAT) << setfill('0') << bookSeats.at(i) << endl;
    }
}

void logUnSuccessfulRequestResult(ostream &clog, pid_t clientPID, int motive)
{
    clog << setw(WIDTH_PID) << setfill('0') << clientPID << " ";
    clog << getCode(motive) << endl;
}

void saveClientBookings(ostream &cbook, const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        cbook << setw(WIDTH_SEAT) << setfill('0') << x << endl;
}