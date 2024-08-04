#include "lsb.h"
#include <stdexcept>

uint32_t ask_i(ResultType a,ResultType b,const lsb_point &x){
    if(!x.dependi) return x.Ri;
    if(a.flag&&x.Qi==a.id) return a.value;
    if(b.flag&&x.Qi==b.id) return b.value;
    std::cerr<<"invaid call to ask_i in lsb\n";
    throw std::runtime_error("invaid call to ask_i in lsb");
}
uint32_t ask_j(ResultType a,ResultType b,const lsb_point &x){
    if(!x.dependj) return x.Rj;
    if(a.flag&&x.Qj==a.id) return a.value;
    if(b.flag&&x.Qj==b.id) return b.value;
    std::cerr<<"invaid call to ask_j in lsb\n";
    throw std::runtime_error("invaid call to ask_j in lsb");
}
void my_lsb::execute(ResultType rs, to_lsb rob) {
	if (rob.need_clear) {
		clear();
		return;
	}
	auto cr = cache->get_result();
	update_depend(cr, rs, rob);

	if (cache->free()) {
		int index = get_next(cr, rs, rob);
		if (index != -1) {
			list_nxt[index].busy = false;
			if (list_nxt.head == index) list_nxt.pop();
			auto const &x = list[index];
			uint32_t RItrue = ask_i(cr,rs,x), RJtrue = ask_j(cr,rs,x);
			cache->set(RItrue + x.offset, RJtrue, x.id, x.mem_type,x.imm_type,x.op_type);
		}
	}
}

void my_lsb::add(lsb_point x) {
	if (list.full()){
        std::cerr<<"lsb is full.\n";
        throw std::runtime_error("lsb is full.");
    }
    x.busy=true;
	list_nxt.push(x);
}

bool my_lsb::full() {
	return list.full();
}

int my_lsb::get_next(ResultType lsb, ResultType rs, to_lsb rob) {
	if (list.empty()) return -1;
	int i = list.head;
	while (true) {
		if (list[i].busy &&
			(!list[i].dependi || (lsb.flag && list[i].Qi == lsb.id) || (rs.flag && list[i].Qi == rs.id)) &&
			(!list[i].dependj || (lsb.flag && list[i].Qj == lsb.id) || (rs.flag && list[i].Qj == rs.id)) &&
			(!list[i].wait || list[i].id == rob.id))
			return i;
		if (list[i].busy && (list[i].imm_type == my_imm_type::S&&list[i].mem_type!=my_mem_type::Hang))
			break;
		i = (i + 1) % list.get_size();
		if (i == list.tail) break;
	}
	return -1;
}

void my_lsb::update_depend(ResultType cr, ResultType rs, to_lsb rob) {
	for (int i = 0; i < list.get_size(); ++i) {
		if (!list[i].busy) continue;
		if (cr.flag && list[i].dependi && list[i].Qi == cr.id) {
			list_nxt[i].dependi = false;
			list_nxt[i].Qi = 0;
			list_nxt[i].Ri = cr.value;
		}
		if (cr.flag && list[i].dependj && list[i].Qj == cr.id) {
			list_nxt[i].dependj = false;
			list_nxt[i].Qj = 0;
			list_nxt[i].Rj = cr.value;
		}
		if (rs.flag && list[i].dependi && list[i].Qi == rs.id) {
			list_nxt[i].dependi = false;
			list_nxt[i].Qi = 0;
			list_nxt[i].Ri = rs.value;
		}
		if (rs.flag && list[i].dependj && list[i].Qj == rs.id) {
			list_nxt[i].dependj = false;
			list_nxt[i].Qj = 0;
			list_nxt[i].Rj = rs.value;
		}
		if (rob.flag && list[i].wait && list[i].id == rob.id)
			list_nxt[i].wait = false;
	}
}
