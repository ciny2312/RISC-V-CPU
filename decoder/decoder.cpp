#include "decoder.h"
#include <stdexcept>
void my_decoder::tick() { cnt = 0; }

my_imm_type my_decoder::get_imm() const { return imm; }

my_op_type my_decoder::get_op() const { return op; }

my_alu_type my_decoder::get_alu() const { return alu; }

my_branch_type my_decoder::get_branch() const { return branch; }

void my_decoder::decode(uint32_t ins) {
  instruction = ins;

  auto opcode = get_bits(instruction, 0, 6);
  if (opcode & 0b10000) {
    op = my_op_type::Arith;
  } else if (opcode == 0b0000011) {
    op = my_op_type::MemRead;
  } else if (opcode == 0b0100011) {
    op = my_op_type::MemWrite;
  } else {
    op = my_op_type::Branch;
  }

  opcode = get_bits(instruction, 0, 6);
  if (opcode == 0b0110011) {
    imm = my_imm_type::R;
  } else if (opcode == 0b0010011 || opcode == 0b1100111) {
    auto funct3 = get_bits(instruction, 12, 14);
    if (funct3 == 0b001 || funct3 == 0b101) {
      imm = my_imm_type::Ishift;
    } else {
      imm = my_imm_type::I;
    }
  } else if (opcode == 0b0000011) {
    imm = my_imm_type::I;
  } else if (opcode == 0b0100011) {
    imm = my_imm_type::S;
  } else if (opcode == 0b1100011) {
    imm = my_imm_type::B;
  } else if (opcode == 0b1101111) {
    imm = my_imm_type::J;
  } else if (opcode == 0b0010111 || opcode == 0b0110111) {
    imm = my_imm_type::U;
  } else {
    throw std::runtime_error("Invalid opcode when decode");
  }

  opcode = get_bits(instruction, 0, 6);
  if (imm == my_imm_type::J || imm == my_imm_type::B || imm == my_imm_type::S) {
    alu = my_alu_type::Add;
  } else if (imm == my_imm_type::U) {
    if (opcode == 0b0010111) {
      alu = my_alu_type::Add;
    } else {
      alu = my_alu_type::UseLatter;
    }
  } else if (imm == my_imm_type::R) {
    auto funct3 = get_bits(instruction, 12, 14);
    auto funct7 = get_bits(instruction, 27, 31);
    if (funct3 == 0b000) {
      alu = funct7 ? my_alu_type::Sub : my_alu_type::Add;
    } else if (funct3 == 0b001) {
      alu = my_alu_type::ShiftL;
    } else if (funct3 == 0b010) {
      alu = my_alu_type::SetLTSigned;
    } else if (funct3 == 0b011) {
      alu = my_alu_type::SetLTUnsigned;
    } else if (funct3 == 0b100) {
      alu = my_alu_type::Xor;
    } else if (funct3 == 0b101) {
      alu = funct7 ? my_alu_type::ShiftRArith : my_alu_type::ShiftRLogical;
    } else if (funct3 == 0b110) {
      alu = my_alu_type::Or;
    } else if (funct3 == 0b111) {
      alu = my_alu_type::And;
    } else {
      throw std::runtime_error("Invalid R Type funct3 when decode");
    }
  } else if (opcode == 0b0000011) {
    alu = my_alu_type::Add;
  } else {
    auto funct3 = get_bits(instruction, 12, 14);
    auto funct7 = get_bits(instruction, 27, 31);
    if (funct3 == 0b000) {
      alu = my_alu_type::Add;
    } else if (funct3 == 0b001) {
      alu = my_alu_type::ShiftL;
    } else if (funct3 == 0b010) {
      alu = my_alu_type::SetLTSigned;
    } else if (funct3 == 0b011) {
      alu = my_alu_type::SetLTUnsigned;
    } else if (funct3 == 0b100) {
      alu = my_alu_type::Xor;
    } else if (funct3 == 0b101) {
      alu = funct7 ? my_alu_type::ShiftRArith : my_alu_type::ShiftRLogical;
    } else if (funct3 == 0b110) {
      alu = my_alu_type::Or;
    } else if (funct3 == 0b111) {
      alu = my_alu_type::And;
    } else {
      throw std::runtime_error("Invalid I Type funct3 ");
    }
  }

  if (imm != my_imm_type::B) {
    branch = my_branch_type::Eq;
  } else {
    auto funct3 = get_bits(instruction, 12, 14);
    if(funct3==0b000) branch=my_branch_type::Eq;
    else if(funct3==0b101) branch=my_branch_type::Ge;
    else if(funct3==0b111) branch=my_branch_type::GeU;
    else if(funct3==0b100) branch=my_branch_type::Lt;
    else if(funct3==0b110) branch=my_branch_type::LtU;
    else if(funct3==0b001) branch=my_branch_type::Ne;
    else{
      std::cerr<<funct3<<"No match branch type\n";
      throw std::runtime_error("No match branch type");
    }
  }
}


std::byte DecodeByte(const std::string &data) {
  std::size_t index;
  auto value = std::stoul(data, &index, 16);
  if (index != 2) {
    std::cerr << "Error occurred when transforming  " << data << " to byte" << std::endl;
    throw std::runtime_error("Invalid input");
   }
  return static_cast<std::byte>(value);
}

uint32_t DecodeAddress(const std::string &data) {
  std::size_t index;
  auto value = std::stoul(data, &index, 16);
  if (index != 8) {
    std::cerr << "Error occurred when transforming  " << data << " to address" << std::endl;
    throw std::runtime_error("Invalid input");
  }
  return static_cast<uint32_t>(value);
}


const std::map<uint32_t, std::byte> &read_file::get_data() const {
  return mp;
}


void read_file::load_data() {
  uint32_t pos=0;
  while (!std::cin.eof()) {
    std::string s;
    std::cin >> s;
    if (s.empty()) {
      continue;
    }
    if (s[0] == '@') {
      pos = DecodeAddress(s.substr(1));
    } else {
      auto val = DecodeByte(s);
      mp[pos++] = val;
    }
  }
}