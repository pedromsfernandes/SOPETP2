#include <iostream>
#include "utils.h"

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " <num_room_seats> <num_ticket_offices> <open_time> " << endl;
    return 1;
}
if (!(isNumber(argv[1]) && isNumber(argv[2]) && isNumber(argv[3])))
    return invalidArguments();

return 0;
