#include "user.hpp"
#include "services.hpp"
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <pistache/net.h>
#include <pistache/router.h>
#include <pistache/config.h>
#include <pistache/cookie.h>
#include <pistache/stream.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>  
#include "nlohmann/json.hpp"

void Services::config_routes()
{
    //login site
    Pistache::Rest::Routes::Get(chat_router, "/", Pistache::Rest::Routes::bind(&Services::get_login_site, this));
    Pistache::Rest::Routes::Get(chat_router, "/style.css", Pistache::Rest::Routes::bind(&Services::get_login_site, this));
    
    
    //login handler triggered by login butten
    Pistache::Rest::Routes::Post(chat_router, "/login", Pistache::Rest::Routes::bind(&Services::login_handler, this));
    Pistache::Rest::Routes::Get(chat_router, "/chatsite", Pistache::Rest::Routes::bind(&Services::get_chat_site, this));
    Pistache::Rest::Routes::Get(chat_router, "/forgott_password", Pistache::Rest::Routes::bind(&Services::temp_password_handling, this));
    //registration  
    Pistache::Rest::Routes::Post(chat_router, "/registration", Pistache::Rest::Routes::bind(&Services::registration_handler, this));
    Pistache::Rest::Routes::Post(chat_router, "/reg/name", Pistache::Rest::Routes::bind(&Services::reg_name_validation, this));
    Pistache::Rest::Routes::Post(chat_router, "/reg/email", Pistache::Rest::Routes::bind(&Services::reg_email_validation, this));
    Pistache::Rest::Routes::Post(chat_router, "/reg/passweord", Pistache::Rest::Routes::bind(&Services::reg_password_validation, this));
    Pistache::Rest::Routes::Post(chat_router, "/reg/comfirm_passweord", Pistache::Rest::Routes::bind(&Services::reg_confirmp_calidation, this));
}


void Services::is_port_used(int port_num)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   struct sockaddr_in serv_addr;
   bzero(&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port_num);

   if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0) {
       close(sockfd);
       std::cout << "socket connetcted sucesfully, port is free!" << std::endl;
   } else {
       close(sockfd);
       std::cout << "socket cant connect, the port is used :-( " << std::endl;
   }
}

std::string Services::w_space(std::string body_str)     //   %20
{
    for(int i = 0; i < body_str.size() - 2; ++i)
    {
        if(body_str.substr(i, 3) == "%20")
        {
            body_str.erase(i, 3);
            body_str.insert(i, " ");
        }
        else if(body_str.substr(i, 3) == "%40")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "@");
        }
        else if(body_str.substr(i, 3) == "%26")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "&");
        }
        else if(body_str.substr(i, 3) == "%24")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "$");
        }
        else if(body_str.substr(i, 3) == "%2B")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "+");
        }
        else if(body_str.substr(i, 3) == "%2C")
        {
            body_str.erase(i, 3);
            body_str.insert(i, ",");
        }
        else if(body_str.substr(i, 3) == "%2F")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "/");
        }
        else if(body_str.substr(i, 3) == "%3A")
        {
            body_str.erase(i, 3);
            body_str.insert(i, ":");
        }
        else if(body_str.substr(i, 3) == "%3B")
        {
            body_str.erase(i, 3);
            body_str.insert(i, ";");
        }
        else if(body_str.substr(i, 3) == "%3D")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "=");
        }
        else if(body_str.substr(i, 3) == "%3F")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "?");
        }
        else if(body_str.substr(i, 3) == "%60")
        {
            body_str.erase(i, 3);
            body_str.insert(i, "'");
        }

    }
    return body_str;
}

bool Services::is_valid_session(const Request &request)
{
     // Access the CookieJar from the request object
    const Pistache::Http::CookieJar& cookieJar = request.cookies();

    // Check if the cookie for session identification exists
    if (cookieJar.has("session_cookie")) {
        // Get the session cookie
        Pistache::Http::Cookie sessionCookie = cookieJar.get("session_cookie");

        // Your session validation logic here
        // For example, check if the session is active, not expired, etc.
        if (1 == 1) {
            return true; // Session is valid
        }
    }

    return false; // Session is not valid or no session cookie found
}

std::string Services::temp_cookie_generator(std::string email)     //combination of user email and current time --> easy to crrack with brute force
{
    auto start = std::chrono::system_clock::now();
    // Some computation here
    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::string cookie = email[0] + std::to_string(elapsed_seconds.count());

    return cookie;
}

void Services::get_login_site(const Request &request, Response response)
{
  std::cout << "Received a GET request, get_login_site is called" << std::endl;

  // Check the request URI
  const std::string uri = request.resource();

  if (uri == "/")
  {
    // Serve the index.html file
    Http::serveFile(response, "../../frontend/loginpage/index.html");
  } 
  else if (uri == "/style.css") 
  {
    // Serve the style.css file
    Http::serveFile(response, "../../frontend/loginpage/style.css");
  }  
  else 
  {
    // Handle other cases if needed
    response.send(Http::Code::Not_Found, "Resource not found");
  }
}

void Services::login_handler(const Request &request, Response response)
{
    std::cout << "received a POST request, login_handler is called." << std::endl;

    std::string body_data = w_space(request.body());
    std::cout << "body data request : " << body_data << std::endl;

    
    int and_simbol = body_data.find("&");
   
    std::string email_addres = body_data.substr(9, and_simbol - 9);       //-1 as the simbol and -9 as teh caracternumber of "email_addres="
    std::string password = body_data.substr(and_simbol + 10);

    std::cout << "---------------------- input datas ------------------" << std::endl;
    std::cout << "password: " << password << ", after getting data out" << std::endl;
    std::cout << "email_addres: " << email_addres << ", after getting data out" << std::endl;

    // Check the request URI
    const std::string uri = request.resource();

    if (Users::valid_user(email_addres) == false)
    {
        
        std::string login_email_validation = R"(
        <div hx-target="this" hx-swap="outerHTML">
				<button hx-post="/login"
					hx-target="body"
					hx-include="closest form" 
					hx-trigger="click replace">Sign In
			</button>
            <div class='error-message'>Wrong email address.</div>
		</div>
        )";

        response.send(Http::Code::Ok, login_email_validation);

    } 
    else if(!Users::valid_password(email_addres, password))
    {
        std::string login_email_validation = R"(
        <div hx-target="this" hx-swap="outerHTML">
				<button hx-post="/login"
					hx-target="body"
					hx-include="closest form" 
					hx-trigger="click replace">Sign In
			</button>
            <div class='error-message'>Wrong password.</div>
		</div>
        )";

        response.send(Http::Code::Ok, login_email_validation);
    }
    else
    {
        response.headers().add<Pistache::Http::Header::Location>("/chatsite");
        response.send(Pistache::Http::Code::See_Other);
    }
    
}

void Services::get_chat_site(const Request &request, Response response)
{
    std::cout<< "post chat site handler was called" << std::endl;
    try
    {
        std::string htmlContent;
        std::ifstream htmlFile("../../frontend/chatsite/index.html");
        std::getline(htmlFile, htmlContent, '\0');

        std::string cssContent;
        std::ifstream cssFile("../../frontend/chatsite/style.css");
        std::getline(cssFile, cssContent, '\0');

        htmlContent += "\n<style>" + cssContent + "</style>";

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Text, Html));

        response.send(Http::Code::Ok, htmlContent);
    }
    catch(std::exception& e)
    {
        std::cout << "exception occurred: " << e.what() << std::endl;
        response.send(Http::Code::Not_Found, "Resource not found");
    }

}

void Services::reg_email_validation(const Request &request, Response response)
{
    std::cout << "received a POST request, reg_email_validation is called." << std::endl;

    std::string body_data = w_space(request.body());

    std::string user_name = body_data.substr(5, body_data.find("&") - 5);
    std::string first_arese = body_data.erase(0, body_data.find("&") + 1);
    std::string email_addres = first_arese.substr(6, body_data.find("&") - 6);

    std::regex pattern("^\\w+.\\w+@\\w+.com$");
    bool valid_email = std::regex_match(email_addres, pattern);

    if(Users::email_alredy_taken(email_addres) == true)
    {
        std::string email_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
				<input type="email" name="email" value=")" + email_addres + R"(" placeholder="Email" hx-post="/reg/email" hx-indicator="#ind"/>
                <div class='error-message' >This email addres is alredy taken.</div>
        </div>
        )";

        response.send(Http::Code::Ok, email_part);
        return;
    }
    if(valid_email == true)
    {
        std::string email_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
				<input type="email" name="email" value=")" + email_addres + R"(" placeholder="Email" hx-post="/reg/email" hx-indicator="#ind"/>
		</div>
        )";

        response.send(Http::Code::Ok, email_part);
        return;
    }
    if(valid_email == false)
    {
        std::string email_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
				<input type="email" name="email" value=")" + email_addres + R"(" placeholder="Email" hx-post="/reg/email" hx-indicator="#ind"/>
                <div class='error-message' >Please enter a valid email address.</div>
        </div>
        )";

        response.send(Http::Code::Ok, email_part);
        return;
    }

}

void Services::reg_name_validation(const Request &request, Response response)
{
    std::cout << "received a POST request, reg_name_validation is called." << std::endl;

    std::string body_data = w_space(request.body());

    std::string user_name = body_data.substr(5, body_data.find("&") - 5);

    std::regex pattern("^[A-Z][a-z]+\\s[A-Z][a-z]+$");
    bool name_validating = std::regex_match(user_name, pattern);

    if(name_validating == true)
    {
        std::string name_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
            <input type="text" name="name" value=")" + user_name + R"(" placeholder="Name" hx-post="/reg/name" hx-indicator="#ind"/>
        </div>
        )";

    response.send(Http::Code::Ok, name_part);
    }
    else
    {
        std::string name_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
            <input type="text" name="name" value=")" + user_name + R"(" placeholder="Name" hx-post="/reg/name" hx-indicator="#ind"/>
            <div class='error-message' >Your name should contain 2 words start with apper letters.</div>
        </div>
    )";

    response.send(Http::Code::Ok, name_part);
    }
}

void Services::reg_password_validation(const Request &request, Response response)
{
    std::cout << "received a POST request, reg_password_validation is called." << std::endl;

    std::string body_data = w_space(request.body());

    std::string user_name = body_data.substr(5, body_data.find("&") - 5);
    std::string first_arese = body_data.erase(0, body_data.find("&") + 1);
    std::string email_addres = first_arese.substr(6, body_data.find("&") - 6);
    std::string second_erase = first_arese.erase(0, first_arese.find("&") + 1);
    std::string password = second_erase.substr(9, second_erase.find("&") - 9);

    //Minimum eight characters, at least one uppercase letter, one lowercase letter and one number:
    std::regex pattern("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    bool valid_password = std::regex_match(password, pattern);

    if(valid_password == true)
    {
        std::string password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<input type="password" name="password" value=")" + password + R"(" placeholder="Password" hx-post="/reg/passweord" hx-indicator="#ind"/>
		</div>
        )";

        response.send(Http::Code::Ok, password_part);
    }
    else
    {
        std::string password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<input type="password" name="password" value=")" + password + R"(" placeholder="Password" hx-post="/reg/passweord" hx-indicator="#ind"/>
            <div class='error-message' >Minimum eight characters, at least one uppercase letter, one lowercase letter and one number.</div>
        </div>
        )";

        response.send(Http::Code::Ok, password_part);
    }
} 

void Services::reg_confirmp_calidation(const Request &request, Response response)
{
    std::cout << "received a POST request, reg_confirmp_calidation is called." << std::endl;

    std::string body_data = w_space(request.body());
    std::cout << body_data << std::endl;

    std::string user_name = body_data.substr(5, body_data.find("&") - 5);
    std::string first_arese = body_data.erase(0, body_data.find("&") + 1);
    std::string email_addres = first_arese.substr(6, body_data.find("&") - 6);
    std::string second_erase = first_arese.erase(0, first_arese.find("&") + 1);
    std::string password = second_erase.substr(9, second_erase.find("&") - 9);
    std::string third_erase = first_arese.erase(0, first_arese.find("&") + 1);
    std::string confirm_password = third_erase.substr(third_erase.find("=") + 1);

    std::cout << password << std::endl;
    std::cout << confirm_password << std::endl;

    if(password == confirm_password)
    {
        std::string confimr_password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<input type="password" name="repeta_password" value=")" + confirm_password + R"(" placeholder="Repeta password" hx-post="/reg/comfirm_passweord" hx-indicator="#ind"/>
		</div>
        )";

        response.send(Http::Code::Ok, confimr_password_part);
    }
    else
    {
        std::string confimr_password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<input type="password" name="repeta_password" value=")" + confirm_password + R"(" placeholder="Repeta password" hx-post="/reg/comfirm_passweord" hx-indicator="#ind"/>
            <div class='error-message' >Your two password should be the same.</div>       
        </div>
        )";

        response.send(Http::Code::Ok, confimr_password_part);
    }
}

void Services::registration_handler(const Request &request, Response response)
{
    std::cout << "received a POST request, registration_handler is called." << std::endl;

    std::string body_data = w_space(request.body());

    std::string user_name = body_data.substr(5, body_data.find("&") - 5);
    std::string first_arese = body_data.erase(0, body_data.find("&") + 1);
    std::string email_addres = first_arese.substr(6, body_data.find("&") - 6);
    std::string second_erase = first_arese.erase(0, first_arese.find("&") + 1);
    std::string password = second_erase.substr(9, second_erase.find("&") - 9);
    std::string third_erase = first_arese.erase(0, first_arese.find("&") + 1);
    std::string confirm_password = third_erase.substr(third_erase.find("=") + 1);

    std::regex pattern_email("^\\w+.\\w+@\\w+.com$");
    bool valid_email = std::regex_match(email_addres, pattern_email);

    if(valid_email == false)
    {
        std::string email_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<button type="submit" hx-post="/registration" hx-include="closest form" hx-trigger="click">Sign Up</button>
            <div class='error-message'>Please correct your data!</div>
        </div>
        )";

        response.send(Http::Code::Ok, email_part);
        return;
    }
    
    std::regex pattern_name("^[A-Z][a-z]+\\s[A-Z][a-z]+$");
    bool name_validating = std::regex_match(user_name, pattern_name);

    if(name_validating == false)
    {
        std::string name_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<button type="submit" hx-post="/registration" hx-include="closest form" hx-trigger="click">Sign Up</button>
            <div class='error-message'>Please correct your data!</div>
        </div>
        )";

        response.send(Http::Code::Ok, name_part);
        return;
    }

    std::regex pattern_password("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    bool valid_password = std::regex_match(password, pattern_password);

    if(valid_password == false)
    {
        std::string password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<button type="submit" hx-post="/registration" hx-include="closest form" hx-trigger="click">Sign Up</button>
            <div class='error-message'>Please correct your data!</div>
        </div>
        )";

        response.send(Http::Code::Ok, password_part);
        return;
    }

    if(password != confirm_password)
    {
        std::string confimr_password_part = R"(
        <div hx-target="this" hx-swap="outerHTML">
			<button type="submit" hx-post="/registration" hx-include="closest form" hx-trigger="click">Sign Up</button>
            <div class='error-message'>Please correct your data!</div>
        </div>
        )";

        response.send(Http::Code::Ok, confimr_password_part);
        return;
    }


    Users::insert_into_user_table(email_addres, password, user_name);

    response.headers().add<Pistache::Http::Header::Location>("/chatsite");
    response.send(Pistache::Http::Code::See_Other);
}

void Services::temp_password_handling(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    std::cout << "received a GET request, temp_password_handling was called." << std::endl;

    std::string modalHtml = R"(
    <div class="modal-dialog modal-dialog-centered">
    <div class="modal-content">
    <div class="modal-header">
      <h5 class="modal-title">Modal title</h5>
    </div>
    <div class="modal-body">
      <p>Modal body text goes here.</p>
    </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
        </div>
    </div>
    </div>
    )";

    // Set the response body to the generated HTML
    response.send(Pistache::Http::Code::Ok, modalHtml);
}

void Services::run()
{
   std::cout << "Server started! " << "on port: " << m_port_num << " with " << m_num_threads << " threads.\n";
   m_end_point->init(Pistache::Http::Endpoint::options().threads(m_num_threads));
   std::cout << "endpoint was initialized \n";

   Services::is_port_used(m_port_num);
    
   config_routes();

   m_end_point->setHandler(chat_router.handler());
   std::cout << "handler was set \n"; 

   std::cout << "server should be start now :-) \n";

   m_end_point->serve();
   std::cout << "server started.\n"; 
}

