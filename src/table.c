#include "table.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string.h>
#include <stdlib.h>

#include "core.h"
#include "update.h"

static Rectangle cases[TABLE_SIZE][COLUMNS];
static int *ids;
static int ids_len;
static textBoxState textState;
static int pos;

void initTable(Produit *tab, int size)
{
    int x, y;
    int width = 100;
    int height = 25;

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            char *data;

            switch (j) {
                case 0:
                    data = strdup(TextFormat("%d", tab[i].id));
                    break;
                case 1:
                    data = strdup(TextFormat("%s", tab[i].nom_produit));
                    break;
                case 2:
                    data = strdup(TextFormat("%s", tab[i].nom_user));
                    break;
                case 3:
                    data = strdup(TextFormat("%.2f", tab[i].prix));
                    break;
                case 4:
                    data = strdup(TextFormat("%d", tab[i].quantite));
                    break;
                case 5:
                    data = strdup(TextFormat("%d", tab[i].seuil));
                    break;
                case 6:
                    data = strdup(TextFormat("%d/%d/%d", tab[i].last_entree.a,
                                                  tab[i].last_entree.m,
                                                  tab[i].last_entree.j));
                    break;
                case 7:
                    data = strdup(TextFormat("%d/%d/%d", tab[i].last_sortie.a,
                                                  tab[i].last_sortie.m,
                                                  tab[i].last_sortie.j));
                    break;
            }

            int temp = MeasureText(data, FONT_SIZE) + 10;
            if (temp > width)
                width = temp;
        }
    }

    int table_h = height * size + size;
    int table_w = width * COLUMNS + COLUMNS;

    x = SCREEN_WIDTH / 2.0f - table_w / 2.0f;
    y = (SCREEN_HEIGHT - STATUS_BAR_WIDTH) / 2.0f - 200 + STATUS_BAR_WIDTH;
    
    for (int i = 0; i < TABLE_SIZE+1; i++) {
        x = SCREEN_WIDTH / 2.0f - table_w / 2.0f;
        for (int j = 0; j < COLUMNS; j++) {
            cases[i][j] = (Rectangle) {x, y, width, height};
            x += width + 1;
        }
        y += height + 1;
    }

    y = (SCREEN_HEIGHT - STATUS_BAR_WIDTH) / 2.0f - 200 + STATUS_BAR_WIDTH;
}

Produit *tableUpdate(Produit *tab, int *size, AppState *state, Produit **select)
{
    float mwm = (GetMouseWheelMove() * SCROLLING_SPEED);

    for (int i = 0; i < *size+1; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            cases[i][j].y += mwm;

            if (*state == ADD) {
                *size += 1;
                tab = realloc(tab, *size * sizeof(Produit)); 
            } else if (!j && i) {
                if (CheckCollisionPointRec(GetMousePosition(), cases[i][j])) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (*state == SELECT) {
                            if (!ids) {
                                ids = malloc(sizeof(int));
                                ids[ids_len++] = tab[i-1].id;
                            } else {
                                if (!idsInclude(tab[i-1].id)) {
                                    ids = realloc(ids, ++ids_len * sizeof(int));
                                    ids[ids_len - 1] = tab[i-1].id;
                                } else {
                                    for (int k = 0; k < ids_len - 1; k++) {
                                        if (ids[k] == tab[i-1].id) {
                                            for (int h = k; h < ids_len - 1; h++) {
                                                ids[h] = ids[h + 1];
                                            }
                                            ids_len--;
                                        }
                                    }
                                    if (ids[ids_len - 1] == tab[i-1].id)
                                        ids_len--;
                                    ids = (int *) realloc(ids, ids_len * sizeof(int));
                                }
                            }
                        } else {
                            *state = UPDATE;
                            *select = (tab + i - 1) ;
                        }
                    }
                    else {
                        *select = NULL;
                    }
                }
            } else if (!i) {
                if (CheckCollisionPointRec(GetMousePosition(), cases[i][j])) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (j == 3)
                            triProduitPrix(tab, *size);
                        else if (j == 1) 
                            triProduitNom(tab, *size);
                    }
                }
            }
        }
    }
    return tab;
}

Produit * tableDraw(Produit *tab, int *size, AppState *state, char *userName)
{
    char *data;
    Color color1, color2;
    int n = *size;
    ClearBackground(RAYWHITE);
    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (i == 0) {
                color1 = NEARBLACK;
                color2 = RAYWHITE;
                switch (j) {
                    case 0:
                        data = strdup("ID");
                        break;
                    case 1:
                        data = strdup("Nom Produit");
                        break;
                    case 2:
                        data = strdup("Nom Utilisateur");
                        break;
                    case 3:
                        data = strdup("Prix");
                        break;
                    case 4:
                        data = strdup("Quantite");
                        break;
                    case 5:
                        data = strdup("Seuil");
                        break;
                    case 6:
                        data = strdup("Date dernier entree");
                        break;
                    case 7:
                        data = strdup("Date dernier sortie");
                        break;
                }
            } else {
                switch (j) {
                    case 0:
                        data = strdup(TextFormat("%d", tab[i-1].id));
                        break;
                    case 1:
                        data = strdup(TextFormat("%s", tab[i-1].nom_produit));
                        break;
                    case 2:
                        data = strdup(TextFormat("%s", tab[i-1].nom_user));
                        break;
                    case 3:
                        data = strdup(TextFormat("%.2f", tab[i-1].prix));
                        break;
                    case 4:
                        data = strdup(TextFormat("%d", tab[i-1].quantite));
                        break;
                    case 5:
                        data = strdup(TextFormat("%d", tab[i-1].seuil));
                        break;
                    case 6:
                        data = strdup(TextFormat("%d/%d/%d", tab[i-1].last_entree.a,
                                                    tab[i-1].last_entree.m,
                                                    tab[i-1].last_entree.j));
                        break;
                    case 7:
                        data = strdup(TextFormat("%d/%d/%d", tab[i-1].last_sortie.a,
                                                    tab[i-1].last_sortie.m,
                                                    tab[i-1].last_sortie.j));
                        break;
                }
                color1 = RAYWHITE;
                color2 = NEARBLACK;
            }
            DrawRectangleRec(cases[i][j], color1);
            DrawRectangleLines(cases[i][j].x - 1, cases[i][j].y - 1, cases[i][j].width + 2, cases[i][j].height + 2, NEARBLACK);
            DrawText(data, cases[i][j].x + 4, cases[i][j].y + 4, (i) ? FONT_SIZE : FONT_SIZE_SMALL, color2);
        }
        if ((pos == i-1) && i && *state == SEARCH) {
            DrawCircle(cases[i][0].x - 10, cases[i][0].y + 10, 5, BLUE);
        }
    }
    DrawRectangle(0, 0, SCREEN_WIDTH, STATUS_BAR_WIDTH, LIGHTGRAY);
    if (GuiButton((Rectangle) {10, 10, 50, 50}, "Ajouter"))  tab = ajouterProduit(tab, size, userName);
    if (GuiButton((Rectangle) {10 + 50 + 2, 10, 50, 50}, "Rechercher")) {
        if (*state == DISPLAY) {
            *state = SEARCH;
            initSearchScreen(); 
        }
        else
            *state = DISPLAY;
    }
    if (GuiButton((Rectangle) {10 + 2 * 50 + 4, 10, 50, 50}, "Supprimer")) {
        if (*state == DISPLAY)
            *state = SELECT;
        else
            *state = DISPLAY;
    }
    return tab;
}

void initIDS(void)
{
    ids = NULL;
    ids_len = 0;
}

void drawSelectScreen(Produit *tab, int *size, AppState *state)
{
    for (int i = 0; i < *size+1; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (i && idsInclude(tab[i-1].id)) {
                DrawCircle(cases[i][j].x - 10, cases[i][j].y + 10, 5, RED);
            }
        }
    }

    if (GuiButton((Rectangle) {SCREEN_WIDTH - 155, SCREEN_HEIGHT - 60, 150, 50}, "Supprimer")) {
        for (int i = 0; i < ids_len; i++) {
            supprimerProduit(tab, size, ids[i]);
        }
        
        *state = DISPLAY;
        memset(ids, 0, ids_len * sizeof(int));
        ids_len = 0;
        ids = realloc(ids, ids_len *sizeof(int));
    }
}

bool idsInclude(int id) {
    for (int i = 0; i < ids_len; i++) {
        if (ids[i] == id)
            return true;
    }
    return false;
}

void initSearchScreen(void) 
{
    textState.frameCounter = 0;
    textState.mouseOnText = false;
    strcpy(textState.buffer, "");
    textState.letterCount = strlen(textState.buffer);

    pos = -1;
}

void drawSearchScreen(Produit *tab, int *size, AppState *state) 
{
    char *text = "Nom Produit : ";
    DrawText(text, 300, 30, 20, BLACK);
    textInput((Rectangle) {450, 10, 250, 50}, &textState, true);
    if (GuiButton((Rectangle) {SCREEN_WIDTH - 65, 10, 60, 50}, "Recherche")) {
        char *nom = textState.buffer;
        pos = rechercheProduit(tab, *size, nom);
        if (pos == -1) {
            DrawText("Produit non trouve", 300, 100, 20, BLACK);
        }
    }
}