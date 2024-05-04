#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define STATUS_BAR_WIDTH 75
#define FONT_SIZE 20
#define FONT_SIZE_SMALL 10
#define SCROLLING_SPEED 20

#define COLUMNS 8
#define TABLE_SIZE 100

#define MAX_INPUT_CHARS 10

#define NEARBLACK CLITERAL(Color) {15, 15, 15, 255}

#define DB_PATH "db.csv"

typedef enum {
    LOGIN,
    DISPLAY,
    UPDATE,
    ADD,
    SELECT,
    SEARCH
} AppState;

typedef struct {
    char buffer[MAX_INPUT_CHARS + 1];
    int letterCount;
    bool mouseOnText;
    int frameCounter;
} textBoxState;

typedef enum {
    ID_INPUT,
    UPDATING,
    ERROR
} UpdateState;

typedef struct {
    int j, m, a;
} Date;

typedef struct {
    int id;
    char nom_produit[30];
    char description[100];
    char nom_user[30];
    float prix;
    int quantite;
    int seuil;
    Date last_entree;
    Date last_sortie;
} Produit;

#endif