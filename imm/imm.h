#pragma once
#include"../decoder/decoder.h"
#include <cstdint>
class my_imm{
    int cnt=0;
    public:
    void tick();
    uint32_t get_imm(uint32_t instruction,my_imm_type t1,my_alu_type t2);
};