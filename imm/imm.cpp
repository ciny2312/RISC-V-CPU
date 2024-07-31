#include"imm.h"
#include <cstdint>
#include <fstream>
#include <stdexcept>
void my_imm::tick(){
    cnt=0;
}
uint32_t my_imm::get_imm(uint32_t instruction, my_imm_type t1, my_alu_type t2){
    if(cnt){
        throw std::runtime_error("get 2 imm in 1 tick");
    }
    cnt++;
  if (t1 == my_imm_type::R) {
    throw std::runtime_error("Warn: Trying to get immediate from R Type instrucions");
  }
  if (t1 == my_imm_type::I) {
    if (t2 == my_alu_type::SetLTUnsigned) {
      return get_bits(instruction, 20, 31);
    }
    return get_bits_signed(instruction, 20, 31);
  }
  if (t1 == my_imm_type::Ishift) {
    return get_bits(instruction, 20, 24);
  }
  if (t1 == my_imm_type::S) {
    auto lo = get_bits(instruction, 7, 11);
    auto hi = get_bits(instruction, 25, 31);
    auto val = hi << 5 | lo;
    val = sign_extend(val, 12);
    return val;
  }
  if (t1 == my_imm_type::B) {
    auto bit12 = get_bits(instruction, 31, 31);
    auto bit5_10 = get_bits(instruction, 25, 30);
    auto bit1_4 = get_bits(instruction, 8, 11);
    auto bit_11 = get_bits(instruction, 7, 7);
    auto val = bit12 << 12 | bit5_10 << 5 | bit_11 << 11 | bit1_4 << 1;
    val = sign_extend(val, 13);
    return val;
  }
  if (t1 == my_imm_type::U) {
    auto val = get_bits(instruction, 12, 31);
    return val << 12;
  }
  if (t1 == my_imm_type::J) {
    auto bit20 = get_bits(instruction, 31, 31);
    auto bit1_10 = get_bits(instruction, 21, 30);
    auto bit11 = get_bits(instruction, 20, 20);
    auto bit12_19 = get_bits(instruction, 12, 19);
    auto val = bit20 << 20 | bit1_10 << 1 | bit11 << 11 | bit12_19 << 12;
    val = sign_extend(val, 21);
    return val;
  }
  throw std::runtime_error("Invali immediate type");
}