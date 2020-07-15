#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <functional>

#include "websocket.cpp"

#include "json.hpp"

int main()
{
    bool done = false;
    std::string input;
    websocket_endpoint endpoint("NTI4OTc3MTM1NjEyOTg1MzY5.XwwFLA.FoG5yFqwqbu8K7r8BCoBpCPzl-Q");

    while (!done)
    {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit")
        {
            done = true;
        }
        else if (input == "help")
        {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "send <connection id> <message>\n"
                << "close <connection id> [<close code:default=1000>] [<close reason>]\n"
                << "show <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        }
        else if (input.substr(0, 7) == "connect")
        {
            endpoint.connect();
        }
        else if (input.substr(0, 4) == "send")
        {
            std::istringstream ss(input);

            std::string cmd;

            std::string message = "{\"op\":1,\"d\":null}";
            // std::string message = "{\"op\":1}";

            ss >> cmd;

            endpoint.send(message);
        }
        else if (input.substr(0, 5) == "close")
        {
            std::stringstream ss(input);

            std::string cmd;
          
            int id;
            int close_code = websocketpp::close::status::normal;
            std::string reason;

            ss >> cmd >> id >> close_code;
            std::getline(ss, reason);

            endpoint.close(close_code, reason);
        }
        else
        {
            std::cout << "> Unrecognized Command" << std::endl;
        }
    }

    return 0;
}
