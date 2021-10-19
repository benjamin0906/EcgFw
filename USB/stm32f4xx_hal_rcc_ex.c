/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extension RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extension peripheral:
  *           + Extended Peripheral Control functions
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCCEx HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Constants
  * @{
  */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  *  @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions
 *  @brief  Extended Peripheral Control functions
 *
@verbatim
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.
    [..]
    (@) Important note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in
        order to modify the RTC Clock source, as consequence RTC registers (including
        the backup registers) and RCC_BDCR register are set to their reset values.

@endverbatim
  * @{
  */

#if defined(STM32F446xx)
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, LTDC RTC and TIM).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval HAL status
  */


/**
  * @brief  Get the RCC_PeriphCLKInitTypeDef according to the internal
  *         RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tempreg;

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_I2S_APB2 |\
                                        RCC_PERIPHCLK_SAI1     | RCC_PERIPHCLK_SAI2     |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_CEC      | RCC_PERIPHCLK_FMPI2C1  |\
                                        RCC_PERIPHCLK_CLK48     | RCC_PERIPHCLK_SDIO     |\
                                        RCC_PERIPHCLK_SPDIFRX;

  /* Get the PLLI2S Clock configuration --------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SM = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM) >> RCC_PLLI2SCFGR_PLLI2SM_Pos);
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);
  PeriphClkInit->PLLI2S.PLLI2SP = (uint32_t)((((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos) + 1U) << 1U);
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);
  /* Get the PLLSAI Clock configuration --------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIM = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM) >> RCC_PLLSAICFGR_PLLSAIM_Pos);
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);
  PeriphClkInit->PLLSAI.PLLSAIP = (uint32_t)((((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos) + 1U) << 1U);
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);
  /* Get the PLLSAI/PLLI2S division factors ----------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) >> RCC_DCKCFGR_PLLI2SDIVQ_Pos);
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> RCC_DCKCFGR_PLLSAIDIVQ_Pos);

  /* Get the SAI1 clock configuration ----------------------------------------*/
  PeriphClkInit->Sai1ClockSelection = __HAL_RCC_GET_SAI1_SOURCE();

  /* Get the SAI2 clock configuration ----------------------------------------*/
  PeriphClkInit->Sai2ClockSelection = __HAL_RCC_GET_SAI2_SOURCE();

  /* Get the I2S APB1 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb1ClockSelection = __HAL_RCC_GET_I2S_APB1_SOURCE();

  /* Get the I2S APB2 clock configuration ------------------------------------*/
  PeriphClkInit->I2sApb2ClockSelection = __HAL_RCC_GET_I2S_APB2_SOURCE();

  /* Get the RTC Clock configuration -----------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));

  /* Get the CEC clock configuration -----------------------------------------*/
  PeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();

  /* Get the FMPI2C1 clock configuration -------------------------------------*/
  PeriphClkInit->Fmpi2c1ClockSelection = __HAL_RCC_GET_FMPI2C1_SOURCE();

  /* Get the CLK48 clock configuration ----------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();

  /* Get the SDIO clock configuration ----------------------------------------*/
  PeriphClkInit->SdioClockSelection = __HAL_RCC_GET_SDIO_SOURCE();

  /* Get the SPDIFRX clock configuration -------------------------------------*/
  PeriphClkInit->SpdifClockSelection = __HAL_RCC_GET_SPDIFRX_SOURCE();

  /* Get the TIM Prescaler configuration -------------------------------------*/
  if ((RCC->DCKCFGR & RCC_DCKCFGR_TIMPRE) == RESET)
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;
  }
  else
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  }
}

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_SAI1: SAI1 peripheral clock
  *            @arg RCC_PERIPHCLK_SAI2: SAI2 peripheral clock
  *            @arg RCC_PERIPHCLK_I2S_APB1: I2S APB1 peripheral clock
  *            @arg RCC_PERIPHCLK_I2S_APB2: I2S APB2 peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t tmpreg1 = 0U;
  /* This variable used to store the SAI clock frequency (value in Hz) */
  uint32_t frequency = 0U;
  /* This variable used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0U;
  /* This variable used to store the SAI clock source */
  uint32_t saiclocksource = 0U;
  uint32_t srcclk = 0U;
  /* This variable used to store the VCO Output (value in Hz) */
  uint32_t vcooutput = 0U;
  switch (PeriphClk)
  {
  case RCC_PERIPHCLK_SAI1:
  case RCC_PERIPHCLK_SAI2:
    {
      saiclocksource = RCC->DCKCFGR;
      saiclocksource &= (RCC_DCKCFGR_SAI1SRC | RCC_DCKCFGR_SAI2SRC);
      switch (saiclocksource)
      {
      case 0U: /* PLLSAI is the clock source for SAI*/
        {
          /* Configure the PLLSAI division factor */
          /* PLLSAI_VCO Input  = PLL_SOURCE/PLLSAIM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
          {
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM));
          }
          else
          {
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIM)));
          }
          /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
          /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
          tmpreg1 = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24U;
          frequency = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6U))/(tmpreg1);

          /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
          tmpreg1 = (((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> 8U) + 1U);
          frequency = frequency/(tmpreg1);
          break;
        }
      case RCC_DCKCFGR_SAI1SRC_0: /* PLLI2S is the clock source for SAI*/
      case RCC_DCKCFGR_SAI2SRC_0: /* PLLI2S is the clock source for SAI*/
        {
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
          {
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));
          }
          else
          {
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM)));
          }

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
          tmpreg1 = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> 24U;
          frequency = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U))/(tmpreg1);

          /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
          tmpreg1 = ((RCC->DCKCFGR & RCC_DCKCFGR_PLLI2SDIVQ) + 1U);
          frequency = frequency/(tmpreg1);
          break;
        }
      case RCC_DCKCFGR_SAI1SRC_1: /* PLLR is the clock source for SAI*/
      case RCC_DCKCFGR_SAI2SRC_1: /* PLLR is the clock source for SAI*/
        {
          /* Configure the PLLI2S division factor */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
          {
            /* In Case the PLL Source is HSI (Internal Clock) */
            vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
          }
          else
          {
            /* In Case the PLL Source is HSE (External Clock) */
            vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
          }

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          /* SAI_CLK_x = PLL_VCO Output/PLLR */
          tmpreg1 = (RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U;
          frequency = (vcoinput * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U))/(tmpreg1);
          break;
        }
      case RCC_DCKCFGR_SAI1SRC: /* External clock is the clock source for SAI*/
        {
          frequency = EXTERNAL_CLOCK_VALUE;
          break;
        }
      case RCC_DCKCFGR_SAI2SRC: /* PLLSRC(HSE or HSI) is the clock source for SAI*/
        {
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
          {
            /* In Case the PLL Source is HSI (Internal Clock) */
            frequency = (uint32_t)(HSI_VALUE);
          }
          else
          {
            /* In Case the PLL Source is HSE (External Clock) */
            frequency = (uint32_t)(HSE_VALUE);
          }
          break;
        }
      default :
        {
          break;
        }
      }
      break;
    }
  case RCC_PERIPHCLK_I2S_APB1:
    {
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB1_SOURCE();
      switch (srcclk)
      {
      /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_EXT:
        {
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;
          break;
        }
      /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLI2S:
        {
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));
          }
          else
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));
          }

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));
          break;
        }
      /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB1CLKSOURCE_PLLR:
        {
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
          }
          else
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
          }

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));
          break;
        }
      /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB1CLKSOURCE_PLLSRC:
        {
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            frequency = HSE_VALUE;
          }
          else
          {
            frequency = HSI_VALUE;
          }
          break;
        }
        /* Clock not enabled for I2S*/
      default:
        {
          frequency = 0U;
          break;
        }
      }
      break;
    }
  case RCC_PERIPHCLK_I2S_APB2:
    {
      /* Get the current I2S source */
      srcclk = __HAL_RCC_GET_I2S_APB2_SOURCE();
      switch (srcclk)
      {
        /* Check if I2S clock selection is External clock mapped on the I2S_CKIN pin used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_EXT:
        {
          /* Set the I2S clock to the external clock  value */
          frequency = EXTERNAL_CLOCK_VALUE;
          break;
        }
        /* Check if I2S clock selection is PLLI2S VCO output clock divided by PLLI2SR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLI2S:
        {
          /* Configure the PLLI2S division factor */
          /* PLLI2S_VCO Input  = PLL_SOURCE/PLLI2SM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));
          }
          else
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SM));
          }

          /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6U) & (RCC_PLLI2SCFGR_PLLI2SN >> 6U)));
          /* I2S_CLK = PLLI2S_VCO Output/PLLI2SR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 28U) & (RCC_PLLI2SCFGR_PLLI2SR >> 28U)));
          break;
        }
        /* Check if I2S clock selection is PLL VCO Output divided by PLLR used as I2S clock */
      case RCC_I2SAPB2CLKSOURCE_PLLR:
        {
          /* Configure the PLL division factor R */
          /* PLL_VCO Input  = PLL_SOURCE/PLLM */
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
          }
          else
          {
            /* Get the I2S source clock value */
            vcoinput = (uint32_t)(HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
          }

          /* PLL_VCO Output = PLL_VCO Input * PLLN */
          vcooutput = (uint32_t)(vcoinput * (((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6U) & (RCC_PLLCFGR_PLLN >> 6U)));
          /* I2S_CLK = PLL_VCO Output/PLLR */
          frequency = (uint32_t)(vcooutput /(((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28U) & (RCC_PLLCFGR_PLLR >> 28U)));
          break;
        }
        /* Check if I2S clock selection is HSI or HSE depending from PLL source Clock */
      case RCC_I2SAPB2CLKSOURCE_PLLSRC:
        {
          if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSE)
          {
            frequency = HSE_VALUE;
          }
          else
          {
            frequency = HSI_VALUE;
          }
          break;
        }
        /* Clock not enabled for I2S*/
      default:
        {
          frequency = 0U;
          break;
        }
      }
      break;
    }
  }
  return frequency;
}
#endif /* STM32F446xx */

#if defined(STM32F469xx) || defined(STM32F479xx)

#endif /* STM32F469xx || STM32F479xx */









#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)
/**
  * @brief  Select LSE mode
  *
  * @note   This mode is only available for STM32F410xx/STM32F411xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx  devices.
  *
  * @param  Mode specifies the LSE mode.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSE_LOWPOWER_MODE:  LSE oscillator in low power mode selection
  *            @arg RCC_LSE_HIGHDRIVE_MODE: LSE oscillator in High Drive mode selection
  * @retval None
  */
void HAL_RCCEx_SelectLSEMode(uint8_t Mode)
{
  /* Check the parameters */
  assert_param(IS_RCC_LSE_MODE(Mode));
  if(Mode == RCC_LSE_HIGHDRIVE_MODE)
  {
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);
  }
  else
  {
    CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);
  }
}

#endif /* STM32F410xx || STM32F411xE || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
 *  @brief  Extended Clock management functions
 *
@verbatim   
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the 
    activation or deactivation of PLLI2S, PLLSAI.
@endverbatim
  * @{
  */

#if defined(RCC_PLLI2S_SUPPORT)
/**
  * @brief  Enable PLLI2S.
  * @param  PLLI2SInit  pointer to an RCC_PLLI2SInitTypeDef structure that
  *         contains the configuration information for the PLLI2S
  * @retval HAL status
  */


/**
  * @brief  Disable PLLI2S.
  * @retval HAL status
  */


#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_PLLSAI_SUPPORT)
/**
  * @brief  Enable PLLSAI.
  * @param  PLLSAIInit  pointer to an RCC_PLLSAIInitTypeDef structure that
  *         contains the configuration information for the PLLSAI
  * @retval HAL status
  */


/**
  * @brief  Disable PLLSAI.
  * @retval HAL status
  */


#endif /* RCC_PLLSAI_SUPPORT */

/**
  * @}
  */

#if defined(STM32F446xx)
/**
  * @brief  Returns the SYSCLK frequency
  *
  * @note   This function implementation is valid only for STM32F446xx devices.
  * @note   This function add the PLL/PLLR System clock source
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note     If SYSCLK source is PLL or PLLR, function returns values based on HSE_VALUE(**)
  *           or HSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note     (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *               16 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *
  * @note   This function can be used by the user application to compute the
  *         baudrate for the communication peripherals or configure other parameters.
  *
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  *
  * @retval SYSCLK frequency
  */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
  uint32_t pllm = 0U;
  uint32_t pllvco = 0U;
  uint32_t pllp = 0U;
  uint32_t pllr = 0U;
  uint32_t sysclockfreq = 0U;

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
    {
      sysclockfreq = HSI_VALUE;
       break;
    }
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
    {
      sysclockfreq = HSE_VALUE;
      break;
    }
    case RCC_CFGR_SWS_PLL:  /* PLL/PLLP used as system clock  source */
    {
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
      SYSCLK = PLL_VCO / PLLP */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      if(__HAL_RCC_GET_PLL_OSCSOURCE() != RCC_PLLSOURCE_HSI)
      {
        /* HSE used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSE_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSI_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      pllp = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U) *2U);

      sysclockfreq = pllvco/pllp;
      break;
    }
    case RCC_CFGR_SWS_PLLR:  /* PLL/PLLR used as system clock  source */
    {
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
      SYSCLK = PLL_VCO / PLLR */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      if(__HAL_RCC_GET_PLL_OSCSOURCE() != RCC_PLLSOURCE_HSI)
      {
        /* HSE used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSE_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSI_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      pllr = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos);

      sysclockfreq = pllvco/pllr;
      break;
    }
    default:
    {
      sysclockfreq = HSI_VALUE;
      break;
    }
  }
  return sysclockfreq;
}
#endif /* STM32F446xx */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, PLL, PLLI2S and PLLSAI OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL status
  */


#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) ||\
    defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) || defined(STM32F413xx) || defined(STM32F423xx)
/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this API. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this API. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @note   This function add the PLL/PLLR factor management during PLL configuration this feature
  *         is only available in STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx devices
  * @retval HAL status
  */


/**
  * @brief  Configures the RCC_OscInitStruct according to the internal
  * RCC configuration registers.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that will be configured.
  *
  * @note   This function is only available in case of STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx/STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx devices.
  * @note   This function add the PLL/PLLR factor management
  * @retval None
  */
void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  /* Set all possible values for the Oscillator type parameter ---------------*/
  RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;

  /* Get the HSE configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSEBYP) == RCC_CR_HSEBYP)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_BYPASS;
  }
  else if((RCC->CR &RCC_CR_HSEON) == RCC_CR_HSEON)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_ON;
  }
  else
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_OFF;
  }

  /* Get the HSI configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSION) == RCC_CR_HSION)
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_ON;
  }
  else
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_OFF;
  }

  RCC_OscInitStruct->HSICalibrationValue = (uint32_t)((RCC->CR &RCC_CR_HSITRIM) >> RCC_CR_HSITRIM_Pos);

  /* Get the LSE configuration -----------------------------------------------*/
  if((RCC->BDCR &RCC_BDCR_LSEBYP) == RCC_BDCR_LSEBYP)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_BYPASS;
  }
  else if((RCC->BDCR &RCC_BDCR_LSEON) == RCC_BDCR_LSEON)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_ON;
  }
  else
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_OFF;
  }

  /* Get the LSI configuration -----------------------------------------------*/
  if((RCC->CSR &RCC_CSR_LSION) == RCC_CSR_LSION)
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_ON;
  }
  else
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_OFF;
  }

  /* Get the PLL configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_PLLON) == RCC_CR_PLLON)
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;
  }
  else
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_OFF;
  }
  RCC_OscInitStruct->PLL.PLLSource = (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
  RCC_OscInitStruct->PLL.PLLM = (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM);
  RCC_OscInitStruct->PLL.PLLN = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);
  RCC_OscInitStruct->PLL.PLLP = (uint32_t)((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) + RCC_PLLCFGR_PLLP_0) << 1U) >> RCC_PLLCFGR_PLLP_Pos);
  RCC_OscInitStruct->PLL.PLLQ = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos);
  RCC_OscInitStruct->PLL.PLLR = (uint32_t)((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos);
}
#endif /* STM32F410xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
