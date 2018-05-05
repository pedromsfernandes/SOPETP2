#ifndef _LOG_SERVER_H_
#define _LOG_SERVER_H_

#include <sys/types.h>
#include <vector>
#include <fstream>

extern std::ofstream slog;

void logTicketOfficeOpen(int ticketOfficeNum);

void logTicketOfficeClose(int ticketOfficeNum);

void logSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                          const std::vector<int> &bookSeats);

void logUnSuccessfulRequest(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &seats,
                            int motive);

void saveServerBookings(const std::vector<int> &bookSeats);

void logRequestData(int ticketOfficeNum, pid_t clientPID, const std::vector<int> &prefSeats,
                    const std::vector<int> &bookSeats);

#endif /* _LOG_SERVER_H_*/
