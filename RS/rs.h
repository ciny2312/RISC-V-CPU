#pragma once
#include "../alu/alu.h"
#include "../types.h"
#include <array>
struct rs_point {
  my_alu_type type;
  uint32_t Ri, Rj, Qi, Qj, id;
  bool dependi, dependj, busy;
};

class my_rs {
  void update_depend(ResultType lsb);
  int todo_nxt(ResultType lsb);
  std::array<rs_point, 32> list_nxt;

public:
  my_alu *alu;
  std::array<rs_point, 32> list;
  void add(rs_point const &entry);
  void execute(ResultType lsb, bool need_clear);
  ResultType export_data() const;
  void tick();
  void init(my_alu *_alu) {
    alu = _alu;
    for (auto &x: list_nxt) x.busy = false;
  }
  bool full();
  void clear();
};