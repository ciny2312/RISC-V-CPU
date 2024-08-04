#pragma once
#include "LSB/lsb.h"
#include "alu/alu.h"
#include "imm/imm.h"
#include "decoder/func.h"
#include "decoder/ins.h"
#include "memory/memory.h"
#include "register/reg.h"
#include "ROB/rob.h"
#include "types.h"
#include <cstdint>
#include<iostream>
class my_CPU{
	my_cache cache;
	my_pre predictor;
    my_alu alu;
    my_imm imm;
    my_reg reg;
    my_mem *mem;
    my_rob rob;
    my_rs rs;
    my_lsb lsb;
    my_instruction ins;
    my_func func;
    int cnt_clock=0;
    public:
    void execute(){
	//	if(ins.PC==4112) std::cerr<<"a10:"<<reg.R[15]<<' '<<reg.depend[15]<<' '<<reg.R[10]<<' '<<reg.depend[10]<<std::endl;
		ins.execute(*mem,!func.flag_nxt,func.set_PC,func.newPC,rob.need_clear,rob.PC);
		func.execute(ins,rob,rs,lsb,reg);
		rob.execute(reg,rs.export_data(),lsb.export_data(),predictor);
		lsb.execute(rs.export_data(),rob.ask_to_lsb());
		cache.execute(rob.need_clear);
		rs.execute(lsb.export_data(),rob.need_clear);
		reg.execute(rob.need_clear);
    }	
    void tick() {
		// down level
        cnt_clock++;
    //922231    if(cnt_clock<=50)
	//		std::cout<<cnt_clock<<"  ";
		ins.tick();
		func.tick();
		rob.tick();
		alu.tick();
		lsb.tick();
		cache.tick();
		rs.tick();
		reg.tick();
		predictor.tick();
	}
	void init(my_mem *mp){
		mem=mp;
		cache.init(mp);
		lsb.init(&cache);
		func.init(&predictor);
		rs.init(&alu);
		rob.init();
		predictor.init();
		ins.init();
		reg.init();
		alu.init();
		tick();
	}
};