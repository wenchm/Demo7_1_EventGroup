/* keyled.h */
 #include "main.h"	//在main.h中定义了Keys、LEDs和Buzzer引脚的宏
//表示4个按键的枚举类型
 typedef enum {
 	KEY_NONE = 0,	//没有按键被按下
	KEY_LEFT, 		//KeyLeft键
	KEY_RIGHT, 	//KeyRight键
	KEY_UP,			//KeyUp键
	KEY_DOWN, 	//KeyDown键
 }KEYS;

#define KEY_WAIT_ALWAYS 0 	//作为函数ScanKeys()的一种参数，表示一直等待按键输入
//轮询方式扫描按键，timeout=KEY_WAIT_ALWAYS时一直扫描，否则等待时间timeout，单位ms
KEYS ScanPressedKey(uint32_t timeout);

#ifdef LED1_Pin	 	//LED1的控制
	#define LED1_Toggle()	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)					//输出翻转
	#define LED1_ON()		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET) 	//输出0，亮
	#define LED1_OFF() 		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)		//输出1，灭
#endif

#ifdef LED2_Pin 	//LED2的控制
	#define LED2_Toggle()	HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin)					//输出翻转
	#define LED2_ON()		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)	//输出0，亮
	#define LED2_OFF()		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)		//输出1，灭
#endif
//新增LED3、LED4
#ifdef LED3_Pin 	//LED3的控制
	#define LED3_Toggle()	HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin)					//输出翻转
	#define LED3_ON()		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)	//输出0，亮
	#define LED3_OFF()		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)		//输出1，灭
#endif

#ifdef LED4_Pin 	//LED4的控制
	#define LED4_Toggle()	HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin)					//输出翻转
	#define LED4_ON()		HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_RESET)	//输出0，亮
	#define LED4_OFF()		HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET)		//输出1，灭
#endif

#ifdef Buzzer_Pin	//蜂鸣器的控制
	#define Buzzer_Toggle()	HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin)				//输出翻转
	#define Buzzer_ON()		HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN RESET)//输出0,蜂鸣器不响
	#define Buzzer_OFF()	HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO _PIN_SET) //输出1,蜂鸣器响
#endif
