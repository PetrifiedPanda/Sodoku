#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

void print(const int8_t sodoku[9][9]);
void solve_it(int8_t sodoku[9][9]);
void solve_rec(int8_t sodoku[9][9]);

int main() {
    int8_t sodoku[9][9] = {
        {8, 0, 5, 0, 7, 0, 0, 2, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 3, 0, 0, 0, 0},
        {2, 0, 0, 3, 0, 0, 8, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 9, 0},
        {0, 4, 0, 0, 0, 0, 0, 6, 5},
        {0, 1, 0, 0, 5, 0, 0, 0, 0},
        {4, 3, 0, 0, 8, 7, 0, 0, 0},
        {6, 0, 0, 0, 0, 0, 0, 7, 0}
    };
    
    print(sodoku);
    solve_it(sodoku);
    print(sodoku);
    return 0;
}

// Fancy print function

void print(const int8_t sodoku[9][9]) {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0) {
            for (int i = 0; i < 25; ++i)
                printf("-");
            printf("\n");
        }
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0)
                printf("| ");
            if (sodoku[i][j] == 0)
                printf("  ");
            else
                printf("%d ", sodoku[i][j]);
        }
        printf("|");
        printf("\n");
    }

    for (int i = 0; i < 25; ++i)
        printf("-");
    printf("\n");
}

// Helper functions used by both implementations

bool can_place(const int8_t sodoku[9][9], int row, int column, int8_t placement) {
    // Check if number already in row
    for (int j = 0; j < 9; ++j) {
        if (j == column)
            continue;
        if (sodoku[row][j] == placement)
            return false;
    }

    // Check if number already in column
    for (int i = 0; i < 9; ++i) {
        if (i == row)
            continue;
        if (sodoku[i][column] == placement)
            return false;
    }

    // Check if number already in quadrant
    int quadrantRow = floor(row / 3.0) * 3;
    int quadrantCol = floor(column / 3.0) * 3;
    for (int i = quadrantRow; i < quadrantRow + 3; ++i) {
        for (int j = quadrantCol; j < quadrantCol + 3; ++j) {
            if (i == row && j == column)
                continue;
            
            if (sodoku[i][j] == placement)
                return false;
        }
    }

    return true;
}

void set_preset(bool preset[9][9], const int8_t sodoku[9][9]) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            preset[i][j] = sodoku[i][j] != 0;
}

// Iterative implementation

void backtrack(int8_t sodoku[9][9], const bool preset[9][9], int* i, int* j) {
    if (!preset[*i][*j])
        sodoku[*i][*j] = 0;

    // Because j will be incremented when the loop ends, it is decremented by 2
    if (*j == 0) {
        *i -= 1;
        *j = 7;
    } else {
        *j -= 2;
    }
}

void solve_it(int8_t sodoku[9][9]) {
    bool preset[9][9];
    set_preset(preset, sodoku);

    bool backtracking = false;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (preset[i][j]) {
                if (backtracking)
                    backtrack(sodoku, preset, &i, &j);
                continue;
            }

            int8_t prevP = sodoku[i][j];
            for (int8_t p = prevP + 1; p < 10; ++p) {
                if (can_place(sodoku, i, j, p)) {
                    sodoku[i][j] = p;
                    break;
                }
            }

            if (sodoku[i][j] == prevP) {
                backtracking = true;
                backtrack(sodoku, preset, &i, &j);
            } else {
                backtracking = false;
            }
         }
    }

}

// Recursive Implementation

void solve_rec_impl(int8_t sodoku[9][9], const bool preset[9][9], int i, int j, bool backtracking) {
    if (i < 9 && j < 9) {
        if (!preset[i][j]) {
            int8_t prevVal = sodoku[i][j];
            for (int k = prevVal + 1; k < 10; ++k) {
                if (can_place(sodoku, i, j, k)) {
                    sodoku[i][j] = k;
                    break;
                }
            }

            if (sodoku[i][j] == prevVal) {
                sodoku[i][j] = 0;
                backtracking = true;
            }
            else {
                backtracking = false;
            }
        }

        if (backtracking) {
            if (j == 0)
                solve_rec_impl(sodoku, preset, i - 1, 8, true);
            else
                solve_rec_impl(sodoku, preset, i, j - 1, true);
        } else {
            if (j == 8)
                solve_rec_impl(sodoku, preset, i + 1, 0, false);
            else
                solve_rec_impl(sodoku, preset, i, j + 1, false);
        }
    }
}

void solve_rec(int8_t sodoku[9][9]) {
    bool preset[9][9];
    set_preset(preset, sodoku);
    solve_rec_impl(sodoku, preset, 0, 0, false);
}