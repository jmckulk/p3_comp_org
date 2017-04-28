#include <iostream>
#include "MSI.h"


int main(int argc, char** argv){

  input my_input;
  my_input.open_file(argv[1]);

  memory my_memory;
  bus my_bus;

  processor p0(0);
  processor p1(1);

  my_input.print_header();

  for(std::vector<struct inputs>::iterator it = my_input.my_vector.begin(); it != my_input.my_vector.end(); ++it){
    p0.print_line((*it).action, (*it).processor, (*it).address);
    my_bus.print();
    p1.print_line((*it).action, (*it).processor, (*it).address);
    std::cout << std::endl;
  }

  return 0;
}
