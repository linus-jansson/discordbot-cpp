/* 
    Websocket close codes: 
        (implement later to understand close codes from discord)  
        https://tools.ietf.org/html/rfc6455#section-7.4 

    JSON PARSING:
        -> IT IS ONLY NEEDED TO PARSE THE MESSAGE WHEN REACIVING IT FROM DISCORD;
            -> WHEN SENDING A MESSAGE IT CAN BE SENT AS A STRING AS LONG AS IT HAS JSON FORMAT


    TODO:
        -> MOVE WEBSOCKET METHODS TO PRIVATE, ONLY PUBLID SHOULD BE GET STATUS 
*/

#ifndef HANDYBOT_GATEWAY
#define HANDYBOT_GATEWAY

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/connection_hdl.hpp>

#include "json.hpp"

#include "limpanLibs/lib/time.h"

#include <iostream>

#include <chrono>
#include <thread>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client_endpoint; // client_endpoint, this_client

constexpr unsigned int hash(const char *key, unsigned int i = 0)
{
    return !key[i] ? 0 : (hash(key, i + 1) * 31) + key[i] - 'A';
}

namespace handypp
{
    class client // change to client class
    {
    public:
        client(std::string _token) : bot_token(_token)
        {

            this_client.clear_access_channels(websocketpp::log::alevel::all);
            this_client.clear_error_channels(websocketpp::log::elevel::all);

            this_client.init_asio();
            this_client.start_perpetual();

            m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client_endpoint::run, &this_client);
        }

        ~client()
        {
            this_client.stop_perpetual();

            std::cout << "> Closing connection " << std::endl;

            websocketpp::lib::error_code ec;
            this_client.close(connection, websocketpp::close::status::going_away, "", ec);
            if (ec)
            {
                std::cout << "> Error closing connection: "
                          << ec.message() << std::endl;
            }

            m_thread->join();
        }

        void on_open(client_endpoint *c, websocketpp::connection_hdl hdl)
        {
            // m_status = "Open";

            // client_endpoint::connection_ptr con = c->get_con_from_hdl(hdl);
            // m_server = con->get_response_header("Server");

            std::cout << "Connection Created!\n";
        }

        void on_fail(client_endpoint *c, websocketpp::connection_hdl hdl)
        {
            // m_status = "Failed";

            // client::connection_ptr con = c->get_con_from_hdl(hdl);
            // m_server = con->get_response_header("Server");
            // m_error_reason = con->get_ec().message();

            std::cout << "Something went wrong\n";
        }

        void on_close(client_endpoint *c, websocketpp::connection_hdl hdl)
        {
            // m_status = "Closed";

            client_endpoint::connection_ptr con = c->get_con_from_hdl(connection);

            std::stringstream s;
            s << "close code: " << con->get_remote_close_code() << " ("
              << websocketpp::close::status::get_string(con->get_remote_close_code())
              << "), close reason: " << con->get_remote_close_reason();
            std::string m_error_reason = s.str();
            std::cout << "closing.. >> " << m_error_reason << std::endl;
        }

        void on_message(websocketpp::connection_hdl, client_endpoint::message_ptr msg)
        {

            if (msg->get_opcode() == websocketpp::frame::opcode::text)
            {
                // ws_messages.push_back(msg->get_payload()); // Just a practical memeory leak. Constantly increasing
                auto obj = nlohmann::json::parse(msg->get_payload());

                const auto op = obj["op"];
                const auto t = obj["t"];
                const auto d = obj["d"];

                std::cout << obj.dump(4) << "\n";

                if (!obj["s"].is_null())
                {
                    heartbeatSeq = obj["s"];
                }

                GATEWAY_OPCODES code = op;
                switch (code)
                {
                case DISPATCH:
                    // Check which event that has been sent by discord
                    switch (hash(t.dump().c_str()))
                    {
                    case hash("READY"):
                        READY = true;
                        std::cout << d.dump(4) << std::endl;
                        break;
                    default:
                        std::cout << "unkown event" << std::endl;
                        break;
                    }
                case HEARTBEAT:
                    // SHOULD SEND A HEART ACKE BACK
                case IDENTIFY:
                case STATUS_UPDATE:
                case VOICE_SERVER_PING:
                case RESUME:
                    break;
                case RECONNECT:
                    reconnect();
                    break;
                case REQUEST_GUILD_MEMBERS:
                    break;
                case INVALID_SESSION:

                    limpans_tid.setTimeout(5100);
                    identify();

                    // The inner d key is a boolean that indicates whether the session may be resumable or not.

                    break;
                case HELLO:
                    if (sessionId.length() == 0)
                    {
                        heartbeatInterval = obj["d"]["heartbeat_interval"]; // Store the heatbeat interval for know later how often it should sendHeatbeat();
                        // std::cout << heartbeatInterval << std::endl;
                        identify();
                    }
                    else
                    {
                        resume();
                    }

                    break;
                case HEARTBEAT_ACK:
                    //If a client does not receive a heartbeat ack between its attempts at sending heartbeats, it should immediately terminate the connection with a non-1000 close code, reconnect, and attempt to resume.
                    limpans_tid.setTimeout(2500);
                    heartbeat(op);

                    // sendHeartbeat();
                    break;
                default:
                    break;
                }
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

            this_client.set_tls_init_handler([](websocketpp::connection_hdl) {
                return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
            });

            client_endpoint::connection_ptr con = this_client.get_connection(uri, ec);

            if (ec)
            {
                std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            }

            con->set_open_handler(websocketpp::lib::bind(
                &client::on_open,
                this,
                &this_client,
                websocketpp::lib::placeholders::_1));

            con->set_fail_handler(websocketpp::lib::bind(
                &client::on_fail,
                this,
                &this_client,
                websocketpp::lib::placeholders::_1));

            con->set_close_handler(websocketpp::lib::bind(
                &client::on_close,
                this,
                &this_client,
                websocketpp::lib::placeholders::_1));

            con->set_message_handler(websocketpp::lib::bind(
                &client::on_message,
                this,
                websocketpp::lib::placeholders::_1,
                websocketpp::lib::placeholders::_2));

            this_client.connect(con);

            connection = con->get_handle();

            // return new_id;
        }
        void close(websocketpp::close::status::value code, std::string reason)
        {
            websocketpp::lib::error_code ec;

            this_client.close(connection, code, reason, ec);
            if (ec)
            {
                std::cout << "> Error initiating close: " << ec.message() << std::endl;
            }
        }

        void send(std::string message)
        {

            websocketpp::lib::error_code ec;

            this_client.send(connection, message, websocketpp::frame::opcode::text, ec);
            if (ec)
            {
                std::cout << "> Error sending message: " << ec.message() << std::endl;
                return;
            }
        }

        void run()
        {
            connect();
        }

        bool isReady()
        {
            return READY;
        }

        // std::string get_status() const
        // {
        //     return m_status;
        // }

    private:
        void sendHeartbeat(int seq)
        {
            std::string message;

            nlohmann::json obj;
            obj["op"] = 1;
            obj["d"] = seq;

            message = obj.dump();
            // std::cout << message << "\n"; // DEBUG print for the heartbeat json object
            send(message);
        }

        void heartbeat(int opcode)
        {
            if (READY == true)
            {
                sendHeartbeat(heartbeatSeq);
                // std::cout << heartbeatSeq << "\n";
            }
        }

        void identify()
        {
            std::string message;
            nlohmann::json obj;

            obj["op"] = 2;
            obj["d"]["token"] = bot_token;
            obj["d"]["properties"]["$os"] = "linux";
            obj["d"]["properties"]["$browser"] = "LimpanCpp";
            obj["d"]["properties"]["$device"] = "LimpanCpp";

            message = obj.dump();
            std::cout << obj.dump(4) << std::endl;

            send(message);
        }

        void resume()
        {
            std::string message;
            nlohmann::json obj;

            obj["op"] = 6;
            obj["d"]["token"] = bot_token;
            obj["d"]["session_id"] = sessionId;
            obj["d"]["seq"] = heartbeatSeq;

            message = obj.dump();
            std::cout << obj.dump(4) << std::endl;

            send(message);
        }

        void reconnect()
        {
            /// close code 1012: Indicates that the service is restarted. A client may reconnect and if
            /// if it chooses to do so, should reconnect using a randomized delay of
            /// 5-30s
            close(1012, "Reconnect");
            ws_status = "reconnect";
            std::cout << "reconnecting...";
            limpans_tid.setTimeout(6000);
            connect();

            resume();
        }

        client_endpoint this_client;

        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

        websocketpp::connection_hdl connection;

        limpan::time limpans_tid;

        std::string uri = "wss://gateway.discord.gg/?v=6&encoding=json";

        std::string ws_status;

        std::vector<nlohmann::json> ws_messages;

        int heartbeatInterval = 0;
        int heartbeatSeq = -1;

        std::string sessionId;
        std::string bot_token;

        bool READY = false;
        bool isQutting = false;

        enum GATEWAY_OPCODES
        {
            DISPATCH = 0,              //Recive: An event was dispatched.
            HEARTBEAT = 1,             //Send/Recive: Fired periodically by the client to keep the connection alive.
            IDENTIFY = 2,              //Send: Starts a new session during the initial handshake.
            STATUS_UPDATE = 3,         //Send: Update the client's presence.
            VOICE_STATE_UPDATE = 4,    //Send: Used to join/leave or move between voice channels.
            VOICE_SERVER_PING = 5,     //used for voice ping checking
            RESUME = 6,                //Send: Resume a previous session that was disconnected.
            RECONNECT = 7,             //Receive: You should attempt to reconnect and resume immediately.
            REQUEST_GUILD_MEMBERS = 8, //Send: Request information about offline guild members in a large guild.
            INVALID_SESSION = 9,       //Receive: The session has been invalidated. You should reconnect and identify/resume accordingly.
            HELLO = 10,                //Receive: Sent immediately after connecting, contains the heartbeat_interval to use.
            HEARTBEAT_ACK = 11,        //Receive: Sent in response to receiving a heartbeat to acknowledge that it has been received.
        };

        enum GATEWAY_CLOSE_CODES
        {
            UNKOWN_ERROR = 4000,          //We're not sure what went wrong. Try reconnecting?
            UNKOWN_OPCODE = 4001,         //You sent an invalid Gateway opcode or an invalid payload for an opcode. Don't do that!
            DECODE_ERROR = 4002,          //You sent an invalid payload to us. Don't do that!
            NOT_AUTHENTICATED = 4003,     //You sent us a payload prior to identifying.
            AUTHENTICATION_FAILED = 4004, //The account token sent with your identify payload is incorrect.
            ALREADY_AUTHENTICATED = 4005, //You sent more than one identify payload. Don't do that!
            INVALID_SEQ = 4007,           //The sequence sent when resuming the session was invalid. Reconnect and start a new session.
            RATE_LIMITED = 4008,          //Woah nelly! You're sending payloads to us too quickly. Slow it down! You will be disconnected on receiving this.
            SESSION_TIMED_OUT = 4009,     //Your session timed out. Reconnect and start a new one.
            INVALID_SHARD = 4010,         //You sent us an invalid shard when identifying.
            SHARDING_REQUIRED = 4011,     //The session would have handled too many guilds - you are required to shard your connection in order to connect.
            INVALID_API_VERSION = 4012,   //You sent an invalid version for the gateway.
            INVALID_INTENT = 4013,        //You sent an invalid intent for a Gateway Intent. You may have incorrectly calculated the bitwise value.
            DISALLOWED_INTENT = 4014,     //You sent a disallowed intent for a Gateway Intent. You may have tried to specify an intent that you have not enabled or are not whitelisted for.
        };
    };
} // namespace handypp

#endif