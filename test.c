/*
 * test.c — unit-тесты для lab6/main.c
 * Компиляция: gcc -Wall -DTEST_MODE test.c -o test -lm
 * Запуск:     ./test.exe
 */

#define TEST_MODE  // отключает main() в main.c
#undef N           // снимаем макрос N чтобы не мешал
#include "main.c"  // подключаем все функции из main.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ────────────────────────────────────────────────────
//  Вспомогательные функции для тестов
// ────────────────────────────────────────────────────

// Собирает inorder-обход дерева в массив
static void collect_inorder(Vertex *p, int *arr, int *idx) {
    if (p == NULL) return;
    collect_inorder(p->Left, arr, idx);
    arr[(*idx)++] = p->Data;
    collect_inorder(p->Right, arr, idx);
}

// Проверяет, что массив отсортирован по возрастанию
static int is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] >= arr[i + 1]) return 0;
    return 1;
}

// Освобождает дерево
static void free_tree(Vertex *p) {
    if (!p) return;
    free_tree(p->Left);
    free_tree(p->Right);
    free(p);
}

// Создаёт одиночный узел
static Vertex *make_node(int val) {
    Vertex *n = malloc(sizeof(Vertex));
    n->Data = val;
    n->balance = 0;
    n->Left = n->Right = NULL;
    return n;
}

// ────────────────────────────────────────────────────
//  Тесты Size()
// ────────────────────────────────────────────────────

void test_size_null() {
    assert(Size(NULL) == 0);
    printf("PASS  Size(NULL) == 0\n");
}

void test_size_single_node() {
    Vertex *t = make_node(42);
    assert(Size(t) == 1);
    free_tree(t);
    printf("PASS  Size(single node) == 1\n");
}

void test_size_after_avl_insert() {
    Vertex *t = NULL;
    int vals[] = {10, 20, 30, 5, 15};
    for (int i = 0; i < 5; i++) insert_AVL(vals[i], &t);
    assert(Size(t) == 5);
    free_tree(t);
    printf("PASS  Size после 5 вставок в AVL == 5\n");
}

// ────────────────────────────────────────────────────
//  Тесты Height()
// ────────────────────────────────────────────────────

void test_height_null() {
    assert(Height(NULL) == 0);
    printf("PASS  Height(NULL) == 0\n");
}

void test_height_single_node() {
    Vertex *t = make_node(1);
    assert(Height(t) == 1);
    free_tree(t);
    printf("PASS  Height(single node) == 1\n");
}

void test_height_avl_is_logarithmic() {
    // AVL из 100 элементов должен иметь высоту <= 1.45 * log2(100) ≈ 10
    Vertex *t = NULL;
    for (int i = 1; i <= 100; i++) insert_AVL(i, &t);
    int h = Height(t);
    assert(h <= 15); // строгая граница для AVL
    free_tree(t);
    printf("PASS  Height AVL(100 элем.) <= 15, реальная: %d\n", h);
}

// ────────────────────────────────────────────────────
//  Тесты CheckSum()
// ────────────────────────────────────────────────────

void test_checksum_null() {
    assert(CheckSum(NULL) == 0);
    printf("PASS  CheckSum(NULL) == 0\n");
}

void test_checksum_known_values() {
    Vertex *t = NULL;
    // вставляем 1,2,3 → сумма = 6
    insert_AVL(1, &t);
    insert_AVL(2, &t);
    insert_AVL(3, &t);
    assert(CheckSum(t) == 6);
    free_tree(t);
    printf("PASS  CheckSum({1,2,3}) == 6\n");
}

void test_checksum_large() {
    // сумма 1..10 = 55
    Vertex *t = NULL;
    for (int i = 1; i <= 10; i++) insert_AVL(i, &t);
    assert(CheckSum(t) == 55);
    free_tree(t);
    printf("PASS  CheckSum({1..10}) == 55\n");
}

// ────────────────────────────────────────────────────
//  Тесты AVL: свойство BST (inorder отсортирован)
// ────────────────────────────────────────────────────

void test_avl_bst_property_random() {
    Vertex *t = NULL;
    int vals[] = {50, 20, 80, 10, 30, 70, 90, 5, 15, 25};
    int n = sizeof(vals) / sizeof(vals[0]);
    for (int i = 0; i < n; i++) insert_AVL(vals[i], &t);

    int out[100]; int idx = 0;
    collect_inorder(t, out, &idx);

    assert(idx == n);
    assert(is_sorted(out, idx));
    free_tree(t);
    printf("PASS  AVL inorder отсортирован (произвольный ввод)\n");
}

void test_avl_bst_property_reversed() {
    // вставка в обратном порядке — AVL должен перебалансироваться
    Vertex *t = NULL;
    for (int i = 10; i >= 1; i--) insert_AVL(i, &t);

    int out[20]; int idx = 0;
    collect_inorder(t, out, &idx);
    assert(is_sorted(out, idx));
    free_tree(t);
    printf("PASS  AVL inorder отсортирован (обратный ввод)\n");
}

void test_avl_no_duplicates() {
    Vertex *t = NULL;
    insert_AVL(5, &t);
    insert_AVL(5, &t); // дубликат — не должен добавляться
    assert(Size(t) == 1);
    free_tree(t);
    printf("PASS  AVL не добавляет дубликаты\n");
}

// ────────────────────────────────────────────────────
//  Тесты AverageHeight()
// ────────────────────────────────────────────────────

void test_average_height_null() {
    assert(AverageHeight(NULL) == 0.0f);
    printf("PASS  AverageHeight(NULL) == 0.0\n");
}

void test_average_height_single() {
    Vertex *t = make_node(1);
    assert(AverageHeight(t) == 1.0f);
    free_tree(t);
    printf("PASS  AverageHeight(single node) == 1.0\n");
}

void test_average_height_less_than_height() {
    // средняя высота всегда <= максимальной
    Vertex *t = NULL;
    for (int i = 1; i <= 15; i++) insert_AVL(i, &t);
    float avg = AverageHeight(t);
    int   h   = Height(t);
    assert(avg <= (float)h);
    free_tree(t);
    printf("PASS  AverageHeight <= Height для AVL(15 элем.)\n");
}

// ────────────────────────────────────────────────────
//  MAIN
// ────────────────────────────────────────────────────

int main() {
    printf("═══════════════════════════════════════\n");
    printf("  Unit-тесты lab6 (AVL + дерево)\n");
    printf("═══════════════════════════════════════\n\n");

    printf("── Size() ──────────────────────────────\n");
    test_size_null();
    test_size_single_node();
    test_size_after_avl_insert();

    printf("\n── Height() ────────────────────────────\n");
    test_height_null();
    test_height_single_node();
    test_height_avl_is_logarithmic();

    printf("\n── CheckSum() ──────────────────────────\n");
    test_checksum_null();
    test_checksum_known_values();
    test_checksum_large();

    printf("\n── AVL: свойство BST ───────────────────\n");
    test_avl_bst_property_random();
    test_avl_bst_property_reversed();
    test_avl_no_duplicates();

    printf("\n── AverageHeight() ─────────────────────\n");
    test_average_height_null();
    test_average_height_single();
    test_average_height_less_than_height();

    printf("\n═══════════════════════════════════════\n");
    printf("  Все тесты пройдены успешно!\n");
    printf("═══════════════════════════════════════\n");
    return 0;
}
