#include "logClient.h"
#include "macros.h"
#include "utils.h"
#include <iomanip>
#include <string>

using namespace std;

void logSuccessfulRequestResult(int clientPID, const std::vector<int> &bookSeats)
{
    int size = bookSeats.size();

    for (int i = 0; i < size; i++)
    {
        clog << setw(5) << setfill('0') << clientPID << " ";
        clog << setw(2) << setfill('0') << i << ".";
        clog << setw(2) << setfill('0') << size << " ";
        clog << setw(4) << setfill('0') << bookSeats.at(i) << endl;
    }
}

void logUnSuccessfulRequestResult(int clientPID, int motive)
{
    clog << setw(5) << setfill('0') << clientPID << " ";
    clog << getCode(motive) << endl;
}

void saveClientBookings(const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        clog << setw(WIDTH_SEAT) << setfill('0') << x << endl;
}