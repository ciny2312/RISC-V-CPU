#pragma once
#include "../LSB/lsb.h"
#include "../ROB/rob.h"
#include "../RS/rs.h"
#include "../predictor/pre.h"
#include "../register/reg.h"
#include "../types.h"
#include "decoder.h"
#include "ins.h"

class my_func {
  my_decoder dec;
  my_imm imm;
  bool set_PC_nxt = false;
  uint32_t newPC_nxt = 0;

  rob_point toRob;
  lsb_point toLSB;
  rs_point toRS;
  bool add_rob = false, add_lsb = false, add_rs = false;

  void updata_depend(my_rob &rob, my_rs &rs, my_lsb &lsb, my_reg &regs);

public:
  bool set_PC = false;
  uint32_t newPC = 0;
  my_pre *pre;
  bool flag_nxt = true;
  void execute(my_instruction &iu, my_rob &rob, my_rs &rs, my_lsb &lsb,
               my_reg &regs);
  void tick() {
    set_PC = set_PC_nxt;
    newPC = newPC_nxt;
    flag_nxt = true;
  }
	void init(my_pre *p) {
		set_PC_nxt = false;
		newPC_nxt = 0;
		flag_nxt = true;
		pre = p;
	}
};