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
  int rh_count;
  int rm_count;
  int wh_count;
  int wm_count;
public:
  void WB(void);
  void RW(void);
  void WW(void);
};



struct inputs{
  char processor;
  char action;
  char address[4];
};

class input {
private:
  std::vector<inputs> my_vector;
  std::string input_string;
public:
  void open_file(std::string);
  void parse(void);
};
