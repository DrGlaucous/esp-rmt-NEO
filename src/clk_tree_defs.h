/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif



/*
*   some of the preprocessor defs that this library uses come out of the new (5.0.0) version of espidf.
*   Resultingly, some features are erroniously disabled.
*   To mitigate this, I'm putting those essential definitions here.
*   The definitions I'm including only cover the generic "esp32" class.
*   This may result in compatibility issues! (enable what you need)
*   To change what ESP this is compatable with, change the contents of ./include/driver/hal and use the defs from the proper board (get these from the espidf release)
*   framework-espidf\components\soc\esp32\include\soc\soc_caps.h
*/
//#define SOC_RMT_GROUPS                  1U /*!< One RMT group */
//#define SOC_RMT_TX_CANDIDATES_PER_GROUP 8  /*!< Number of channels that capable of Transmit in each group */
//#define SOC_RMT_RX_CANDIDATES_PER_GROUP 8  /*!< Number of channels that capable of Receive in each group */
//#define SOC_RMT_CHANNELS_PER_GROUP      8  /*!< Total 8 channels */
//#define SOC_RMT_MEM_WORDS_PER_CHANNEL   64 /*!< Each channel owns 64 words memory */
//#define SOC_RMT_SUPPORT_REF_TICK        1  /*!< Support set REF_TICK as the RMT clock source */
//#define SOC_RMT_CHANNEL_CLK_INDEPENDENT 1  /*!< Can select different source clock for each channel */

//not all these are supported by the bog standatd esp32
//#define CONFIG_PM_ENABLE 1
//#define SOC_RMT_SUPPORT_AHB 1
//#define SOC_RMT_SUPPORT_XTAL 1
//#define SOC_RMT_SUPPORT_AHB 1
//#define SOC_RMT_SUPPORT_RC_FAST 1

//turns out this is the only new def between 4.5 and 5.0
#define SOC_RMT_SUPPORT_APB             1  /*!< Support set APB as the RMT clock source */



/*
 ************************* ESP32 Root Clock Source ****************************
 * 1) Internal 8MHz RC Oscillator: RC_FAST (usually referred as FOSC or CK8M/CLK8M in TRM and reg. description)
 *
 *    This RC oscillator generates a ~8.5MHz clock signal output as the RC_FAST_CLK.
 *    The ~8.5MHz signal output is also passed into a configurable divider, which by default divides the input clock
 *    frequency by 256, to generate a RC_FAST_D256_CLK (usually referred as 8md256 or simply d256 in reg. description).
 *
 *    The exact frequency of RC_FAST_CLK can be computed in runtime through calibration on the RC_FAST_D256_CLK.
 *
 * 2) External 2~40MHz Crystal Clock: XTAL
 *
 * 3) Internal 150kHz RC Oscillator: RC_SLOW (usually referrred as RTC in TRM or reg. description)
 *
 *    This RC oscillator generates a ~150kHz clock signal output as the RC_SLOW_CLK. The exact frequency of this clock
 *    can be computed in runtime through calibration.
 *
 * 4) External 32kHz Crystal Clock (optional): XTAL32K
 *
 *    The clock source for this XTAL32K_CLK can be either a 32kHz crystal connecting to the 32K_XP and 32K_XN pins
 *    or a 32kHz clock signal generated by an external circuit. The external signal must be connected to the 32K_XN pin.
 *    Additionally, a 1nF capacitor must be placed between the 32K_XP pin and ground. In this case, the 32K_XP pin
 *    cannot be used as a GPIO pin.
 *
 *    XTAL32K_CLK can also be calibrated to get its exact frequency.
 */

/* With the default value of CK8M_DFREQ = 172, RC_FAST clock frequency is 8.5 MHz +/- 7% */
#define SOC_CLK_RC_FAST_FREQ_APPROX         8500000                             /*!< Approximate RC_FAST_CLK frequency in Hz */
/* With the default value of DCAP, range is +/- 5% */
#define SOC_CLK_RC_SLOW_FREQ_APPROX         150000                              /*!< Approximate RC_SLOW_CLK frequency in Hz */
#define SOC_CLK_RC_FAST_D256_FREQ_APPROX    (SOC_CLK_RC_FAST_FREQ_APPROX / 256) /*!< Approximate RC_FAST_D256_CLK frequency in Hz */
#define SOC_CLK_XTAL32K_FREQ_APPROX         32768                               /*!< Approximate XTAL32K_CLK frequency in Hz */

// Naming convention: SOC_ROOT_CLK_{loc}_{type}_[attr]
// {loc}: EXT, INT
// {type}: XTAL, RC
// [attr] - optional: [frequency], FAST, SLOW
/**
 * @brief Root clock
 */
typedef enum {
    SOC_ROOT_CLK_INT_RC_FAST,          /*!< Internal 8MHz RC oscillator */
    SOC_ROOT_CLK_INT_RC_SLOW,          /*!< Internal 150kHz RC oscillator */
    SOC_ROOT_CLK_EXT_XTAL,             /*!< External 2~40MHz crystal */
    SOC_ROOT_CLK_EXT_XTAL32K,          /*!< External 32kHz crystal/clock signal */
} soc_root_clk_t;

/**
 * @brief CPU_CLK mux inputs, which are the supported clock sources for the CPU_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_CPU_CLK_SRC_XTAL = 0,              /*!< Select XTAL_CLK as CPU_CLK source */
    SOC_CPU_CLK_SRC_PLL = 1,               /*!< Select PLL_CLK as CPU_CLK source (PLL_CLK is the output of 40MHz crystal oscillator frequency multiplier, can be 480MHz or 320MHz) */
    SOC_CPU_CLK_SRC_RC_FAST = 2,           /*!< Select RC_FAST_CLK as CPU_CLK source */
    SOC_CPU_CLK_SRC_APLL = 3,              /*!< Select APLL_CLK as CPU_CLK source */
    SOC_CPU_CLK_SRC_INVALID,               /*!< Invalid CPU_CLK source */
} soc_cpu_clk_src_t;

/**
 * @brief RTC_SLOW_CLK mux inputs, which are the supported clock sources for the RTC_SLOW_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_RTC_SLOW_CLK_SRC_RC_SLOW = 0,      /*!< Select RC_SLOW_CLK as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_XTAL32K = 1,      /*!< Select XTAL32K_CLK as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_RC_FAST_D256 = 2, /*!< Select RC_FAST_D256_CLK (referred as FOSC_DIV or 8m_d256/8md256 in TRM and reg. description) as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_INVALID,          /*!< Invalid RTC_SLOW_CLK source */
} soc_rtc_slow_clk_src_t;

/**
 * @brief RTC_FAST_CLK mux inputs, which are the supported clock sources for the RTC_FAST_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_RTC_FAST_CLK_SRC_XTAL_D4 = 0,      /*!< Select XTAL_D4_CLK (may referred as XTAL_CLK_DIV_4) as RTC_FAST_CLK source */
    SOC_RTC_FAST_CLK_SRC_XTAL_DIV = SOC_RTC_FAST_CLK_SRC_XTAL_D4, /*!< Alias name for `SOC_RTC_FAST_CLK_SRC_XTAL_D4` */
    SOC_RTC_FAST_CLK_SRC_RC_FAST = 1,      /*!< Select RC_FAST_CLK as RTC_FAST_CLK source */
    SOC_RTC_FAST_CLK_SRC_INVALID,          /*!< Invalid RTC_FAST_CLK source */
} soc_rtc_fast_clk_src_t;

// Naming convention: SOC_MOD_CLK_{[upstream]clock_name}_[attr]
// {[upstream]clock_name}: APB, APLL, (BB)PLL, etc.
// [attr] - optional: FAST, SLOW, D<divider>, F<freq>
/**
 * @brief Supported clock sources for modules (CPU, peripherals, RTC, etc.)
 *
 * @note enum starts from 1, to save 0 for special purpose
 */
typedef enum {
    // For CPU domain
    SOC_MOD_CLK_CPU = 1,                       /*!< CPU_CLK can be sourced from XTAL, PLL, RC_FAST, or APLL by configuring soc_cpu_clk_src_t */
    // For RTC domain
    SOC_MOD_CLK_RTC_FAST,                      /*!< RTC_FAST_CLK can be sourced from XTAL_D4 or RC_FAST by configuring soc_rtc_fast_clk_src_t */
    SOC_MOD_CLK_RTC_SLOW,                      /*!< RTC_SLOW_CLK can be sourced from RC_SLOW, XTAL32K, or RC_FAST_D256 by configuring soc_rtc_slow_clk_src_t */
    // For digital domain: peripherals, WIFI, BLE
    SOC_MOD_CLK_APB,                           /*!< APB_CLK is highly dependent on the CPU_CLK source */
    SOC_MOD_CLK_PLL_D2,                        /*!< PLL_D2_CLK is derived from PLL, it has a fixed divider of 2 */
    SOC_MOD_CLK_PLL_F160M,                     /*!< PLL_F160M_CLK is derived from PLL, and has a fixed frequency of 160MHz */
    SOC_MOD_CLK_XTAL32K,                       /*!< XTAL32K_CLK comes from the external 32kHz crystal, passing a clock gating to the peripherals */
    SOC_MOD_CLK_RC_FAST,                       /*!< RC_FAST_CLK comes from the internal 8MHz rc oscillator, passing a clock gating to the peripherals */
    SOC_MOD_CLK_RC_FAST_D256,                  /*!< RC_FAST_D256_CLK comes from the internal 8MHz rc oscillator, divided by 256, and passing a clock gating to the peripherals */
    SOC_MOD_CLK_XTAL,                          /*!< XTAL_CLK comes from the external crystal (2~40MHz) */
    SOC_MOD_CLK_REF_TICK,                      /*!< REF_TICK is derived from APB, it has a fixed frequency of 1MHz even when APB frequency changes */
    SOC_MOD_CLK_APLL,                          /*!< APLL is sourced from PLL, and its frequency is configurable through APLL configuration registers */
} soc_module_clk_t;


//////////////////////////////////////////////////GPTimer///////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of GPTimer
 *
 * The following code can be used to iterate all possible clocks:
 * @code{c}
 * soc_periph_gptimer_clk_src_t gptimer_clks[] = (soc_periph_gptimer_clk_src_t)SOC_GPTIMER_CLKS;
 * for (size_t i = 0; i< sizeof(gptimer_clks) / sizeof(gptimer_clks[0]); i++) {
 *     soc_periph_gptimer_clk_src_t clk = gptimer_clks[i];
 *     // Test GPTimer with the clock `clk`
 * }
 * @endcode
 */
#define SOC_GPTIMER_CLKS {SOC_MOD_CLK_APB}

/**
 * @brief Type of GPTimer clock source
 */
typedef enum {
    GPTIMER_CLK_SRC_APB = SOC_MOD_CLK_APB,     /*!< Select APB as the source clock */
    GPTIMER_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB, /*!< Select APB as the default choice */
} soc_periph_gptimer_clk_src_t;

/**
 * @brief Type of Timer Group clock source, reserved for the legacy timer group driver
 */
typedef enum {
    TIMER_SRC_CLK_APB = SOC_MOD_CLK_APB,     /*!< Timer group source clock is APB */
    TIMER_SRC_CLK_DEFAULT = SOC_MOD_CLK_APB, /*!< Timer group source clock default choice is APB */
} soc_periph_tg_clk_src_legacy_t;

//////////////////////////////////////////////////LCD///////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of LCD
 */
#define SOC_LCD_CLKS {SOC_MOD_CLK_PLL_D2, SOC_MOD_CLK_APLL, SOC_MOD_CLK_XTAL}

/**
 * @brief Type of LCD clock source
 */
typedef enum {
    LCD_CLK_SRC_PLL160M = SOC_MOD_CLK_PLL_D2, /*!< Select PLL_D2 (default to 160MHz) as the source clock */
    LCD_CLK_SRC_APLL = SOC_MOD_CLK_APLL,      /*!< Select APLL as the source clock */
    LCD_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,      /*!< Select XTAL as the source clock */
    LCD_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_D2, /*!< Select PLL_D2 (default to 160MHz) as the default choice */
} soc_periph_lcd_clk_src_t;

//////////////////////////////////////////////////RMT///////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of RMT
 */
#define SOC_RMT_CLKS {SOC_MOD_CLK_APB, SOC_MOD_CLK_REF_TICK}

/**
 * @brief Type of RMT clock source
 */
typedef enum {
    RMT_CLK_SRC_APB = SOC_MOD_CLK_APB,           /*!< Select APB as the source clock */
    RMT_CLK_SRC_REF_TICK = SOC_MOD_CLK_REF_TICK, /*!< Select REF_TICK as the source clock */
    RMT_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB,       /*!< Select APB as the default choice */
} soc_periph_rmt_clk_src_t;

//ALTER
/**
 * @brief Type of RMT clock source, reserved for the legacy RMT driver. We don't need this because we're still using the legacy backend
 */
//typedef enum {
//    RMT_BASECLK_APB = SOC_MOD_CLK_APB,      /*!< RMT source clock is APB CLK */
//    RMT_BASECLK_REF = SOC_MOD_CLK_REF_TICK, /*!< RMT source clock is REF_TICK */
//    RMT_BASECLK_DEFAULT = SOC_MOD_CLK_APB,  /*!< RMT source clock default choice is APB */
//} soc_periph_rmt_clk_src_legacy_t;

///////////////////////////////////////////////////UART/////////////////////////////////////////////////////////////////

/**
 * @brief Type of UART clock source, reserved for the legacy UART driver
 */
typedef enum {
    UART_SCLK_APB = SOC_MOD_CLK_APB,           /*!< UART source clock is APB CLK */
    UART_SCLK_REF_TICK = SOC_MOD_CLK_REF_TICK, /*!< UART source clock is REF_TICK */
    UART_SCLK_DEFAULT = SOC_MOD_CLK_APB,       /*!< UART source clock default choice is APB */
} soc_periph_uart_clk_src_legacy_t;

//////////////////////////////////////////////////MCPWM/////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of MCPWM Timer
 */
#define SOC_MCPWM_TIMER_CLKS {SOC_MOD_CLK_PLL_F160M}

/**
 * @brief Type of MCPWM timer clock source
 */
typedef enum {
    MCPWM_TIMER_CLK_SRC_PLL160M = SOC_MOD_CLK_PLL_F160M, /*!< Select PLL_F160M as the source clock */
    MCPWM_TIMER_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_F160M, /*!< Select PLL_F160M as the default clock choice */
} soc_periph_mcpwm_timer_clk_src_t;

/**
 * @brief Array initializer for all supported clock sources of MCPWM Capture Timer
 */
#define SOC_MCPWM_CAPTURE_CLKS {SOC_MOD_CLK_APB}

/**
 * @brief Type of MCPWM capture clock source
 */
typedef enum {
    MCPWM_CAPTURE_CLK_SRC_APB = SOC_MOD_CLK_APB,     /*!< Select APB as the source clock */
    MCPWM_CAPTURE_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB, /*!< SElect APB as the default clock choice */
} soc_periph_mcpwm_capture_clk_src_t;

///////////////////////////////////////////////////I2S//////////////////////////////////////////////////////////////////
/**
 * @brief Array initializer for all supported clock sources of I2S
 */
#define SOC_I2S_CLKS {SOC_MOD_CLK_PLL_D2, SOC_MOD_CLK_APLL}

/**
 * @brief I2S clock source enum
 *
 */
typedef enum {
    I2S_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_D2,                   /*!< Select PLL_D2 as the default source clock  */
    I2S_CLK_SRC_PLL_160M = SOC_MOD_CLK_PLL_D2,                  /*!< Select PLL_D2 as the source clock */
    I2S_CLK_SRC_APLL = SOC_MOD_CLK_APLL,                        /*!< Select APLL as the source clock */
} soc_periph_i2s_clk_src_t;

/////////////////////////////////////////////////I2C////////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of I2C
 */
#define SOC_I2C_CLKS {SOC_MOD_CLK_APB}

/**
 * @brief Type of I2C clock source.
 */
typedef enum {
    I2C_CLK_SRC_APB = SOC_MOD_CLK_APB,
    I2C_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB,
} soc_periph_i2c_clk_src_t;

//////////////////////////////////////////////////SDM//////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of SDM
 */
#define SOC_SDM_CLKS {SOC_MOD_CLK_APB}

/**
 * @brief Sigma Delta Modulator clock source
 */
typedef enum {
    SDM_CLK_SRC_APB = SOC_MOD_CLK_APB,     /*!< Select APB as the source clock */
    SDM_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB, /*!< Select APB as the default clock choice */
} soc_periph_sdm_clk_src_t;

#ifdef __cplusplus
}
#endif
