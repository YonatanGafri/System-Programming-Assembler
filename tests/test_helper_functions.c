#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "definitions.h"
#include "helper.h"

void test_add_postfix() {
    flag err = OFF;
    char *res1 = add_postfix("test", ".as", ADD, &err);
    char *res2 = add_postfix("test.as", ".am", CHANGE, &err);

    assert(err == OFF);
    assert(strcmp(res1, "test.as") == 0);
    free(res1);

    assert(err == OFF);
    assert(strcmp(res2, "test.am") == 0);
    free(res2);

    printf("test_add_postfix passed\n");
}

void test_fget_word() {
    flag err = OFF;
    FILE *f = fopen("test_fget_word.txt", "w");
    char *w1, *w2, *w3, *w4;
    fputs("hello world,this\n is a test", f);
    fclose(f);

    f = fopen("test_fget_word.txt", "r");
    w1 = fget_word(f, &err);
    assert(strcmp(w1, "hello") == 0);
    free(w1);

    w2 = fget_word(f, &err);
    assert(strcmp(w2, "world") == 0);
    free(w2);

    w3 = fget_word(f, &err);
    assert(strcmp(w3, "this") == 0);
    free(w3);

    /* newline causes fget_word to return NULL as per implementation */
    w4 = fget_word(f, &err);
    assert(w4 == NULL);

    fclose(f);
    remove("test_fget_word.txt");

    printf("test_fget_word passed\n");
}

void test_fget_line() {
    flag err = OFF;
    char *l1, *l2;
    FILE *f = fopen("test_fget_line.txt", "w");
    fputs("line 1\nline 2", f);
    fclose(f);

    f = fopen("test_fget_line.txt", "r");
    l1 = fget_line(f, &err);
    /* In the code: fgets(line, size + 2, file); if (line[size] == '\n') line[size] = '\0';
       So newline is stripped. */
    assert(strcmp(l1, "line 1") == 0);
    free(l1);

    l2 = fget_line(f, &err);
    assert(strcmp(l2, "line 2") == 0);
    free(l2);

    fclose(f);
    remove("test_fget_line.txt");

    printf("test_fget_line passed\n");
}

void test_get_word() {
    flag err = OFF;
    char *w1 = get_word("hello world", &err);
    char str[] = "   test,word";
    char *ptr = str;
    char *w2;

    assert(strcmp(w1, "hello") == 0);
    free(w1);

    while(*ptr && isspace(*ptr)) ptr++;

    w2 = get_word(ptr, &err);
    assert(strcmp(w2, "test") == 0);
    free(w2);

    printf("test_get_word passed\n");
}

void test_find_word_in_file() {
    FILE *f = fopen("test_find.txt", "w");
    int line1, line2, line3;
    fputs("hello there\nhow are you\nI am fine", f);
    fclose(f);

    line1 = find_word_in_file("test_find.txt", "how");
    assert(line1 == 2);

    line2 = find_word_in_file("test_find.txt", "missing");
    assert(line2 == -1);

    line3 = find_word_in_file("nonexistent.txt", "hello");
    assert(line3 == -1);

    remove("test_find.txt");

    printf("test_find_word_in_file passed\n");
}

int main() {
    test_add_postfix();
    test_fget_word();
    test_fget_line();
    test_get_word();
    test_find_word_in_file();
    printf("All helper_functions tests passed!\n");
    return 0;
}
