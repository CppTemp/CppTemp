#include "menu.h"
#include "stdbool.h"
#include "string.h"
#include "lcd16x2.h"

void initMenu(struct option options[])		  //
{											  // Funkcja do inicjalizacji menu czyli
	strcpy(options[0].name, "1. TEMP/HUM");   // przypisanie poszczególnym opcjom nazwy
	strcpy(options[1].name, "2. TIME/DATE");  //
	strcpy(options[2].name, "3. SET TIME");   // Ilość opcji można zmienić w
	strcpy(options[3].name, "4. SET DATE");   // stałej numOfOptions w main.c
	strcpy(options[4].name, "5. HISTORY");    //
}

int checkIfBack(int currState) // Funkcja sprawdzająca czy przycisk powrotu został wciśnięty podczas odświeżania co sekundę obrazu
{
	for(int i=0;i<20;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 0)
			return 1;
		HAL_Delay(50);
	}
	return currState;
}

void showMenu(int num, int pos, struct option options[] ) // Funkcja do pokazywania opcji wedle pozycji X
{
	if(pos < num-1)  // Jeśli X wskazuje do przedostatniej opcji
	{
		lcd16x2_printf("%s x",options[pos].name);
		lcd16x2_2ndLine();
		lcd16x2_printf("%s",options[pos+1].name);
	}
	else // Jeśli X wskazuje na ostatnią opcje
	{
		lcd16x2_printf("%s",options[pos-1].name);
		lcd16x2_2ndLine();
		lcd16x2_printf("%s x",options[num-1].name);
	}
}

void verifyPosition(int num, int *pos) // Weryfikacja pozycji X aby nie przekraczała dozwolonych opcji
{
	if(*pos >= num-1)   // Jeśli X ma przesunąć się niżej niż ostatnia opcja
		*pos=num-1;     // Ustaw na ostatnią opcję
	else if(*pos == -1) // Jeśli X ma przesunąć się wyżej niż pierwsza opcja
		*pos=0;         // Ustaw pierwszą opcję
}

int action(int num, int *pos) // Funkcja do obsługi przycisków będąc w menu głównym
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) //DOWN
	{
		*pos+=1; // Przesuń pozycje X w dół
		verifyPosition(num, pos);
		HAL_Delay(100);
		return 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0) //UP
	{
		*pos-=1; // Przesuń pozycje X w górę
		verifyPosition(num, pos);
		HAL_Delay(100);
		return 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 0) //MIDDLE
	{
		HAL_Delay(100);
		return *pos+2; // Wybierz opcję pod pozycją X
	}

	return 0; //NONE
}
