#include <iostream>

using namespace std;

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " <num_room_seats> <num_ticket_offices> <open_time> " << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        return usage(argv);

    return 0;
}