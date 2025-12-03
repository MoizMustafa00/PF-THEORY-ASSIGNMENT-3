#include <stdio.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};

void displayEmployees(struct Employee emp[], int n)
{
    printf("\n---- Employee Records ----\n");
    printf("%-10s %-20s %-20s %-10s\n", "ID", "Name", "Designation", "Salary");

    int i;
    for (i = 0; i < n; i++) {
        printf("%-10d %-20s %-20s %-10.2f\n",
               emp[i].id,
               emp[i].name,
               emp[i].designation,
               emp[i].salary);
    }
}

void findHighestSalary(struct Employee emp[], int n)
{
    int index = 0;
    int i;

    for (i = 1; i < n; i++) {
        if (emp[i].salary > emp[index].salary)
            index = i;
    }

    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\n", emp[index].id);
    printf("Name: %s\n", emp[index].name);
    printf("Designation: %s\n", emp[index].designation);
    printf("Salary: %.2f\n", emp[index].salary);
}

void searchEmployee(struct Employee emp[], int n)
{
    int choice;
    printf("\nSearch by:\n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int searchID;
        printf("Enter ID to search: ");
        scanf("%d", &searchID);

        int i;
        for (i = 0; i < n; i++) {
            if (emp[i].id == searchID) {
                printf("Record Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
        printf("Employee not found.\n");
    }
    else if (choice == 2) {
        char searchName[50];
        printf("Enter Name to search: ");
        getchar();  // consume leftover newline
        fgets(searchName, sizeof(searchName), stdin);

        // Remove newline from fgets
        searchName[strcspn(searchName, "\n")] = '\0';

        int i;
        for (i = 0; i < n; i++) {
            if (strcmp(emp[i].name, searchName) == 0) {
                printf("Record Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
        printf("Employee not found.\n");
    }
    else {
        printf("Invalid choice.\n");
    }
}

int main()
{
    int n;
    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee emp[n];

    int i;
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d:\n", i + 1);

        printf("ID: ");
        scanf("%d", &emp[i].id);

        getchar(); // clear buffer

        printf("Name: ");
        fgets(emp[i].name, sizeof(emp[i].name), stdin);
        emp[i].name[strcspn(emp[i].name, "\n")] = '\0';

        printf("Designation: ");
        fgets(emp[i].designation, sizeof(emp[i].designation), stdin);
        emp[i].designation[strcspn(emp[i].designation, "\n")] = '\0';

        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }

    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    return 0;
}

