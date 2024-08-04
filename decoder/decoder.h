#pragma once
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include "../types.h"
class my_decoder{
    uint32_t instruction;
    my_op_type op;
    my_alu_type alu;
    my_imm_type imm;
    my_alu_type branch;
    public:
    void decode(uint32_t);
    my_alu_type get_alu()const;
    my_imm_type get_imm()const;
    my_op_type get_op()const;
    my_alu_type get_branch()const;
};

class read_file {
  std::map<uint32_t, std::byte> mp;
  public:
  void load_data();
  const std::map<uint32_t, std::byte> &get_data() const;

};
inline uint32_t get_bits(uint32_t data, int begin, int end){
  data >>= begin;
  data &= (1 << (end - begin + 1)) - 1;
  return data;
}
inline uint32_t sign_extend(uint32_t data, int len){
  auto sign = data & 1 << (len - 1);
  if (sign) {
    auto mask = UINT32_MAX ^ (1 << len) - 1;
    data |= mask;
  }
  return data;
}
inline uint32_t get_bits_signed(uint32_t data, int begin, int end){
  data = get_bits(data, begin, end);
  return sign_extend(data, end - begin + 1);
}
inline int32_t into_signed(uint32_t x){ 
  int32_t val=~(x-1);
  val=-val;
  return val;
}