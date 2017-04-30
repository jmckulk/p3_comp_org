#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

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

void print_line(struct inputs, Processor*, Processor*, int *, Bus*);
void print_header(void);
void print_stats(Processor, Processor, Bus);

int main(int argc, char** argv){

  struct inputs my_input;

  int memory[512], i;
  for(i = 0; i < 512; i++){
    memory[i] = 0;
  }

  Processor p0(0);
  Processor p1(1);
  Bus bus;
  print_header();

  for(string line; getline(cin, line);){
    if(line.at(0) == '1' || line.at(0) == '0'){
      sscanf(line.c_str(), "%d%c%x", &my_input.processor, &my_input.action, &my_input.word_address);

      my_input.word_address = my_input.word_address / 4;
      my_input.line_address = my_input.word_address;

      if((my_input.line_address % 2) == 1) my_input.line_address--;
      print_line(my_input, &p0, &p1, memory, &bus);
      cout << endl;
    } else {
      break;
    }
  }
  print_stats(p0, p1, bus);
  return 0;
}

void print_header(void){
  std::cout << "\t processor 0\t\t\tbus\t\t\t processor1     \n";
  std::cout << "-----------------------------------------------";
  std::cout << "------------------------------------------\n";
  std::cout << "action\t\t  cache contents\taction\t\taction\t\t  cache contents\n";
  std::cout << "\t\t  addr  wrd0 wrd1\t\t\t\t\t  addr  wrd0 wrd1\n";
  std::cout << "\t\tI  ----- ---- ----\t\t\t\t\tI  ----- ---- ----\n";
};

void print_stats(Processor p0, Processor p1, Bus bus){
  cout << endl <<"Stats" << endl;
  cout << "  processor 0\t\tprocessor 1\t\tbus" << endl;
  cout << "-------------------------------------------";
  cout << "----------------" << endl;
  cout << "read hits\t" << p0.rh_count << "\tread hits\t";
  cout << p1.rh_count << "\tREADs\t" << bus.reads << endl;
  cout << "read misses\t" << p0.rm_count << "\tread misses\t";
  cout << p1.rm_count << "\tRIMs\t" << bus.rims << endl;
  cout << "write hits\t" << p0.wh_count << "\twrite hits\t";
  cout << p1.wh_count << "\tWBs\t" << bus.wbs << endl;
  cout << "write misses\t" << p0.wm_count << "\twrite misses\t";
  cout << p1.wm_count << "\tINVs\t" << bus.invs << endl;
  cout << "-------------------------------------------";
  cout << "----------------" << endl;
  cout << "hit rate  ";
  p0.total = p0.rh_count + p0.rm_count + p0.wh_count + p0.wm_count;
  cout << fixed << setprecision(1) << (float)(p0.rh_count + p0.wh_count)/p0.total * 100;
  cout << "%\t\thit rate ";
  p1.total = p1.rh_count + p1.rm_count + p1.wh_count + p1.wm_count;
  cout << fixed << setprecision(1) << (float)(p1.rh_count+p1.wh_count)/p1.total *100;
  cout << "%";
  bus.total = bus.reads + bus.rims + bus.wbs + bus.invs;
  cout << "\t\ttotal\t" << dec << bus.total;
  cout << endl;
}



void print_line(struct inputs input, Processor *p0, Processor *p1, int * memory, Bus *bus){
  // do stuff
  if(input.action == 'r'){// read
    if(input.processor == 0){ // processor 0
      if(input.line_address != p0->line_address){ // cache miss
        p0->rm_count++;
        p0->line_address = input.line_address;
        p0->word_address = input.word_address;
        p0->cache_line_1 = memory[input.line_address];
        p0->cache_line_2 = memory[input.line_address+1];
        switch(p0->my_state){
          case 'I':
            //new state S bus action read
            p0->my_state = 'S';
            bus->state = "READ";
            bus->reads++;
            break;
          case 'S':
            // new state s bus action read
            p0->my_state = 'S';
            bus->state = "READ";
            bus->reads++;
            break;
          case 'M':
            // new state S bus action WBr then read
            p0->my_state = 'S';
            bus->state = "WBr/R";
            bus->wbs++;
            break;
        }
      } else {  // cache hit
        p0->rh_count++;
        switch(p0->my_state){
          case 'M':
            // new state M bus action (none)
            p0->my_state = 'M';
            bus->state = "(none)";
          break;
        }
      }
      if(input.line_address == p1->line_address){
        switch(p1->my_state){
          case 'S':
            if(bus->state == "READ"){
              //new state S
              p1->my_state = 'S';
            } else if (bus->state == "RIM"){
              // new state I
              p1->my_state = 'I';
              p1->line_address = 900;
              p1->word_address = 900;
            } else if (bus->state == "INV"){
              // new state I
              p1->my_state = 'I';
              p1->line_address = 900;
              p1->word_address = 900;
            }
            break;
          case 'M':
            if(bus->state == "READ"){
              // new state S revised bus action RD/WB
              p1->my_state = 'S';
              bus->state = "RD/WB";
              bus->wbs++;
              // bus->reads++;
            } else if(bus->state == "RIM"){
              // new state I revised bus action RIM/WB
              p1->my_state = 'I';
              p1->line_address = 900;
              p1->word_address = 900;
              bus->state = "RIM/WB";
              bus->wbs++;
              // bus->rims++;
            }
            break;
        }
      }
    } else { // processor 1 read
      if(input.line_address != p1->line_address){ // cache miss
        p1->rm_count++;
        p1->line_address = input.line_address;
        p1->word_address = input.word_address;
        p1->cache_line_1 = memory[input.line_address];
        p1->cache_line_2 = memory[input.line_address+1];
        switch(p1->my_state){
          case 'I':
            //new state S bus action read
            p1->my_state = 'S';
            bus->state = "READ";
            bus->reads++;
            break;
          case 'S':
            // new state s bus action read
            p1->my_state = 'S';
            bus->state = "READ";
            bus->reads++;
            break;
          case 'M':
            // new state S bus action WBr then read
            p1->my_state = 'S';
            bus->state = "WBr/R";
            bus->wbs++;
            break;
        }
      } else {  // cache hit
        p1->rh_count++;
        switch(p1->my_state){
          case 'M':
            // new state M bus action (none)
            p1->my_state = 'M';
            bus->state = "(none)";
          break;
        }
      }
      if(input.line_address == p0->line_address){
        switch(p0->my_state){
          case 'S':
            if(bus->state == "READ"){
              //new state S
              p0->my_state = 'S';
            } else if (bus->state == "RIM"){
              // new state I
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
            } else if (bus->state == "INV"){
              // new state I
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
            }
            break;
          case 'M':
            if(bus->state == "READ"){
              // new state S revised bus action RD/WB
              p0->my_state = 'S';
              bus->state = "RD/WB";
              bus->wbs++;
            } else if(bus->state == "RIM"){
              // new state I revised bus action RIM/WB
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
              bus->state = "RIM/WB";
              bus->wbs++;
            }
            break;
        }
      }
    }
  } else { // write
    if(input.processor == 0){ // processor 0 write
        if(input.line_address != p0->line_address){ // cache miss
          p0->wm_count++;
          p0->line_address = input.line_address;
          p0->cache_line_1 = memory[input.line_address];
          p0->cache_line_2 = memory[input.line_address+1];
          if(input.line_address == input.word_address) {
            p0->cache_line_1++;
            memory[input.word_address]++;
          } else {
            p0->cache_line_2++;
            memory[input.word_address]++;
          }
          switch(p0->my_state){
            case 'I':
              //new state M bus action rim
              p0->my_state = 'M';
              bus->state = "RIM";
              bus->rims++;
              break;
            case 'S':
              // new state M bus action rim
              p0->my_state = 'M';
              bus->state = "RIM";
              bus->rims++;
              break;
            case 'M':
              // new state M bus action WBr then rim
              p0->my_state = 'M';
              bus->state = "WBr/RIM";
              bus->wbs++;
              break;
          }
        } else {  // cache hit
          p0->wh_count++;
          if(input.line_address == input.word_address) {
            p0->cache_line_1++;
            memory[input.word_address]++;
          } else {
            p0->cache_line_2++;
            memory[input.word_address]++;
          }
          switch(p0->my_state){
            case 'M':
              // new state M bus action (none)
              p0->my_state = 'M';
              bus->state = "(none)";
              break;
            case 'S':
              // ns M bs INV
              p0->my_state = 'M';
              bus->state = "INV";
              bus->invs++;
              break;
          }
        }
        if(input.line_address == p1->line_address){
          switch(p1->my_state){
            case 'S':
              if(bus->state == "READ"){
                //new state S
                p1->my_state = 'S';
              } else if (bus->state == "RIM"){
                // new state I
                p1->my_state = 'I';
                p1->line_address = 900;
                p1->word_address = 900;
              } else if (bus->state == "INV"){
                // new state I
                p1->my_state = 'I';
                p1->line_address = 900;
                p1->word_address = 900;
              }
              break;
            case 'M':
              if(bus->state == "READ"){
                // new state S revised bus action RD/WB
                p1->my_state = 'S';
                bus->state = "RD/WB";
                bus->wbs++;
              } else if(bus->state == "RIM"){
                // new state I revised bus action RIM/WB
                p1->my_state = 'I';
                p1->line_address = 900;
                p1->word_address = 900;
                bus->state = "RIM/WB";
                bus->wbs++;
              }
              break;
        }
      }
    } else { // processor 1 write
      if(input.line_address != p1->line_address){ // cache miss
        p1->wm_count++;
        p1->line_address = input.line_address;
        p1->word_address = input.word_address;
        p1->cache_line_1 = memory[input.line_address];
        p1->cache_line_2 = memory[input.line_address+1];
        if(input.line_address == input.word_address) {
          p1->cache_line_1++;
          memory[input.word_address]++;
        } else {
          p1->cache_line_2++;
          memory[input.word_address]++;
        }
        switch(p1->my_state){
          case 'I':
            //new state M bus action rim
            p1->my_state = 'M';
            bus->state = "RIM";
            bus->rims++;
            break;
          case 'S':
            // new state M bus action rim
            p1->my_state = 'M';
            bus->state = "RIM";
            bus->rims++;
            break;
          case 'M':
            // new state M bus action WBr then rim
            p1->my_state = 'M';
            bus->state = "WBr/RIM";
            bus->wbs++;
            break;
        }
      } else {  // cache hit
        p1->wh_count++;
        if(input.line_address == input.word_address) {
          p1->cache_line_1++;
          memory[input.word_address]++;
        } else {
          p1->cache_line_2++;
          memory[input.word_address]++;
        }
        switch(p1->my_state){
          case 'M':
            // new state M bus action (none)
            p1->my_state = 'M';
            bus->state = "(none)";
            break;
          case 'S':
            // ns M bs INV
            p1->my_state = 'M';
            bus->state = "INV";
            bus->invs++;
            break;
        }
      }
      if(input.line_address == p0->line_address){
        switch(p0->my_state){
          case 'S':
            if(bus->state == "READ"){
              //new state S
              p0->my_state = 'S';
            } else if (bus->state == "RIM"){
              // new state I
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
            } else if (bus->state == "INV"){
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
            }
            break;
          case 'M':
            if(bus->state == "READ"){
              // new state S revised bus action RD/WB
              p0->my_state = 'S';
              bus->state = "RD/WB";
              bus->wbs++;
            } else if(bus->state == "RIM"){
              // new state I revised bus action RIM/WB
              p0->my_state = 'I';
              p0->line_address = 900;
              p0->word_address = 900;
              bus->state = "RIM/WB";
              bus->wbs++;
            }
            break;
        }
      }
    }
  }
  // print stuff
  if(bus->state == "WBr/R"){
    cout << "\t\t\t\t\tWB" << endl;
    bus->state = "READ";
    bus->wbs++;
  } else if(bus->state == "WBr/RIM"){
    cout << "\t\t\t\t\tWB" <<endl;
    bus->state = "RIM";
    bus->wbs++;
  }
  if(input.processor == 0){
    if(input.action == 'r') cout << "read  ";
    else cout << "write ";
    cout << hex << p0->word_address*4;
    cout << "\t" << p0->my_state;
    cout << "    " << p0->line_address*4;
    cout << "    " << p0->cache_line_1;
    cout << "    " << p0->cache_line_2;
  } else {
    cout << "\t\t";
    if(p0->my_state == 'I') p0->Invalid();
    else {
      cout << p0->my_state;
      cout << "    " << p0->line_address*4;
      cout << "    " << p0->cache_line_1;
      cout << "    " << p0->cache_line_2;
    }
  }
  cout << "\t" << bus->state << "\t\t";
  if(input.processor == 1){
    if(input.action == 'r') cout << "read  ";
    else cout << "write ";
    cout << hex << p1->word_address*4;
    cout << "\t" << p1->my_state;
    cout << "    " << p1->line_address*4;
    cout << "    " << p1->cache_line_1;
    cout << "    " << p1->cache_line_2;
  } else {
    cout << "\t\t";
    if(p1->my_state == 'I') p1->Invalid();
    else {
      cout << p1->my_state;
      cout << "    " << p1->line_address*4;
      cout << "    " << p1->cache_line_1;
      cout << "    " << p1->cache_line_2;
    }
  }

}
