#pragma once
#include "memory/memory.h"
#include "types.h"
#include <cstdint>
#include <stdexcept>

class my_cache {
	bool data_ready_nxt=false;
	my_mem_type type_nxt=my_mem_type::Hang;
	my_imm_type imm_type;
	my_op_type op_type;
	uint32_t turn_nxt=0,addr_nxt=0, value_nxt=0, id_nxt=0;

	bool is_set;

public:
	my_mem *mem;

	bool data_ready=false;
	my_mem_type type=my_mem_type::Hang;
	my_imm_type imm_type_nxt;
	my_op_type op_type_nxt;
	uint32_t turn=0,addr=0, value=0, id=0;
	void set(uint32_t addr_, uint32_t value_, uint32_t id_, my_mem_type type_,my_imm_type imm_,my_op_type op_) {
		if (turn != 0){
			std::cerr<<"Cache is working!\n";
			throw std::runtime_error("Cache is working!");
		}
		turn_nxt = 2;
		data_ready_nxt = false;
		addr_nxt = addr_;
		value_nxt = value_;
		id_nxt = id_;
		type_nxt = type_;
		imm_type_nxt=imm_;
		op_type_nxt=op_;
		is_set = true;
	}

	void execute(bool need_clear) {
		if (need_clear) {
			clear();
			return;
		}
		if (turn > 0) {
			turn_nxt = turn - 1;
			data_ready_nxt = (turn == 1 && type != my_mem_type::Hang);
		}
		else if (turn == 0) {
			if(imm_type==my_imm_type::S&&(type==my_mem_type::Byte||type==my_mem_type::Half||type==my_mem_type::Word)) mem->store(addr, value,type);
			if (!is_set)
				type_nxt = my_mem_type::Hang;
		}
	}

	ResultType get_result() {
		if (!data_ready || type == my_mem_type::Hang)
			return {false, 0, 0};
		uint32_t val = 1;
		if((imm_type==my_imm_type::I||imm_type==my_imm_type::Ishift)&&op_type == my_op_type::MemRead)
        	val=mem->load(addr, type);
		return {true, id, val};
	}

	void tick() {
		turn = turn_nxt;
		data_ready = data_ready_nxt;
		addr = addr_nxt;
		value = value_nxt;
		id = id_nxt;
		type = type_nxt;
		imm_type=imm_type_nxt;
		op_type=op_type_nxt;
		is_set = false;
	}
	bool ready() { return data_ready; }
	bool free() { return turn == 0; }
	void init(my_mem *mp){
		mem = mp;
		clear();
	}
	void clear() {
		turn_nxt = 0;
		data_ready_nxt = false;
		type_nxt = my_mem_type::Hang;
		addr_nxt = value_nxt = id_nxt = 0;
	}

};