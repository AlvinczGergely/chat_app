#include "generator.hpp"
#include "user.hpp"
#include "services.hpp"

#include <SQLiteCpp/SQLiteCpp.h>
#include <pistache/endpoint.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace Pistache;


int main()
{
  Generators test_ob;
  Users user_ob;
  
  //-------------------------------- Generator part --------------------------------//

  /*

  std::cout << test_ob.phone_number_generator() << std::endl << std::endl;  
 
  std::cout << test_ob.birth_date_generator() << std::endl << std::endl;

  std::cout << test_ob.random_number_generator(5) << std::endl << std::endl;

  std::cout << "after random number, befor name generator" << std::endl;
  
  std::cout << "first name: " << test_ob.name_generator().first_name << std::endl
            << "last name: " << test_ob.name_generator().last_name << std::endl
            << "full name: " << test_ob.name_generator().full_name << std::endl << std::endl;
  
  std::cout << test_ob.residence_generator(3) << std::endl << std::endl;


  std::cout << test_ob.file_name_generator() <<std::endl << std::endl;

  //-------------------------------- SQlitecpp part --------------------------------//
  
  user_ob.insert_into_user_table(test_ob.name_generator().full_name, 
                                 test_ob.random_number_generator(5), 
                                 test_ob.birth_date_generator(),  
                                 "dark mode");

  user_ob.valid_user("Milah Allen");
  user_ob.valid_user("Ugyse vagy benne XD");

  */

  //-------------------------------- Server part --------------------------------//

  std::cout << "main started" << std::endl;

  try
  {
    std::cout<< "entered try block" << std::endl;
    Services server;
    server.run();
  }
  catch (const std::exception &bang)
  {
    return 1;
  }
  catch(...)
  {
    std::cout<< "in the cathc block of the server part :-(" << std::endl;
    return 1;
  }

  /*-------------------------------- End of main --------------------------------*/
  std::cout << "end of main methode --> OK";



 return 0;
}
