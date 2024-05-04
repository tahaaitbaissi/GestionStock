#ifndef LOGIN_H_
#define LOGIN_H_

#include <raylib.h>
#include "defines.h"

Rectangle getTextBox(void);
void loginUpdate(AppState *state, char **output);
void loginDraw(void);
char *getUserName(void);

#endif