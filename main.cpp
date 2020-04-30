
#include <iostream>
#include <string>
// #include "json.hpp"
// #include "asio.hpp"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

// #include "websocket.h"

// #include "zlib-1.2.11/zlib.h"


typedef websocketpp::client<websocketpp::config::asio_client> client;

// for convenience
// using json = nlohmann::json;

int main()
{
    // std::cout << "Hello World!\n";
    
    // json j = {
    //     {"pi", 3.141},
    //     {"happy", true},
    //     {"name", "Niels"},
    //     {"nothing", nullptr},
    //     {"answer", {
    //         {"everything", 42}
    //     }},
    //     {"list", {1, 0, 2}},
    //     {"object", {
    //         {"currency", "USD"},
    //         {"value", 42.99}
    //     }}
    // };

    //  std::string s = j.dump(4);
    //  std::cout << s << std::endl;

    bool done = false;
    std::string input;
    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);
        if (input == "quit") {
            done = true;
        } else if (input == "help") {
            std::cout
                << "\nCommand List:\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else {
            std::cout << "Unrecognized Command" << std::endl;
        }
    }
    return 0;

}