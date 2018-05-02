#include "logServer.h"
#include "macros.h"
#include "utils.h"
#include <iomanip>
#include <string>

using namespace std;

void logTicketOfficeOpen(int ticketOfficeNum)
{
    slog << setw(WIDTH_TO) << setfill('0') << ticketOfficeNum << "-OPEN" << endl;
}

void logTicketOfficeClose(int ticketOfficeNum)
{
    slog << setw(WIDTH_TO) << setfill('0') << ticketOfficeNum << "-CLOSED" << endl;
}

void logRequestData(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                    const std::vector<int> &bookSeats)
{
    slog << setw(WIDTH_TO) << setfill('0') << ticketOfficeNum << "-";
    slog << setw(WIDTH_PID) << setfill('0') << clientPID << "-";
    slog << setw(WIDTH_NT) << setfill('0') << bookSeats.size() << ": ";

    for (auto x : prefSeats)
        slog << setw(WIDTH_SEAT) << setfill('0') << x << " ";
}

/*
FALTA TRATAR DA LARGURA DOS LUGARES
*/
void logSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                          const std::vector<int> &bookSeats)
{

    logRequestData(ticketOfficeNum, clientPID, prefSeats, bookSeats);

    slog << "- ";

    for (auto x : bookSeats)
        slog << setw(WIDTH_SEAT) << setfill('0') << x << " ";

    slog << endl;
}

void logUnSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &seats,
                            int motive)
{
    slog << setw(WIDTH_TO) << setfill('0') << ticketOfficeNum << "-";
    slog << setw(WIDTH_PID) << setfill('0') << clientPID << "-";
    slog << setw(WIDTH_NT) << setfill('0') << seats.size() << ": ";

    for (auto x : seats)
        slog << setw(WIDTH_SEAT) << setfill('0') << x << " ";

    slog << getCode(motive) << endl;
}

void saveServerBookings(const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        slog << setw(WIDTH_SEAT) << setfill('0') << x << endl;
}