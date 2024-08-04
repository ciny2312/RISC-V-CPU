#include "rob.h"
#include <stdexcept>

void my_rob::tick() {
  list = list_nxt;
  PC = PC_nxt;
  need_clear = need_clear_nxt;
}
void my_rob::execute(my_reg &regs, ResultType rs, ResultType lsb, my_pre &pre) {
  if (need_clear) {
    clear();
    return;
  }
  need_clear_nxt = false;
  if (rs.flag) {
    list_nxt[rs.id].value = rs.value;
    list_nxt[rs.id].flag = true;
//    std::cout<<"rs: "<<rs.id<<' '<<rs.value<<' '<<list_nxt[rs.id].id<<std::endl;
  }
  if (lsb.flag) {
    list_nxt[lsb.id].value = lsb.value;
    list_nxt[lsb.id].flag = true;
//    std::cout<<"lsb: "<<lsb.id<<' '<<lsb.value<<std::endl;
  }
  if (list.empty()) return;
  auto id = list.head;
  auto const &x = list[id];
  if (x.flag) {
    list_nxt.pop();
    if (x.t == my_rob_type::R) {
      regs.set_val(x.id, id, x.value);
    } else if (x.t == my_rob_type::B) {
      if ((x.value & 1) ^ (x.jumpAddr & 1)) {
        PC_nxt = x.jumpAddr & (~1);
        need_clear_nxt = true;
        pre.update(x.instrAddr, false);
      } else
        pre.update(x.instrAddr, true);
    } else if (x.t == my_rob_type::S) {
      // do nothing
    } else if (x.t == my_rob_type::EXIT) {
      std::cout<< (regs.R[10] & 255u);
      exit(0);
    }
  }
}

void my_rob::add(rob_point x, my_reg &regs) {
  if (list.full()) {
    std::cerr << "rob list is full\n";
    throw std::runtime_error("rob list is full");
  }
  if (x.t == my_rob_type::R)
    regs.set_depend(x.id, list.tail);
  x.busy = true;
  list_nxt.push(x);
}

void my_rob::clear() {
  list_nxt.clear();
  need_clear_nxt = false;
  PC_nxt = 0;
}