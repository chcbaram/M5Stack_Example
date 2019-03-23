#ifndef _MENU_H_
#define _MENU_H_

#include "hangul.h"


void menuInit(void);
void menuAdd(const char *menu_str, void (*setup_func)(void) = NULL, void (*loop_func)(void) = NULL);
void menuUpdate(void);

#endif