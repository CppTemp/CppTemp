/*
 * functionalities.c
 *
 *  Created on: 11 maj 2023
 *      Author: Filip
 */
#include "lcd16x2.h"
#include "stdio.h"
#include "stdbool.h"

void showTempAndHum(float temp, float hum) // Pokaż temperaturę oraz wilgotność powietrza
{
	lcd16x2_clear();
	lcd16x2_printf("Temp: %.1f C",temp);
	lcd16x2_2ndLine();
	lcd16x2_printf("Humi: %.1f %%",hum);
}

void showTimeAndDate(RTC_DateTypeDef sDate, RTC_TimeTypeDef sTime) // Pokaż czas i datę
{
	char currTime[30];
	char currDate[30];
	sprintf(currDate,"Date: %02d.%02d.%02d",sDate.Date,sDate.Month,sDate.Year);
	sprintf(currTime,"Time: %02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
	lcd16x2_clear();
	lcd16x2_printf("%s",currDate);
	lcd16x2_2ndLine();
	lcd16x2_printf("%s",currTime);
}

void showSettingUpTime(int *pos, RTC_TimeTypeDef sTime) // Wyświetlenie czasu przy edycji
{
	lcd16x2_clear();
	lcd16x2_printf("Time: %02d.%02d.%02d", sTime.Hours,sTime.Minutes,sTime.Seconds);
	lcd16x2_2ndLine();
	lcd16x2_setCursor(1, *pos);
	lcd16x2_printf("^");
}

void setTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime) // Ustawianie czasu
{
	int position = 7; // Pozycja strzałki która wskazuje na obecnie edytowany element

	for(int i=0;i<3;i++) // Edycja kolejno 3 kolumn czasu
	{
		showSettingUpTime(&position, *sTime);
		HAL_Delay(500);
		while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1) // Powtarzaj dopóki MIDDLE BUTTON nie zostanie naciśnięty
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0) // Jeśli wciśnięty UP BUTTON
			{
				switch(i) // Obsługa kolumny w zależności od iteracji pętli edycji
				{
					case 0:
						if(sTime->Hours>=23) sTime->Hours=0; // Dodatkowy warunek nie pozwalający
						else sTime->Hours+=1;				 // przekroczyć dozwolonej wartości
						break;
					case 1:
						if(sTime->Minutes>=59) sTime->Minutes=0;
						else sTime->Minutes+=1;
						break;
					case 2:
						if(sTime->Seconds>=59) sTime->Seconds=0;
						else sTime->Seconds+=1;
						break;
				}
				showSettingUpTime(&position, *sTime); // Wyświetlenie czasu przy każdej zmianie wartości w kolumnie
			}
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) // Jeśli wciśnięty DOWN BUTTON
			{
				switch(i)
				{
					case 0:
						if(sTime->Hours<=0) sTime->Hours=23;
						else sTime->Hours-=1;
						break;
					case 1:
						if(sTime->Minutes<=0) sTime->Minutes=59;
						else sTime->Minutes-=1;
						break;
					case 2:
						if(sTime->Seconds<=0) sTime->Seconds=59;
						else sTime->Seconds-=1;
					    break;
				}
				showSettingUpTime(&position, *sTime);
			}
			HAL_Delay(100); // Czasowy bufor dla wciśnięcia przycisku
		}
		position+=3; // Przesuń pozycje strzałki na kolejną kolumne
	}
	HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN); // Ustaw czas
}

void showSettingUpDate(int *pos, RTC_DateTypeDef sDate) // Wyświetlenie daty przy edycji
{
	lcd16x2_clear();
	lcd16x2_printf("Date: %02d.%02d.%02d",sDate.Date,sDate.Month,sDate.Year);
	lcd16x2_2ndLine();
	lcd16x2_setCursor(1, *pos);
	lcd16x2_printf("^");
}

void setDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate) // Ustawienie daty działa na tej samej zasadzie co czasu
{
	int position = 7;

	for(int i=0;i<3;i++)
	{
		showSettingUpDate(&position, *sDate);
		HAL_Delay(500);
		while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1)
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0) //UP
			{
				switch(i)
				{
					case 0:
						if(sDate->Date>=31) sDate->Date=0;
						else sDate->Date+=1;
						break;
					case 1:
						if(sDate->Month>=12) sDate->Month=0;
						else sDate->Month+=1;
						break;
					case 2:
						if(sDate->Year>=99) sDate->Year=0;
						else sDate->Year+=1;
						break;
				}
				showSettingUpDate(&position, *sDate);
			}
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) //DOWN
			{
				switch(i)
				{
					case 0:
						if(sDate->Date<=0) sDate->Date=31;
						else sDate->Date-=1;
						break;
					case 1:
						if(sDate->Month<=0) sDate->Month=12;
						else sDate->Month-=1;
						break;
					case 2:
						if(sDate->Year<=0) sDate->Year=99;
						else sDate->Year-=1;
					    break;
				}
				showSettingUpDate(&position, *sDate);
			}
			HAL_Delay(100);
		}
		position+=3;
	}
	HAL_RTC_SetDate(hrtc, sDate, RTC_FORMAT_BIN);
}


void showAlarm(float temp, float hum, bool isSet)
{
	lcd16x2_clear();
	if(isSet)
	{
		lcd16x2_printf("Alarm is set");
		lcd16x2_2ndLine();
		lcd16x2_printf("T:%.1fC H:%.1f%%",temp,hum);
	}
	else lcd16x2_printf("Alarm is not set");

	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1);
}

void showSettingUpAlarm(float temp, float hum, bool isSet)
{
	lcd16x2_clear();
	lcd16x2_printf("Alarm:");
	if(isSet)
		lcd16x2_printf("ENABLED");
	else
		lcd16x2_printf("DISABLED");
	lcd16x2_2ndLine();
	lcd16x2_printf("T:%.1fC H:%.1f%%",temp,hum);
}

void setAlarm(float *temp, float *hum, bool *isSet)
{
	showSettingUpAlarm(*temp, *hum, *isSet);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0 || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0)
		{
			if(*isSet) *isSet = false;
			else *isSet = true;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		lcd16x2_setCursor(0,15);
		lcd16x2_printf("<");
		HAL_Delay(200);
	}
	HAL_Delay(200);

	if(*isSet == false) return;

	showSettingUpAlarm(*temp, *hum, *isSet);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0) //UP
		{
			if(*temp>=99) *temp=-99;
			else *temp+=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) //DOWN
		{
			if(*temp<=-99) *temp=99;
			else *temp-=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		HAL_Delay(100);
	}
	HAL_Delay(200);

	showSettingUpAlarm(*temp, *hum, *isSet);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0) //UP
		{
			if(*hum>=99) *hum=0;
			else *hum+=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) //DOWN
		{
			if(*hum<=0) *hum=99;
			else *hum-=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		HAL_Delay(100);
	}
	HAL_Delay(200);
}

//void checkAlarm(float currTemp, float currHum, float alarmTemp, float alarmHum)
//{
//	if(currTemp >= alarmTemp || currHum >=)
//}
