#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "websocket.h"
#include "json.hpp"

using json = nlohmann::json;

std::string jsonParse(json & content)
{

    std::string parsedContent = content.dump(3);
    return parsedContent;

}



int main() {


    // bool running = true;

    // std::string input;
    // websocket_endpoint endpoint;
    // // connection_metadata::ptr metadata;

    // int id = endpoint.connect("wss://gateway.discord.gg/?v=6&encoding=json&compress=true");
    // if (id != -1) {
    //     std::cout << "> Created connection with id " << id << std::endl;
    // }

    // connection_metadata::ptr metadata = endpoint.get_metadata(id);

    // std::ostringstream oldStream;
    // oldStream << *metadata;
    // std::string oldMetaData = oldStream.str();
    // // jsonParse(metadata);
    // // std::vector<json> lastMessage = metadata.get_messages();



    // while (running) {

    //     if (metadata) {
    //         std::ostringstream newStream;
    //         newStream << *metadata;
    //         std::string newMetaData = newStream.str();

    //         if( newMetaData != oldMetaData )
    //         {
    //             std::cout << newMetaData << std::endl;
    //             oldMetaData = newMetaData;

    //             for( auto& j : metadata->get_messages() )
    //             {
    //                 std::cout << j.dump(4) << '\n';
    //             }
    //             std::string message = "{'op': 1}";

    //             endpoint.send(id, message);

                
    //         }
 
    //         // std::cout << *metadata << std::endl;
    //     } else {
    //         std::cout << "> Unknown connection id " << id << std::endl;
    //     }



        // std::cout << "Enter Command: ";
        // std::getline(std::cin, input);

        // if (input == "quit") {
        //     running = false;
        // } else if (input == "help") {
        //     std::cout
        //         << "\nCommand List:\n"
        //         << "connect <ws uri>\n"
        //         << "send <connection id> <message>\n"
        //         << "close <connection id> [<close code:default=1000>] [<close reason>]\n"
        //         << "show <connection id>\n"
        //         << "help: Display this help text\n"
        //         << "quit: Exit the program\n"
        //         << std::endl;
        // } else if (input.substr(0,7) == "connect") {
            
        //     int id = endpoint.connect(input.substr(8));
        //     int id = endpoint.connect("wss://gateway.discord.gg/?v=6&encoding=json");
        //     if (id != -1) {
        //         std::cout << "> Created connection with id " << id << std::endl;
        //     }
        // } else if (input.substr(0,4) == "send") {
        //     std::stringstream ss(input);
            
        //     std::string cmd;
        //     int id;
        //     std::string message;
            
        //     ss >> cmd >> id;
        //     std::getline(ss,message);
            
        //     endpoint.send(id, message);
        // } else if (input.substr(0,5) == "close") {
        //     std::stringstream ss(input);
            
        //     std::string cmd;
        //     int id;
        //     int close_code = websocketpp::close::status::normal;
        //     std::string reason;
            
        //     ss >> cmd >> id >> close_code;
        //     std::getline(ss,reason);
            
        //     endpoint.close(id, close_code, reason);
        // } else if (input.substr(0,4) == "show") {
        //     int id = atoi(input.substr(5).c_str());

        //     connection_metadata::ptr metadata = endpoint.get_metadata(id);
        //     if (metadata) {
        //         std::cout << *metadata << std::endl;
        //     } else {
        //         std::cout << "> Unknown connection id " << id << std::endl;
        //     }
        // } else {
        //     std::cout << "> Unrecognized Command" << std::endl;
        // }



    }

    return 0;
}