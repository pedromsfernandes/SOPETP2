#include <iomanip>
#include <string>
#include "logClient.h"
#include "macros.h"
#include "utils.h"

using namespace std;

void logSuccessfulRequestResult(ostream &clog, pid_t clientPID, const std::vector<int> &bookSeats)
{
    int size = bookSeats.size();

    for (int i = 0; i < size; i++)
    {
        clog << setw(WIDTH_PID) << setfill(FILL) << clientPID << " ";
        clog << setw(WIDTH_XXNN / 2) << setfill(FILL) << i << ".";
        clog << setw(WIDTH_XXNN / 2) << setfill(FILL) << size << " ";
        clog << setw(WIDTH_SEAT) << setfill(FILL) << bookSeats.at(i) << endl;
    }
}

void logUnSuccessfulRequestResult(ostream &clog, pid_t clientPID, int motive)
{
    clog << setw(WIDTH_PID) << setfill(FILL) << clientPID << " ";
    clog << getCode(motive) << endl;
}

void saveClientBookings(ostream &cbook, const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        cbook << setw(WIDTH_SEAT) << setfill(FILL) << x << endl;
}