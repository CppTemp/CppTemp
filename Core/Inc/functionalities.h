/*
 * functionalities.h
 *
 *  Created on: 11 maj 2023
 *      Author: Filip
 */

#ifndef INC_FUNCTIONALITIES_H_
#define INC_FUNCTIONALITIES_H_

void showTempAndHum(float temp, float hum);
void showTimeAndDate(RTC_DateTypeDef sDate, RTC_TimeTypeDef sTime);
void setTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime);
void showSettingUpTime(int *pos, RTC_TimeTypeDef sTime);
void setDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate);
void showSettingUpDate(int *pos, RTC_DateTypeDef sDate);
void showAlarm(float temp, float hum, bool isSet);
void setAlarm(float *temp, float *hum, bool *isSet);
void showSettingUpAlarm(float temp, float hum, bool isSet);

#endif /* INC_FUNCTIONALITIES_H_ */
