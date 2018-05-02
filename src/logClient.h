#ifndef _LOG_CLIENT_H_
#define _LOG_CLIENT_H_

#include <vector>
#include <fstream>
#include <sys/types.h>

void logSuccessfulRequestResult(std::ostream &clog, pid_t clientPID, const std::vector<int> &bookSeats);

void logUnSuccessfulRequestResult(std::ostream &clog, pid_t clientPID, int motive);

void saveClientBookings(std::ostream &cbook, const std::vector<int> &bookSeats);

#endif /* _LOG_CLIENT_H_*/
