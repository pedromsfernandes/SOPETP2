#include "ticketOffice.h"
#include "macros.h"
#include <cstdlib>

using namespace std;

Seat::Seat()
{
    clientId = 0;
    seatNum = 0;
}

Seat::Seat(int seatNum) : seatNum(seatNum)
{
    clientId = 0;
}

void Seat::bookSeat(int clientID)
{
    this->clientId = clientID;
}

bool Seat::isSeatFree() const
{
    return clientId == 0;
}

void Seat::freeSeat()
{
    clientId = 0;
}

int Seat::getClientId() const
{
    return clientId;
}

int Seat::getSeatNum() const
{
    return seatNum;
}

Request::Request()
{
    clientPID = 0;
    num_wanted_seats = 0;
    prefSeats = vector<int>();
}

Request::Request(pid_t clientPID, int num_wanted_seats, vector<int> prefSeats)
{
    this->clientPID = clientPID;
    this->num_wanted_seats = num_wanted_seats;
    this->prefSeats = prefSeats;
}

int isSeatFree(Seat *seats, int seatNum)
{
    unsigned int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            if (seats[i].isSeatFree())
                return 1;

            break;
        }
    }

    //DELAY();
    return 0;
}

void bookSeat(Seat *seats, int seatNum, int clientId)
{
    unsigned int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            seats[i].bookSeat(clientId);
            break;
        }
    }

    //DELAY();
}

void freeSeat(Seat *seats, int seatNum)
{
    unsigned int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            seats[i].freeSeat();
            break;
        }
    }

    //DELAY();
}

Seat *initSeats(int num_seats)
{
    Seat *seats = new Seat[num_seats];

    for (int i = 0; i < num_seats; i++)
        seats[i] = Seat(i + 1);

    return seats;
}