#ifndef _LOG_CLIENT_H_
#define _LOG_CLIENT_H_

#include <vector>
#include <fstream>

extern std::ofstream clog;

void logSuccessfulRequestResult(int clientPID, const std::vector<int> &bookSeats);

void logUnSuccessfulRequestResult(int clientPID, int motive);

void saveClientBookings(const std::vector<int> &bookSeats);

#endif /* _LOG_CLIENT_H_*/
