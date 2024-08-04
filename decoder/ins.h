#pragma once
#include "../memory/memory.h"
#include "../types.h"
#include "decoder.h"
#include <cstdint>
class my_instruction {
public:
  uint32_t PC = 0, instruction, instrAddr;
  bool wait = false, flag = false;

private:
  uint32_t PC_nxt = 0, instruction_nxt, instrAddr_nxt;
  bool wait_nxt = false, flag_nxt = false;

public:
  void execute(my_mem &mem, bool decoder_need_stall, bool jump_by_decoder,
               uint32_t newPC_from_decoder, bool jump_by_RoB,
               uint32_t newPC_from_RoB) {
    if (!jump_by_RoB) {
      if (decoder_need_stall) {
        flag_nxt = true;
        return;
      }
      if (wait) {
        flag_nxt = false;
        wait_nxt = false;
        return;
      }
    }
    uint32_t usedPC = jump_by_RoB ? newPC_from_RoB
                                  : (jump_by_decoder ? newPC_from_decoder : PC);
    instruction_nxt = mem.load(usedPC, Word);

    PC_nxt = usedPC + 4;
    instrAddr_nxt = usedPC;
    flag_nxt = true;

    switch (instruction_nxt & 0b01111111) {
      case 0b1101111:
        wait_nxt = true;
        break;
      case 0b1100111:
        wait_nxt = true;
        break;
      case 0b1100011:
        wait_nxt = true;
        break;
      }
  }
  void tick() {
    PC = PC_nxt;
    wait = wait_nxt;
    flag = flag_nxt;
    instruction = instruction_nxt;
    instrAddr = instrAddr_nxt;
  }
	void init() {
		PC_nxt = 0x00000000;
		wait_nxt = false;
		flag_nxt = false;
	}

  void stall_by_decoder() {
    PC_nxt = PC;
    wait_nxt = wait;
    flag_nxt = flag;
    instruction_nxt = instruction;
    instrAddr_nxt = instrAddr;
  }
};