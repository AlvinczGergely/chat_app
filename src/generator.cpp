#include "generator.h"
#include <vector>
#include <random>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <map>
#include <cmath>

#include "pugixml/src/pugixml.hpp"


std::string Generators::phone_number_generator()
{
    std::string mobile_number = "+36 ";
    std::vector<std::string> service_provider = {"20 ", "30 ", "40 ", "50 ", "70 ", "80 ", "90 "};   //szolgáltatók karakterei

    srand (time(NULL));
    int ran = rand() % 6;                                   // random választás szolgáltatóból
    mobile_number += service_provider[ran];
    
    for(int i = 0; i < 7; i++)                           //maradék 7 karakter 0 és 9 közötti számokkal
    {
      if(i == 3) {mobile_number += " ";}
      int less_num_rand = rand() % 9; 
      mobile_number += std::to_string(less_num_rand);
    }

    return mobile_number;                               //teljes szám visszaadása
}

std::string Generators::birth_date_generator()
{
    int year = 0;
    int month = 0;
    int day = 0;

    srand (time(NULL));

    year = rand() % 74 + 1950;
    month = rand() % 12 + 1;

    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
      {day = rand() % 31 + 1;}
    else if(month == 2)
      {day = rand() % 28 + 1;}
    else
      {day = rand() % 30 + 1;}

    std::string s_month = std::to_string(month);
    if(s_month.length() == 1)
      {
        s_month.insert(0, 1, '0');    
      }

    std::string s_day = std::to_string(day);
    if(s_day.length() == 1)
      {
        s_day.insert(0, 1, '0');
      }

    std::string date = std::to_string(year) + "-" + s_month + "-" + s_day; 
    
    return date;
}

std::string Generators::random_number_generator (int digit_number)  
{
    srand (time(NULL));

    int num = rand() % 89999 + 10000;
    // 5 digit min: 10.000 max: 89.999
    // 4 digit min: 1.000 max: 8.999
    // 3 digit min: 100 max: 899
    // 2 digit min: 10 max: 89
    // 1 digit min: 1 max: 9
    return std::to_string(num);
}

Generators::name_variables Generators::name_generator()
{

    pugi::xml_document doc_one;                                //pugi xml 
    pugi::xml_document doc_two;

    pugi::xml_parse_result result_first_name = doc_one.load_file("../../src/XML_test_files/first_names.xml");       //file beimportálása
    pugi::xml_parse_result result_last_name = doc_two.load_file("../../src/XML_test_files/last_names.xml");  

    pugi::xml_node first_names = doc_one.child("first_names");         //node létrehozása
    pugi::xml_node last_names = doc_two.child("last_names");

    std::vector<std::string> first_names_vec;
    std::vector<std::string> last_names_vec;

    for(auto name_node = first_names.child("name"); name_node; name_node = name_node.next_sibling())
    {
      first_names_vec.push_back(name_node.text().as_string()); 
    }

    for(auto name_node = last_names.child("name"); name_node; name_node = name_node.next_sibling())
    {
      last_names_vec.push_back(name_node.text().as_string()); 
    }

    srand (time(NULL));

    std::string ran_first_name(first_names_vec[rand() % first_names_vec.size()]);   //if we need to check the parts of the names
    std::string ran_last_name(last_names_vec[rand() % last_names_vec.size()]); 
    std::string ran_full_name = ran_first_name + " " + ran_last_name; 

    name_variables result = {ran_first_name, ran_last_name, ran_full_name};

    return result;
} 

std::string Generators::residence_generator(int residence_variant)
{
    pugi::xml_document doc_zipcode;
    pugi::xml_document doc_city;
    pugi::xml_document doc_street;

    pugi::xml_parse_result zip_codes = doc_zipcode.load_file("../../src/XML_test_files/post code.xml");
    pugi::xml_parse_result city_names = doc_city.load_file("../../src/XML_test_files/city_list.xml");
    pugi::xml_parse_result street_names = doc_street.load_file("../../src/XML_test_files/streets.xml");

    pugi::xml_node post_code_parent_node = doc_zipcode.child("zipcodes");
    pugi::xml_node city_parent_node = doc_city.child("gb_cities");
    pugi::xml_node street_parent_node = doc_street.child("streets");

    std::vector<std::string> zipcodes;
    std::vector<std::string> cities;
    std::vector<std::string> streets;

    //zipcodes vector uploading
    for(auto zipcode_node = post_code_parent_node.child("zipcode"); zipcode_node; zipcode_node = zipcode_node.next_sibling())
    {
      zipcodes.push_back(zipcode_node.text().as_string());
    }

    //city vector uploading
    for(auto city_node = city_parent_node.child("city"); city_node; city_node = city_node.next_sibling())
    {
      cities.push_back(city_node.text().as_string());
    }

    //streets vector uploading
    for(auto street_node = street_parent_node.child("street"); street_node; street_node = street_node.next_sibling())
    {
      streets.push_back(street_node.text().as_string());
    }

    std::string full_residence;
    srand (time(NULL));
    int flat_num = rand() % 99 + 1;
    int floor_num = rand() % 20 + 1;
    char cch = 'a' + rand()%26;
    
    // variable generator
    if(residence_variant == 1)    // zipcode + city + street + house number
    {
      full_residence = zipcodes[rand() % zipcodes.size()] + ", city: " + cities[rand() % cities.size()] + ", street: " + 
                       streets[rand() % streets.size()] + ", house number: " + std::to_string(flat_num);
    }
    else if(residence_variant == 2)    // zipcode + city + street + floor number + door number 
    {
      full_residence = zipcodes[rand() % zipcodes.size()] + ", city: " + cities[rand() % cities.size()] + ", street: " + 
                       streets[rand() % streets.size()] + ", floor: " + std::to_string(floor_num) + ", door: " + std::to_string(flat_num);
    }
    else if(residence_variant == 3)    // zipcode + city + street + building number + floor number + door number
    {
      full_residence = zipcodes[rand() % zipcodes.size()] + ", city: " + cities[rand() % cities.size()] + ", street: " + 
                       streets[rand() % streets.size()] + ", boulding code: " + cch + ", floor: " +
                       std::to_string(floor_num) + ", door: " + std::to_string(flat_num);
    }

    return full_residence;
}

std::string Generators::file_name_generator()
{
  std::vector<std::string> file_extensions = {"txt", "doc", "docx", "pdf", "ppt", "pptx", "xls", "xlsx", "csv", 
                                              "jpg", "jpeg", "png", "gif", "bmp", "tif", "tiff", "mp3", "wav", "avi", 
                                              "mp4", "mkv", "mov", "zip", "rar", "7z"};

  pugi::xml_document doc_file_names;
  pugi::xml_parse_result file_names = doc_file_names.load_file("../../src/XML_test_files/file_names.xml");
  pugi::xml_node file_name_parent_node = doc_file_names.child("files");

  std::vector<std::string> f_names;

  for(auto file_name_node = file_name_parent_node.child("file"); file_name_node; file_name_node = file_name_node.next_sibling())
  {
    f_names.push_back(file_name_node.text().as_string());
  }

  srand (time(NULL));
  std::string full_file_name = f_names[rand() % f_names.size()] + "." + file_extensions[rand() % file_extensions.size()];
  
  return full_file_name;
}

std::string Generators::email_adress_generator()
{
    pugi::xml_document doc_one;                                
    pugi::xml_document doc_two;


    pugi::xml_parse_result result_first_name = doc_one.load_file("../../src/XML_test_files/first_names.xml");       //file beimportálása
    pugi::xml_parse_result result_last_name = doc_two.load_file("../../src/XML_test_files/last_names.xml");

    pugi::xml_node first_names = doc_one.child("first_names");
    pugi::xml_node last_names = doc_two.child("last_names");

    std::vector<std::string> first_names_vec;
    std::vector<std::string> last_names_vec;

    for(auto name_node = first_names.child("name"); name_node; name_node = name_node.next_sibling())
    {
      first_names_vec.push_back(name_node.text().as_string()); 
    }

    for(auto name_node = last_names.child("name"); name_node; name_node = name_node.next_sibling())
    {
      last_names_vec.push_back(name_node.text().as_string()); 
    }

    srand (time(NULL));

    std::string ran_first_name(first_names_vec[rand() % first_names_vec.size()]);   //if we need to check the parts of the names
    std::string ran_last_name(last_names_vec[rand() % last_names_vec.size()]);

    std::string email_adress = ran_first_name + '.' +  ran_last_name + '@' + "gmail.com";

    return email_adress;
}
