#include <bits/stdc++.h>
#include "cpu.h"
#include "decoder/decoder.h"
using namespace std;
int main(){
//    freopen("testcases/expr.data","r",stdin);
    read_file r;
    r.load_data();
    my_CPU cpu(r.get_data());
    while(true){
        cpu.execute();
        cpu.tick();
    }
    return 0;
}