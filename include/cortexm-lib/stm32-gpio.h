#ifndef GPIO_H
#define GPIO_H

//STM32F030x6
//STM32F030x8
//STM32F031x6
//STM32F038xx
//STM32F042x6
//STM32F048xx
//STM32F051x8
//STM32F058xx
//STM32F070x6
//STM32F070xB
//STM32F071xB
//STM32F072xB
//STM32F078xx
//STM32F091xC
//STM32F098xx
//STM32F030xC

//STM32F405xx
//STM32F415xx
//STM32F407xx
//STM32F417xx
//STM32F427xx
//STM32F437xx
//STM32F429xx
//STM32F439xx
//STM32F401xC
//STM32F401xE
//STM32F410Tx
//STM32F410Cx
//STM32F410Rx
//STM32F411xE
//STM32F446xx
//STM32F469xx
//STM32F479xx
//STM32F412Cx
//STM32F412Zx
//STM32F412Rx
//STM32F412Vx
//STM32F413xx
//STM32F423xx

#if defined(STM32F030x6) || defined(STM32F030x8) || defined(STM32F051x8) || defined(STM32F072xB)
#include "stm32f0xx.h"
#elif defined (STM32L052xx)
#include "stm32l0xx.h"
#elif defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F407xx) || defined(STM32F411xE) || defined(STM32F429xx)
#include "stm32f4xx.h"
#elif defined(STM32F746xx)
#include "stm32f7xx.h"
#else
#error Unsupported device
#endif

#include <cstdint>


class Pin {
public:
    enum class Mode {  // GPIOx->MODER
        Input = 0x0,
        Output = 0x1,
        AF = 0x2,
        Analog = 0x3
    };

    enum class OutputType {  // GPIOx->OTYPER
        PushPull = 0,
        OpenDrain = 1,
    };

    enum class Speed {  // GPIOx->OSPEEDR
        Low = 0,
        Medium = 1,
#if defined(STM32F030x6) || defined(STM32F072xB)
        High = 3
#else
        High = 2,
        VeryHigh = 3
#endif
    };

    enum class Pull {  // GPIOx->PUPDR
        PullNone = 0,
        PullUp = 1,
        PullDown = 2
    };

    constexpr Pin()
        : g(nullptr), n(0)
    { }

    constexpr Pin(GPIO_TypeDef* const gpio, int pin)
        : g(gpio), n(pin)
    { }

    Pin& set_mode(Mode m) {
        uint32_t moder = g->MODER;

        moder &= ~(3 << (n * 2));
        moder |=  static_cast<int>(m) << (n * 2);

        g->MODER = moder;

        return *this;
    }

    Pin& set_type(OutputType t) {
        uint32_t otyper = g->OTYPER;

        otyper &= ~(1 << n);
        otyper |= static_cast<int>(t) << n;

        g->OTYPER = otyper;

        return *this;
    }

    Pin& set_pull(Pull p) {
        uint32_t pupdr = g->PUPDR;

        pupdr &= ~(3 << (n * 2));
        pupdr |= static_cast<int>(p) << (n * 2);

        g->PUPDR = pupdr;

        return *this;
    }

    /**
      Set alternate function
      set_mode(Mode::AF)
      See table 12 in RM0360
    */
    Pin& set_af(uint8_t af) {
        uint32_t afr = g->AFR[n < 8 ? 0 : 1];
        uint32_t n_ = n < 8 ? n : n - 8;

        afr &= ~(0xf << (n_ * 4));
        afr |=  af << (n_ * 4);

        g->AFR[n < 8 ? 0 : 1] = afr;

        return *this;
    }

    Pin& set_speed(Speed s) {
        uint32_t ospeedr = g->OSPEEDR;

        ospeedr &= ~(3 << (n * 2));
        ospeedr |=  static_cast<int>(s) << (n * 2);

        g->OSPEEDR = ospeedr;

        return *this;
    }

    Pin& set(bool val) {
        if (val) {
            set_high();
        } else {
            set_low();
        }

        return *this;
    }

    Pin& set_high() {
        g->BSRR = 1 << n;

        return *this;
    }

    Pin& set_low() {
        // only some families have BRR
        g->BSRR = 1 << (n + 16);

        return *this;
    }

    Pin& toggle() {
        g->ODR ^= 1 << n;

        return *this;
    }

    bool value() {
        return g->IDR & (1 << n) ? true : false;
    }

    // TODO configuration lock (see RM0090 section 8.4.8)

private:
    GPIO_TypeDef * const g;
    int n;
};

#endif // GPIO_H
