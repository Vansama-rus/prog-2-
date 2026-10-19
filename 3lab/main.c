#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct room {
    char name[50];
    int  level;
    int  number;
    int  resolution;
};

struct node {
    struct room      data;
    struct node     *next;
};

void pushBack(struct node **head, struct room r) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = r;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct node *cur = *head;
    while (cur->next != NULL)
        cur = cur->next;

    cur->next = newNode;
}

void printList(struct node *head) {
    if (head == NULL) {
        printf("Список пуст.\n");
        return;
    }

    char *sep = "+----+----------------------+---------+-------+--------+";
    printf("%s\n", sep);
    printf("| %-2s | %-20s         | %-7s | %-5s | %-6s |\n",
           "#", "Название", "Уровень", "Номер", "Размер");
    printf("%s\n", sep);

    int i = 1;
    struct node *cur = head;
    while (cur != NULL) {
        printf("| %-2d | %-20s | %-7d | %-5d | %-6d |\n",
               i++,
               cur->data.name,
               cur->data.level,
               cur->data.number,
               cur->data.resolution);
        cur = cur->next;
    }
    printf("%s\n", sep);
}

void freeList(struct node **head) {
    struct node *cur = *head;
    while (cur != NULL) {
        struct node *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    *head = NULL;
}

struct room randomRoom(int index) {
    const char *names[] = {
        "Dungeon Hall", "Dark Cave",   "Lava Room",
        "Ice Corridor", "Spider Nest", "Throne Room",
        "Crypt",        "Arena",       "Library",    "Vault"
    };
    const int COUNT = sizeof(names) / sizeof(names[0]);

    struct room r;
    strncpy(r.name, names[rand() % COUNT], sizeof(r.name) - 1);
    r.name[sizeof(r.name) - 1] = '\0';
    r.level      = 1  + rand() % 10;
    r.number     = index + 1;
    r.resolution = 5  + rand() % 46;
    return r;
}

int main() {
    srand(time(NULL));

    int n;
    printf("Введите количество комнат: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Количество должно быть больше 0.\n");
        return 1;
    }

    struct node *list = NULL;

    for (int i = 0; i < n; i++)
        pushBack(&list, randomRoom(i));

    printf("\n=== Список комнат (%d шт.) ===\n", n);
    printList(list);

    freeList(&list);
    return 0;
}
