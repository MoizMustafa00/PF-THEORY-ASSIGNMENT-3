#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;
    char fullName[100];
    char batch[50];
    char membership[10];
    char regDate[20];
    char dob[20];
    char interest[10];
} Student;

Student *database = NULL;
int total = 0;

int stricmp(const char *a, const char *b) {
    while (*a && *b) {
        char ca = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        char cb = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return *a - *b;
}

void loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("No previous database found. Creating new.\n");
        return;
    }

    Student temp;
    while (fread(&temp, sizeof(Student), 1, fp)) {
        Student *tmp = realloc(database, (total + 1) * sizeof(Student));
        if (!tmp) {
            perror("Memory allocation failed");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        database = tmp;
        database[total] = temp;
        total++;
    }

    fclose(fp);
}

void saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error saving database");
        return;
    }

	int i;
    for (i = 0; i < total; i++) {
        fwrite(&database[i], sizeof(Student), 1, fp);
    }

    fclose(fp);
}

int findStudent(int id) {
    int i;
	for (i = 0; i < total; i++) {
        if (database[i].studentID == id)
            return i;
    }
    return -1;
}

void addStudent(const char *filename) {
    Student s;

    printf("Enter Student ID: ");
    scanf("%d", &s.studentID);
    getchar(); // consume newline

    if (findStudent(s.studentID) != -1) {
        printf("Student ID already exists!\n");
        return;
    }

    printf("Enter Full Name: ");
    fgets(s.fullName, sizeof(s.fullName), stdin);
    s.fullName[strcspn(s.fullName, "\n")] = 0;

    printf("Enter Batch (CS/SE/AI/Cyber Security): ");
    fgets(s.batch, sizeof(s.batch), stdin);
    s.batch[strcspn(s.batch, "\n")] = 0;

    printf("Enter Membership (IEEE/ACM): ");
    fgets(s.membership, sizeof(s.membership), stdin);
    s.membership[strcspn(s.membership, "\n")] = 0;

    printf("Enter Registration Date (YYYY-MM-DD): ");
    fgets(s.regDate, sizeof(s.regDate), stdin);
    s.regDate[strcspn(s.regDate, "\n")] = 0;

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    fgets(s.dob, sizeof(s.dob), stdin);
    s.dob[strcspn(s.dob, "\n")] = 0;

    printf("Enter Interest (IEEE/ACM/Both): ");
    fgets(s.interest, sizeof(s.interest), stdin);
    s.interest[strcspn(s.interest, "\n")] = 0;

    Student *tmp = realloc(database, (total + 1) * sizeof(Student));
    if (!tmp) {
        perror("Memory allocation failed");
        return;
    }
    database = tmp;
    database[total] = s;
    total++;

    saveDatabase(filename);
    printf("Student Added Successfully!\n");
}

void updateStudent(const char *filename) {
    int id;
    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    getchar(); // consume newline

    int pos = findStudent(id);
    if (pos == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("Enter New Batch (leave empty to keep current): ");
    char input[50];
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strncpy(database[pos].batch, input, sizeof(database[pos].batch)-1);
    }

    printf("Enter New Membership (IEEE/ACM) (leave empty to keep current): ");
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strncpy(database[pos].membership, input, sizeof(database[pos].membership)-1);
    }

    saveDatabase(filename);
    printf("Record Updated!\n");
}

void deleteStudent(const char *filename) {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    getchar();

    int pos = findStudent(id);
    if (pos == -1) {
        printf("Student Not Found!\n");
        return;
    }

	int i;
    for (i = pos; i < total - 1; i++)
        database[i] = database[i + 1];
    total--;

    if (total > 0) {
        Student *tmp = realloc(database, total * sizeof(Student));
        if (!tmp) {
            perror("Memory allocation failed");
            return;
        }
        database = tmp;
    } else {
        free(database);
        database = NULL;
    }

    saveDatabase(filename);
    printf("Student Deleted!\n");
}

void displayAll() {
    if (total == 0) {
        printf("No records found.\n");
        return;
    }

	int i;
    for (i = 0; i < total; i++) {
        printf("\n----------------------------\n");
        printf("Student ID: %d\n", database[i].studentID);
        printf("Name: %s\n", database[i].fullName);
        printf("Batch: %s\n", database[i].batch);
        printf("Membership: %s\n", database[i].membership);
        printf("Reg Date: %s\n", database[i].regDate);
        printf("DOB: %s\n", database[i].dob);
        printf("Interest: %s\n", database[i].interest);
    }
}

void batchReport() {
    char batchName[50];
    printf("Enter batch (CS/SE/AI/Cyber Security): ");
    getchar();
    fgets(batchName, sizeof(batchName), stdin);
    batchName[strcspn(batchName, "\n")] = 0;

    printf("\n--- Students in Batch %s ---\n", batchName);

    int found = 0;
    int i;
	for (i = 0; i < total; i++) {
        if (stricmp(database[i].batch, batchName) == 0) {
            printf("ID: %d | Name: %s | Membership: %s\n",
                   database[i].studentID,
                   database[i].fullName,
                   database[i].membership);
            found = 1;
        }
    }

    if (!found)
        printf("No students found in this batch.\n");
}

int main() {
    const char *filename = "members.dat";
    loadDatabase(filename);

    int choice;
    while (1) {
        printf("\n==== MEMBERSHIP SYSTEM MENU ====\n");
        printf("1. Register New Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("4. View All Students\n");
        printf("5. Batch-wise Report\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(filename); break;
            case 2: updateStudent(filename); break;
            case 3: deleteStudent(filename); break;
            case 4: displayAll(); break;
            case 5: batchReport(); break;
            case 6: 
                saveDatabase(filename);
                free(database);
                printf("Exiting... Data Saved.\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

