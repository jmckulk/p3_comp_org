#include "MSI.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// #include <vector>

extern struct inputs input;

// processor
Processor::Processor(char num){
  processor_num = num;
  my_state = 'I';
  rh_count = 0;
  rm_count = 0;
  wh_count = 0;
  wm_count = 0;
  line_address = 0;
  word_address = 0;
  cache_line_2 = 0;
  cache_line_1 = 0;
  total = 0;
};

void Processor::WB(char address[3]){
  std::cout << "M  "<< address;
};

void Processor::RW(char address[3]){
  std::cout << "S  " << address << " ---- ----";
}

void Processor::Invalid(void){
  std::cout << "I  ----- ---- ----";
}

Bus::Bus(void){
  this->state = "";
  this->reads = 0;
  this->rims = 0;
  this->invs = 0;
  this->wbs = 0;
  this->total = 0;
}
