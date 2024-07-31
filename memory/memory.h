#pragma once
#include"../decoder/decoder.h"
#include <cstdint>
#include<map>
enum my_mem_type{
    Byte,Half,Word
};
class my_mem{
    std::map<uint32_t,std::byte>mp;
    int turn=0,clock_per_visit;
    bool busy;uint32_t cur_pos;
    public:
    my_mem(){
        busy=false;turn=0,clock_per_visit=1;
    }
    my_mem(const std::map<uint32_t,std::byte>&_mp,int x){
        busy=false;turn=0,clock_per_visit=x;
        mp=_mp;
    }
    void tick();
    void store(uint32_t pos,uint32_t val,my_mem_type t);
    uint32_t load(uint32_t pos,my_mem_type t,bool sign);
};