#ifndef _LOG_CLIENT_H_
#define _LOG_CLIENT_H_

#include <vector>
#include <fstream>

void logSuccessfulRequestResult(std::ostream &clog, int clientPID, const std::vector<int> &bookSeats);

void logUnSuccessfulRequestResult(std::ostream &clog, int clientPID, int motive);

void saveClientBookings(std::ostream &cbook, const std::vector<int> &bookSeats);

#endif /* _LOG_CLIENT_H_*/
