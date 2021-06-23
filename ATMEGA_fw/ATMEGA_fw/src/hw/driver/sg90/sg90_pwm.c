#include "sg90/sg90_pwm.h"
#include "timer.h"

#ifdef _USE_HW_SG90

static const uint8_t timer_ch_tbl[] = {_DEF_TIM3, };

bool		begin(void);
bool		start(void);
bool		stop(void);
bool		setOcr(uint8_t channel_, uint16_t ocr_);
uint16_t	getOcr(uint8_t channel_);
bool		setIcr(uint16_t icr_);
uint16_t	getIcr(void);


bool sg90PWMDriver(sg90_driver_t *p_driver)
{
	bool ret = true;
	
	p_driver->is_init = true;
	p_driver->begin = begin;
	p_driver->start = start;
	p_driver->stop = stop;
	p_driver->setOcr = setOcr;
	p_driver->getOcr = getOcr;
	p_driver->setIcr = setIcr;
	p_driver->getIcr = getIcr;
	
	return ret;
}

bool begin(void)
{
	bool ret = timerBegin(timer_ch_tbl[0]);
	return ret;
}

bool start(void)
{
	bool ret = timerStart(timer_ch_tbl[0]);
	return ret;
}

bool stop(void)
{
	bool ret = timerStop(timer_ch_tbl[0]);
	return ret;
}

bool setOcr(uint8_t channel_, uint16_t ocr_)
{
	bool ret = timer16SetOcr(timer_ch_tbl[0], ocr_, channel_);
	return ret;
}

uint16_t getOcr(uint8_t channel_)
{
	uint16_t ret = timer16GetOcr(timer_ch_tbl[0], channel_);
	return ret;
}

bool setIcr(uint16_t icr_)
{
	bool ret = timer16SetIcr(timer_ch_tbl[0], icr_);
	return ret;
}

uint16_t getIcr(void)
{
	uint16_t ret = timer16GetIcr(timer_ch_tbl[0]);
	return ret;
}


#endif