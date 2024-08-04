#pragma once
#include"../decoder/decoder.h"
#include "../types.h"
#include <cstdint>
#include<map>
class my_mem{
    std::map<uint32_t,std::byte>mp;
    uint32_t cur_pos;
    public:
    my_mem(const std::map<uint32_t,std::byte>&_mp,int x){
        mp=_mp;
    }
    void store(uint32_t pos,uint32_t val,my_mem_type t);
    uint32_t load(uint32_t pos,my_mem_type t);
};