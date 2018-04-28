#include "logServer.h"
#include "macros.h"
#include "utils.h"
#include <iomanip>
#include <string>

using namespace std;

void logTicketOfficeOpen(int ticketOfficeNum)
{
    slog << setw(2) << setfill('0') << ticketOfficeNum << "-OPEN" << endl;
}

void logTicketOfficeClose(int ticketOfficeNum)
{
    slog << setw(2) << setfill('0') << ticketOfficeNum << "-CLOSED" << endl;
}

void logRequestData(int ticketOfficeNum, int clientId, const std::vector<int> &prefSeats,
                    const std::vector<int> &bookSeats)
{
    slog << setw(2) << setfill('0') << ticketOfficeNum << "-";
    slog << setw(5) << setfill('0') << clientId << "-";
    slog << setw(2) << setfill('0') << bookSeats.size() << ": ";

    for (auto x : prefSeats)
        slog << setw(4) << setfill('0') << x << " ";
}

/*
FALTA TRATAR DA LARGURA DOS LUGARES
*/
void logSuccessfulRequest(int ticketOfficeNum, int clientId, const std::vector<int> &prefSeats,
                          const std::vector<int> &bookSeats)
{

    logRequestData(ticketOfficeNum, clientId, prefSeats, bookSeats);

    slog << "- ";

    for (auto x : bookSeats)
        slog << setw(4) << setfill('0') << x << " ";

    slog << endl;
}

void logUnSuccessfulRequest(int ticketOfficeNum, int clientId, const std::vector<int> &seats,
                            int motive)
{
    slog << setw(2) << setfill('0') << ticketOfficeNum << "-";
    slog << setw(5) << setfill('0') << clientId << "-";
    slog << setw(2) << setfill('0') << seats.size() << ": ";

    for (auto x : seats)
        slog << setw(4) << setfill('0') << x << " ";

    slog << getCode(motive) << endl;
}

void saveServerBookings(const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        slog << setw(WIDTH_SEAT) << setfill('0') << x << endl;
}