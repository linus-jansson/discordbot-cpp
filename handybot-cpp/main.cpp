#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <functional>

#include "json.hpp"
#include <cpr/cpr.h>

#include "client.hpp"

int main()
{
    bool done = true;
    std::string input;

    handypp::client client("XXXXXXXXXXXXXXXXXXXX");

    client.run();

    while (client.isReady())
    {
        std::cout << "Bot is running!\n";
    }
    
    

    // std::string bodymessage;

    // nlohmann::json obj;

    // obj["content"] = "Hello world";

    // bodymessage = obj.dump();

    // auto ssl = cpr::Ssl(cpr::ssl::TLSv1_2{});
    // cpr::Session session;

    // cpr::Header head;
    // head.insert({"Content-Type", "application/json"});
    // head.insert({"Authorization", "Bot XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx"});

    // session.SetUrl(cpr::Url{"https://discord.com/api/v6/channels/706041540690313288/messages"});
    // session.SetHeader(head);
    // session.SetBody(cpr::Body{bodymessage});
    // session.SetOption(cpr::VerifySsl(false)); // Don't use this forever. Not safe and can cause man in the middle attacks. Before official release get certificates to work
    // // session.SetOption(ssl);

    // std::cout << "debug log" << std::endl;

    // auto res = session.Post();

    // //  DEBUG for checking if the message was succsesfull
    // if (res.status_code != 200)
    // {
    //     std::cerr << "Error [code=" << res.status_code << ", error=" << res.error.message << "] making request" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Request took " << res.elapsed << std::endl;
    //     std::cout << "Body:" << std::endl
    //               << res.text;
    // }


    while (!done)
    {
        std::getline(std::cin, input);
        if (input == "quit")
            done = true;
        else if (input == "post")
        {
            try
            {
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }

    return 0;
}
