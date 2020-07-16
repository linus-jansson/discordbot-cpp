#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <functional>

#include "json.hpp"
#include <cpr/cpr.h>

#include "websocket.cpp"

int main()
{
    bool done = true;
    std::string input;

    // websocket_endpoint client("XXXXXXXXXXXXXXXXXX"); // SNÄLLA LEAKA INTE DET HÄR IGEN :/

    std::string bodymessage;

    nlohmann::json obj;

    obj["content"] = "Hello world";

    bodymessage = obj.dump();

    auto ssl = cpr::Ssl(cpr::ssl::TLSv1_2{});
    cpr::Session session;

    cpr::Header head;
    head.insert({"Content-Type", "application/json"});
    head.insert({"Authorization", "Bot XXXXXXXXXXXXXXXXXX"});

    session.SetUrl(cpr::Url{"https://discord.com/api/v6/channels/525450532094214154/messages"});
    session.SetHeader(head);
    session.SetBody(cpr::Body{bodymessage});
    session.SetOption(cpr::VerifySsl(false));
    // session.SetOption(ssl);

    std::cout << "debug log" << std::endl;
    // DOES NOT WORK ( NO response ) SSÖ Doesnt work. When making a normal http request everything works as normal
    auto res = session.Post();

    if (res.status_code != 200)
    {
        std::cerr << "Error [code=" << res.status_code << ", error=" << res.error.message << "] making request" << std::endl;
    }
    else
    {
        std::cout << "Request took " << res.elapsed << std::endl;
        std::cout << "Body:" << std::endl
                  << res.text;
    }

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
