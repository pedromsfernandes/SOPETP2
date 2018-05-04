#include "logServer.h"
#include "macros.h"
#include "utils.h"
#include <iomanip>
#include <string>

using namespace std;

void logTicketOfficeOpen(int ticketOfficeNum)
{
    slog << setw(WIDTH_TO) << setfill(FILL) << ticketOfficeNum << "-OPEN" << endl;
}

void logTicketOfficeClose(int ticketOfficeNum)
{
    slog << setw(WIDTH_TO) << setfill(FILL) << ticketOfficeNum << "-CLOSED" << endl;
}

void logRequestData(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                    const std::vector<int> &bookSeats)
{
    slog << setw(WIDTH_TO) << setfill(FILL) << ticketOfficeNum << "-";
    slog << setw(WIDTH_PID) << setfill(FILL) << clientPID << "-";
    slog << setw(WIDTH_NT) << setfill(FILL) << bookSeats.size() << ": ";

    int size = prefSeats.size();
    for(int i = 0; i < MAX_CLI_SEATS; i++)
    {
        if(i < size)
            slog  << setw(WIDTH_SEAT) << setfill(FILL) << prefSeats.at(i) << " ";
        else
            slog  << "     ";
    }
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
    slog << setw(WIDTH_TO) << setfill(FILL) << ticketOfficeNum << "-";
    slog << setw(WIDTH_PID) << setfill(FILL) << clientPID << "-";
    slog << setw(WIDTH_NT) << setfill(FILL) << seats.size() << ": ";

    int size = seats.size();
    for(int i = 0; i < MAX_CLI_SEATS; i++)
    {
        if(i < size)
            slog  << setw(WIDTH_SEAT) << setfill(FILL) << seats.at(i) << " ";
        else
            slog  << "     ";
    }

    slog << "- " << getCode(motive) << endl;
}

void saveServerBookings(const std::vector<int> &bookSeats)
{
    for (auto x : bookSeats)
        slog << setw(WIDTH_SEAT) << setfill(FILL) << x << endl;
}