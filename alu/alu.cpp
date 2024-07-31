#include "alu.h"
#include <stdexcept>
void my_alu::tick() {
    cnt=0;
}
uint32_t my_alu::calculate(uint32_t a, uint32_t b, my_alu_type t) {
  if(cnt){
    throw std::runtime_error("2 ALU calculate in 1 tick");
  }
  cnt++;
  switch (t) {
    case my_alu_type::Add:
        return a + b;
    case my_alu_type::And:
        return a & b;
    case my_alu_type::Or:
        return a | b;
    case my_alu_type::Sub:
        return a - b;
    case my_alu_type::Xor:
        return a ^ b;
    case my_alu_type::ShiftL: {
        auto val = b & 0b11111;
        return a << val;
    }
    case my_alu_type::ShiftRArith: {
        auto val = b & 0b11111;
        a >>= val;
        return sign_extend(a, 32 - val);
    }
    case my_alu_type::ShiftRLogical: {
        auto val = b & 0b11111;
        a >>= val;
        return a;
    }
    case my_alu_type::UseLatter:
        return b;
    case my_alu_type::SetLTSigned: {
        int32_t aa = into_signed(a);
        int32_t bb = into_signed(b);
        return aa < bb;
    }
    case my_alu_type::SetLTUnsigned:
        return a < b;
    default: {
        throw std::runtime_error("Invalid ALU Type");
    }
  }
}