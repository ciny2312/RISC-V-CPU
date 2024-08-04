#pragma once
#include "../types.h"
#include <array>
#include <cstdint>
class my_pre {
  inline int hash(uint32_t pc) { return (pc >> 2) & ((1 << 6) - 1); }

  std::array<int, 1 << 6> his;
  std::array<std::array<my_pre_type, 1 << 4>, 1 << 6> stat;

public:
  long long total=0,success=0;

  bool get_prediction(uint32_t pc) {
    auto ind = hash(pc);
    return stat[ind][his[ind]] == my_pre_type::yes_weak ||
           stat[ind][his[ind]] == my_pre_type::yes_strong;
  }
  void update(uint32_t pc, bool ok) {
    ++total;
    if (ok)
      ++success;
    bool jumped = (get_prediction(pc) == ok);
    int ind = hash(pc);
    auto &st = stat[ind][his[ind]];
    switch (st) {
    case not_strong:
      if (jumped)
        st = not_weak;
      break;
    case not_weak:
      if (jumped)
        st = yes_weak;
      else
        st = not_strong;
      break;
    case yes_weak:
      if (jumped)
        st = yes_strong;
      else
        st = not_weak;
      break;
    case yes_strong:
      if (!jumped)
        st = yes_weak;
      break;
    }
    his[ind] = ((his[ind] << 1) | jumped) & ((1 << 4) - 1);
  }
  void tick() {}	
  void init() {
		stat.fill({});
		his.fill(0);
		total = success = 0;
	}

};