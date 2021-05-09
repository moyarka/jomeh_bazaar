#include "market.hpp"
#include <iostream>
using namespace std;

int main()
{
    Market market;
    string command_line;
    while(getline(cin, command_line))
    {
        try 
        {
            market.processCommand(command_line);
        }
        catch(exception* ex)
        {
            cout << ex->what() << endl;
            delete ex;
        }
        catch(invalid_argument ia) {
            cout << "Bad Request" << endl;
        }
    }
}