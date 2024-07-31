#include "cpu.h"
#include <cstdint>
#include <fstream>
#include <iostream>
my_CPU::my_CPU(const std::map<uint32_t,std::byte>&mp):mem(mp,1){
  PC=0;
}
void my_CPU::tick() {
  alu.tick();
  imm.tick();
  dec.tick();
  mem.tick();
  reg.tick();
  cnt_clock++;
//  std::cerr<<"CLOCK:"<<cnt_clock<<std::endl;
}
void my_CPU::execute() {
  uint32_t instruction = mem.load(PC,Word,true);
  if (instruction == 0x0ff00513) {
    std::cout << (reg.load(10) & 255u) << std::endl;
    exit(0);
  }
  dec.decode(instruction);
  auto alu_type = dec.get_alu();
  auto imm_type = dec.get_imm();
  auto op_type = dec.get_op();
  auto rs1 = get_bits(instruction, 15, 19);
  auto rs2 = get_bits(instruction, 20, 24);
  auto rd = get_bits(instruction, 7, 11);
  auto a = reg.load(rs1);
  auto b = reg.load(rs2);
  if (imm_type == my_imm_type::R) {
    auto val = alu.calculate(a, b, alu_type);
    reg.store(rd, val);
    PC += 4;
  } else if (imm_type == my_imm_type::I || imm_type == my_imm_type::Ishift) {
    b = imm.get_imm(instruction, imm_type, alu_type);
    auto val = alu.calculate(a, b, alu_type);
    if (op_type == my_op_type::Arith) {
      reg.store(rd, val);
      PC += 4;
    } else if (op_type == my_op_type::MemRead) {
      uint32_t sign_ext = get_bits(instruction, 14, 14);
      uint32_t width = get_bits(instruction, 12, 13);
      if(width==0) val = mem.load(val, Byte, sign_ext);
      else if(width==1) val = mem.load(val, Half, sign_ext);
      else val = mem.load(val, Word, sign_ext);
      reg.store(rd, val);
      PC += 4;
    } else if (op_type == my_op_type::Branch) {
      reg.store(rd, PC + 4);
      PC = val;
    } else {
      std::cerr<<"Invalid operation in I Type\n";
      throw std::runtime_error("Invalid operation in I Type");
    }
  } else if (imm_type == my_imm_type::S) {
    auto imm_num = imm.get_imm(instruction, imm_type, alu_type);
    auto addr = alu.calculate(a, imm_num, alu_type);
    auto width = get_bits(instruction, 12, 14);
    if(width==0) mem.store(addr, b, Byte);
    else if(width==1) mem.store(addr, b, Half);
    else mem.store(addr, b, Word);
    PC += 4;
  } else if (imm_type == my_imm_type::B) {
    auto branch = dec.get_branch();
    auto imm_num = imm.get_imm(instruction, imm_type, alu_type);
    auto val = alu.calculate(PC, imm_num, alu_type);
    bool flag;
    switch (branch) {
      case my_branch_type::Eq:
        flag = a == b;
        break;
      case my_branch_type::Ne:
        flag = a != b;
        break;
      case my_branch_type::GeU:
        flag = a >= b;
        break;
      case my_branch_type::LtU:
        flag = a < b;
        break;
      case my_branch_type::Ge:
        flag = into_signed(a) >= into_signed(b);
        break;
      case my_branch_type::Lt:
        flag = into_signed(a) < into_signed(b);
        break;
      default:
        std::cerr<<"Invalid branch type\n";
        throw std::runtime_error("Invalid branch type");
    }
    PC = flag ? val : PC + 4;
  } else if (imm_type == my_imm_type::U) {
    auto imm_num = imm.get_imm(instruction, imm_type, alu_type);
    auto val = alu.calculate(PC, imm_num, alu_type);
    reg.store(rd, val);
    PC += 4;
  } else if (imm_type == my_imm_type::J) {
    auto imm_num = imm.get_imm(instruction, imm_type, alu_type);
    reg.store(rd, PC + 4);
    PC = alu.calculate(PC, imm_num, alu_type);
  }
}