#include "hc05.h"
#include "uart.h"
#include "gpio.h"

#ifdef _USE_HW_HC05


bool hc05SetName(hc05_t *p_hc05, char *name);
bool hc05SetPSWD(hc05_t *p_hc05, char *pswd);
bool hc05SetMode(hc05_t *p_hc05, uint8_t mode);
bool hc05SetBaud(hc05_t *p_hc05, uint32_t baud);
bool hc05SetPair(hc05_t *p_hc05, char *slave_addr);

bool hc05Init(void)
{
	return true;
}

bool hc05Open(hc05_t *p_hc05, uint8_t ch_, uint32_t baud_)
{
	bool ret = true;
	
	p_hc05->ch = ch_;
	p_hc05->baud = baud_;
	p_hc05->error_code = HC05_ERROR_NO_ERROR;

	if (!uartOpen(ch_, baud_))
	{
		return false;
	}
	
	gpioPinWrite(_DEF_GPIO_BT_CFG, _DEF_PIN_SET);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_PIN_SET);
	delay(10);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_PIN_RESET);
	
	if (!hc05SetBaud(p_hc05, baud_))
	{
		//uartPrintf(_DEF_UART0, "fail\n");
		ret = false;
	}
	//uartPrintf(_DEF_UART0, "1\n");
	
	if (!hc05SetName(p_hc05, "gsrtest"))
	{
		//uartPrintf(_DEF_UART0, "fail\n");
		ret = false;
	}
	//uartPrintf(_DEF_UART0, "2\n");
	if (!hc05SetPSWD(p_hc05, "gsr"))
	{
		//uartPrintf(_DEF_UART0, "fail\n");
		ret = false;
	}
	//uartPrintf(_DEF_UART0, "3\n");
	if (!hc05SetMode(p_hc05, HC05_MODE_MASTER))
	{
		//uartPrintf(_DEF_UART0, "fail\n");
		ret = false;
	}
	//uartPrintf(_DEF_UART0, "4\n");
	if (p_hc05->role == HC05_MODE_MASTER)
	{
		//uartPrintf(_DEF_UART0, "fail\n");
		if (hc05SetPair(p_hc05, "2016,11,289519"))
		{
			ret = false;
		}
	}
	//uartPrintf(_DEF_UART0, "5\n");
	gpioPinWrite(_DEF_GPIO_BT_CFG, _DEF_PIN_RESET);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_PIN_SET);
	delay(10);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_PIN_RESET);
	
	p_hc05->is_open = ret;
	return ret;
}

bool hc05IsOpen(hc05_t *p_hc05)
{
	return p_hc05->is_open;
}

uint32_t hc05Available(hc05_t *p_hc05)
{
	return uartAvailable(p_hc05->ch);
}

uint8_t hc05Read(hc05_t *p_hc05)
{
	return uartRead(p_hc05->ch);
}

uint32_t hc05Write(hc05_t *p_hc05, uint8_t *p_data, uint32_t length)
{
	return uartWrite(p_hc05->ch, p_data, length);
}

uint32_t hc05Printf(hc05_t *p_hc05, char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(p_hc05->ch, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}

bool hc05flush(hc05_t *p_hc05)
{
	return uartFlush(p_hc05->ch);
}



uint8_t hc05GetResponse(hc05_t *p_hc05)
{
	uint8_t ret = HC05_ERROR_NO_ERROR;
	char resp[64];
	char *result = NULL;
	int index = 0;
	/*
	while (uartAvailable(p_hc05->ch) > 0)
	{
		resp[index++] = (char)uartRead(p_hc05->ch);
	}
	resp[index] = 0;
	uartPrintf(_DEF_UART0, "%s\n", resp);
	if (strstr(resp, "OK/r/n") != NULL)
	{
		return ret;
	}
	else
	{
		result = strtok(resp, ":");
		if (!strstr(result, "ERROR"))
		{
			
			result = strtok(NULL, "(");
			ret = atoi(result);
		}
		else
		{
			ret = HC05_ERROR_AT_CMD_ERROR;
		}
	}
	*/
	
	uint32_t starttick = millis();
	
	while(true)
	{
		if (millis() - starttick >= 5000)
		{
			break;
		}
		
		if (uartAvailable(p_hc05->ch) > 0)
		{
			resp[index++] = (char)uartRead(p_hc05->ch);
		}
		resp[index] = 0;
		char str[64];
		strncpy(str, resp, strlen(resp)+1);		
		if (strstr(str, "OK/r/n") != NULL)
		{	
			//uartPrintf(_DEF_UART0, "OK\r\n");
			break;
		}
		else
		{
			result = strtok(str, ":");
			if (strstr(result, "ERROR") != NULL)
			{
				result = strtok(NULL, "(");
				//uartPrintf(_DEF_UART0, "ERROR:(%s\r\n", result);
				ret = atoi(result);
				break;
			}
			/*
			else
			{
				//uartPrintf(_DEF_UART0, "unknown\r\n");
				ret = HC05_ERROR_AT_CMD_ERROR;
				break;
			}
			*/
		}
		
	}
	uartPrintf(_DEF_UART0, "%s", resp);
	
	return ret;
}


bool hc05SetName(hc05_t *p_hc05, char *name)
{
	bool ret = true;
	uint8_t atCommand[64];
	strncpy((char*)(p_hc05->name), name, strlen(name) + 1);
	sprintf((char *)&atCommand[0], "AT+NAME=%s\r\n", (char*)&(p_hc05->name[0]));
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	if (hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		ret = false;
	}
	return ret;
}

bool hc05SetPSWD(hc05_t *p_hc05, char *pswd)
{
	bool ret = true;
	uint8_t atCommand[64];
	strncpy((char*)(p_hc05->pswd), pswd, strlen(pswd) + 1);
	sprintf((char *)&atCommand[0], "AT+PSWD=%s\r\n", (char*)&(p_hc05->pswd[0]));
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	uint8_t resp = hc05GetResponse(p_hc05);
	if ( resp != HC05_ERROR_NO_ERROR || resp != HC05_ERROR_SPP_LIB_REPEAT_INIT)
	{
		ret = false;
	}
	return ret;
}

bool hc05SetMode(hc05_t *p_hc05, uint8_t mode)
{
	bool ret = true;
	uint8_t atCommand[64];
	p_hc05->role = mode;
	sprintf((char *)&atCommand[0], "AT+ROLE=%d\r\n", (p_hc05->role));
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	if (hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		ret = false;
	}
	return ret;
}

bool hc05SetBaud(hc05_t *p_hc05, uint32_t baud)
{
	bool ret = true;
	uint8_t atCommand[64];
	p_hc05->baud = baud;
	sprintf((char *)&atCommand[0], "AT+UART=%ld,0,0\r\n", (p_hc05->baud));
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	if (hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		ret = false;
	}
	return ret;
}

bool hc05SetPair(hc05_t *p_hc05, char *slave_addr)
{
	bool ret = true;
	uint8_t atCommand[64];
	sprintf((char *)&atCommand[0], "AT+INIT\r\n");
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	if (hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		ret = false;
	}
	
	sprintf((char *)&atCommand[0], "AT+INQ\r\n");
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	uint32_t starttick = millis();
	
	while(hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		if (millis() - starttick >= 10000)
		{
			ret = false;
			break;
		}
	}
	
	strncpy((char*)(p_hc05->slave_addr), slave_addr, strlen(slave_addr) + 1);
	sprintf((char *)&atCommand[0], "AT+PAIR=%s,20\r\n", (char*)(p_hc05->slave_addr));
	uartWrite(p_hc05->ch, &atCommand[0], strlen((char *)&atCommand[0]));
	
	if (hc05GetResponse(p_hc05) != HC05_ERROR_NO_ERROR)
	{
		ret = false;
	}
	return ret;
}






#endif