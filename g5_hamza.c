#include <stdio.h>
#include <stdbool.h>

#define N 9

typedef struct {
    int grille[N][N];
} Sudoku;

bool est_valide(Sudoku sudoku, int ligne, int col, int num) {
    for (int i = 0; i < N; i++) {
        if (sudoku.grille[ligne][i] == num || sudoku.grille[i][col] == num) {
            return false;
        }
    }

    int startRow = ligne - ligne % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku.grille[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

bool remplir_grille(Sudoku *sudoku) {
    int ligne, col;
    bool trouve_case_vide = false;

    for (ligne = 0; ligne < N; ligne++) {
        for (col = 0; col < N; col++) {
            if (sudoku->grille[ligne][col] == 0) {
                trouve_case_vide = true;
                break;
            }
        }
        if (trouve_case_vide) break;
    }

    if (!trouve_case_vide) return true;

    for (int num = 1; num <= 9; num++) {
        if (est_valide(*sudoku, ligne, col, num)) {
            sudoku->grille[ligne][col] = num;
            if (remplir_grille(sudoku)) return true;
            sudoku->grille[ligne][col] = 0;
        }
    }
    return false;
}

void generer_grille(Sudoku *sudoku) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku->grille[i][j] = 0;
        }
    }
    remplir_grille(sudoku);
}

void afficher_grille(Sudoku sudoku) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (sudoku.grille[i][j] == 0)
                printf(". ");
            else
                printf("%d ", sudoku.grille[i][j]);
        }
        printf("\n");
    }
}

bool saisir_valeur(Sudoku *sudoku) {
    int ligne, col, val;
    printf("Entrez ligne (0-8), colonne (0-8) et valeur (1-9): ");
    scanf("%d %d %d", &ligne, &col, &val);

    if (ligne < 0 || ligne >= N || col < 0 || col >= N || val < 1 || val > 9 || sudoku->grille[ligne][col] != 0)
        return false;

    sudoku->grille[ligne][col] = val;
    return true;
}

bool est_grille_valide(Sudoku sudoku) {
    for (int ligne = 0; ligne < N; ligne++) {
        for (int col = 0; col < N; col++) {
            if (sudoku.grille[ligne][col] != 0 && !est_valide(sudoku, ligne, col, sudoku.grille[ligne][col])) {
                return false;
            }
        }
    }
    return true;
}

void sauvegarder_grille(Sudoku sudoku, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%d ", sudoku.grille[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void charger_grille(Sudoku *sudoku, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(f, "%d", &sudoku->grille[i][j]);
        }
    }
    fclose(f);
}

int main() {
    Sudoku sudoku;
    generer_grille(&sudoku);

    printf("Grille générée:\n");
    afficher_grille(sudoku);

    char choix;
    do {
        printf("Voulez-vous modifier une case? (o/n): ");
        scanf(" %c", &choix);

        if (choix == 'o') {
            if (!saisir_valeur(&sudoku)) {
                printf("Entrée invalide, essayez encore.\n");
            }
        }

        printf("Grille actuelle:\n");
        afficher_grille(sudoku);

    } while (choix != 'n');

    printf("La solution est-elle valide? %s\n", est_grille_valide(sudoku) ? "Oui" : "Non");

    sauvegarder_grille(sudoku, "sudoku.txt");
    charger_grille(&sudoku, "sudoku.txt");

    printf("Grille chargée depuis le fichier:\n");
    afficher_grille(sudoku);

    return 0;
}
