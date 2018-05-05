#ifndef _LOG_CLIENT_H_
#define _LOG_CLIENT_H_

#include <sys/types.h>
#include <vector>
#include <fstream>

void logSuccessfulRequestResult(std::ostream &clog, pid_t clientPID, const std::vector<int> &bookSeats);

void logUnSuccessfulRequestResult(std::ostream &clog, pid_t clientPID, int motive);

void saveClientBookings(std::ostream &cbook, const std::vector<int> &bookSeats);

#endif /* _LOG_CLIENT_H_*/
