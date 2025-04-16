#include <sodium.h>

int tets(void){
    if(sodium_init()<0){
        printf("not working fella");
    }
    return 0;
}
