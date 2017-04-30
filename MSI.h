#include <cstdint>
#include <string>



struct inputs{
  int processor;
  char action;
  int word_address;
  int line_address;
};

class Processor {
public:
//var
  char my_state;
  int line_address;
  int word_address;
  uint32_t cache_line_1;
  uint32_t cache_line_2;
  int processor_num;
  int rh_count;
  int rm_count;
  int wh_count;
  int wm_count;
  int total;
//functions
  Processor(char);
  void print_line(inputs);
  void WB(char[3]);
  void RW(char[3]);
  void WW(void);
  void Invalid(void);
};

class Bus {
public:
  std::string state;
  int reads;
  int rims;
  int wbs;
  int invs;
  int total;
  Bus();
};
