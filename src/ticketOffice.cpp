#include <cstdlib>
#include "ticketOffice.h"
#include "macros.h"

using namespace std;

Seat::Seat()
{
    clientId = 0;
    seatNum = 0;
    seatMutex = PTHREAD_MUTEX_INITIALIZER;
}

Seat::Seat(int seatNum) : seatNum(seatNum)
{
    clientId = 0;
    seatMutex = PTHREAD_MUTEX_INITIALIZER;
}

void Seat::bookSeat(int clientID)
{
    this->clientId = clientID;
    //DELAY();
    pthread_mutex_unlock(&seatMutex);
}

bool Seat::isSeatFree()
{
    pthread_mutex_lock(&seatMutex);
    if (clientId)
    {
        //DELAY();
        pthread_mutex_unlock(&seatMutex);
        return false;
    }
    //DELAY();
    return true;
}

void Seat::freeSeat()
{
    pthread_mutex_lock(&seatMutex);
    clientId = 0;
    //DELAY();
    pthread_mutex_unlock(&seatMutex);
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

pid_t Request::getClientPID() const
{
    return clientPID;
}

int Request::getNumWantedSeats() const
{
    return num_wanted_seats;
}

std::vector<int> Request::getPrefSeats() const
{
    return prefSeats;
}

int isSeatFree(Seat *seats, int seatNum)
{
    return ((Seat *)seats + (seatNum - 1))->isSeatFree();
}

void bookSeat(Seat *seats, int seatNum, int clientId)
{
    ((Seat *)seats + (seatNum - 1))->bookSeat(clientId);
}

void freeSeat(Seat *seats, int seatNum)
{
    ((Seat *)seats + (seatNum - 1))->freeSeat();
}

Seat *initSeats(int num_seats)
{
    Seat *seats = new Seat[num_seats];

    for (int i = 0; i < num_seats; i++)
        seats[i] = Seat(i + 1);

    return seats;
}