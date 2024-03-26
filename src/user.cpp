#include <SQLiteCpp/SQLiteCpp.h>
#include <pistache/endpoint.h>

#include "SQLiteCpp/Column.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "user.h"
using namespace Pistache;


void Users::insert_into_user_table(std::string email_addres, std::string password, std::string user_name) 
{
  try 
  { 
  SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

  SQLite::Transaction transaction(db);
  SQLite::Statement query(db, "INSERT INTO Users (email_addres, password, user_name, cookie) VALUES (?, ?, ?, ?)");
  query.bind(1, email_addres);
  query.bind(2, password);
  query.bind(3, user_name);
  query.bind(4);

  query.exec();
  transaction.commit();
  }
  
  catch (std::exception& e)
  {
    std::cout << "exception after insert_into_user_table was called: " << e.what() << std::endl;
  } 

}

void Users::insert_cookie_into_users(std::string email_addres, std::string cookie)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    SQLite::Transaction transaction(db);
    SQLite::Statement query(db, "UPDATE Users SET cookie = ? where email_addres = ?");
    query.bind(1, cookie);
    query.bind(2, email_addres);

    query.exec();
    transaction.commit();
  }

  catch (std::exception& e)
  {
    std::cout << "exception after insert_cookie_into_users was called: " << e.what() << std::endl;
  } 
}

void Users::update_password_in_users(std::string email_addres, std::string new_password)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    SQLite::Transaction transaction(db);
    SQLite::Statement query(db, "UPDATE Users SET passwrod = ? where email_addres = ?");
    query.bind(2, new_password);
    query.bind(1, email_addres);

    query.exec();
    transaction.commit();
  }

  catch (std::exception& e)
  {
    std::cout << "exception after update_password_in_users was called: " << e.what() << std::endl;
  } 
}

bool Users::valid_user(std::string email_addres)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    SQLite::Statement n_query(db, "SELECT email_addres FROM Users");

    while (n_query.executeStep())
    {
      std::string current_email_addres = n_query.getColumn(0);
      std::cout << "Current email address from database: " << current_email_addres << std::endl;

      if (current_email_addres == email_addres)
      {
        std::cout << "user found in data base" << std::endl;
        return true;
      }
    }

  }
  catch(const SQLite::Exception& e)
  {
    std::cout << "SQLite exception after valid_user was called: " << e.what() << std::endl;
    std::cout << "Error code: " << e.getErrorCode() << std::endl;
    return false;
  } 
  
  std::cout << "user: " << email_addres << ", was not found in the data base" << std::endl;
  return false;
}

bool Users::valid_password(std::string email_addres, std::string password)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    SQLite::Statement n_query(db, "SELECT password FROM Users Where email_addres = ?");
    n_query.bind(1, email_addres);

    while (n_query.executeStep())
    {
      std::string current_password = n_query.getColumn(0);

      if (current_password == password)
      {
        std::cout << "user found in data base" << std::endl;
        return true;
      }
    }

    std::cout << "possword is incorrect: "  << std::endl;
    return false;
  }
  catch(const std::exception& e)
  {
    std::cout << "exception after valid_password was called: " << e.what() << std::endl;
    return false;
  }
}

bool Users::email_alredy_taken(std::string email_addres)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    SQLite::Statement n_query(db, "SELECT email_addres FROM Users");

    while (n_query.executeStep())
    {
      std::string current_email_addres = n_query.getColumn(0);
      std::cout << "Current email address from database: " << current_email_addres << std::endl;

      if (current_email_addres == email_addres)
      {
        std::cout << "email addres is found in the db" << std::endl;
        return true;
      }
    }

  }
  catch(const SQLite::Exception& e)
  {
    std::cout << "SQLite exception after valid_user was called: " << e.what() << std::endl;
    std::cout << "Error code: " << e.getErrorCode() << std::endl;
  } 
  
  std::cout << "user: " << email_addres << ", email is free to use" << std::endl;
  return false;
}

bool Users::check_cookie(std::string cookie)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    SQLite::Statement n_query(db, "SELECT cookie FROM Users");

    while (n_query.executeStep())
    {
      std::string database_cookie = n_query.getColumn(0);

      if (database_cookie == cookie)
      {
        std::cout << "db containse the cookie" << std::endl;
        return true;
      }
    }

    std::cout << "cookie was not founde"  << std::endl;
    return false;
  }
  catch(const std::exception& e)
  {
    std::cout << "exception after check_cookie was called: " << e.what() << std::endl;
  }
  return false;
}

bool Users::delete_cookie(std::string cookie_value)
{
  try
  {
    SQLite::Database db("../../db/Users.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    SQLite::Transaction transaction(db);
    SQLite::Statement query(db, "UPDATE Users SET cookie = NULL where cookie = ?");
    query.bind(1, cookie_value);

    query.exec();
    transaction.commit();
  }
  catch (std::exception& e)
  {
    std::cout << "exception after delete_cookie was called: " << e.what() << std::endl;
    return false;
  } 
  return true;
}