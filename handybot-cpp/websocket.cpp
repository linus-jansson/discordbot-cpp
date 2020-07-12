/* 
    Websocket close codes: 
        (implement later to understand close codes from discord)  
        https://tools.ietf.org/html/rfc6455#section-7.4 

    JSON PARSING:
        -> IT IS ONLY NEEDED TO PARSE THE MESSAGE WHEN REACIVING IT FROM DISCORD;
            -> WHEN SENDING A MESSAGE IT CAN BE SENT AS A STRING AS LONG AS IT HAS JSON FORMAT

*/
#include <websocketpp/config/asio_client.hpp>

#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <websocketpp/common/connection_hdl.hpp>

#include "json.hpp"

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

class websocket_endpoint
{
public:
    websocket_endpoint()
    {
        ws_client.clear_access_channels(websocketpp::log::alevel::all);
        ws_client.clear_error_channels(websocketpp::log::elevel::all);

        ws_client.init_asio();
        ws_client.start_perpetual();

        m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &ws_client);

        connect();
    }

    ~websocket_endpoint()
    {
        ws_client.stop_perpetual();

        std::cout << "> Closing connection " << std::endl;

        websocketpp::lib::error_code ec;
        ws_client.close(connection, websocketpp::close::status::going_away, "", ec);
        if (ec)
        {
            std::cout << "> Error closing connection: "
                      << ec.message() << std::endl;
        }

        m_thread->join();
    }

    void on_open(client *c, websocketpp::connection_hdl hdl)
    {
        // m_status = "Open";

        // client::connection_ptr con = c->get_con_from_hdl(hdl);
        // m_server = con->get_response_header("Server");

        std::cout << "Connection Created!\n";
    }

    void on_fail(client *c, websocketpp::connection_hdl hdl)
    {
        // m_status = "Failed";

        // client::connection_ptr con = c->get_con_from_hdl(hdl);
        // m_server = con->get_response_header("Server");
        // m_error_reason = con->get_ec().message();

        std::cout << "Something went wrong\n";
    }

    void on_close(client *c, websocketpp::connection_hdl hdl)
    {
        // m_status = "Closed";
        // client::connection_ptr con = c->get_con_from_hdl(hdl);
        // std::stringstream s;
        // s << "close code: " << con->get_remote_close_code() << " ("
        //   << websocketpp::close::status::get_string(con->get_remote_close_code())
        //   << "), close reason: " << con->get_remote_close_reason();
        // m_error_reason = s.str();
        client::connection_ptr con = c->get_con_from_hdl(connection);

        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " ("
          << websocketpp::close::status::get_string(con->get_remote_close_code())
          << "), close reason: " << con->get_remote_close_reason();
        std::string m_error_reason = s.str();
        std::cout << "closing.." << m_error_reason << std::endl;
    }

    void on_message(websocketpp::connection_hdl, client::message_ptr msg)
    {

        /* JSON PARSE CONTENT TO CHECK IF OPCODE == 11. IF IT DOES IT SHOULD DO A HEARTBEAT AFTER THE INTERVALL */
        // ????? ^^ ?????
        if (msg->get_opcode() == websocketpp::frame::opcode::text)
        {
            ws_messages.push_back(msg->get_payload());
            auto j = nlohmann::json::parse(msg->get_payload());

            auto opcode = j["op"];

            if (!j["s"].is_null())
            {
                heartbeatSeq = j["s"];
            }
            

            OPCodes code = opcode;
            switch (code)
            {
            case DISPATCH:
            case HEARTBEAT:
            case IDENTIFY:
            case STATUS_UPDATE:
            case VOICE_SERVER_PING:
            case RESUME:
            case RECONNECT:
            case REQUEST_GUILD_MEMBERS:
            case INVALID_SESSION:
            case HELLO:
                heartbeatInterval = j["d"]["heartbeat_interval"]; // Store the heatbeat interval for know later how often it should sendHeatbeat();
                std::cout << heartbeatInterval << std::endl;
                // identify();

            case HEARTBEAT_ACK:
                heartbeat(opcode);
                std::cout << opcode << "\n";

                // sendHeartbeat();
            default:
                break;
            }

            // std::cout << "<< " << j.dump(4) << "\n";
        }
        else
        {
            ws_messages.push_back(websocketpp::utility::to_hex(msg->get_payload()));
            std::cout << "<< " << websocketpp::utility::to_hex(msg->get_payload()) << "\n";
        }
    }

    void connect()
    {
        websocketpp::lib::error_code ec;

        ws_client.set_tls_init_handler([](websocketpp::connection_hdl) {
            return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
        });

        client::connection_ptr con = ws_client.get_connection(uri, ec);

        if (ec)
        {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        }

        con->set_open_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_open,
            this,
            &ws_client,
            websocketpp::lib::placeholders::_1));

        con->set_fail_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_fail,
            this,
            &ws_client,
            websocketpp::lib::placeholders::_1));

        con->set_close_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_close,
            this,
            &ws_client,
            websocketpp::lib::placeholders::_1));

        con->set_message_handler(websocketpp::lib::bind(
            &websocket_endpoint::on_message,
            this,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2));

        ws_client.connect(con);

        connection = con->get_handle();

        // return new_id;
    }
    void close(int id, websocketpp::close::status::value code, std::string reason)
    {
        websocketpp::lib::error_code ec;

        ws_client.close(connection, code, reason, ec);
        if (ec)
        {
            std::cout << "> Error initiating close: " << ec.message() << std::endl;
        }
    }

    void send(std::string message)
    {

        websocketpp::lib::error_code ec;

        ws_client.send(connection, message, websocketpp::frame::opcode::text, ec);
        if (ec)
        {
            std::cout << "> Error sending message: " << ec.message() << std::endl;
            return;
        }

        this->record_sent_message(message);
    }

    

    // std::string get_status() const
    // {
    //     return m_status;
    // }

    void record_sent_message(std::string message)
    {
        ws_messages.push_back(message);
    }

private:
    void sendHeartbeat(int seq)
    {
        std::string message = "{\"op\":1,\"s\":null}";

        if (seq <= 0)
        {
            auto obj = nlohmann::json::parse(message);
            obj["s"] = seq;

            message = obj.dump();
        }

        std::cout << message;
        
        send(message);
    }

    void heartbeat(int opcode)
    {
        if (opcode == 11)
        {
            sendHeartbeat(heartbeatSeq);
        }
    }

    void identify();

    client ws_client;

    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    websocketpp::connection_hdl connection;

    std::string uri = "wss://gateway.discord.gg/?v=6&encoding=json";

    std::vector<nlohmann::json> ws_messages;

    int heartbeatInterval = 0;
    int heartbeatSeq = 0;

    int sessionId = 0;

    enum OPCodes
    {
        DISPATCH = 0,              //dispatches an event
        HEARTBEAT = 1,             //used for ping checking
        IDENTIFY = 2,              //used for client handshake
        STATUS_UPDATE = 3,         //used to update the client status
        VOICE_STATE_UPDATE = 4,    //used to join / move / leave voice channels
        VOICE_SERVER_PING = 5,     //used for voice ping checking
        RESUME = 6,                //used to resume a closed connection
        RECONNECT = 7,             //used to tell clients to reconnect to the gateway
        REQUEST_GUILD_MEMBERS = 8, //used to request guild members
        INVALID_SESSION = 9,       //used to notify client they have an invalid session id
        HELLO = 10,                //sent immediately after connecting, contains heartbeat and server debug information
        HEARTBEAT_ACK = 11,        //sent immediately following a client heartbeat that was received
    };
};
