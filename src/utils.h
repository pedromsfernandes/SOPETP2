#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <vector>
#include "ticketOffice.h"

std::vector<std::string> splitList(const std::string &list);

int invalidArguments();

bool isNumber(const std::string &str);

std::string getCode(int motive);

std::string FIFOname(pid_t pid);

std::string getRequestString(pid_t pid, int num_wanted_seats, const vector<int> &prefSeats);

Request *parseRequestString(const std::string request);

#endif /* _UTILS_H_ */
