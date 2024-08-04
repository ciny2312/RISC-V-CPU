#pragma once
#include "../types.h"
#include <array>
#include <cstdint>
class my_pre {
  inline int hash(uint32_t PC) { return ((PC >> 3) + 5) & ((1 << 6) - 1); }

  std::array<int, 1 << 6> vis;
  std::array<std::array<my_pre_type, 1 << 4>, 1 << 6> predictor;

public:
  long long total=0,success=0;

  bool get_prediction(uint32_t PC) {
    auto pos = hash(PC);
    return predictor[pos][vis[pos]] == my_pre_type::AC1 ||
           predictor[pos][vis[pos]] == my_pre_type::AC2;
  }
  void update(uint32_t PC, bool right) {
    ++total;
    if (right)
      ++success;
    bool jumped = (get_prediction(PC) == right);
    int pos = hash(PC);
    auto &st = predictor[pos][vis[pos]];
    switch (st) {
    case WA2:
      if (jumped)
        st = WA1;
      break;
    case WA1:
      if (jumped)
        st = AC1;
      else
        st = WA2;
      break;
    case AC1:
      if (jumped)
        st = AC2;
      else
        st = WA1;
      break;
    case AC2:
      if (!jumped)
        st = AC1;
      break;
    }
    vis[pos] = ((vis[pos] << 1) | jumped) & ((1 << 4) - 1);
  }
  void tick() {}	
  void init() {
		predictor.fill({});
		vis.fill(0);
		total = success = 0;
	}

};