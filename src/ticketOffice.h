#ifndef _TICKET_OFFICE_H_
#define _TICKET_OFFICE_H_

#include <sys/types.h>
#include <vector>

class Seat
{
private:
  int seatNum;
  int clientId;

public:
  Seat();
  Seat(int seatNum);
  bool isSeatFree() const;
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
};

Seat *initSeats(int num_seats);

int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif /* _TICKET_OFFICE_H_ */