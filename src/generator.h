#ifndef GENERATOR_HPP
#define GENERATOR_HPP

# include <string>
# include <iostream>

#include "pugixml/src/pugixml.hpp"

class Generators
{
private:
  int digit_number = 0;
  int residence_variant = 0;

  struct name_variables
  {
    std::string first_name;
    std::string last_name;
    std::string full_name;
  };

public:
 
  std::string phone_number_generator();

  std::string birth_date_generator();

  static std::string random_number_generator(int digit_number);

  name_variables name_generator();

  std::string residence_generator(int residence_variant);

  std::string file_name_generator();

  std::string email_adress_generator();

};


#endif