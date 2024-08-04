#pragma once
#include <array>
#include <iostream>
#include <stdexcept>

class my_reg {
  std::array<uint32_t, 32> R_nxt;
  std::array<uint32_t, 32> Q_nxt;
  std::array<bool, 32> depend_nxt;
  std::array<bool, 32> vis;

public:
  std::array<uint32_t, 32> R;
  std::array<uint32_t, 32> Q;
  std::array<bool, 32> depend;

  void tick() {
    R = R_nxt;
    Q = Q_nxt;
    depend = depend_nxt;
    vis.fill(false);
  }
	void init() {
		R_nxt.fill(0);
		Q_nxt.fill(0);
		depend_nxt.fill(0);
	}
  void execute(bool flag){
    if(flag) clear();
  }
  void set_depend(uint32_t x, uint32_t id) {
    if (x == 0)
      return;
    depend_nxt[x] = true;
    Q_nxt[x] = id;
    vis[x] = true;
  }
  void set_val(uint32_t x, uint32_t id, uint32_t val) {
    if (x > 31) {
      std::cerr << "out of register bound\n";
      throw std::runtime_error("out of register bound");
    }
    if (x == 0)
      return;
  //  std::cerr<<x<<' '<<val<<' '<<(Q[x]!=id)<<std::endl;
    R_nxt[x] = val;
    if (!vis[x]) {
      depend_nxt[x] = (Q[x] != id);
      if (Q[x] == id)
        Q_nxt[x] = 0;
    }
  }
  void clear() {
    Q_nxt.fill(0);
    depend_nxt.fill(false);
    return;
  }
};