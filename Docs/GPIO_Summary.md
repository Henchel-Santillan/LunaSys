# GPIO Summary

## Preface

GPIO (General-Purpose Input-Output) are uncommitted software-controllable digital signal pins that can be configured as input, output, or both. By default, they have no pre-defined purpose and are unused.

On the STM32F4 board used in LunaSys, the GPIOs can be configured in five different modes:

1. Input floating
2. Analog
3. Output
4. Alternate function (AF)
5. External Interrupt (EXTI)

In both __Output__ and __Alternate Function__ modes, the IO is set to be either `Open Drain (OD)` or `Push-Pull (PP)`. 

### Open-Drain

In implementing an open-drain output, a single NMOS transistor has its drain terminal connected to an output pin (which connects to some device). When this terminal is open (the device is off), the pin will be left floating in a high-impedance state. In other words, the NMOS will pull the output pin to 0V (GND) when it is activated (the device is on), and leave it floating otherwise.

See Figure 1 for a diagram.

<p align="center">
    <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/gpio_opendrain.png">
</p>

<p align="center"><b>Figure 1: Open-Drain configuration</b></p>

Open-Drain is often used for two-way single line communication interfaces, like I2C for instance.

### Push-Pull

Push-pull is the more common GPIO mode, and is used to drive two output levels: "pulling" to __0V__, and "pushing" to power supply voltage __VDD__. "Pulling" to 0V or GND means sinking current from the load, whereas "pushing" to power supply voltage denotes sourcing current to the load. 

In a simplified but practical implementation of a push-pull output, the gates of both a __PMOS__ and an __NMOS__ transistor are connected to an internal signal line. An output pin forms a junction with the drain terminals of the MOSFETs. See Figure 2 for a diagram.

<p align="center">
    <img src="https://github.com/Henchel-Santillan/LunaSys/blob/master/Docs/Res/gpio_pushpull.png">
</p>

<p align="center"><b>Figure 2: MOSFET circuit illustrating the push-pull configuration</b></p>

When the internal signal is set to 0 (logic level LOW), the PMOS is activated and the NMOS becomes open. Current flows from VDD to the output pin through the PMOS. On the other hand, when the internal signal is set to 1 (logic level HIGH), the PMOS is now open while the NMOS is activated. This causes current to flow from the output pin through the NMOS to GND. The former is the "push" phase, since current is sourced to the load by "pushing" to VDD. The latter is the "pull" phase, since a current sink from the load took place by "pulling" to GND.

Push-Pull is suitable for communication interfaces that may use single-direction lines, such as UART and SPI.

<br />

## HAL

The GPIO driver relies on a structure called `GPIO_InitTypeDef`. This struct has 5 fields, summarized in Table 1.

<br />

__Table 1: Summary of Fields, `GPIO_Init_TypeDef`__

| Field | Description |
|:-----:|-------------|
| Pin   | The GPIO pins to be configured, of the form GPIO_PIN_x, where x lies between 0 and 15 inclusive |
| Mode  | The operating mode for the pins specified using the `Pin` field; see the __Configuration__ section above for more details |
| Pull  | The pull-up or pull-down activation for the selected pins, most relevant to pins configured as input floating |
| Speed | Controls the __slew rate__, or the rise and fall times (or the rate at which a signal changes between LOW and HIGH); applicable when the GPIO is in output mode |
| Alternate | Defines the peripheral to be connected to the selected pins, labelled as `GPIO_AFx_PPP`, where `AFx` is the _alternate function index_ and `PPP` is the _peripheral instance_ |

The STM32F4 HAL provides API functions for setting up, reading, writing, and toggling GPIO pins:

1. `HAL_GPIO_Init` / `HAL_GPIO_DeInit`;
2. `HAL_GPIO_ReadPin` / `HAL_GPIOWritePin`; and
3. `HAL_GPIO_TogglePin`.

Below are the general steps followed to configure GPIOs:

1. Enable GPIO AHB clock using `__HAL_RCC_GPIOx_CLK_ENABLE()`, where x is from A to H (for LQFP64 STM32F446RE, only A to D are applicable). 
2. Configure the IO mode, pull-up or pull-down resistor, speed (in the case of output of alternate function mode selection), the alternate member or field (in the case of alternate function mode selection), and the pin using `HAL_GPIO_Init()` and the `GPIO_InitTypeDef` struct. Note that if a pin is to be used as an ADC channel (as was the case for the potentiometer), __analog__ mode is required.

If the GPIO mode is chosen to be EXTI (External Interrupt), the edge sensitivity (falling, rising) must be specified. For STM32F4 series MCUs, there are a total of 16 EXTI lines connected internally to GPIOs (`EXTI0`, `EXTI1`, `EXTI2`, `EXTI3`, `EXTI4`, `EXTI9_5`, and `EXTI15_10`), according to Table 38 in [RM0390](https://www.st.com/resource/en/reference_manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf). Details on what the user application should do in the event external interrupt mode is selected is detailed in the [HAL documentation](https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf). 
