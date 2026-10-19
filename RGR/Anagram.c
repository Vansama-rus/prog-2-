#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 10
#define MAX_BYTES  4

typedef struct {
    char bytes[MAX_BYTES + 1];
    int  len;
} Char;

int parse_utf8(const char *str, Char out[], int max) {
    int count = 0;
    int i = 0;
    int slen = (int)strlen(str);

    while (i < slen && count < max) {
        unsigned char c = (unsigned char)str[i];
        int clen;

        if      (c >= 0xF0) clen = 4;
        else if (c >= 0xE0) clen = 3;
        else if (c >= 0xC0) clen = 2;
        else                clen = 1;

        out[count].len = clen;
        for (int j = 0; j < clen && (i + j) < slen; j++)
            out[count].bytes[j] = str[i + j];
        out[count].bytes[clen] = '\0';

        count++;
        i += clen;
    }
    return count;
}

long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}

void swap_chars(Char *a, Char *b) {
    Char tmp = *a;
    *a = *b;
    *b = tmp;
}

static long long count = 0;

void permute(Char chars[], int n, int start) {
    if (start == n) {
        count++;
        printf("%6lld. ", count);
        for (int i = 0; i < n; i++)
            printf("%s", chars[i].bytes);
        printf("\n");
        return;
    }

    for (int i = start; i < n; i++) {
        swap_chars(&chars[start], &chars[i]);
        permute(chars, n, start + 1);
        swap_chars(&chars[start], &chars[i]);
    }
}

int main(int argc, char *argv[]) {
    system("chcp 65001 > nul 2>&1");

    if (argc < 2) {
        fprintf(stderr, "Использование: %s <файл_со_словом>\n", argv[0]);
        fprintf(stderr, "Пример:        %s word.txt\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Ошибка открытия файла");
        return 1;
    }

    char buf[256];
    if (!fgets(buf, sizeof(buf), f)) {
        fprintf(stderr, "Ошибка: файл пуст или не читается\n");
        fclose(f);
        return 1;
    }
    fclose(f);

    buf[strcspn(buf, "\r\n")] = '\0';
    char *word = buf;
    while (*word == ' ') word++;
    int wlen = (int)strlen(word);
    while (wlen > 0 && word[wlen - 1] == ' ') word[--wlen] = '\0';

    if (wlen == 0) {
        fprintf(stderr, "Ошибка: файл не содержит слова\n");
        return 1;
    }

    Char chars[MAX_CHARS];
    int n = parse_utf8(word, chars, MAX_CHARS);

    if (n == 0) {
        fprintf(stderr, "Ошибка: не удалось распознать символы\n");
        return 1;
    }
    if (n > MAX_CHARS) {
        fprintf(stderr, "Ошибка: слово слишком длинное (максимум %d букв)\n", MAX_CHARS);
        return 1;
    }

    long long total = factorial(n);

    printf("  Слово:          %s\n", word);
    printf("  Букв:           %d\n", n);
    printf("  Всего анаграмм: %lld  (%d!)\n\n", total, n);

    if (total > 100000) {
        printf("  [!] Предупреждение: %lld анаграмм — вывод может быть долгим.\n\n", total);
    }

    permute(chars, n, 0);

    printf("\n  Итого выведено: %lld анаграмм\n", count);
    return 0;
}
