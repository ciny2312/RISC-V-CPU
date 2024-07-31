#include"cpu.h"
#include "decoder/decoder.h"
#include <cstdint>
using namespace std;
int main(){
    uint32_t n;
    cin>>n;
    union Word {
        int32_t sign;
        uint32_t unsign;
    };
    uint32_t x=n;cout<<x<<endl;
    auto v1=std::byte(x & ((1<<8)-1));
    auto v2=static_cast<std::byte>(x >> 8 & ((1<<8)-1));

    auto vv1=uint32_t(v1);
    auto vv2=static_cast<uint32_t>(v1);
    cout<<vv1<<endl<<vv2;
    Word lhs{};
    lhs.unsign = n;
//    cout<<into_signed(get_bits_signed(n,0,30))<<endl<<into_signed(n)<<endl<<lhs.sign;
    return 0;
}