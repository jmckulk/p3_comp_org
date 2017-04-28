#include <cstdint>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string>
#include <vector>


enum State {M, S, I};
enum Bus_action {READ, RIM, INV, WBr, WBc};

class processor {
private:
  State my_state;
  uint32_t cache_line_1;
  uint32_t cache_line_2;
  int processor_num;
  int rh_count;
  int rm_count;
  int wh_count;
  int wm_count;
public:
  processor(char);
  void print_line(char, int, char[3]);
  void WB(char[3]);
  void RW(char[3]);
  void WW(void);
  void Invalid(void);
};

class bus{
private:
public:
  void print();
};

class memory{
private:
  uint8_t main[512];
public:
  memory();
  uint8_t get_memory(char address[3]);
};
struct inputs{
  int processor;
  char action;
  char address[3];
};

class input {
private:
  std::string input_string;
public:
  std::vector<inputs> my_vector;
  struct inputs get_input(void);
  void open_file(std::string);
  void parse(void);
  void print_header(void);
};
