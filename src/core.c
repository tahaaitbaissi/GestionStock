#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "update.h"

static Rectangle arr[8][2];
static textBoxState states[8];

Produit getProduit(void)
{
    Produit x;
    printf("Saisir les informations:\n");
    printf("\tID : ");
    scanf("%d", &x.id);
    printf("\tNom de produit : ");
    scanf("%s", x.nom_produit);
    printf("\tNom de l'utilisateur : ");
    scanf("%s", x.nom_user);
    printf("\tPrix : ");
    scanf("%f", &x.prix);
    printf("\tQuantite : ");
    scanf("%d", &x.quantite);
    printf("\tSeuil : ");
    scanf("%d", &x.seuil);
    x.last_entree.j = 0;
    x.last_entree.m = 0;
    x.last_entree.a = 0;
    x.last_sortie.j = 0;
    x.last_sortie.m = 0;
    x.last_sortie.a = 0;
    return x;
}

void printProduit(Produit x)
{
    printf("\tID : %d\n", x.id);
    printf("\tNom de produit : %s\n", x.nom_produit);
    printf("\tNom de utilisateur : %s\n", x.nom_user);
    printf("\tPrix : %.2f\n", x.prix);
    printf("\tQuantite : %d\n", x.quantite);
    printf("\tSeuil : %d\n", x.seuil);
    printf("\tDate de derniere entree: %d/%d/%d\n", 
           x.last_entree.a,
           x.last_entree.m,
           x.last_entree.j);
     printf("\tDate de derniere sortie: %d/%d/%d\n", 
           x.last_sortie.a,
           x.last_sortie.m,
           x.last_sortie.j);   
}

void initAddScreen(void) {
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
        strcpy(states[i].buffer, "");
        states[i].letterCount = strlen(states[i].buffer);
    }
}

int getID(Produit *tab, int size)
{
    int max;
    if (!size)
        return 1;
    max = tab[0].id;
    for (int i = 1; i < size; i++) {
        if (tab[i].id > max)
            max = tab[i].id;
    }
    return max + 1;
}

Produit *ajouterProduit(Produit *tab, int *size, char *userName)
{
    int N = *size;
    int h, id = getID(tab, N);

    char temp[1024], *rest, *token;
    if (N == 0)
        tab = (Produit *) malloc(++N * sizeof(Produit));
    else
        tab = (Produit *) realloc(tab, ++N * sizeof(Produit));

    if (!tab) {
        printf("ERREUR : Allocation.\n");
        return NULL;
    }

    EndDrawing();
    initAddScreen();
    while (!WindowShouldClose()) {   
        BeginDrawing(); 
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
                    if (!i) {
                        strcpy(states[i].buffer, TextFormat("%d", id));
                        textInput(arr[i][j], states + i, false);
                    } else if (i == 2) {
                        strcpy(states[i].buffer, userName);
                        textInput(arr[i][j], states + i, false);
                    } else
                        textInput(arr[i][j], states + i, true);
                }
            }
        }
        if (GuiButton((Rectangle) {SCREEN_WIDTH - 155, SCREEN_HEIGHT - 60, 150, 50}, "Confirmer")) {
            for (int k = 0; k < 8; k++) {
                switch (k)
                {
                case 0:
                    tab[N-1].id = atoi(states[k].buffer);
                    break;
                
                case 1:
                    strcpy(tab[N-1].nom_produit, states[k].buffer);
                    break;

                case 2:
                    strcpy(tab[N-1].nom_user, states[k].buffer);
                    break;

                case 3:
                    tab[N-1].prix = atof(states[k].buffer);
                    break;
                
                case 4:
                    tab[N-1].quantite = atoi(states[k].buffer);
                    break;

                case 5:
                    tab[N-1].seuil = atoi(states[k].buffer);
                    break;

                case 6:
                    strcpy(temp, states[k].buffer);
                    rest = temp;
                    h = 0;
                    while ((token = strtok_r(rest, "/", &rest))) {
                        if (h == 0)
                            tab[N-1].last_entree.a = atoi(token);
                        else if (h == 1)
                            tab[N-1].last_entree.m = atoi(token);
                        else
                            tab[N-1].last_entree.j = atoi(token);
                        h++;
                    }
                    break;

                case 7:
                    strcpy(temp, states[k].buffer);
                    rest = temp;
                    h = 0;
                    while ((token = strtok_r(rest, "/", &rest))){
                        if (h == 0)
                            tab[N-1].last_sortie.a = atoi(token);
                        else if (h == 1)
                            tab[N-1].last_sortie.m = atoi(token);
                        else
                            tab[N-1].last_sortie.j = atoi(token);
                        h++;
                    }
                    break;
                }
            }
            *size = N;
            return tab;
        }
        EndDrawing();
    }

    *size = N;
    return tab;
}

void supprimerProduit(Produit *tab, int *size , int id)
{
    int N = *size;
    int i, j;

    if (!N) {
        printf("ERREUR : Tableau vide (Supprimer).\n");
        return;
    } else if (!checkIdExiste(tab, N, id)) {
        printf("ERREUR : Aucun Produit avec ce id (%d) est trouvee.\n", id);
        return;
    }

    for (i = 0; i < N - 1; i++) {
        if (tab[i].id == id) {
            for (j = i; j < N - 1; j++) {
                tab[j] = tab[j + 1];
            }
            N--;
        }
    }
    if (tab[N - 1].id == id)
        N--;
    tab = (Produit *) realloc(tab, N * sizeof(Produit));
    *size = N;
}

bool checkIdExiste(Produit *tab, int size, int id)
{
    int i;
    for (i = 0; i < size; i++) {
        if (tab[i].id == id)
            return true;
    }
    return false;
}

void modifierProduit(Produit *tab, int size, int id)
{
    int i;

    if (!size) {
        printf("ERREUR : Tableau vide (Modifier).\n");
        return;
    } else if (!checkIdExiste(tab, size, id)) {
        printf("ERREUR : Aucun Produit avec ce id (%d) est trouvee.\n", id);
        return;
    }

    for (i = 0; i < size; i++) {
        if (tab[i].id == id) {
            tab[i] = getProduit();
            return;
        }
    }
}

int rechercheProduit(Produit *tab, int size, char *nom)
{
    int i;
    for (i = 0; i < size; i++) {
        if (!strcmp(tab[i].nom_produit, nom)) {
            return i;
        }
    }
    return -1;
}

void triProduitNom(Produit *tab, int size)
{
    int i , j, min;
    Produit temp;
    for (i = 0; i < size - 1; i++)
    {
        min = i;
        for (j = i + 1; j < size; j++)
        {
            if (strcmp(tab[j].nom_produit, tab[min].nom_produit) < 0)
                min = j;
        }
        if (min != i)
        {
            temp = tab[i];
            tab[i] = tab[min];
            tab[min] = temp;
        }
    }
}

void triProduitPrix(Produit *tab, int size)
{
    int i , j, min;
    Produit temp;
    for (i = 0; i < size - 1; i++)
    {
        min = i;
        for (j = i + 1; j < size; j++)
        {
            if (tab[j].prix < tab[min].prix)
                min = j;
        }
        if (min != i)
        {
            temp = tab[i];
            tab[i] = tab[min];
            tab[min] = temp;
        }
    }
}

void save_db(Produit *tab, int size)
{
    int i;
    FILE *fp = fopen(DB_PATH, "w");
    if (!fp) {
        printf("ERREUR : Cant open file %s.\n", DB_PATH);
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(fp, "%d, %s, %s, %.2f, %d, %d, %d/%d/%d, %d/%d/%d\n",
                tab[i].id,
                tab[i].nom_produit,
                tab[i].nom_user,
                tab[i].prix,
                tab[i].quantite,
                tab[i].seuil,
                tab[i].last_entree.a,
                tab[i].last_entree.m,
                tab[i].last_entree.j,
                tab[i].last_sortie.a,
                tab[i].last_sortie.m,
                tab[i].last_sortie.j);
    }

    fclose(fp);
}

void load_db(Produit *tab)
{
    FILE *fp = fopen(DB_PATH, "r");

    Produit x;

    char *rest, temp[1024];
    char *token;
    int i = 0;

    if (!fp) {
        printf("Cant open file.\n");
        return;
    }

    char buffer[1024];

    int row = 0, col = 0;
    while (fgets(buffer, 1024, fp)) {
        col = 0;
        row++;

        char *value = strtok(buffer, ", ");
        while (value) {
            switch (col)
            {
            case 0:
                x.id = atoi(value);
                break;
            
            case 1:
                strcpy(x.nom_produit, value);
                break;

            case 2:
                strcpy(x.nom_user, value);
                break;

            case 3:
                x.prix = atof(value);
                break;
            
            case 4:
                x.quantite = atoi(value);
                break;

            case 5:
                x.seuil = atoi(value);
                break;

            case 6:
                strcpy(temp, value);
                rest = temp;
                i = 0;
                while ((token = strtok_r(rest, "/", &rest))) {
                    if (i == 0)
                        x.last_entree.a = atoi(token);
                    else if (i == 1)
                        x.last_entree.m = atoi(token);
                    else
                        x.last_entree.j = atoi(token);
                    i++;
                }
                break;

            case 7:
                strcpy(temp, value);
                rest = temp;
                i = 0;
                while ((token = strtok_r(rest, "/", &rest))){
                    if (i == 0)
                        x.last_sortie.a = atoi(token);
                    else if (i == 1)
                        x.last_sortie.m = atoi(token);
                    else
                        x.last_sortie.j = atoi(token);
                    i++;
                }
                break;
            }
            value = strtok(NULL, ", ");
            col++;
        }
        tab[row - 1] = x;
    }
    printf("LOG: Loaded %d items.\n", row);
    fclose(fp);
}

int size_db(void)
{
    FILE *fp = fopen(DB_PATH, "r");
    int i = 0;

    if (!fp) {
        printf("Cant open file.\n");
        return 0;
    }

    char buffer[1024];

    while (fgets(buffer, 1024, fp)) {
        i++;
    }
    return i;
}