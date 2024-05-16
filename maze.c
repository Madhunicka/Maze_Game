#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining size of maze
#define MAX_SIZE 100
#define MIN_SIZE 5

//defining maze and player position
char maze[MAX_SIZE][MAX_SIZE];
int height = 0, width = 0;
int player_x = -1, player_y = -1;

//function to print maze
void print_maze() {
    printf("\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == player_y && j == player_x) {
                printf("X");
            } else {
                printf("%c", maze[i][j]);
            }
        }
        printf("\n");
    }
}


//function to load maze

int load_maze(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        return 1; // File error
    }

    char line[MAX_SIZE + 2]; // +2 for newline and null terminator
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') line[--len] = '\0'; // Remove trailing newline

        if (width == 0) width = len;
        if (len != width) {
            fclose(file);
            return 2; // Invalid maze (unequal row lengths)
        }

        if (height >= MAX_SIZE) {
            fclose(file);
            return 2; // Invalid maze (too tall)
        }

        strcpy(maze[height], line);
        height++;
    }
    fclose(file);

    if (height < MIN_SIZE || height > MAX_SIZE ||
        width < MIN_SIZE || width > MAX_SIZE) {
        return 2; // Invalid maze (wrong size)
    }

    int start_count = 0, end_count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == 'S') {
                player_x = j;
                player_y = i;
                start_count++;
            } else if (maze[i][j] == 'E') {
                end_count++;
            } else if (maze[i][j] != '#' && maze[i][j] != ' ') {
                return 2; // Invalid character in maze
            }
        }
    }

    if (start_count != 1 || end_count != 1) {
        return 2; // Invalid maze (wrong number of 'S' or 'E')
    }

    return 0; // Success
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <mazefile path>\n", argv[0]);
        return 1; // Argument error
    }

    int result = load_maze(argv[1]);
    if (result != 0) {
        printf("Error loading maze: %d\n", result);
        return result; // Return specific error code
    }

    printf("Maze loaded successfully. Starting game...\n");
    print_maze();

    char input;
    while (1) {
        printf("Enter your move (WASD to move, M to show map, Q to quit): ");
        if (scanf(" %c", &input) != 1) break;

        int new_x = player_x;
        int new_y = player_y;

        if (input == 'W' || input == 'w') new_y--;
        else if (input == 'A' || input == 'a') new_x--;
        else if (input == 'S' || input == 's') new_y++;
        else if (input == 'D' || input == 'd') new_x++;
        else if (input == 'M' || input == 'm') {
            print_maze();
            continue;
        } else if (input == 'Q' || input == 'q') {
            printf("Quitting game.\n");
            break;
        } else {
            printf("Invalid input. Please enter WASD, M, or Q.\n");
            continue;
        }

        if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height || maze[new_y][new_x] == '#') {
            printf("Invalid move.\n");
            continue;
        }

        player_x = new_x;
        player_y = new_y;

        if (maze[player_y][player_x] == 'E') {
            printf("Congratulations! You've reached the exit.\n");
            break;
        }
    }

    return 0;
}
