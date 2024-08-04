#include "rs.h"
#include <stdexcept>

void my_rs::tick() { list = list_nxt; }

ResultType my_rs::export_data() const {
  return {alu->flag, alu->id, alu->calculate()};
}

void my_rs::add(rs_point const &entry) {
  int pos = -1;
  for (int i = 0; i < list.max_size(); ++i) {
    if (!list[i].busy) {
      pos = i;
      break;
    }
  }
  if (pos == -1) {
    std::cerr << "RS is full\n";
    throw std::runtime_error("RS is full.");
  }
  list_nxt[pos] = entry;
  list_nxt[pos].busy = true;
}
uint32_t ask_i(my_alu *alu,ResultType lsb,rs_point &x){
    if(!x.dependi) return x.Ri;
    if(alu->flag&&x.Qi==alu->id) return alu->calculate();
    if(lsb.flag&&x.Qi==lsb.id) return lsb.value;
    std::cerr<<"invaid call to ask_i\n";
    throw std::runtime_error("invaid call to ask_i");
}
uint32_t ask_j(my_alu *alu,ResultType lsb,rs_point &x){
    if(!x.dependj) return x.Rj;
    if(alu->flag&&x.Qj==alu->id) return alu->calculate();
    if(lsb.flag&&x.Qj==lsb.id) return lsb.value;
    std::cerr<<"invaid call to ask_j\n";
    throw std::runtime_error("invaid call to ask_j");
}
void my_rs::execute(ResultType lsb, bool need_clear) {
  if (need_clear) {
    clear();
    return;
  }
  update_depend(lsb);
  int index = todo_nxt(lsb);
  if (index >= 0) {
    alu->updata_value(list[index].type, ask_i(alu,lsb,list[index]),ask_j(alu,lsb,list[index]), list[index].id);
    list_nxt[index].busy = false;
  }
}

void my_rs::update_depend(ResultType lsb) {
  for (int i = 0; i < list.max_size(); ++i) {
    if (!list[i].busy)
      continue;
    if (alu->flag) {
      if (list[i].dependi && list[i].Qi == alu->id) {
        list_nxt[i].Qi = 0;
        list_nxt[i].Ri = alu->calculate();
        list_nxt[i].dependi = false;
      }
      if (list[i].dependj && list[i].Qj == alu->id) {
        list_nxt[i].Qj = 0;
        list_nxt[i].Rj = alu->calculate();
        list_nxt[i].dependj = false;
      }
    }
    if (lsb.flag) {
      if (list[i].dependi && list[i].Qi == lsb.id) {
        list_nxt[i].Qi = 0;
        list_nxt[i].Ri = lsb.value;
        list_nxt[i].dependi = false;
      }
      if (list[i].dependj && list[i].Qj == lsb.id) {
        list_nxt[i].Qj = 0;
        list_nxt[i].Rj = lsb.value;
        list_nxt[i].dependj = false;
      }
    }
  }
}

int my_rs::todo_nxt(ResultType lsb) {
  for (int i = 0; i < list.max_size(); ++i)
    if (list[i].busy &&
        (!list[i].dependi || (lsb.flag && list[i].Qi == lsb.id) ||
         (alu->flag && list[i].Qi == alu->id)) &&
        (!list[i].dependj || (lsb.flag && list[i].Qj == lsb.id) ||
         (alu->flag && list[i].Qj == alu->id)))
      return i;
  return -1;
}

bool my_rs::full() {
  for (auto &x : list)
    if (!x.busy)
      return false;
  return true;
}

void my_rs::clear() {
  for (auto &x : list_nxt)
    x.busy = false;
  alu->updata_value(my_alu_type::Hang, 0, 0, 0);
}