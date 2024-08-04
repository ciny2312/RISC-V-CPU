#include "func.h"
#include <cstdint>
int cnt = 0;
void my_func::execute(my_instruction &iu, my_rob &rob, my_rs &rs, my_lsb &lsb,
                      my_reg &regs) {
//  if (++cnt <= 50)
//    std::cout << "PC: " << iu.PC  <<" r10: "<<regs.R[10]<<' '<<" r11: "<<regs.R[11]<< std::endl;
  set_PC_nxt = false;
  newPC_nxt = 0;
  if (rob.need_clear) {
    flag_nxt = true;
    return;
  }
  if (!iu.flag)
    return;
  flag_nxt = true;
  set_PC_nxt = false;
  add_rob = false;
  add_lsb = false;
  add_rs = false;

  if (iu.instruction == 0x0ff00513) {
    if (rob.full()) {
      flag_nxt = false;
      return;
    }
    toRob = {my_rob_type::EXIT, 0, 0, iu.instrAddr, 0, true};
    add_rob = true;
    flag_nxt = false;

    updata_depend(rob, rs, lsb, regs);
    if (add_rob)
      rob.add(toRob, regs);
    if (add_rs)
      rs.add(toRS);
    if (add_lsb)
      lsb.add(toLSB);
    if (!flag_nxt) {
      iu.stall_by_decoder();
    }
    return ;
  }

  dec.decode(iu.instruction);
  auto alu_type = dec.get_alu();
  auto imm_type = dec.get_imm();
  auto op_type = dec.get_op();
  auto rs1 = get_bits(iu.instruction, 15, 19);
  auto rs2 = get_bits(iu.instruction, 20, 24);
  auto rd = get_bits(iu.instruction, 7, 11);
  if (imm_type == my_imm_type::R) {
    if (rob.full() || rs.full()) {
      flag_nxt = false;
    } else {
      toRob = {my_rob_type::R, rd, 0, iu.instrAddr, 0, false};
      toRS = {alu_type,         regs.R[rs1],      regs.R[rs2],
              regs.Q[rs1],      regs.Q[rs2],      rob.add_index(),
              regs.depend[rs1], regs.depend[rs2], false};
      add_rob = true;
      add_rs = true;
//      std::cerr<<"R\n";
    }
  } else if (imm_type == my_imm_type::I || imm_type == my_imm_type::Ishift) {
    auto imm_num = imm.get_imm(iu.instruction, imm_type, alu_type);
    if (op_type == my_op_type::Arith) {
      if (rob.full() || rs.full()) {
        flag_nxt = false;
      } else {
        toRob = {my_rob_type::R, rd, 0, iu.instrAddr, 0, false};
        toRS = {alu_type, regs.R[rs1],     imm_num,          regs.Q[rs1],
                0,        rob.add_index(), regs.depend[rs1], false};
        add_rob = true;
        add_rs = true;
      }
  //    std::cerr<<"I ARITH\n";

    } else if (op_type == my_op_type::MemRead) {
      uint32_t sign_ext = get_bits(iu.instruction, 14, 14);
      uint32_t width = get_bits(iu.instruction, 12, 13);
      if (rob.full() || lsb.full()) {
        flag_nxt = false;
      } else {
        toRob = rob_point{my_rob_type::R, rd, 0, iu.instrAddr, 0, false, false};
        if (width == 0) {
          toLSB =
              lsb_point{sign_ext ? my_mem_type::ByteSign : my_mem_type::Byte,
                        imm_type,
                        op_type,
                        regs.R[rs1],
                        0,
                        regs.Q[rs1],
                        0,
                        imm_num,
                        rob.add_index(),
                        regs.depend[rs1],
                        false,
                        false,
                        false};
  //    std::cerr<<"I LOAD BYTE\n";
        } else if (width == 1) {
          toLSB =
              lsb_point{sign_ext ? my_mem_type::HalfSign : my_mem_type::Half,
                        imm_type,
                        op_type,
                        regs.R[rs1],
                        0,
                        regs.Q[rs1],
                        0,
                        imm_num,
                        rob.add_index(),
                        regs.depend[rs1],
                        false,
                        false,
                        false};
  //    std::cerr<<"I LOAD HALF\n";
        } else {
          toLSB =
              lsb_point{my_mem_type::Word,
                        imm_type,
                        op_type,
                        regs.R[rs1],
                        0,
                        regs.Q[rs1],
                        0,
                        imm_num,
                        rob.add_index(),
                        regs.depend[rs1],
                        false,
                        false,
                        false};
  //    std::cerr<<"I LOAD WORD\n";
        }
        add_rob = true;
        add_lsb = true;
      }
    } else if (op_type == my_op_type::Branch) {
      if (rob.full() || regs.depend[rs1]) {
        flag_nxt = false;
      } else {
        toRob = {my_rob_type::R, rd, iu.PC, iu.instrAddr, 0, true};
        add_rob = true;
        set_PC_nxt = true;
        newPC_nxt = (regs.R[rs1] + imm_num) & 0xfffffffe;
      }
  //    std::cerr<<"I Branch\n";
    } else {
      std::cerr << "Invalid operation in I Type\n";
      throw std::runtime_error("Invalid operation in I Type");
    }
  } else if (imm_type == my_imm_type::S) {
    if (rob.full() || lsb.full()) {
      flag_nxt = false;
    } else {

      auto imm_num = imm.get_imm(iu.instruction, imm_type, alu_type);
      toRob = {my_rob_type::S, rd, 0, iu.instrAddr, 0, false};

      auto width = get_bits(iu.instruction, 12, 14);
      if (width == 0) {
        toLSB = lsb_point{my_mem_type::Byte, imm_type,         op_type,
                          regs.R[rs1],       regs.R[rs2],      regs.Q[rs1],
                          regs.Q[rs2],       imm_num,          rob.add_index(),
                          regs.depend[rs1],  regs.depend[rs2], true};
  //    std::cerr<<"S BYTE\n";
      } else if (width == 1) {
        toLSB = lsb_point{my_mem_type::Half, imm_type,         op_type,
                          regs.R[rs1],       regs.R[rs2],      regs.Q[rs1],
                          regs.Q[rs2],       imm_num,          rob.add_index(),
                          regs.depend[rs1],  regs.depend[rs2], true};
  //    std::cerr<<"S HALF\n";
      } else {
        toLSB = lsb_point{my_mem_type::Word, imm_type,         op_type,
                          regs.R[rs1],       regs.R[rs2],      regs.Q[rs1],
                          regs.Q[rs2],       imm_num,          rob.add_index(),
                          regs.depend[rs1],  regs.depend[rs2], true};
  //    std::cerr<<"S WORD\n";
      }
      add_rob = true;
      add_lsb = true;
    }
  } else if (imm_type == my_imm_type::B) {
    if (rob.full() || rs.full()) {
      flag_nxt = false;
    } else {
      auto imm_num = imm.get_imm(iu.instruction, imm_type, alu_type);
      auto branch = dec.get_branch();
      bool jump = pre->get_prediction(iu.instrAddr);
      uint32_t predict = jump ? iu.instrAddr + imm_num : iu.instrAddr + 4;
      uint32_t other = jump ? iu.instrAddr + 4 : iu.instrAddr + imm_num;
      toRob = {my_rob_type::B, 0, 0, iu.instrAddr, other | jump, false};
      toRS = {branch,      regs.R[rs1],     regs.R[rs2],      regs.Q[rs1],
              regs.Q[rs2], rob.add_index(), regs.depend[rs1], regs.depend[rs2]};
      add_rob = true;
      add_rs = true;
      set_PC_nxt = true;
      newPC_nxt = predict;
  //    std::cerr<<"B\n";
    }

  } else if (imm_type == my_imm_type::U) {
    if (rob.full()) {
      flag_nxt = false;
    } else {

      auto imm_num = imm.get_imm(iu.instruction, imm_type, alu_type);
      if (alu_type == my_alu_type::UseLatter)
        toRob = {my_rob_type::R, rd, imm_num, iu.instrAddr, 0, true};
      else if (alu_type == my_alu_type::Add)
        toRob = {my_rob_type::R, rd, iu.instrAddr + imm_num,
                 iu.instrAddr,   0,  true};
      add_rob = true;
  //    std::cerr<<"U\n";
    }

  } else if (imm_type == my_imm_type::J) {
    if (rob.full()) {
      flag_nxt = false;
    } else {
      auto imm_num = imm.get_imm(iu.instruction, imm_type, alu_type);
      toRob = {my_rob_type::R, rd, iu.PC, iu.instrAddr, 0, true};
      add_rob = true;
      set_PC_nxt = true;
      newPC_nxt = iu.instrAddr + imm_num;
  //    std::cerr<<"J\n";
    }
  }
  updata_depend(rob, rs, lsb, regs);
  if (add_rob)
    rob.add(toRob, regs);
  if (add_rs)
    rs.add(toRS);
  if (add_lsb)
    lsb.add(toLSB);
  if (!flag_nxt) {
    iu.stall_by_decoder();
  }
}

void updata(ResultType &a, ResultType &b, my_rob &rob, uint32_t &R, uint32_t &Q,
            bool &flag) {
  if (!flag)
    return;
  if (rob.list[Q].flag) {
    R = rob.list[Q].value;
    flag = false;
  } else if (a.flag && a.id == Q) {
    R = a.value;
    flag = false;
  } else if (b.flag && b.id == Q) {
    R = b.value;
    flag = false;
  }
}
void my_func::updata_depend(my_rob &rob, my_rs &rs, my_lsb &lsb, my_reg &regs) {
  auto a = rs.export_data(), b = lsb.export_data();
  if (add_rs) {
    updata(a, b, rob, toRS.Ri, toRS.Qi, toRS.dependi);
    updata(a, b, rob, toRS.Rj, toRS.Qj, toRS.dependj);
  }
  if (add_lsb) {
    updata(a, b, rob, toLSB.Ri, toLSB.Qi, toLSB.dependi);
    updata(a, b, rob, toLSB.Rj, toLSB.Qj, toLSB.dependj);
  }
}