#ifndef CORE_H_
#define CORE_H_

#include "defines.h"
#include <stdbool.h>

void printProduit(Produit x);
Produit getProduit(void);
Produit *ajouterProduit(Produit *tab, int *size, char *userName);
void supprimerProduit(Produit *tab, int *size, int id);
bool checkIdExiste(Produit *tab, int size, int id);
void modifierProduit(Produit *tab, int size, int id);
int rechercheProduit(Produit *tab, int size, char *nom);
void triProduitNom(Produit *tab, int size);
void triProduitPrix(Produit *tab, int size);
void save_db(Produit *tab, int size);
void load_db(Produit *tab);
int size_db(void);

#endif