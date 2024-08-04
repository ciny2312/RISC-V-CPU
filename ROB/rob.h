#pragma once
#include "../predictor/pre.h"
#include "../register/reg.h"
#include "../LSB/lsb.h"
#include "../types.h"
#include <array>

struct rob_point {
  my_rob_type t;
  uint32_t id, value, instrAddr, jumpAddr;
  bool flag, busy;
};
class my_rob {

  my_array<rob_point, 16> list_nxt;
  bool need_clear_nxt=false;
  uint32_t PC_nxt=0;
  void clear();

public:
  my_array<rob_point, 16> list;
  bool need_clear=false;
  uint32_t PC=0;
  void execute(my_reg &regs, ResultType rs, ResultType lsb, my_pre &predictor);
  void tick();
  void init() { clear(); }

  bool full() { return list.full(); }
  void add(rob_point entry, my_reg &regs);
  uint32_t add_index() const { return list.tail; }
  to_lsb ask_to_lsb() const {
    to_lsb x;x.need_clear=need_clear;
    x.flag=!list.empty();x.id=list.head;
    return x;
  }
};