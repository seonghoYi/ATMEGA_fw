#ifndef SG90_H_
#define SG90_H_

#ifdef _USE_HW_SG90


typedef struct
{
	bool is_init;
	
	bool		(*begin)(void);
	bool		(*start)(void);
	bool		(*stop)(void);
	bool		(*setOcr)(uint8_t channel_, uint16_t ocr_);
	uint16_t	(*getOcr)(uint8_t channel_);
	bool		(*setIcr)(uint16_t icr_);
	uint16_t	(*getIcr)(void);
} sg90_driver_t;


bool servoInit(void);
bool servoIsInit(void);
void servoRun(uint16_t L_ocr, uint16_t R_ocr);
void servoStop(void);

#endif
#endif /* SG90_H_ */