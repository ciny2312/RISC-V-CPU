#pragma once
#include"../decoder/decoder.h"
#include"../imm/imm.h"
#include <cstdint>
#include <fstream>
class my_alu{
    int cnt=0;
    public:
    void tick();
    uint32_t calculate(uint32_t a,uint32_t b,my_alu_type t);
};