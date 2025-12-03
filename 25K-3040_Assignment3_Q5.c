#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *readLine(FILE *fp) {
    size_t cap = 128;
    size_t len = 0;
    char *buf = malloc(cap);
    if (!buf) {
        fprintf(stderr, "malloc failed in readLine: %s\n", strerror(errno));
        exit(1);
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\r') continue;
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) {
                free(buf);
                fprintf(stderr, "realloc failed in readLine: %s\n", strerror(errno));
                exit(1);
            }
            buf = tmp;
        }
        buf[len++] = (char)c;
        if (c == '\n') break;
    }

    if (len == 0 && c == EOF) {
        free(buf);
        return NULL;
    }

    if (len > 0 && buf[len - 1] == '\n') {
        len--;
    }

    buf[len] = '\0';

    char *exact = realloc(buf, len + 1);
    if (!exact) {
        return buf;
    }
    return exact;
}

void ensureCapacity(char ***linesPtr, size_t *capacityPtr, size_t minCap) {
    if (*capacityPtr >= minCap) return;
    size_t newCap = (*capacityPtr == 0) ? 4 : *capacityPtr;
    while (newCap < minCap) newCap *= 2;
    char **tmp = realloc(*linesPtr, newCap * sizeof(char *));
    if (!tmp) {
        fprintf(stderr, "realloc failed in ensureCapacity: %s\n", strerror(errno));
        exit(1);
    }
    *linesPtr = tmp;
    *capacityPtr = newCap;
}

void insertLine(char ***linesPtr, size_t *countPtr, size_t *capacityPtr, size_t index, const char *text) {
    if (index > *countPtr) index = *countPtr;
    ensureCapacity(linesPtr, capacityPtr, (*countPtr) + 1);
    if (index < *countPtr) {
        memmove(&((*linesPtr)[index + 1]), &((*linesPtr)[index]), ((*countPtr) - index) * sizeof(char *));
    }
    size_t tlen = strlen(text);
    char *line = malloc(tlen + 1);
    if (!line) {
        fprintf(stderr, "malloc failed in insertLine: %s\n", strerror(errno));
        exit(1);
    }
    memcpy(line, text, tlen + 1);
    (*linesPtr)[index] = line;
    (*countPtr)++;
}

void deleteLine(char ***linesPtr, size_t *countPtr, size_t *capacityPtr, size_t index) {
    if (*countPtr == 0) return;
    if (index >= *countPtr) return;
    free((*linesPtr)[index]);
    if (index + 1 < *countPtr) {
        memmove(&((*linesPtr)[index]), &((*linesPtr)[index + 1]), ((*countPtr) - index - 1) * sizeof(char *));
    }
    (*countPtr)--;
}

void printAllLines(char **lines, size_t count) {
    size_t i;
    for (i = 0; i < count; ++i) {
        printf("%zu: %s\n", i + 1, lines[i]);
    }
}

void freeAll(char **lines, size_t count) {
    size_t i;
    for (i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free(lines);
}

void shrinkToFit(char ***linesPtr, size_t *capacityPtr, size_t count) {
    if (count == 0) {
        free(*linesPtr);
        *linesPtr = NULL;
        *capacityPtr = 0;
        return;
    }
    char **tmp = realloc(*linesPtr, count * sizeof(char *));
    if (!tmp) {
        return;
    }
    *linesPtr = tmp;
    *capacityPtr = count;
}

void saveToFile(char **lines, size_t count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        return;
    }
    size_t i;
    for (i = 0; i < count; ++i) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
}

void loadFromFile(char ***linesPtr, size_t *countPtr, size_t *capacityPtr, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file for reading: %s\n", filename);
        return;
    }
    if (*linesPtr) {
        freeAll(*linesPtr, *countPtr);
        *linesPtr = NULL;
        *countPtr = 0;
        *capacityPtr = 0;
    }
    char *ln;
    while ((ln = readLine(fp)) != NULL) {
        ensureCapacity(linesPtr, capacityPtr, (*countPtr) + 1);
        (*linesPtr)[*countPtr] = ln;
        (*countPtr)++;
    }
    fclose(fp);
}

void printMenu() {
    printf("Commands:\n");
    printf("I index  -> insert line at index (0-based). After entering command, type the line on next line.\n");
    printf("D index  -> delete line at index (0-based)\n");
    printf("P        -> print all lines\n");
    printf("S file   -> save to file\n");
    printf("L file   -> load from file\n");
    printf("R        -> shrinkToFit\n");
    printf("Q        -> quit\n");
}

int main(void) {
    char **lines = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t i;

    ensureCapacity(&lines, &capacity, 4);

    printMenu();
    char cmd[16];
    while (1) {
        if (scanf("%15s", cmd) != 1) break;
        if (strcmp(cmd, "I") == 0) {
            long idx;
            if (scanf("%ld", &idx) != 1) break;
            int c = getchar();
            char *line = readLine(stdin);
            if (!line) line = strdup("");
            insertLine(&lines, &count, &capacity, (size_t)idx, line);
            free(line);
        } else if (strcmp(cmd, "D") == 0) {
            long idx;
            if (scanf("%ld", &idx) != 1) break;
            deleteLine(&lines, &count, &capacity, (size_t)idx);
        } else if (strcmp(cmd, "P") == 0) {
            printAllLines(lines, count);
        } else if (strcmp(cmd, "S") == 0) {
            char filename[256];
            if (scanf("%255s", filename) != 1) break;
            saveToFile(lines, count, filename);
        } else if (strcmp(cmd, "L") == 0) {
            char filename[256];
            if (scanf("%255s", filename) != 1) break;
            loadFromFile(&lines, &count, &capacity, filename);
        } else if (strcmp(cmd, "R") == 0) {
            shrinkToFit(&lines, &capacity, count);
        } else if (strcmp(cmd, "Q") == 0) {
            break;
        } else {
            int ch;
            while ((ch = getchar()) != EOF && ch != '\n');
        }
    }

    freeAll(lines, count);
    return 0;
}
