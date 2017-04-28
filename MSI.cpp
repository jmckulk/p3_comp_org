#include "MSI.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


processor::processor(char num){
  processor_num = num;
  my_state = I;
  rh_count = 0;
  rm_count = 0;
  wh_count = 0;
  wm_count = 0;
  cache_line_2 = 0;
  cache_line_1 = 0;
};

void processor::WB(char address[3]){
  std::cout << "M  "<< address << " ---- ----";
};

void processor::RW(char address[3]){
  std::cout << "S  " << address << " ---- ----";
}

void processor::Invalid(void){
  std::cout << "I ---- ---- ----";
}

void processor::print_line(char action, int processor_num, char address[3]){
  if(processor_num == processor::processor_num){
    if(action == 'r'){
      std::cout << "read  " << address << "\t";
    } else {
      std::cout << "write " << address << "\t";
    }
    switch(action){
      case 'r':
        RW(address);
        break;
      case 'w':
        WB(address);
        break;
    }
  } else {
    std::cout << "\t\t";
    Invalid();
  }
};
//bus
void bus::print(void){
  std::cout << "\tbus\t\t";
};



// memory
memory::memory(void){
  int i;
  for(i = 0; i < 512; i++){
    main[i] = 0;
  }
}
// input
void input::print_header(void){
  std::cout << "\tprocessor 0\t\t\tbus\t\t\tprocessor1     \n";
  std::cout << "-----------------------------------------------";
  std::cout << "------------------------------------------\n";
  std::cout << "action\t\tcache contents\t\taction\t\taction\t\tcache contents\n";
  std::cout << "\t\taddr  wrd0 wrd1\t\t\t\t\t\taddr  wrd0 wrd1\n";
  std::cout << "\t\tI ---- ---- ----\t\t\t\t\tI ---- ---- ----\n";
};

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
      if(token.at(0) == '1'){
        my_input.processor = 1;
      } else {
        my_input.processor = 0;
      }
      my_input.action = token.at(1);
      my_input.address[0] = token.at(2);
      my_input.address[1] = token.at(3);
      my_input.address[2] = token.at(4);
      my_vector.push_back(my_input);
    }
  }
}
