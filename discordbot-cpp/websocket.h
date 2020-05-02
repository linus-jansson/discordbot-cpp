#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include "json.hpp"

#include "client.h"

using json = nlohmann::json;

#include <functional>

typedef websocketpp::client<websocketpp::config::asio_tls_client> wsClient;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

namespace handyBot
{
    class discord_websocket_client : discordClient
    {
    public:
        discord_websocket_client()
        {
            init();

            start();



        }
        ~discord_websocket_client()
        {
            socket.stop_perpetual()

        }
        bool connect(std::string &uri)
        {

        }
        bool disconnect(std::string &uri)
        {

        }
        void send(std::string &message)
        {

        }
    private:
        void init()
        {
            socket.clear_access_channels(websocketpp::log::alevel::all);
            socket.clear_error_channels(websocketpp::log::elevel::all);

            socket.init_asio();
            socket.start_perpetual();

            socket.set_tls_init_handler(std::bind(&on_tls_init));
                // Set ping tyimeout
            socket.set_pong_timeout(0);

            m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&wsClient::run, &socket);
        }

        wsClient socket;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

        static context_ptr on_tls_init() {

            // MY SNEK WAY TO GET TLS TO WORK
            return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
        }
    };
}
