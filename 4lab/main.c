#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE   256
#define MAX_NAME   100
#define MAX_LEVEL   20

typedef struct {
    char name[MAX_NAME];
    int  salary;
    char level[MAX_LEVEL];
} Employee;

void printTable(Employee *staff, int count) {
    const char *sep = "+----+---------------------------+---------+---------+";

    printf("%s\n", sep);
    printf("| %-2s | %-25s | %-7s | %-7s |\n",
           "#", "Имя", "Зарплата", "Уровень");
    printf("%s\n", sep);

    for (int i = 0; i < count; i++) {
        printf("| %-2d | %-25s | %-7d | %-7s |\n",
               i + 1,
               staff[i].name,
               staff[i].salary,
               staff[i].level);
    }

    printf("%s\n", sep);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Использование: %s <файл.csv>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    }

    Employee *staff = NULL;
    int count = 0;

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file) != NULL) {

        line[strcspn(line, "\r\n")] = '\0';

        if (strlen(line) == 0) continue;

        Employee emp;
        char *token;

        token = strtok(line, ";");
        if (token == NULL) { fprintf(stderr, "Ошибка: неверный формат строки\n"); continue; }
        strncpy(emp.name, token, MAX_NAME - 1);
        emp.name[MAX_NAME - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) { fprintf(stderr, "Ошибка: отсутствует зарплата\n"); continue; }
        emp.salary = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) { fprintf(stderr, "Ошибка: отсутствует уровень\n"); continue; }
        strncpy(emp.level, token, MAX_LEVEL - 1);
        emp.level[MAX_LEVEL - 1] = '\0';

        Employee *tmp = realloc(staff, (count + 1) * sizeof(Employee));
        if (tmp == NULL) {
            fprintf(stderr, "Ошибка: нехватка памяти\n");
            free(staff);
            fclose(file);
            return 1;
        }
        staff = tmp;
        staff[count++] = emp;
    }

    if (ferror(file)) {
        fprintf(stderr, "Ошибка при чтении файла\n");
        free(staff);
        fclose(file);
        return 1;
    }

    fclose(file);

    if (count == 0) {
        printf("Файл не содержит данных.\n");
    } else {
        printf("\n=== Сотрудники из \"%s\" (%d чел.) ===\n\n", argv[1], count);
        printTable(staff, count);
    }

    free(staff);
    return 0;
}
