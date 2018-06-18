#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <utility>

#ifdef FMT_DEBUG
#   define __enable_irq()
#   define __disable_irq()
#else
//#   include <stm32f4xx.h>
#endif

#include "interrupts.h"


template <int Size, typename ElementType = char>
class RingBuffer {
public:
    RingBuffer()
    : head(0), tail(0), is_empty(true) {
    }

    bool put(ElementType e) {
        DisableInterrupts _di;  // TODO ?

        if (head == tail && !is_empty) {  // full
            return false;
        }

        is_empty = false;
        data[head] = std::move(e);
        head = (head + 1) % Size;

        return true;
    }

    bool get(ElementType& e) {
        if (is_empty) {
            return false;
        }

        e = std::move(data[tail]);
        tail = (tail + 1) % Size;

        if (head == tail) {
            is_empty = true;
        }

        return true;
    }

private:
    ElementType data[Size];
    volatile uint32_t head;  // TODO smaller data type?
    volatile uint32_t tail;
    volatile bool is_empty;
};

#endif // RING_BUFFER_H
