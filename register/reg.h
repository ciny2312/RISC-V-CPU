#pragma once
#include <fstream>
#include <iostream>
#include <cstdint>
class my_reg{
    uint32_t reg[32];
    int cnt_load=0,cnt_store=0;
    public:
    uint32_t load(uint32_t);
    void store(uint32_t,uint32_t);
    void tick();
};