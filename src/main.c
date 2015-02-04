#include "hal/hal.h"

// __attribute__((OS_main)) tells the compiler that this function never returns and
// saves us precious space
__attribute__((OS_main)) int main(void) {

	while(1);

}