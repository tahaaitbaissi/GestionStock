#ifndef UPDATE_H_
#define UPDATE_H_

#include <raylib.h>
#include "defines.h"

void textInput(Rectangle textBox, textBoxState *state, bool flag);
void initUpdateScreen(Produit prod);
void drawUpdateScreen(AppState *state, Produit *select);

#endif