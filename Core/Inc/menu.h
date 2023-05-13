/*
 * menu.h
 *
 *  Created on: May 10, 2023
 *      Author: Filip
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_
#include "stdbool.h"

struct option
{
	char name[16];
};

void initMenu(struct option options[]);
void showMenu(int num, int pos, struct option options[] );
void verifyPosition(int num, int *pos);
int action(int num, int *pos);
int checkIfBack(int currState);

#endif /* SRC_MENU_H_ */
