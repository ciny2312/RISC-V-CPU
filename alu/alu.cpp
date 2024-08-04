#include "alu.h"
#include <cstdint>
#include <stdexcept>
void my_alu::tick() {
  a = a_nxt;
  b = b_nxt;
  t = t_nxt;
  id = id_nxt;
  flag = flag_nxt;
  flag_nxt = false;
}
void my_alu::updata_value(my_alu_type _t, uint32_t _a, uint32_t _b,
                          uint32_t _id) {
  a_nxt = _a;
  b_nxt = _b;
  t_nxt = _t;
  id_nxt = _id;
  flag_nxt = _t != my_alu_type::Hang;
}
uint32_t my_alu::calculate() {
  if(!flag) return 0;
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
    return sign_extend(a>>val, 32 - val);
  }
  case my_alu_type::ShiftRLogical: {
    auto val = b & 0b11111;
    return a>>val;
  }
  case my_alu_type::UseLatter:
    return b;
  case my_alu_type::SetLTSigned: {
    return into_signed(a) < into_signed(b);
  }
  case my_alu_type::SetLTUnsigned:
    return a < b;
  case my_alu_type::Eq:
    return a == b;
  case my_alu_type::Ge:
    return into_signed(a) >= into_signed(b);
  case my_alu_type::GeU:
    return a >= b;
  case my_alu_type::Lt:
    return into_signed(a) < into_signed(b);
  case my_alu_type::LtU:
    return a < b;
  case my_alu_type::Ne:
    return a != b;
  default: {
    return 0;
  }
  }
}