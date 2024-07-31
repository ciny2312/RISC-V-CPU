#include"reg.h"
uint32_t my_reg::load(uint32_t pos){
    if(pos>=32){
        std::cerr<<"extend load bound"<<std::endl;
        throw std::runtime_error("Invalid register");
    }
    if(cnt_load==2){
        std::cerr<<"3 load in 1 tick"<<std::endl;
        throw std::runtime_error("Invalid load");
    }
    cnt_load++;
    return reg[pos];
}
void my_reg::store(uint32_t pos, uint32_t val){
    if(pos>=32){
        std::cerr<<"extend store bound"<<std::endl;
        throw std::runtime_error("Invalid register");
    }
    if(cnt_store==1){
        std::cerr<<"2 store in 1 tick"<<std::endl;
        throw std::runtime_error("Invalid store");
    }
    cnt_store++;
    if(pos) reg[pos]=val;
}
void my_reg::tick(){
    cnt_load=cnt_store=0;
}