/* keyled.c */
#include	"keyled.h"

//轮询方式扫描4个按键,返回按键值
//timeout单位ms，若timeout=0表示一直扫描，直到有键按下
KEYS ScanPressedKey(uint32_t timeout)
{
	KEYS key=KEY_NONE;
	uint32_t  tickstart = HAL_GetTick();	//当前计数值
	const uint32_t  btnDelay=10;			//按键按下阶段的抖动，延时再采样时间
	GPIO_PinState keyState;

	while(1)
	{
#ifdef	KeyLeft_Pin		 //如果定义了KeyLeft，就可以检测KeyLeft
		keyState=HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin); //PA6=KeyLeft,低输入有效,LED1
		if (keyState==GPIO_PIN_RESET)
		{
			HAL_Delay(btnDelay);  //前抖动期
			keyState=HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin); //再采样
			if (keyState ==GPIO_PIN_RESET)
				return	KEY_LEFT;
		}
#endif

#ifdef	KeyRight_Pin 	//如果定义了KeyRight，就可以检测KeyRight
		keyState=HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin); //PA0=KeyRight,低输入有效,LED0
		if (keyState==GPIO_PIN_RESET)
		{
			HAL_Delay(btnDelay); //前抖动期
			keyState=HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin);//再采样
			if (keyState ==GPIO_PIN_RESET)
				return	KEY_RIGHT;
		}
#endif

#ifdef	KeyDown_Pin		//如果定义了KeyDown，就可以检测KeyDown
		keyState=HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin); //PA1=KeyDown,输入低电平时蜂鸣
		if (keyState==GPIO_PIN_RESET)
		{
			HAL_Delay(btnDelay); //前抖动期
			keyState=HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin);//再采样
			if (keyState ==GPIO_PIN_RESET)
				return	KEY_DOWN;
		}
#endif

#ifdef	KeyUp_Pin		//如果定义了KeyUp，就可以检测KeyUp
		keyState=HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin); //PA7=KeyUp,输出低电平时双闪
		if (keyState== GPIO_PIN_RESET)
		{
			HAL_Delay(btnDelay); //10ms 抖动期
			keyState=HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin);//再采样
			if (keyState == GPIO_PIN_RESET)
				return	KEY_UP;
		}
#endif

		if (timeout != KEY_WAIT_ALWAYS)  //没有按键按下时，会计算超时，timeout时退出
		{
			if ((HAL_GetTick() - tickstart) > timeout)
				break;
		}
	}

	return	key;
}
