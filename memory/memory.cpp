#include "memory.h"
#include <cstdint>
void my_mem::store(uint32_t pos, uint32_t val, my_mem_type t){
//  std::cout<<"store ——————> pos: "<<pos<<' '<<"val: "<<val<<std::endl;
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
int cnt_2=0;
uint32_t my_mem::load(uint32_t pos, my_mem_type t){
  uint32_t val;
  switch (t) {
    case my_mem_type::Byte: {
		  if (mp.count(pos) == 0) return 0;
      val = uint32_t(mp[pos]);
      break;
    }
    case my_mem_type::ByteSign: {
		  if (mp.count(pos) == 0) return 0;
      val= into_signed(uint32_t(mp[pos]));
      break;
    }
    case my_mem_type::Half: {
		  if (mp.count(pos) == 0||mp.count(pos+1) == 0) return 0;
      uint16_t v= uint16_t(mp[pos+1]) << 8 | uint16_t(mp[pos]);
      val=v;
      break;
    }
    case my_mem_type::HalfSign:{
		  if (mp.count(pos) == 0||mp.count(pos+1) == 0) return 0;
      uint16_t v= uint16_t(mp[pos+1]) << 8 | uint16_t(mp[pos]);
      val=into_signed(uint32_t(v));
      break;

    }
    case my_mem_type::Word: {
      val = 0;
    //  if(++cnt_2<=50)
    //    std::cerr<<uint32_t(mp[4])<<' '<<uint32_t(mp[5])<<std::endl;
      for (int i = 3; i >= 0; --i) {
		    if (mp.count(pos+i) == 0) return 0;
        val <<= 8;
        val |= uint32_t(mp[pos + i]);
      }
      break;
    }
  }
//  std::cout<<"load ——————> pos: "<<pos<<' '<<"val: "<<val<<std::endl;
  return val;
}