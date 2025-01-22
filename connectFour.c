#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
// string.h
// dla strcmp, ponieważ stringi w C są wskaźnikami: 
// "==" jedynie porównałoby miejsce w pamięci dwóch stringów, nie ich wartości

#define ROWS 6       // preprocesory w celu uniknięcia magicznych numerów
#define COLS 7

// funkcja tworząca pustą planszę w pamięci
void initializeBoard(const char* board[ROWS][COLS]) {
    int i, j;
    // przechodzimy przez rzędy
    for (i = 0; i < ROWS; i++) {
        // a teraz przez każdą kolumnę
        for (j = 0; j < COLS; j++) {
            board[i][j] = " "; // wszystkie pola są puste
        }
    }
}

// funkcja wypisująca planszę
void printBoard(const char* board[ROWS][COLS]) {
    int i, j;
    printf("\n"); 
    // przechodzimy przez wszystkie rzędy oraz kolumny 
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("| %-2s", board[i][j]); 
            // wypisujemy wartość pola, wypełnienie na dwa miejsca, źeby plansza zawsze dobrze się wczytywała
        }
        printf("|\n");

        // wypisujemy linie oddzielające rzędy
        for (j = 0; j < COLS; j++) {
            printf("----"); // 4 myślniki na kolumne
        }
        printf("-\n");
    }

    // numery kolumn, które ułatwią rozgrywkę
    printf("  1   2   3   4   5   6   7  \n");
}

// funkcja, która sprawdza czy ruch jest poprawny
bool isValidMove(const char* board[ROWS][COLS], int col) {
    // sprawdzamy czy numer kolumny jest między 1 a 7 i czy w kolumnie jest miejsce
    if (col < 0 || col >= COLS || strcmp(board[0][col], " ") != 0) {
        return false; 
    }
    return true;
}

// funkcja wstawiająca dysk do konkretnej kolumny
int dropDisc(const char* board[ROWS][COLS], int col, const char* disc) {
    int row;
    // zaczyna od dolnego rzędu i idziemy w górę
    for (row = ROWS - 1; row >= 0; row--) {
        if (strcmp(board[row][col], " ") == 0) { // jeśli znajdziemy puste pole
            board[row][col] = disc;  // to umieścimy w nim dysk
            return row;
        }
    }
    return -1;
}

// funkcja sprawdzająca czy gracz wygrał
bool checkWin(const char* board[ROWS][COLS], int row, int col, const char* disc) {
    // ustalamy kierunki do sprawdzenia {rząd, kolumna}
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    
    for (int d = 0; d < 4; d++) {
        int count = 1; // checkWin zaczyna się tam gdzie rzuciliśmy dysk, więc ustawiamy licznik na 1
        
        // idziemy w kierunku "dodatnim"
        int r = row + directions[d][0];
        int c = col + directions[d][1];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && strcmp(board[r][c], disc) == 0) {
            count++;
            r = r + directions[d][0];
            c = c + directions[d][1];
        }

        // idziemy w kierunku "ujemnym"
        r = row - directions[d][0];
        c = col - directions[d][1];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && strcmp(board[r][c], disc) == 0) {
            count++;
            r = r - directions[d][0];
            c = c - directions[d][1];
        }

        // jeśli znaleźliśmy 4 dyski w tym samym kierunku, funkcja zwraca TRUE, obecny gracz wygrywa
        if (count >= 4) {
            return true;
        }
    }

    return false;
}

// funkcja sprawdza czy plansza jest pełna
bool isBoardFull(const char* board[ROWS][COLS]) {
    int col;
    // sprawdzamy górne pole w każdej kolumnie
    for (col = 0; col < COLS; col++) {
        if (strcmp(board[0][col], " ") == 0) {
            return false; // jeśli jakiekolwiek pole na górze którejkolwiek kolumny jest puste, to plansza nie jest pełna
        }
    }
    return true;
}

int main() {
    const char* board[ROWS][COLS]; 
    // tworzymy miejsce na planszę: const char* to odpowiednik stringa w C++, tyle że w C jest wskaźnikiem
    initializeBoard(board);

    const char* players[2] = {"⚪", "⚫"}; // Gracz 1: ⚪ (biały dysk), Gracz 2: ⚫ (czarny dysk)
    int currentPlayer = 0; // zaczyna gracz 1 (0 + 1) — biały
    int row, col;

    printf("Welcome to Connect Four!\n");
    printBoard(board);

    while (1) { // nieskończona pętla; wcześniej stworzone funkcje skończą program przez instrukcję sterującą break
        // obecny gracz wybiera kolumnę:
        printf("\nPlayer %d (%s), pick a column (1-7): ", currentPlayer + 1, players[currentPlayer]);
        scanf("%d", &col);
        col--; // musimy dopasować indeksowanie, z ludzkiego 1-7 do naszego programu (bazującego na 0)

        // Check if the move is valid
        if (!isValidMove(board, col)) {
            printf("Invalid move!!! Try again.\n");
            continue;
        }

        row = dropDisc(board, col, players[currentPlayer]);
        printBoard(board);

        if (checkWin(board, row, col, players[currentPlayer])) {
            printf("\nPlayer %d (%s) wins! Congratz!\n", currentPlayer + 1, players[currentPlayer]);
            break;
        }

        if (isBoardFull(board)) {
            printf("\nIt's a draw! The board's full.\n");
            break;
        }

        // zmieniamy gracza
        currentPlayer = 1 - currentPlayer;
    }
    return 0;
}