#include <stdio.h>

struct Book {
    int id;
    int popularity;
    int lastUsed;  
};

int findBook(struct Book shelf[], int count, int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (shelf[i].id == id) return i;
    }
    return -1;
}

int findLeastUsed(struct Book shelf[], int count) {
    int minIndex = 0;
    int i;
    for (i = 1; i < count; i++) {
        if (shelf[i].lastUsed < shelf[minIndex].lastUsed)
            minIndex = i;
    }
    return minIndex;
}

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    struct Book shelf[1000];
    int count = 0;
    int time = 1;  

    while (Q--) {
        char op[10];
        scanf("%s", op);

        if (op[0] == 'A' && op[1] == 'D') {  
            int x, y;
            scanf("%d %d", &x, &y);

            int pos = findBook(shelf, count, x);

            if (pos != -1) {
                shelf[pos].popularity = y;
                shelf[pos].lastUsed = time++;
            }
            else {
                if (count < capacity) {
                    shelf[count].id = x;
                    shelf[count].popularity = y;
                    shelf[count].lastUsed = time++;
                    count++;
                } 
                else {
                    int lru = findLeastUsed(shelf, count);
                    shelf[lru].id = x;
                    shelf[lru].popularity = y;
                    shelf[lru].lastUsed = time++;
                }
            }
        }

        else if (op[0] == 'A' && op[1] == 'C') {  
            int x;
            scanf("%d", &x);

            int pos = findBook(shelf, count, x);

            if (pos != -1) {
                printf("%d\n", shelf[pos].popularity);
                shelf[pos].lastUsed = time++;
            }
            else {
                printf("-1\n");
            }
        }
    }
    return 0;
}
