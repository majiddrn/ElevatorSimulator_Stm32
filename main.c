/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef unsigned char byte;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

///////////////////Arrows Up \\\\\\\\\\\\\\\\\\\\\

byte arrow_up_0[] = {
  0x00,
  0x04,
  0x0E,
  0x15,
  0x04,
  0x04,
  0x04,
  0x00
};

byte arrow_up_1[] = {
  0x0E,
  0x15,
  0x04,
  0x04,
  0x04,
  0x00,
  0x00,
  0x04
};

byte arrow_up_2[] = {
  0x04,
  0x04,
  0x04,
  0x00,
  0x00,
  0x04,
  0x0E,
  0x15
};

byte arrow_up_3[] = {
  0x04,
  0x00,
  0x00,
  0x04,
  0x0E,
  0x15,
  0x04,
  0x04
};

////////////////////Arrows Down\\\\\\\\\\\\\\\\\\

byte arrow_down_0[] = {
  0x00,
  0x04,
  0x04,
  0x04,
  0x15,
  0x0E,
  0x04,
  0x00
};

byte arrow_down_1[] = {
  0x04,
  0x00,
  0x00,
  0x04,
  0x04,
  0x04,
  0x15,
  0x0E
};

byte arrow_down_2[] = {
  0x15,
  0x0E,
  0x04,
  0x00,
  0x00,
  0x04,
  0x04,
  0x04
};

byte arrow_down_3[] = {
  0x04,
  0x04,
  0x15,
  0x0E,
  0x04,
  0x00,
  0x00,
  0x04
};


int led_on = 1;
int levels_queue[99];
char password[4] = {'1', '2', '3', '4'};
unsigned char data;
unsigned char data2[20] = "";
unsigned char buffer[100] = "";
int position = 0;
int admin_mode = 0;
extern int max_level;
extern int current_level;
extern int wait_time;

int levels_queue[99];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
extern void insert(int data);
extern void get_queue(char str[], int queue[]);
extern void makeEmpty();
extern int removeData();
extern int isEmpty();
extern int peek();
extern int isInQ(int val);
extern int elevator_jorney;
extern int elevator_started;
extern int front;
extern int rear;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char* substr(const char *src, int m, int n) {
	int len = m - n;

	char *dest = (char*)malloc(sizeof(char) * (len+1));

	for (int i = m; i < n && (*(src+1) != '\0'); i++) {
		*dest = *(src+i);
		dest++;
	}

	*dest = '\0';

	return dest - len;
}

int a2i(char* str)
{
    // Initialize result
    int res = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    // return result.
    return res;
}

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if(huart->Instance == USART1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
		if(data != '\n') {
			buffer[position] = data;
			buffer[position+1] = '\0';
			position++;
		} else {
//			unsigned char* buffer_work = (unsigned char*)malloc(sizeof(char) * position);
//			HAL_UART_Transmit_IT(&huart1, buffer, (int)strlen(buffer));

//			if (strstr(buffer, "23"))
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
//			if (strstr(buffer, "21"))
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
//			int i;
//			for (i = 0; buffer[i] != '\0'; i++)
//				buffer_work[i] = buffer[i];
//			buffer_work[i] = '\0';
//			print(buffer_work);

			if(admin_mode) {

				char set_max_level[30] = "ET MAX LEVEL";
				char set_level[30] = "ET LEVEL";
				char set_wait[30] = "ET WAIT";
				char set_led[30] = "ET LED";
				char test[30] = "EST#";
				char start[30] = "TART";
				char emptyq[30] = "MPTYQ";

				strcat(buffer, " ");

				if(strstr(buffer, set_max_level) != NULL) {
					makeEmpty();
					int m_l = -1;
					for (int i = 0; i < strlen(buffer); i++) {
						if (buffer[i] == 'L' && buffer[i + 2] >= '0' && buffer[i + 2] <= '9') {
							char sm2i[5];
							sprintf(sm2i, "%c", buffer[i+2]);
							m_l = a2i(sm2i);
							break;
						}
					}


//					char ss[10] = "";
//					if (m_l == 8) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
//					sprintf(ss, "%d ", m_l);
//					HAL_UART_Transmit_IT(&huart1, ss, 2);
					if(m_l >= 0) {
//						max_level = atoi(buffer[14]);
						max_level = m_l;
						current_level = 0;
						char msg[50] = "";
//						sprintf(msg, "Max level changed to %d.\n", atoi(buffer[14]));
						sprintf(msg, "Max level changed to %d.\n", m_l);
						HAL_UART_Transmit_IT(huart, msg, strlen(msg));
					} else {
						char default_msg[50] = "";
						strcpy(default_msg, "Wrong command for changing max level.\nChanged to default value: 9.\n");
						HAL_UART_Transmit_IT(huart, default_msg, strlen(default_msg));
					}
//				}
				} else if(strstr(buffer, set_level) != NULL) {
					makeEmpty();
					int c_l = -1;
					for (int i = 0; i < strlen(buffer); i++) {
						if (buffer[i] == 'L' && buffer[i + 2] >= '0' && buffer[i + 2] <= '9') {
							char sm2i[5];
							sprintf(sm2i, "%c", buffer[i+2]);
							c_l = a2i(sm2i);
							break;
						}
					}

					if (c_l >= 0) {
						if (c_l >= max_level)
							c_l = max_level;

						current_level = c_l;
						char msg[40] = "";
						sprintf(msg, "Current level changed to %d.\n", c_l);
						HAL_UART_Transmit_IT(huart, msg, strlen(msg));
					}

//					if(isdigit(buffer[10])) {
//						if(atoi(buffer[10]) <= max_level && atoi(buffer[10]) >= 0) {
//							current_level = atoi(buffer[10]);
//							char msg[40] = "";
//							sprintf(msg, "Current level changed to %d.\n", atoi(buffer[10]));
//							HAL_UART_Transmit_IT(huart, msg, sizeof(msg));
//						} else {
//							char default_msg[60]  = "";
//							sprintf(default_msg, "Cant change current level. %d is bigger than MAX_LEVEL(%d).\nCurrent level changed to %d.\n", atoi(buffer[10]), max_level, max_level);
//							HAL_UART_Transmit_IT(huart, default_msg, sizeof(default_msg));
//						}
//					}
//				}
				} else if(strstr(buffer, set_wait) != NULL) {

					int w_t = -1;
					for (int i = 0; i < strlen(buffer); i++) {
						if (buffer[i] == 'T' && buffer[i + 2] >= '1' && buffer[i + 2] <= '9') {
							char sm2i[20] = "";
							for (int j = i + 2; buffer[j] >= '0' && buffer[j] <= '9'; j++) {
								char tmp[3] = "";
								sprintf(tmp, "%c", buffer[j]);
								strcat(sm2i, tmp);
							}
							w_t = a2i(sm2i);
							break;
						}
					}

//					char tmp[20] = "";
//					sprintf(tmp, "%d", w_t);
//					HAL_UART_Transmit_IT(&huart1, tmp, strlen(tmp));

					if (w_t % 100 == 0 && w_t >= 500 && w_t <= 5000) {
						wait_time = w_t;
						char msg[50];
						sprintf(msg, "Wait time changed to %d.\n", w_t);
						HAL_UART_Transmit_IT(huart, msg, strlen(msg));
					} else {
						wait_time = 3000;
						char msg[200] = "Wrong value limit; Wait time changed to default value: 3000.\n ";
						HAL_UART_Transmit_IT(huart, msg, strlen(msg));
					}

//					if(isdigit(buffer[9]) && isdigit(buffer[10]) && isdigit(buffer[11])) {
//						char* buffer_num = substr(buffer, 9, 12);
//						char* buffer_num_2 = (char*)malloc(sizeof(char) * (4));
//						if(isdigit(buffer[9]) && isdigit(buffer[10]) && isdigit(buffer[11]) && isdigit(buffer[12])) {
//							for (int i = 0; buffer_num[i] != '\0'; i++)
//								buffer_num_2[i] = buffer_num[i];
//							buffer_num_2[3] = buffer[12];
//						} else
//							strcpy(buffer_num_2, buffer_num);
//						if(atoi(buffer_num_2) >= 500 && atoi(buffer_num_2) <= 5000 && atoi(buffer_num_2) % 100 == 0) {
//							wait_time = atoi(buffer_num_2);
//							char msg[20] = "";
//							sprintf(msg, "Wait time changed to %d.\n", atoi(buffer_num_2));
//							HAL_UART_Transmit_IT(huart, msg, sizeof(msg));
//						}
//					} else {
//						char default_msg[60] = "";
//						strcpy(default_msg, "Wrong command for changing wait time.\nWait time changed to default value: 3000.\n");
//						HAL_UART_Transmit_IT(huart, default_msg, sizeof(default_msg));
//					}
//				}
				} else if(strstr(buffer, set_led) != NULL) {
					int s_l = -1;

					for (int i = 0; i < strlen(buffer) ; i++) {
						if (buffer[i] == 'O') {
							if (buffer[i + 1] == 'N')
								s_l = 1;
							if (buffer[i + 1] == 'F' && buffer[i + 2] == 'F')
								s_l = 0;

							break;
						}
					}

					if (s_l == -1) {
						led_on = 1;
						char msg[100] = "Wrong format; set to default value: ON.\n";
						HAL_UART_Transmit_IT(huart, msg, strlen(msg));
					} else {
						led_on = s_l;
						if (s_l) {
							char msg[100] = "LED set: ON.\n";
							HAL_UART_Transmit_IT(huart, msg, strlen(msg));
						} else {
							char msg[100] = "LED set: OFF.\n";
							HAL_UART_Transmit_IT(huart, msg, strlen(msg));
						}
					}
//					if(buffer[8] == 'O' && buffer[9] == "N") {
//						led_on = 1;
//						char msg[30] = "";
//						strcpy(msg, "LED_ON changed to ON.\n");
//						HAL_UART_Transmit_IT(huart, msg, sizeof(msg));
//					} else if(buffer[8] == 'O' && buffer[9] == "F" && buffer[10] == "F") {
//						led_on = 0;
//						char msg[30] = "";
//						strcpy(msg, "LED_ON changed to OFF.\n");
//						HAL_UART_Transmit_IT(huart, msg, sizeof(msg));
//					} else {
//						led_on = 1;
//						char default_msg[60] = "";
//						strcpy(default_msg, "Wrong command for changing LED_ON value.\nLED_ON changed to default value: 1.\n");
//						HAL_UART_Transmit_IT(huart, default_msg, sizeof(default_msg));
//					}
//				}
				}else if(strstr(buffer, test) != NULL) {
					int i;
					int hash_i;

					for (i = 0; buffer[i - 1] != '#'; i++);
					hash_i = i;
					for (; i < strlen(buffer); i++) {
						if (buffer[i] >= '0' && buffer[i] <= '9') {
							char ss[10];
							sprintf(ss, "%c", buffer[i]);
							if (!isInQ(a2i(ss)))
								insert(a2i(ss));
						} else
							break;
					}

					if (buffer[hash_i] >= '0' && buffer[i] <= '9') {
						char q[50] = "";
						get_queue(q, levels_queue);
						if (strlen(q) > 5) {
							char err_msg[40] = "Too many arguments.\n";
							HAL_UART_Transmit_IT(huart, err_msg, strlen(err_msg));
						} else {
							strcat(q, " :Queue.\n");
							HAL_UART_Transmit_IT(huart, q, strlen(q));
						}
					} else {
						char str[50] = "Invalid input.\n";
						HAL_UART_Transmit_IT(huart, str, strlen(str));
					}

//					for (int i = 0; buffer[i] != '\0'; i++) {
//						if(isdigit(buffer[i])) {
//							insert(atoi(buffer[i]));
//						}
//					}
//
//					char str[20] = "";
//					get_queue(str, levels_queue);
//					HAL_UART_Transmit_IT(huart, str, sizeof(str));
				} else if (strstr(buffer, start) != NULL) {
					elevator_jorney = peek();
					removeData();
					elevator_started = 0;
					admin_mode = 0;
					char msg[50] = "Elevator started; Adming mode: Disabled.\n";
					HAL_UART_Transmit_IT(huart, msg, strlen(msg));
				} else if (strstr(buffer, emptyq) != NULL) {
					makeEmpty();
					char msg[50] = "Queue became empty.\n";
					HAL_UART_Transmit_IT(huart, msg, strlen(msg));
				}

				//reset buffer
				char str[30] = "\0";
				position = 0;
				strcpy(buffer, str);
//
			} else {
//				HAL_UART_Transmit_IT(&huart1, buffer, strlen(buffer));

//				// Not adming mode
				if(strstr(buffer, "DMIN#") && isEmpty()) {
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
					int pass = 0;
					int pass_it = 0;
					int wrong_pass = 0;
					for (int i = 0;  buffer[i] != '}'; i++) {
						if (pass) {
							if (buffer[i] != password[pass_it++]) {
								wrong_pass = 1;
								break;
							}
						}
						if (buffer[i] == '{')
							pass = 1;
					}
//
					if (!wrong_pass) {
						admin_mode = 1;
						char str[20] = "Welcome";
						HAL_UART_Transmit_IT(&huart1, str, strlen(str));
					}
				}
			}


			//reset buffer
					char str[30] = "\0";
					position = 0;
					strcpy(buffer, str);

		}

//		HAL_UART_Receive_IT(&huart1, &data, sizeof(data));
	}

//	HAL_UART_Receive_IT(&huart1, data, sizeof(data));


}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

//	HAL_UART_Transmit_IT(&huart1, data2, sizeof(data2));

//	for (int i = 0; i < 20; i++)
//		if (data2[i] == '\n'){
//			data2[i] = '\0';
//			break;
//		}

//	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);

//	print(data2);

//	HAL_UART_Transmit(&huart1, data2, sizeof(data2), 1000);

//	if (strstr(data2, "123") != NULL)
//		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
//
//	if (strstr(data2, "321") != NULL)
//		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);

//	if(adminmode==1){
//
//		  if (strstr(data, "SET MAX LEVEL") != NULL) {
//			  maxfloor = getInputNumber();
//			  currentfloor = 0;
//			  emptythelist();
//		  }
//		  else if (strstr(data, "SET LEVEL") != NULL) {
//			  currentfloor = getInputNumber();
//			  emptythelist();
//
//		  }
//		  else if (strstr(data, "SET WAIT") != NULL) {
//			  waitscal = (int)getInputNumber()/100;
//
//		  }
//		  else if (strstr(data, "SET LED") != NULL) {
//			  if(strstr(data, "ON") != NULL){
//				  ledOn = 1;
//
//			  }
//			  else{
//				  ledOn = 0;
//
//				  deActiveLed();
//			  }
//		  }
//		  else if(strstr(data, "TEST#") != NULL){
//
//			  pushInput();
//
//		  }else if(strstr(data, "START") != NULL){
//			  sortfloors();
//			  elevatorbool=1;
//			  adminmode=0;
//
//
//		  }
//		  else if(strstr(data, "SET ADMIN PASS#") != NULL){
//			  int pass = getInputNumber();
//			  if(log10(pass) ==4){
//				  password=pass;
//				  HAL_UART_Transmit(&huart1, passright, sizeof(passright)-1 , 1000);
//			  }
//			  else{
//				  HAL_UART_Transmit(&huart1, passwrong, sizeof(passwrong)-1 , 1000);
//			  }
//		  }
//		  else{
//			  HAL_UART_Transmit(&huart1, commandNotFound, sizeof(commandNotFound)-1 , 1000);
//		  }
//
//	}
//	else{
//		  if (strstr(data, "ADMIN#") != NULL) {
//			  int pass = getInputNumber();
//			  if (pass == password){
//				  adminmode = 1;
//
//				  HAL_UART_Transmit(&huart1, passright, sizeof(passright)-1 , 1000);
//			  }
//			  else{
//				  HAL_UART_Transmit(&huart1, passwrong, sizeof(passwrong)-1 , 1000);
//			  }
//		 }
//		  else{
//
//			  	  HAL_UART_Transmit(&huart1, commandNotFound, sizeof(commandNotFound)-1 , 1000);
//
//		  }
//
//	}

//			strcpy(data2, "");
//			HAL_UART_Receive_IT(&huart1, data2, sizeof(data2));

//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//		if(huart->Instance == USART2){
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
//			if(data != '\n'){
//					buffer[position] = data;
//					// end od string in c
//					buffer[position+1] = '\0';
//				  position++;
//			}else {
//
//				HAL_UART_Transmit_IT(&huart2, buffer, (int)strlen(buffer));
//				position = 0;
//				strcpy(buffer, "\0");
//						//check the buffer for specific string with string.h function
//			}
//
//			//copy your ISR code
//			HAL_UART_Receive_IT(&huart2,&data,sizeof(data));
//
//		}
//
//
////			HAL_UART_Receive_IT(&huart2,&data,sizeof(data));
//
//}
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
//  HAL_TIM_Base_Start_IT(&htim3);

  LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);
  begin(20, 4);

  for (int i = 0; i < 99; i++)
	  levels_queue[i] = -1;

//  setCursor(0, 0);
  createChar(0, arrow_up_0);
  createChar(1, arrow_up_1);
  createChar(2, arrow_up_2);
  createChar(3, arrow_up_3);

  createChar(4, arrow_down_0);
  createChar(5, arrow_down_1);
  createChar(6, arrow_down_2);
  createChar(7, arrow_down_3);
//    home();
//    write(0);
//    print("hi");
//  print(customChar);

  HAL_UART_Receive_IT(&huart1, &data, sizeof(data));

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim4);

//  PWM_Start();
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
//  print("hi");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 239;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 239;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 4800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin
                          |MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
