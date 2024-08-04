#pragma once
#include <cstdint>

enum class my_imm_type{
    R,I,Ishift,S,B,U,J
};
enum class my_alu_type{
    Add,And,Or,Sub,Xor,ShiftL,ShiftRArith,
    ShiftRLogical,UseLatter,SetLTSigned,SetLTUnsigned,
    Eq,Ne,GeU,LtU,Ge,Lt,
    Hang
};
enum my_op_type{
    Arith,MemRead,MemWrite,Branch
};
enum my_pre_type { AC1,AC2,WA1,WA2 };

enum my_mem_type{
    Byte,Half,Word,HalfSign,ByteSign,Hang
};

struct ResultType {
  bool flag;
  uint32_t id;
  uint32_t value;
};

struct to_lsb {
  bool need_clear,flag;
  unsigned id;
};

enum class my_rob_type { 
    R,S,B,EXIT
};