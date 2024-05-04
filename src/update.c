#include "update.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "raygui.h"

static Rectangle arr[8][2];
static textBoxState states[8];

void textInput(Rectangle textBox, textBoxState *state, bool flag) {
    Color color;

    if (CheckCollisionPointRec(GetMousePosition(), textBox) && flag) state->mouseOnText = true;
    else state->mouseOnText = false;

    if (state->mouseOnText) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();

        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (state->letterCount < MAX_INPUT_CHARS)) {
                state->buffer[state->letterCount] = (char) key;
                state->buffer[state->letterCount + 1] = '\0';
                (state->letterCount) += 1;
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE)) {
            (state->letterCount) -= 1;
            if (state->letterCount < 0) state->letterCount = 0;
            state->buffer[state->letterCount] = '\0';
        }
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (state->mouseOnText) state->frameCounter++;
    else state->frameCounter = 0;

    DrawRectangleRec(textBox, LIGHTGRAY);
    if (state->mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
    
    if (!flag) color = GRAY;
    else color = MAROON;
    
    DrawText(state->buffer, (int) textBox.x + 5, (int) textBox.y + 8, 40, color);

    if (state->mouseOnText) {
        if (state->letterCount < MAX_INPUT_CHARS) {
            if (((state->frameCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(state->buffer, 40), (int)textBox.y + 12, 40, MAROON);
        }
    }

}

void initUpdateScreen(Produit prod) {
    int width = 200;
    int height = 50;

    int table_h = height * 8;
    int table_w = width * 2;

    int x = SCREEN_WIDTH / 2.0f - table_w / 2.0f;
    int y = (SCREEN_HEIGHT - STATUS_BAR_WIDTH) / 2.0f - 200;
    
    for (int i = 0; i < 8; i++) {
        x = SCREEN_WIDTH / 2.0f - table_w / 2.0f;
        for (int j = 0; j < 2; j++) {
            arr[i][j] = (Rectangle) {x, y, width, height};
            x += width + 15;
        }
        y += height + 5;
        states[i].frameCounter = 0;
        states[i].mouseOnText = false;
        switch (i) {
            case 0:
                strcpy(states[i].buffer, TextFormat("%d", prod.id));
                break;
            case 1:
                strcpy(states[i].buffer, prod.nom_produit);                
                break;
            case 2:
                strcpy(states[i].buffer, prod.nom_user);
                break;
            case 3:
                strcpy(states[i].buffer, TextFormat("%.2f", prod.prix));
                break;
            case 4:
                strcpy(states[i].buffer, TextFormat("%d", prod.quantite));
                break;
            case 5:
                strcpy(states[i].buffer, TextFormat("%d", prod.seuil));
                break;
            case 6:
                strcpy(states[i].buffer, TextFormat("%d/%d/%d", prod.last_entree.a,
                                                    prod.last_entree.m,
                                                    prod.last_entree.j));
                break;
            case 7:
                strcpy(states[i].buffer, TextFormat("%d/%d/%d", prod.last_sortie.a,
                                                    prod.last_sortie.m,
                                                    prod.last_sortie.j));
                break;
        }
        states[i].letterCount = strlen(states[i].buffer);
    }

}

void updateScreen() {
    return;
}

void drawUpdateScreen(AppState *state, Produit *select) {
    ClearBackground(RAYWHITE);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j++) {
            if (!j) {
                char *text;
                switch (i) {
                    case 0:
                        text = strdup("ID :");
                        break;
                    case 1:
                        text = strdup("Nom Produit :");
                        break;
                    case 2:
                        text = strdup("Nom utilisateur :");
                        break;
                    case 3:
                        text = strdup("Prix :");
                        break;
                    case 4:
                        text = strdup("Quantite :");
                        break;
                    case 5:
                        text = strdup("Seuil :");
                        break;
                    case 6:
                        text = strdup("Derniere entree :");
                        break;
                    case 7:
                        text = strdup("Derniere sortie :");
                        break;
                }
                DrawText(text, arr[i][j].x + 2, arr[i][j].y + 8, 20, GRAY);
            } else {
                if (!i) textInput(arr[i][j], states + i, false); 
                else textInput(arr[i][j], states + i, true);
            }
        }
    }
    if (GuiButton((Rectangle) {SCREEN_WIDTH - 155, SCREEN_HEIGHT - 60, 150, 50}, "Confirmer")) {
        char temp[1024], *rest, *token;
        int h;
        for (int k = 0; k < 8; k++) {
            switch (k)
            {
            case 0:
                select->id = atoi(states[k].buffer);
                break;
            
            case 1:
                strcpy(select->nom_produit, states[k].buffer);
                break;

            case 2:
                strcpy(select->nom_user, states[k].buffer);
                break;

            case 3:
                select->prix = atof(states[k].buffer);
                break;
            
            case 4:
                select->quantite = atoi(states[k].buffer);
                break;

            case 5:
                select->seuil = atoi(states[k].buffer);
                break;

            case 6:
                strcpy(temp, states[k].buffer);
                rest = temp;
                h = 0;
                while ((token = strtok_r(rest, "/", &rest))) {
                    if (h == 0)
                        select->last_entree.a = atoi(token);
                    else if (h == 1)
                        select->last_entree.m = atoi(token);
                    else
                        select->last_entree.j = atoi(token);
                    h++;
                }
                break;

            case 7:
                strcpy(temp, states[k].buffer);
                rest = temp;
                h = 0;
                while ((token = strtok_r(rest, "/", &rest))){
                    if (h == 0)
                        select->last_sortie.a = atoi(token);
                    else if (h == 1)
                        select->last_sortie.m = atoi(token);
                    else
                        select->last_sortie.j = atoi(token);
                    h++;
                }
                break;
            }
        }
        *state = DISPLAY;
    }
}