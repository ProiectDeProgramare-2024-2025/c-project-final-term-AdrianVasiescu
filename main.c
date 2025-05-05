#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"

typedef struct {
    char nume[30];
    int cantitate;
    float pret;
} Produs;

typedef struct {
    char nume[30];
    int cantitateRezervata;
} Rezervare;

void clearScreen() {
    system("clear"); 
}

void pause() {
    printf("\nApasă Enter pentru a reveni la meniu...");
    getchar(); getchar(); 
}

void afisareProduse(const char path[]) {
    clearScreen();
    FILE *filePointer = fopen(path, "r");

    printf("--------------\n");
    printf("Produse disponibile\n");
    printf("--------------\n");

    if (filePointer == NULL) {
        printf(RED "Eroare: fisierul nu poate fi accesat.\n" RESET);
    } else {
        char linie[100];
        int index = 1;
        while (fgets(linie, sizeof(linie), filePointer)) {
            linie[strcspn(linie, "\n")] = '\0';
            Produs p;
            sscanf(linie, "%[^,],%d,%f", p.nume, &p.cantitate, &p.pret);
            printf("%d. %s%s%s - %s%.2f RON%s (%s%d buc%s)\n", index++, CYAN, p.nume, RESET, GREEN, p.pret, RESET, YELLOW, p.cantitate, RESET);
        }
        fclose(filePointer);
    }

    printf("--------------\n");
    pause();
}

void afisareRezervari() {
    clearScreen();
    FILE *filePointer = fopen("rezervari.txt", "r");

    printf("--------------\n");
    printf("Produse rezervate\n");
    printf("--------------\n");

    if (filePointer == NULL) {
        printf(RED "Nu există rezervări sau fisierul nu poate fi deschis.\n" RESET);
        pause();
        return;
    }

    char linie[100];
    int index = 1;
    while (fgets(linie, sizeof(linie), filePointer)) {
        Rezervare r;
        sscanf(linie, "%[^,],%d", r.nume, &r.cantitateRezervata);
        printf("%d. %s%s%s (%s%d buc%s)\n", index++, CYAN, r.nume, RESET, YELLOW, r.cantitateRezervata, RESET);
    }

    fclose(filePointer);
    printf("--------------\n");
    pause();
}

void adaugaProdus(const char path[]) {
    clearScreen();
    FILE *filePointer = fopen(path, "a");

    if (filePointer == NULL) {
        printf(RED "Eroare: fisierul nu poate fi accesat.\n" RESET);
        pause();
        return;
    }

    Produs p;
    printf(" Introdu numele produsului: ");
    getchar();
    scanf("%[^\n]", p.nume);

    printf(" Introdu cantitatea: ");
    scanf("%d", &p.cantitate);

    printf(" Introdu prețul: ");
    scanf("%f", &p.pret);

    fprintf(filePointer, "%s,%d,%.2f\n", p.nume, p.cantitate, p.pret);
    fclose(filePointer);

    printf(GREEN "\nProdusul %s a fost adăugat cu succes!\n" RESET, p.nume);
    pause();
}

void stergeProdus(const char path[]) {
    clearScreen();
    FILE *filePointer = fopen(path, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (filePointer == NULL || tempFile == NULL) {
        printf(RED "Eroare: fisierul nu poate fi accesat.\n" RESET);
        pause();
        return;
    }

    char numeCautat[30];
    printf("Introdu numele produsului de șters: ");
    getchar();
    scanf("%[^\n]", numeCautat);

    char linie[100];
    int gasit = 0;

    while (fgets(linie, sizeof(linie), filePointer)) {
        Produs p;
        sscanf(linie, "%[^,],%d,%f", p.nume, &p.cantitate, &p.pret);

        if (strcmp(p.nume, numeCautat) != 0) {
            fprintf(tempFile, "%s,%d,%.2f\n", p.nume, p.cantitate, p.pret);
        } else {
            gasit = 1;
        }
    }

    fclose(filePointer);
    fclose(tempFile);

    remove(path);
    rename("temp.txt", path);

    if (gasit)
        printf(GREEN "\nProdusul %s a fost șters cu succes!\n" RESET, numeCautat);
    else
        printf(RED "\nProdusul %s nu a fost găsit.\n" RESET, numeCautat);

    pause();
}

void cautaProdus(const char path[]) {
    clearScreen();
    FILE *filePointer = fopen(path, "r");

    if (filePointer == NULL) {
        printf(RED "Eroare: fisierul nu poate fi accesat.\n" RESET);
        pause();
        return;
    }

    char numeCautat[30];
    printf(" Introdu numele produsului căutat: ");
    getchar();
    scanf("%[^\n]", numeCautat);

    char linie[100];
    int gasit = 0;

    while (fgets(linie, sizeof(linie), filePointer)) {
        Produs p;
        sscanf(linie, "%[^,],%d,%f", p.nume, &p.cantitate, &p.pret);
        if (strcmp(p.nume, numeCautat) == 0) {
            printf("%s%s%s - %s%.2f RON%s (%s%d buc%s)\n", CYAN, p.nume, RESET, GREEN, p.pret, RESET, YELLOW, p.cantitate, RESET);
            gasit = 1;
        }
    }

    fclose(filePointer);

    if (!gasit)
        printf(RED "\nProdusul %s nu a fost găsit.\n" RESET, numeCautat);

    pause();
}

void rezervaProdus(const char path[]) {
    clearScreen();
    FILE *filePointer = fopen(path, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *rezFile = fopen("rezervari.txt", "a");

    if (filePointer == NULL || tempFile == NULL || rezFile == NULL) {
        printf(RED "Eroare: fisierul nu poate fi accesat.\n" RESET);
        pause();
        return;
    }

    char numeCautat[30];
    int cantitateDorita;
    int gasit = 0;

    printf(" Numele produsului de rezervat: ");
    getchar();
    scanf("%[^\n]", numeCautat);
    printf("Cantitatea dorită: ");
    scanf("%d", &cantitateDorita);

    char linie[100];

    while (fgets(linie, sizeof(linie), filePointer)) {
        Produs p;
        sscanf(linie, "%[^,],%d,%f", p.nume, &p.cantitate, &p.pret);

        if (strcmp(p.nume, numeCautat) == 0) {
            gasit = 1;
            if (cantitateDorita > p.cantitate) {
                printf(RED " Nu sunt suficiente produse în stoc!\n" RESET);
                fclose(filePointer);
                fclose(tempFile);
                fclose(rezFile);
                remove("temp.txt");
                pause();
                return;
            } else {
                p.cantitate -= cantitateDorita;
                fprintf(tempFile, "%s,%d,%.2f\n", p.nume, p.cantitate, p.pret);
                fprintf(rezFile, "%s,%d\n", p.nume, cantitateDorita);
            }
        } else {
            fprintf(tempFile, "%s,%d,%.2f\n", p.nume, p.cantitate, p.pret);
        }
    }

    fclose(filePointer);
    fclose(tempFile);
    fclose(rezFile);

    remove(path);
    rename("temp.txt", path);

    if (gasit)
        printf(GREEN "\nProdusul %s a fost rezervat cu succes!\n" RESET, numeCautat);
    else
        printf(RED "\nProdusul %s nu a fost găsit.\n" RESET, numeCautat);

    pause();
}

void meniu(int op, const char path[]) {
    switch (op) {
        case 1: adaugaProdus(path); break;
        case 2: afisareProduse(path); break;
        case 3: stergeProdus(path); break;
        case 4: cautaProdus(path); break;
        case 5: rezervaProdus(path); break;
        case 6: afisareRezervari(); break;
        default:
            printf(RED " Opțiune invalidă.\n" RESET);
            pause();
            break;
    }
}

int main() {
    char path[] = "produse.txt";
    int optiune;

    do {
        clearScreen();
        printf("-------MENIU-------\n");
        printf("1 - Adăugare produs\n");
        printf("2 - Afișare produse\n");
        printf("3 - Ștergere produs\n");
        printf("4 - Căutare produs\n");
        printf("5 - Rezervare produs\n");
        printf("6 - Afișare produse rezervate\n");
        printf("0 - Ieșire\n");
        printf("----------------\n");

        printf("Introdu opțiunea: ");
        if (scanf("%d", &optiune) != 1) {
            printf(RED "Input invalid. Introdu un număr.\n" RESET);
            break;
        }

        if (optiune < 0 || optiune > 6) {
            printf(RED "Opțiune invalidă. Vrei să revii la meniu? (y/n): " RESET);
            char rasp;
            getchar();
            scanf(" %c", &rasp);
            if (rasp == 'y' || rasp == 'Y') continue;
            else break;
        }

        meniu(optiune, path);

    } while (optiune != 0);

    return 0;
}

