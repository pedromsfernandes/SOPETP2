#ifndef _LOG_H_
#define _LOG_H_

#include <vector>

void logTicketOfficeOpen(int ticketOfficeNum);

void logTicketOfficeClose(int ticketOfficeNum);

void logSuccessfulRequest(int ticketOfficeNum, int clientId, const std::vector<int> &prefSeats,
                          const std::vector<int> &bookSeats);

void logUnSuccessfulRequest(int ticketOfficeNum, int clientId, const std::vector<int> &seats,
                            int motive);

void saveServerBookings(const std::vector<int> &bookSeats);

void logSuccessfulRequestResult(int clientPID, const std::vector<int> &bookSeats);

void logUnSuccessfulRequestResult(int clientPID, int motive);

void saveClientBookings(const std::vector<int> &bookSeats)

#endif /* _LOG_H_ */
