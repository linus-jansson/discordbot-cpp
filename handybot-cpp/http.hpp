#include <string>
#include <vector>
#include <cpr/cpr.h>

enum requestMethod
{
    Get = 0,
    Post = 1,
    Put = 2,
    Patch = 3,
    Delete = 4,
};

struct headPair
{
    std::string name = "";
    std::string value = "";

    headPair(std::string _name) : name(_name) {}
    headPair(std::string _name, std::string _value) : name(_name), value(_value) {}
};

class httpSession
{
public:
    httpSession()
    {
        // session.SetOption(cpr::Ssl(cpr::ssl::TLSv1_2{})); // Set the version of ssl Not required and uses TLS v1.0 by default.
        session.SetOption(cpr::VerifySsl(false));         // Don't use this forever. Not safe and can cause man in the middle attacks. Before official release get certificates to work
    }

    void setUrl(std::string url)
    {
        session.SetUrl(cpr::Url{url});
    }

    void setHead(std::vector<headPair> header)
    {
        cpr::Header head;
        for (auto part : header)
        {
            head.insert({part.name, part.value});
        }

        session.SetHeader(head);
    }

    void setBody(std::string body)
    {
        session.SetBody(cpr::Body{body});
    }

    cpr::Response request(requestMethod method)
    {
        return perform(method);
    }

private:
    cpr::Session session;

    cpr::Response perform(requestMethod method)
    {
        cpr::Response response;
        switch (method)
        {
        case Get:
            response = session.Get();
            break;
        case Post:
            response = session.Post();
            break;
        case Put:
            response = session.Put();
            break;
        case Patch:
            response = session.Patch();
            break;
        case Delete:
            response = session.Delete();
            break;
        default:
            break;
        }

        return response;
    }
};