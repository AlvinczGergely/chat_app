#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <cstdint>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <pistache/net.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include "user.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <pistache/config.h>
#include <pistache/cookie.h>
#include <pistache/stream.h>
#include <iostream>
#include <chrono>
#include <ctime>  

#include <memory>
#include <string>
#include <thread>

using namespace Pistache;
using json = nlohmann::json;


/*
class HtmxRedirect : public Http::Header 
{    
    NAME("HX-Redirect")
    
    void parse(const std::string& data){}
    void write(std::ostream& os){}
    virtual uint64_t hash()override{}
};
*/


class Services
{
public:

    uint16_t m_port_num;
    unsigned int m_num_threads;
    Pistache::Address m_address;
    std::shared_ptr<Pistache::Http::Endpoint> m_end_point;
    Pistache::Rest::Router chat_router;

    Services(uint16_t port_num = 9090, unsigned int num_threads = std::thread::hardware_concurrency())
        : m_port_num(port_num),
        m_num_threads(num_threads),
        m_address("localhost", port_num),
        m_end_point(std::make_shared<Pistache::Http::Endpoint>(m_address))
    {}


    void config_routes();
    void run();
    void is_port_used(int port_num);
    std::string w_space(std::string body_str);
    std::string temp_cookie_generator(std::string email);


    using Request = Pistache::Rest::Request;
    using Response = Pistache::Http::ResponseWriter;

    void get_login_site(const Request &request, Response response);
    void login_handler(const Request &request, Response response);
    void registration_handler(const Request &request, Response response);
    void password_repeta_check(const Request &request, Response response);
    void get_chat_site(const Request &request, Response response);
    void temp_password_handling(const Request &request, Response response);
    bool is_valid_session(const Request &request);

    void reg_email_validation(const Request &request, Response response);
    void reg_name_validation(const Request &request, Response response);
    void reg_password_validation(const Request &request, Response response);
    void reg_confirmp_calidation(const Request &request, Response response);


    
};

#endif
