#pragma once
#include "../decoder/decoder.h"
#include "../imm/imm.h"
#include <cstdint>
#include <fstream>
class my_alu {
  bool flag_nxt = false;
  uint32_t a_nxt, b_nxt, id_nxt;
  my_alu_type t_nxt;

public:
  bool flag = false;
  uint32_t a, b, id;
  my_alu_type t;
  void tick();
  uint32_t calculate();
  void updata_value(my_alu_type _t, uint32_t _a, uint32_t _b, uint32_t _id);
	void init() { tick(); }
};