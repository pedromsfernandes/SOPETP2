#ifndef _TICKET_OFFICE_H_
#define _TICKET_OFFICE_H_

#include <sys/types.h>
#include <vector>
#include <pthread.h>

class Seat
{
private:
  int seatNum;
  int clientId;
  pthread_mutex_t seatMutex;

public:
  Seat();
  ~Seat();
  Seat(int seatNum);
  bool isSeatFree();
  void bookSeat(int clientId);
  void freeSeat();
  int getSeatNum() const;
  int getClientId() const;
};

class Request
{
private:
  pid_t clientPID;
  int num_wanted_seats;
  std::vector<int> prefSeats;

public:
  Request();
  Request(pid_t clientPID, int num_wanted_seats, std::vector<int> prefSeats);
  pid_t getClientPID() const;
  int getNumWantedSeats() const;
  std::vector<int> getPrefSeats() const;
};

Seat *initSeats(int num_seats);

int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif /* _TICKET_OFFICE_H_ */