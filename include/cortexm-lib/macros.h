#pragma once

#define PERHIPH(x,n) x ## n
#define PERHIPH2(x,n) PERHIPH(x,n)

#define IRQHANDLER(x,n) x ## n ## _IRQHandler
#define IRQHANDLER2(x,n) IRQHANDLER(x,n)

#define IRQn(x,n) x ## n ## _IRQn
#define IRQn2(x,n) IRQn(x,n)
