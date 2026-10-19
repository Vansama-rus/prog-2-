#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>




void *task1_child(void *arg) {
    for (int i = 1; i <= 5; i++)
        printf("  [Дочерний] строка %d\n", i);
    return NULL;
}

void exercise1() {
    printf("\n--- Упр. 1: Создать поток -----------------------\n");
    pthread_t tid;
    pthread_create(&tid, NULL, task1_child, NULL);

    for (int i = 1; i <= 5; i++)
        printf("  [Родительский] строка %d\n", i);

    pthread_join(tid, NULL);
    printf("└------------------------------------------------\n");
}





void *task2_child(void *arg) {
    for (int i = 1; i <= 5; i++)
        printf("  [Дочерний] строка %d\n", i);
    return NULL;
}

void exercise2() {
    printf("\n--- Упр. 2: Ожидание потока ---------------------\n");
    pthread_t tid;
    pthread_create(&tid, NULL, task2_child, NULL);

    /* join ДО цикла родителя — блокирует до завершения дочернего */
    pthread_join(tid, NULL);

    printf("  [Дочерний завершён. Родитель начинает вывод]\n");
    for (int i = 1; i <= 5; i++)
        printf("  [Родительский] строка %d\n", i);
    printf("-------------------------------------------------\n");
}








typedef struct {
    int   id;
    char **lines;
    int   count;
} Args3;

void *task3_worker(void *arg) {
    Args3 *a = (Args3 *)arg;
    for (int i = 0; i < a->count; i++)
        printf("  [Поток %d] %s\n", a->id, a->lines[i]);
    return NULL;
}

void exercise3() {
    printf("\n--- Упр. 3: Параметры потока --------------------\n");

    char *seq1[] = {"Альфа-1",  "Альфа-2",  "Альфа-3"};
    char *seq2[] = {"Бета-1",   "Бета-2",   "Бета-3"};
    char *seq3[] = {"Гамма-1",  "Гамма-2",  "Гамма-3"};
    char *seq4[] = {"Дельта-1", "Дельта-2", "Дельта-3"};

    Args3 args[4] = {
        {1, seq1, 3}, {2, seq2, 3},
        {3, seq3, 3}, {4, seq4, 3}
    };

    pthread_t tids[4];
    for (int i = 0; i < 4; i++)
        pthread_create(&tids[i], NULL, task3_worker, &args[i]);
    for (int i = 0; i < 4; i++)
        pthread_join(tids[i], NULL);

    printf("-------------------------------------------------\n");
}









void *task4_worker(void *arg) {
    int id = *(int *)arg;
    for (int i = 1; i <= 10; i++) {
        printf("  [Поток %d] строка %d\n", id, i);
        sleep(1); // точка отмены — sleep() реагирует на cancel
    }
    return NULL;
}

void exercise4() {
    printf("\n--- Упр. 4: pthread_cancel() --------------------\n");

    pthread_t tids[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
        pthread_create(&tids[i], NULL, task4_worker, &ids[i]);

    sleep(2);

    printf("  [Главный] отменяем все потоки...\n");
    for (int i = 0; i < 4; i++)
        pthread_cancel(tids[i]);
    for (int i = 0; i < 4; i++)
        pthread_join(tids[i], NULL);

    printf("  [Главный] все потоки отменены.\n");
    printf("-------------------------------------------------\n");
}









void cleanup_handler(void *arg) {
    int id = *(int *)arg;
    printf("  [Поток %d] >>> cleanup: поток завершается <<<\n", id);
}

void *task5_worker(void *arg) {
    int id = *(int *)arg;

    /* регистрирация обработчика завершения */
    pthread_cleanup_push(cleanup_handler, arg);

    for (int i = 1; i <= 10; i++) {
        printf("  [Поток %d] строка %d\n", id, i);
        sleep(1);
    }

    pthread_cleanup_pop(0); // 0 = не вызывать при нормальном выходе
    return NULL;
}

void exercise5() {
    printf("\n--- Упр. 5: pthread_cleanup_push() -------------\n");

    pthread_t tids[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
        pthread_create(&tids[i], NULL, task5_worker, &ids[i]);

    sleep(2);

    printf("  [Главный] отменяем все потоки...\n");
    for (int i = 0; i < 4; i++)
        pthread_cancel(tids[i]);
    for (int i = 0; i < 4; i++)
        pthread_join(tids[i], NULL);

    printf("  [Главный] все потоки отменены.\n");
    printf("-------------------------------------------------\n");
}






void *sleepsort_worker(void *arg) {
    int val = *(int *)arg;
    sleep(val);           
    printf("%d ", val);
    fflush(stdout);
    return NULL;
}

void exercise6() {
    printf("\n--- Упр. 6: Sleepsort ---------------------------\n");

    int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("  Исходный массив: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n  Отсортированный: ");
    fflush(stdout);

    pthread_t tids[50];
    for (int i = 0; i < n; i++)
        pthread_create(&tids[i], NULL, sleepsort_worker, &arr[i]);
    for (int i = 0; i < n; i++)
        pthread_join(tids[i], NULL);

    printf("\n-------------------------------------------------\n");
}






int main() {
    exercise1();
    exercise2();
    exercise3();
    exercise4();
    exercise5();
    exercise6();
    return 0;
}
