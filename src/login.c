#include "login.h"

#include "raygui.h"
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 15


static char name[MAX_INPUT_CHARS + 1] = "\0";
static int letterCount = 0;
static bool mouseOnText = false;
static int frameCounter = 0;


Rectangle getTextBox(void)
{
    int val = MAX_INPUT_CHARS * 200 / 8;

    float x = SCREEN_WIDTH / 2.0f - val / 2.0f;
    float y = SCREEN_HEIGHT / 2.0f - 25;
    float width = val;
    float height = 50;

    return (Rectangle) {x, y, width, height};
}

void loginUpdate(AppState *state, char **output)
{
    char *temp;
    if (CheckCollisionPointRec(GetMousePosition(), getTextBox())) mouseOnText = true;
        else mouseOnText = false;

    if (mouseOnText) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();

        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                name[letterCount] = (char) key;
                name[letterCount + 1] = '\0';
                letterCount++;
            }

            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (mouseOnText) frameCounter++;
    else frameCounter = 0;

    if (GuiButton((Rectangle) {SCREEN_WIDTH / 2 - 100 , SCREEN_HEIGHT - 100, 200, 50}, "Login")) {
        *state = DISPLAY;
        // strcpy(*output, name);
        *output = strdup(name);
    }
}

void loginDraw(void)
{
    Rectangle textBox = getTextBox();
    
    ClearBackground(RAYWHITE);

    DrawText("SAISIR LE NOM D'UTILISATEUR", 300, 140, 20, GRAY);

    DrawRectangleRec(textBox, LIGHTGRAY);
    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

    DrawText(name, (int) textBox.x + 5, (int) textBox.y + 8, 40, MAROON);

    if (mouseOnText) {
        if (letterCount < MAX_INPUT_CHARS) {
            if (((frameCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
        } else {
            DrawText("Press BACKSPACE to delete chars...", 230, 350, 20, GRAY);
        }
    }
}

char *getUserName(void)
{
    return name;
}