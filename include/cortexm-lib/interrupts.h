#ifndef INTERRUPTS_H
#define INTERRUPTS_H

//#include <stm32f4xx.h>


/**
  RAII interrupt disabler
*/
class DisableInterrupts {
public:

    DisableInterrupts() {
        __disable_irq();
    }

    ~DisableInterrupts() {
        __enable_irq();
    }
};

#endif // INTERRUPTS_H
