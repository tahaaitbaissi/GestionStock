#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "table.h"
#include "update.h"
#include "defines.h"
#include "core.h"

int main(void)
{
    int n = size_db();
    Produit *arr = NULL;
    AppState state = LOGIN;
    Produit *select = NULL;

    char *userName = NULL;

    arr = (Produit *) malloc(n * sizeof(Produit));
    if (!arr) {
        printf("ERREUR : Allocation.\n");
        return 1;
    }

    load_db(arr);
    initTable(arr, n);
    initIDS();
    initSearchScreen();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stock");
    SetTargetFPS(60);
    int i = 0;
    
    while (!WindowShouldClose())
    {
        if (state == LOGIN) {
            loginUpdate(&state, &userName);
            BeginDrawing();
                loginDraw();
            EndDrawing();
        } else {
            if (state == UPDATE) {
                if (!i)
                    initUpdateScreen(*select);
                i++;
            } else {
                i = 0;
                arr = tableUpdate(arr, &n, &state, &select);
            }
            BeginDrawing();
                if (state == UPDATE)
                    drawUpdateScreen(&state, select);
                else if (state == SELECT) {
                    drawSelectScreen(arr, &n, &state);
                    arr = tableDraw(arr, &n, &state, userName);
                } else if (state == SEARCH) {
                    arr = tableDraw(arr, &n, &state, userName);
                    drawSearchScreen(arr, &n, &state);
                } else
                    arr = tableDraw(arr, &n, &state, userName);
            EndDrawing();
        }
    }

    CloseWindow();
    save_db(arr, n);
    free(arr);

    return 0;
}