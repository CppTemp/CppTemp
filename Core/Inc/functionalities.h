/*
 * functionalities.h
 *
 *  Created on: 11 maj 2023
 *      Author: Filip
 */

#ifndef INC_FUNCTIONALITIES_H_
#define INC_FUNCTIONALITIES_H_

struct history
{
	RTC_TimeTypeDef hTime;
};

void showTempAndHum(DHT_DataTypedef *DHT11_Data);
void showTimeAndDate(RTC_DateTypeDef sDate, RTC_TimeTypeDef sTime);
void setTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime);
void showSettingUpTime(int *pos, RTC_TimeTypeDef sTime);
void setDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate);
void showSettingUpDate(int *pos, RTC_DateTypeDef sDate);
void showAlarm(float temp, float hum, bool isSet);
void setAlarm(float *temp, float *hum, bool *isSet);
void showSettingUpAlarm(float temp, float hum, bool isSet);
bool checkAlarm(DHT_DataTypedef *DHT11_Data, float alarmTemp, float alarmHum, bool alarmState);
void showHistory(struct history alarmHistory[], int counter);
void clrHistory(struct history alarmHistory[], int *counter);

#endif /* INC_FUNCTIONALITIES_H_ */
