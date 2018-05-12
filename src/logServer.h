#ifndef _LOG_SERVER_H_
#define _LOG_SERVER_H_

#include <sys/types.h>
#include <vector>
#include <fstream>
#include "ticketOffice.h"

extern std::ofstream slog;
extern std::ofstream sbook;

void logTicketOfficeOpen(int ticketOfficeNum);

void logTicketOfficeClose(int ticketOfficeNum);

void logSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                          const std::vector<int> &bookSeats);

void logUnSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &seats,
                            int motive);

void logRequestData(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                    const std::vector<int> &bookSeats);

void logBookedSeats(Seat *seats, int num_seats);

#endif /* _LOG_SERVER_H_*/
