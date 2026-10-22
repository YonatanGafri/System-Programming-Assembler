#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "definitions.h"
#include "data_struct.h"

void test_create_new_list() {
    flag err = OFF;
    list *l = create_new_list(&err);
    assert(l != NULL);
    assert(err == OFF);
    assert(return_name(l) == NULL);
    assert(return_value(l) == UNINITIALIZED);
    assert(return_type(l) == UNINITIALIZED);
    assert(return_next(l) == NULL);
    free_list(l);
    printf("test_create_new_list passed\n");
}

void test_add_to_list() {
    flag err = OFF;
    list *l = create_new_list(&err);
    list *node3;

    add_to_list(l, "first", 10, 1, &err);
    assert(err == OFF);
    assert(strcmp(return_name(l), "first") == 0);
    assert(return_value(l) == 10);
    assert(return_type(l) == 1);
    assert(return_next(l) == NULL);

    add_to_list(l, "second", 20, 2, &err);
    assert(err == OFF);
    assert(return_next(l) != NULL);
    assert(strcmp(return_name(return_next(l)), "second") == 0);
    assert(return_value(return_next(l)) == 20);
    assert(return_type(return_next(l)) == 2);

    /* Edge case: NULL name */
    add_to_list(l, NULL, 30, 3, &err);
    assert(err == OFF);
    node3 = return_next(return_next(l));
    assert(node3 != NULL);
    assert(return_name(node3) == NULL);
    assert(return_value(node3) == 30);

    free_list(l);
    printf("test_add_to_list passed\n");
}

void test_list_search() {
    flag err = OFF;
    list *found;
    list *l = create_new_list(&err);
    add_to_list(l, "macro1", 100, MACRO, &err);
    add_to_list(l, "macro2", 200, MACRO, &err);

    found = list_search(l, "macro2");
    assert(found != NULL);
    assert(return_value(found) == 200);

    found = list_search(l, "macro3");
    assert(found == NULL);

    free_list(l);
    printf("test_list_search passed\n");
}

void test_edit_or_add_to_list() {
    flag err = OFF;
    list *l = create_new_list(&err);

    edit_or_add_to_list(l, "var1", 10, DATA, &err);
    assert(strcmp(return_name(l), "var1") == 0);
    assert(return_value(l) == 10);

    edit_or_add_to_list(l, "var1", 20, CODE, &err);
    assert(strcmp(return_name(l), "var1") == 0);
    assert(return_value(l) == 20);
    assert(return_type(l) == CODE);

    free_list(l);
    printf("test_edit_or_add_to_list passed\n");
}

void test_sort_list() {
    flag err = OFF;
    list *sorted;
    list *l = create_new_list(&err);
    add_to_list(l, "a", 30, DATA, &err);
    add_to_list(l, "b", 10, DATA, &err);
    add_to_list(l, "c", 20, DATA, &err);

    sorted = sort_list(l, &err);
    assert(sorted != NULL);
    assert(strcmp(return_name(sorted), "b") == 0);
    assert(strcmp(return_name(return_next(sorted)), "c") == 0);
    assert(strcmp(return_name(return_next(return_next(sorted))), "a") == 0);

    free_list(l);
    free_list(sorted);
    printf("test_sort_list passed\n");
}

void test_edge_cases() {
    flag err = OFF;
    list *empty = create_new_list(&err);
    list *sorted;
    list *found;

    /* list_search on empty list */
    found = list_search(empty, "test");
    assert(found == NULL);

    /* sort_list on empty list */
    sorted = sort_list(empty, &err);
    /* sort_list returns NULL for empty uninitialized list */
    assert(sorted == NULL);

    free_list(empty);
    printf("test_edge_cases passed\n");
}

int main() {
    test_create_new_list();
    test_add_to_list();
    test_list_search();
    test_edit_or_add_to_list();
    test_sort_list();
    test_edge_cases();
    printf("All data_struct tests passed!\n");
    return 0;
}
