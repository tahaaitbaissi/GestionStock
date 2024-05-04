#ifndef TABLE_H_
#define TABLE_H_

#include "defines.h"

void initTable(Produit *tab, int size);
Produit * tableUpdate(Produit *tab, int *size, AppState *state, Produit **select);
Produit * tableDraw(Produit *tab, int *size, AppState *state, char *userName);
void drawSelectScreen(Produit *tab, int *size, AppState *state);
void initIDS(void);
bool idsInclude(int id);
void initSearchScreen(void);
void drawSearchScreen(Produit *tab, int *size, AppState *state);

#endif