#include <stddef.h>
#include <util/delay.h>
#include "hal/hal.h"

#include "comm.h"
#include "protocol.h"

// __attribute__((OS_main)) tells the compiler that this function never
// returns and saves us precious space
__attribute__((OS_main)) int main(void) {

    hal_init(NULL);


    // TODO sleep
    while(1) ;

}
