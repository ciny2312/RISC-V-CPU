#include <bits/stdc++.h>
#include "cpu.h"
#include "decoder/decoder.h"
using namespace std;
int main(){
    freopen("testcases/pi.data","r",stdin);
//    freopen("a.debug","w",stdout);
    read_file r;
    r.load_data();
    my_mem mp(r.get_data(),1);
    my_CPU cpu;
    cpu.init(&mp);
    while(true){
        cpu.execute();
        cpu.tick();
    }
    return 0;
}