#include "MSI.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>




void input::open_file(std::string filename){
  std::ifstream file(filename);
  std::stringstream buffer;

  buffer << file.rdbuf();
  input_string = buffer.str();
  // cout << str << endl;
  parse();
};

void input::parse(void){
  struct inputs my_input;

  std::istringstream iss(input_string);
  std::string token;
  while(std::getline(iss, token, '\n')){
    if(token.at(0) == '1' || token.at(0) == '0'){
      my_input.processor = token.at(0);
      my_input.action = token.at(1);
      my_input.address[0] = token.at(2);
      my_input.address[1] = token.at(3);
      my_input.address[2] = token.at(4);
      my_input.address[3] = '\n';
      my_vector.push_back(my_input);
    }

  }
}
