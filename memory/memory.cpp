#include "memory.h"
#include <cstdint>
void my_mem::tick(){
    if(busy){
        ++turn;
        if(turn==clock_per_visit){
            turn=0;busy=false;
        }
    }
}
void my_mem::store(uint32_t pos, uint32_t val, my_mem_type t){
if (busy) {
    std::cerr<<"Calling store while the memory store is still busy.\n";
    throw std::runtime_error("Calling store while the memory store is still busy.");
  }
  busy = true;
  turn = 0;
  cur_pos= pos;
  switch (t) {
    case my_mem_type::Byte : {
      mp[pos] = std::byte(val & ((1<<8)-1));
      break;
    }
    case my_mem_type::Half : {
      mp[pos] = std::byte(val & ((1<<8)-1));
      mp[pos + 1] = std::byte(val >> 8 & ((1<<8)-1));
      break;
    }
    case my_mem_type::Word : {
      for (int i = 0; i < 4; ++i) {
        mp[pos + i] = std::byte(val & ((1<<8)-1));
        val >>= 8;
      }
      break;
    }
  }
}
uint32_t my_mem::load(uint32_t pos, my_mem_type t, bool sign){
  if (busy && pos == cur_pos) {
    std::cerr<<"Calling LOAD when the address is under modification.\n";
    throw std::runtime_error("Calling LOAD when the address is under modification.");
  }
  uint32_t val;
  switch (t) {
    case my_mem_type::Byte: {
      if (sign) {
//        val = static_cast<char>(mp[pos]); // NOLINT
//        std::cerr<<val<<' ';
        val= into_signed(uint32_t(mp[pos])); //any difference?
//        std::cerr<<val<<std::endl;
      } else {
        val = uint32_t(mp[pos]);
      }
      break;
    }
    case my_mem_type::Half: {
      uint16_t v= uint16_t(mp[pos+1]) << 8 | uint16_t(mp[pos]);
      if (sign) {
        val=into_signed(uint32_t(v));
      } else {
        val=v;
      }
      break;
    }
    case my_mem_type::Word: {
      val = 0;
      for (int i = 3; i >= 0; --i) {
        val <<= 8;
        val |= uint32_t(mp[pos + i]);
      }
      break;
    }
  }
  return val;
}