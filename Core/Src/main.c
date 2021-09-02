/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ping.h"
#include "socket.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wizchip_conf.h"
#define W5500_CS_Pin GPIO_PIN_12
#define W5500_CS_GPIO_Port GPIOB

#define sock 	1
#define port 	1500

uint8_t pDestaddr[4] = {192,168,1,172};

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void W5500_Select(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
	HAL_StatusTypeDef ret = HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}


uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}


void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}


void setwiz() {
	/*initialisation*/

    //specify functions pointers
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

    //Buffer initialisation
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    //***********************set MAC & IP address**********************//
    wiz_NetInfo net_info = {
        .mac  = { 0x00, 0x08, 0xDC, 0x63, 0xAB, 0x92 },
		.ip	  = { 192,168,1,5 },
		//dns  = {192,168,1.1},  ///< DNS server IP Address
		.sn   = {255, 255, 255, 0},
        .dhcp = NETINFO_STATIC	//static ip
    };
    // wizchip_getnetinfo(&net_info);
    wizchip_setnetinfo(&net_info);

    //reset PHY
    ctlwizchip_type PHY_ctr = CW_SET_PHYCONF;
    wiz_PhyConf PHY_conf;
    PHY_conf.by = PHY_CONFBY_SW;   ///< set by @ref PHY_CONFBY_HW or @ref PHY_CONFBY_SW
    PHY_conf.mode =  PHY_MODE_MANUAL;    ///< set by @ref PHY_MODE_MANUAL or @ref PHY_MODE_AUTONEGO
	PHY_conf.speed =   PHY_SPEED_100;  ///< set by @ref PHY_SPEED_10 or @ref PHY_SPEED_100
	PHY_conf.duplex =   PHY_DUPLEX_FULL;  ///< set by @ref PHY_DUPLEX_HALF @ref PHY_DUPLEX_FULL

    ctlwizchip(PHY_ctr, &PHY_conf);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  setwiz();
  /* USER CODE END 2 */

  int8_t ret;

  reestablish:
  //create socket
    while( (ret = wiz_socket(sock, Sn_MR_TCP, port, 0 )) != sock){
    	HAL_Delay(100);
    }
    //Listen mode

    while(1){
    	ret = wiz_listen(sock);
		if (ret == SOCKERR_SOCKCLOSED) {
			goto reestablish;
		} else if (ret == SOCK_OK) {
			break;
		}
		HAL_Delay(100);

    }

    while(1)
    {
  	  if(ret = getSn_SR(sock) == SOCK_ESTABLISHED)
  	  {
  		  uint8_t buf[8] = {0};

  		  while(ret = wiz_recv( sock, buf, sizeof(buf)) == 0);
  		  //something is received
  		  while(ret = wiz_send(sock, buf, sizeof(buf)) == 0);//echo

  		  if(buf[0]=='c' && buf[1] == 'l' && buf[2] == 's')
  		  {
  			  while(wiz_close(sock)!=SOCK_OK);
  			  goto reestablish;
  		  }

  		  else if(buf[0] == 'p' && buf[1] =='i' && buf[2] == 'n' && buf[3] =='g')
  		  {
  			  int8_t tmp = 0;
  			  tmp = ping_auto(0,pDestaddr);
  			      //tmp = ping_count(0,3,pDestaddr);
  			      if(tmp == SUCCESS)
  			      {
  			    	 uint8_t reply[8] = {'S','U','C','C','E','S','\n'};
  			    	 while(ret = wiz_send(sock,reply, sizeof(reply)) == 0);
  			      }
  			      else
  			      {
  			    	  uint8_t reply[8] = {'F','A','I','L','E','D','\n'};
  			    	  while(ret = wiz_send(sock, reply, sizeof(reply)) == 0);
  			      }

  		  }


  		  //something is sent
  	  }


    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	  /** Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /** Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{
	__HAL_RCC_SPI2_CLK_ENABLE();

	  /* SPI2 parameter configuration*/
	  hspi2.Instance = SPI2;
	  hspi2.Init.Mode = SPI_MODE_MASTER;
	  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	  hspi2.Init.NSS = SPI_NSS_SOFT;
	  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;//initially is 32
	  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	  //hspi2.Init.CRCPolynomial = 10;
	  if (HAL_SPI_Init(&hspi2) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : W5500_CS_Pin */
    GPIO_InitStruct.Pin = W5500_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(W5500_CS_GPIO_Port, &GPIO_InitStruct);


}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
