#ifndef _TICKET_OFFICE_H_
#define _TICKET_OFFICE_H_

class Seat
{
  private:
    int seatNum;
    int clientId;

  public:
    Seat(int seatNum);
    bool isSeatFree() const;
    void bookSeat(int clientId);
    void freeSeat();
    int getSeatNum() const;
    int getClientId() const;
};

int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif /* _TICKET_OFFICE_H_ */