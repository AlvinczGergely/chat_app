  #include <gtest/gtest.h>
  #include <regex>

  #include "../src/generator.h"

namespace{
  
  TEST(phone_number_generator_test, generates_valid_phone_number)
  {
    
    Generators generators; 
    std::string phone_number = generators.phone_number_generator(); 
    
    //Testint the +36 strintg value
    ASSERT_EQ(phone_number.substr(0, 3), "+36");
    
    //testing the length of the phone number
    ASSERT_EQ(phone_number.length(), 15);
    
    //testing the operator code
    std::vector<std::string> validPrefixes = {"20 ", "30 ", "40 ", "50 ", "70 ", "80 ", "90 "};
    ASSERT_EQ(std::find(std::begin(validPrefixes), std::end(validPrefixes), phone_number.substr(3, 3)), std::end(validPrefixes));
    
    //testing the remaining part of the number 
    for (int i = 7; i < 15; ++i)
    {
      if(i == 9){continue;}
      ASSERT_GE(isdigit(phone_number[i]), 0);
    }
  }


  TEST(birht_date_generator_test, generates_valid_birth_date)
  {
    Generators generators;
    std::string birth_date = generators.birth_date_generator();   // 1997-07-07
    std::cout << "generated date: " << birth_date << std::endl;

    //Check that the year is valid
    ASSERT_GE(stoi(birth_date.substr(0, 4)), 1950) << "check if year is smaller than 1950";
    ASSERT_LE(stoi(birth_date.substr(0, 4)),  2023) << "check if year is over than 2023";

    std::string fifth_char(1, birth_date[4]);
    std::string seventh_char(1, birth_date[7]);
    ASSERT_STREQ(fifth_char.c_str(), "-") << "fifth caracter check if it is a - or not";      // this pice of s..t only allows c string ... why?
    ASSERT_STREQ(seventh_char.c_str(), "-") << "seventh caracter check if it is a - or not";

    //check if days are valid based of the month
    if(birth_date.substr(5,2) == "01" || "03" || "05" || "07" || "08" || "10" || "12")      // !!!!!!!!!! nem jo, csere
    {
       ASSERT_LE(stoi(birth_date.substr(8, 2)), 31);
    }
    else if(birth_date.substr(5,2) == "02")
      {
        ASSERT_LE(stoi(birth_date.substr(8, 2)), 28);
      }
    else
      {
        ASSERT_LE(stoi(birth_date.substr(8, 2)), 30);
      }

    //check the length of the date 
    ASSERT_EQ(birth_date.length(), 10);
  }


  TEST(random_number_generator_test, generate_valid_random_number)
  {
    Generators generator;
    std::string random_number = generator.random_number_generator(5);
    std::cout << "generated random number: " << random_number << std::endl;

    ASSERT_EQ(random_number.length(), 5);

    ASSERT_GT(std::stoi(random_number), 0);


  }


  TEST(name_generator_test, generater_valid_name)
  {
    Generators generator;
    std::string name_generator = generator.name_generator().full_name;
    std::cout << "generated full name: " << name_generator << std::endl ;
    std::regex pattern("^[A-Z][a-z]+\\s[A-Z][a-z]+$");

    using testing::MatchesRegex;
    ASSERT_TRUE(std::regex_match(name_generator.begin(), name_generator.end(), pattern));
  }
  

  TEST(residence_generator_test, generate_valid_residence_generator)
  {
    Generators generator;
    srand (time(NULL));
    int random_num = rand()%(3-1+1) + 1;
    std::string residence = generator.residence_generator(random_num);

    ASSERT_FALSE(residence.empty());
  }

 
  TEST(file_name_generator_test, generate_valid_file_name)
  {
    Generators generator;
    std::string file_name = generator.file_name_generator();

    std::regex pattern("^[a-zA-Z0-9_]+\\.[a-zA-Z0-9_]+$");
    ASSERT_TRUE(std::regex_match(file_name.begin(), file_name.end(), pattern));
  }

  
  TEST(email_adress_generator_test, generate_valid_email_adres)
  {
    Generators generator;
    std::string email_adres = generator.email_adress_generator();
    std::regex pattern("^\\w+.\\w+@\\w+.com$"); 
    ASSERT_TRUE(std::regex_match(email_adres.begin(), email_adres.end(), pattern));
  }
    
}
