/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
typedef unsigned char byte;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
int digit = 0;
int current_level = 0;
int destination_level = 0;
int max_level = 5;
int warn_buzzer = 0;
int last_time_btn_1 = 0, last_time_btn_2 = 0, last_time_btn_3 = 0, last_time_btn_4 = 0;
int button4_down = 0;
uint8_t going_up = 1;		// when elevator going up
int animation_up = 0;
int going_down = 0;		// when elevator going down
int stopped = 0;		// when elevator is stopped
int levels_queue[] = {};
int elavator_started = 0;
int tim4_count = 0;
int tim4_lcd_counter = 0;


extern TIM_HandleTypeDef htim3;
TIM_HandleTypeDef *pwm_timer = &htim3;	// Point to PWM Timer configured in CubeMX
uint32_t pwm_channel = TIM_CHANNEL_1;   // Select configured PWM channel number

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 50;          // (0 - 1000)
volatile uint32_t last_button_press;

const Tone greensleeves[] = {

		{NOTE_G4, 386},
		{NOTE_AS4, 771},
		{NOTE_C5, 386},
		{NOTE_D5, 580},
		{NOTE_DS5, 193},
		{NOTE_D5,386 },
		{NOTE_C5, 771},
		{NOTE_A4, 386},
		{NOTE_F4, 580},
		{NOTE_G4, 193},
		{NOTE_A4, 386},
		{NOTE_AS4, 771},
		{NOTE_G4, 386},
		{NOTE_G4, 580},
		{NOTE_FS4, 193},
		{NOTE_G4, 386},
		{NOTE_A4, 771},
		{NOTE_FS4, 386},
		{NOTE_D4, 771},
		{NOTE_G4, 386},
		{NOTE_AS4, 771},
		{NOTE_C5, 386},
		{NOTE_D5, 580},
		{NOTE_DS5, 193},
		{NOTE_D5, 386},
		{NOTE_C5, 771},
		{NOTE_A4, 386},
		{NOTE_F4, 580},
		{NOTE_G4, 193},
		{NOTE_A4, 386},
		{NOTE_AS4, 580},
		{NOTE_A4, 193},
		{NOTE_G4, 386},
		{NOTE_FS4, 580},
		{NOTE_E4, 193},
		{NOTE_FS4, 386},
		{NOTE_G4, 2316},
		{NOTE_F5, 2316},
		{NOTE_E5, 193},
		{NOTE_D5, 386},
		{NOTE_C5, 771},
		{NOTE_A4, 386},
		{NOTE_F4, 580},
		{NOTE_G4, 193},
		{NOTE_A4, 386},
		{NOTE_AS4, 771},
		{NOTE_G4, 386},
		{NOTE_G4, 580},
		{NOTE_FS4, 193},
		{NOTE_G4, 386},
		{NOTE_A4, 771},
		{NOTE_FS4, 386},
		{NOTE_D4, 1041},
		{NOTE_F5, 1544},
		{NOTE_E5, 193},
		{NOTE_D5, 386},
		{NOTE_C5, 771},
		{NOTE_A4, 386},
		{NOTE_F4, 580},
		{NOTE_G4, 193},
		{NOTE_A4, 386},
		{NOTE_AS4, 580},
		{NOTE_A4, 193},
		{NOTE_G4, 386},
		{NOTE_FS4, 580},
		{NOTE_E4, 193},
		{NOTE_FS4, 386},
		{NOTE_G4, 2316}

//		  //repeats from the beginning
//
//		  NOTE_G4,8,//1
//		  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,
//		  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
//		  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
//		  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,4, NOTE_G4,8,
//
//		  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,//6
//		  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
//		  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8,
//		  NOTE_G4,-2,
//		  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,
//
//		  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,//11
//		  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
//		  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,04,
//		  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,
//		  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
//
//		  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8,//16
//		  NOTE_G4,-2

};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void decodeNumber(int i) {
	if (i >= 10) i = 0;

	int x1 = i & 1;
	int x2 = i & 2;
	int x3 = i & 4;
	int x4 = i & 8;

	if (x1 > 0) x1 = 1;
	if (x2 > 0) x2 = 1;
	if (x3 > 0) x3 = 1;
	if (x4 > 0) x4 = 1;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, x1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, x2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, x3);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, x4);
}

void setSeg0On() {	// ---X
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
}

void setSeg1On() {	// X---
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
}

void PWM_Start()
{
    HAL_TIM_PWM_Start(pwm_timer, pwm_channel);
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
    if (pwm_freq == 0 || pwm_freq > 20000)
    {
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 0);
    }
    else
    {
        const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
        const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
        const uint32_t timer_clock = internal_clock_freq / prescaler;
        const uint32_t period_cycles = timer_clock / pwm_freq;
        const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

        pwm_timer->Instance->PSC = prescaler - 1;
        pwm_timer->Instance->ARR = period_cycles - 1;
        pwm_timer->Instance->EGR = TIM_EGR_UG;
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, pulse_width); // pwm_timer->Instance->CCR2 = pulse_width;
    }
}

void Change_Melody(const Tone *melody, uint16_t tone_count)
{
    melody_ptr = melody;
    melody_tone_count = tone_count;
    current_tone_number = 0;
}

void Update_Melody()
{
    if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count))
    {
        const Tone active_tone = *(melody_ptr + current_tone_number);
        PWM_Change_Tone(active_tone.frequency, volume);
        current_tone_end = HAL_GetTick() + active_tone.duration;
        current_tone_number++;
    }
}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */
  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  Update_Melody();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)) {
  	  if(HAL_GetTick() - last_time_btn_1 > 300) {
  		  if(destination_level > 0)
  		  		  destination_level -= 1;
  		  last_time_btn_1 = HAL_GetTick();
  	  }
    }

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)) {
  	  if(HAL_GetTick() - last_time_btn_2 > 300) {
  		  if(destination_level < max_level)
  			  destination_level += 1;
  		  last_time_btn_2 = HAL_GetTick();
  	  }
    }

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8)) {
  	  if(HAL_GetTick() - last_time_btn_3 > 300) {

  //		  for starting music
  //		  PWM_Start();
  //		  Change_Melody(greensleeves, ARRAY_LENGTH(greensleeves));

  		  int duplicate = 0;
  		  for(int i = 0; i < ARRAY_LENGTH(levels_queue); i++) {
  			  if(levels_queue[i] == destination_level) {
  				  duplicate = 1;
  				  break;
  			  }
  		  }

  		  if(!duplicate && destination_level != current_level)
  			  levels_queue[ARRAY_LENGTH(levels_queue)] = destination_level;


  		  last_time_btn_3 = HAL_GetTick();
  	  }
    }

    if (button4_down) button4_down = 0;

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)) {
  	  button4_down = 1;
    }

  //  if (button4_down) button4_down = 0;

    if(button4_down == 1) {
    	Change_Melody(NULL, NULL);
    	HAL_TIM_PWM_Stop(pwm_timer, pwm_channel);
    	pwm_timer->Instance->PSC = 0;
    	pwm_timer->Instance->ARR = 9999;
    	warn_buzzer = 1;
//    			  HAL_TIM_Base_Start_IT(&htim4);
    	__HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 2500);
  //  			  button4_down = 0;
    } else {
    	warn_buzzer = 0;
    	__HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 0);
    }
  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
  if(digit == 0) {
  	  setSeg0On();
  	  decodeNumber(destination_level);
  	  digit++;
    } else {
  	  setSeg1On();
  	  decodeNumber(ARRAY_LENGTH(levels_queue));
  	  digit = 0;
    }
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */
  if (warn_buzzer) {
  	  if(tim4_count <= 5) {
  		  PWM_Start();
  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 50);
  		  tim4_count += 1;
  	  } else {
//  	  HAL_TIM_PWM_Stop(pwm_timer, pwm_channel);
  //	  tim4_count = 0;
  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
  		  tim4_count++;
  		  if (tim4_count == 11)
  			  tim4_count = 1;
  	  }
  }

  tim4_lcd_counter++;

  if (tim4_lcd_counter == 10) {
  	  if (going_up) {
  		  home();
  		  print("We are Going Up ");
  		  setCursor(col, row)
  		  write(animation_up++);
  		  if (animation_up == 5)
  			  animation_up = 0;
  	  }
  	  tim4_lcd_counter = 0;
  }

  /* USER CODE END TIM4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
