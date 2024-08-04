#pragma once
#include "../memory/memory.h"
#include "../types.h"
#include "../cache.h"
#include <array>
#include <cstdint>

struct lsb_point {
  my_mem_type mem_type;
  my_imm_type imm_type;
  my_op_type op_type;
  uint32_t Ri, Rj, Qi, Qj, offset, id;
  bool dependi, dependj, wait, busy;
};
template <typename T, uint32_t N> struct my_array {
  T arr[N];
  int head, tail;

  my_array(){
    clear();
  }
  uint32_t get_size() { return N; }
  bool full() { return head == tail && arr[head].busy; }
  bool empty() const { return head == tail && !arr[head].busy; }

  void clear() {
    head = tail = 0;
    for (auto &x : arr)
      x.busy = false;
  }
  void push(T x) {
    arr[tail++] = x;
    if (tail == N)
      tail = 0;
  }
  void pop() {
    arr[head].busy = false;
    if (++head == N)
      head = 0;
    while (head != tail && !arr[head].busy) {
      if (++head == N)
        head = 0;
    }
  }
  T &operator[](int x) { return arr[x]; }
};

class my_lsb {
  my_array<lsb_point, 32> list,list_nxt;
  int get_next(ResultType lsb, ResultType rs, to_lsb rob);
  void update_depend(ResultType cr, ResultType rs, to_lsb rob);
  void clear(){
	  list_nxt.clear();
  };

public:
  my_cache *cache;
  void execute(ResultType rs, to_lsb rob);
  void tick() { list = list_nxt; }	
  void init(my_cache *c) {
		clear();
		cache = c;
	}
  void add(lsb_point x);
  bool full();
  ResultType export_data() { return cache->get_result(); }

};