#pragma once
#include "alu/alu.h"
#include "imm/imm.h"
#include "decoder/decoder.h"
#include "memory/memory.h"
#include "register/reg.h"
#include <cstdint>
#include<iostream>
class my_CPU{
    my_alu alu;
    my_imm imm;
    my_reg reg;
    my_decoder dec;
    my_mem mem;
    int cnt_clock,PC;
    public:
    my_CPU(const std::map<uint32_t,std::byte> &mp);
    void tick();
    void execute();
};