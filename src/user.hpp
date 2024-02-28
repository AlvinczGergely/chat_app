#ifndef USERS_HPP
#define USERS_HPP

#include <string>
#include <iostream>
#include <pistache/endpoint.h>
#include <SQLiteCpp/SQLiteCpp.h>

using namespace Pistache;

struct Users
{
  std::string user_name;
  std::string password;
  std::string birth_date;
  std::string visual_mod;  // dark mode will be the default option

  static void insert_into_user_table(std::string useremail_addres, std::string password, std::string user_name);
  static void insert_cookie_into_users(std::string email_addres, std::string cookie);
  static void update_password_in_users(std::string email_addres, std::string new_password);
  static bool valid_user(std::string email_addres);
  static bool valid_user(std::string email_addres, std::string password);
  static bool valid_password(std::string email_addres, std::string password);
  static bool email_alredy_taken(std::string email_addres);

};


#endif