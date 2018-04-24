#ifndef _TICKET_OFFICE_H_
#define _TICKET_OFFICE_H_

typedef struct
{

} Seat;

int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif /* _TICKET_OFFICE_H_ */