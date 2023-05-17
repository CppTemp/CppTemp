/*
 * functionalities.c
 *
 *  Created on: 11 maj 2023
 *      Author: Filip
 */
#include "lcd16x2.h"
#include "stdio.h"
#include "stdbool.h"
#include "DHT.h"
#include "string.h"
#include "functionalities.h"

#define MIDDLE_BUTTON HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)
#define DOWN_BUTTON HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)
#define UP_BUTTON HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)
#define PRESSED 0
#define RELEASED 1

void showTempAndHum(DHT_DataTypedef *DHT11_Data) // Pokaż temperaturę oraz wilgotność powietrza
{
	lcd16x2_clear();
	lcd16x2_printf("Temp: %.1f C",DHT11_Data->Temperature);
	lcd16x2_2ndLine();
	lcd16x2_printf("Humi: %.1f %%",DHT11_Data->Humidity);
}

void showTimeAndDate(RTC_DateTypeDef sDate, RTC_TimeTypeDef sTime) // Pokaż czas i datę
{
	lcd16x2_clear();
	lcd16x2_printf("Time: %02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
	lcd16x2_2ndLine();
	lcd16x2_printf("Date: %02d.%02d.%02d",sDate.Date,sDate.Month,sDate.Year);
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
		while(MIDDLE_BUTTON == RELEASED)
		{
			if(UP_BUTTON == PRESSED)
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
			else if(DOWN_BUTTON == PRESSED)
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
		while(MIDDLE_BUTTON == RELEASED)
		{
			if(UP_BUTTON == PRESSED)
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
			else if(DOWN_BUTTON == PRESSED) //DOWN
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

void showAlarm(float temp, float hum, bool isSet) // Pokaż ustawiony lub nieustawiony alarm
{
	lcd16x2_clear();
	if(isSet) // ustawiony
	{
		lcd16x2_printf("Alarm is set");
		lcd16x2_2ndLine();
		lcd16x2_printf("T:%.1fC H:%.1f%%",temp,hum);
	}
	else lcd16x2_printf("Alarm is not set"); //nieustawiony
}

void showSettingUpAlarm(float temp, float hum, bool isSet) // Wyświetlanie ustawień alarmu przy edycji
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

void setAlarm(float *temp, float *hum, bool *isSet) // Ustawienie alarmu
{
	showSettingUpAlarm(*temp, *hum, *isSet);
	while(MIDDLE_BUTTON == RELEASED) // Pierwsze ustawienie dotyczy tego czy ma być aktywowany alarm lub nie
	{
		if(UP_BUTTON == PRESSED || DOWN_BUTTON == PRESSED) // z każdym wciśnięciem przycisku góra lub dół
		{
			if(*isSet) *isSet = false; 	// zmień wartość
			else *isSet = true;         // na przeciwną
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		lcd16x2_setCursor(0,15);
		lcd16x2_printf("<");
		HAL_Delay(150);
	}

	HAL_Delay(200);
	if(*isSet == false) return; // Dodatkowy warunek który powraca do menu głównego dla przypadku gdy alarm jest ustawiony jako dezaktywowany
	showSettingUpAlarm(*temp, *hum, *isSet);

	while(MIDDLE_BUTTON == RELEASED) // Drugie ustawienie dotyczy temperatury przy której alarm zostanie włączony
	{
		if(UP_BUTTON == PRESSED)
		{
			if(*temp>=99) *temp=-99; // Dodatkowy warunek nie pozwalający dozwolonych wartości
			else *temp+=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		if(DOWN_BUTTON == PRESSED)
		{
			if(*temp<=-99) *temp=99;
			else *temp-=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		HAL_Delay(100);
	}

	HAL_Delay(200);
	showSettingUpAlarm(*temp, *hum, *isSet);

	while(MIDDLE_BUTTON == RELEASED) // Trzecie ustawienie dotyczy wilgotności powietrza przy której alarm zostanie włączony
	{
		if(UP_BUTTON == PRESSED)
		{
			if(*hum>=99) *hum=0;
			else *hum+=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		if(DOWN_BUTTON == PRESSED)
		{
			if(*hum<=0) *hum=99;
			else *hum-=1;
			showSettingUpAlarm(*temp, *hum, *isSet);
		}
		HAL_Delay(100);
	}
	HAL_Delay(200);
}

bool checkAlarm(DHT_DataTypedef *DHT11_Data, float alarmTemp, float alarmHum, bool alarmState) // Sprawdzanie statusu alarmu
{
	if(alarmState) // Jeśli alarm jest włączony (tj. urządzenie jest w statusie alarmu)
	{
		if(DHT11_Data->Temperature < alarmTemp && DHT11_Data->Humidity < alarmHum) // Jeśli temperatura oraz wiglotność spadły poniżej ustalonej granicy
		{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0); //
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0); // Dezaktywuj
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0); // diody
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0); //
			return false; // Wyłącz alarm (tj. wyjdź ze statusu alarmu)
		}
		else return true; // Pozostaw alarm włączony (tj. pozostań w statusie alarmu)
	}
	else // Jeśli alarm jest wyłączony (tj. urządzenie nie jest w stanie alarmu)
	{
		if(DHT11_Data->Temperature >= alarmTemp || DHT11_Data->Humidity >= alarmHum) // Jeśli temperatura oraz wiglotność przekroczyły ustalone granice
		{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1); //
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1); // Aktywuj
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1); // diody
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1); //
			lcd16x2_clear();
			lcd16x2_printf("ALARM! Temp/Hum");  //
			lcd16x2_2ndLine();					// Wyświetl komunikat o przekroczeniu ustalonych wartości
			lcd16x2_printf("is too HIGH!");		//
			HAL_Delay(3000);
			return true; // Ustaw alarm na włączony (tj. ustaw urządzenie w tryb alarmu)
		}
	}
	return false; // Pozostaw bez zmian
}

void showHistory(struct history alarmHistory[], int counter) // Pokaż historię alarmów
{
	lcd16x2_clear();
	if(counter==0) 						    //
	{										//
		lcd16x2_printf("No alarms");		// Brak alarmów
		HAL_Delay(200);						//
		while(MIDDLE_BUTTON == RELEASED);	//
	}
	else // Gdy są alarmy w historii
	{
		for(int i=0;i<counter;i++)
		{
			lcd16x2_printf("%d: %02d.%02d.%02d", i+1, alarmHistory[i].hTime.Hours, alarmHistory[i].hTime.Minutes, alarmHistory[i].hTime.Seconds);
			HAL_Delay(200);
			while(MIDDLE_BUTTON == RELEASED);
			lcd16x2_clear();
		}
	}
}
