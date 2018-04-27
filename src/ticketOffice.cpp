#include "ticketOffice.h"
#include <cstdlib>

Seat::Seat(int seatNum) : seatNum(seatNum)
{
    clientId = 0;
}

void Seat::bookSeat(int clientID)
{
    this->clientId = clientId;
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

int isSeatFree(Seat *seats, int seatNum)
{
    int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            if (seats[i].isSeatFree())
                return 1;

            break;
        }
    }

    return 0;
}

void bookSeat(Seat *seats, int seatNum, int clientId)
{
    int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            seats[i].bookSeat(clientId);
            break;
        }
    }
}

void freeSeat(Seat *seats, int seatNum)
{
    int size = sizeof(seats) / sizeof(seats[0]);

    for (unsigned int i = 0; i < size; i++)
    {
        if (seats[i].getSeatNum() == seatNum)
        {
            seats[i].freeSeat();
            break;
        }
    }
}