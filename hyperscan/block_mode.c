#include <stdio.h>
#include <hs.h>
#include <stdlib.h>
#include <string.h>

static int onMatch(unsigned int id, unsigned long long from,
                                unsigned long long to, unsigned int flags, void *ctx) {
    printf("id : %d, ", id);
    printf("from : %llu, ", from);
    printf("to : %llu, ", to);
    printf("ctx : %s\n", (char *)ctx);
    printf("--------\n");
    return 0;
}

int main(void)
{
#if 0
    const char *pt[3] = {
        "bcd",
        "fbcdaaabc",
        "aaabc"
    };
#endif
    const char *pt[3] = {0};
    pt[0] = (char *)malloc(3);
    memcpy((char *)pt[0], "bcd", 3);

    pt[1] = (char *)malloc(9);
    memcpy((char *)pt[1], "fbcdaaabc", 9);

    pt[2] = (char *)malloc(5);
    memcpy((char *)pt[2], "aaabc", 5);

    unsigned int ids[3] = {110, 123, 890};
    unsigned int flags[3] = {0, 0, 0};

    flags[0] |= HS_FLAG_SOM_LEFTMOST;
    flags[1] |= HS_FLAG_CASELESS;
    flags[1] |= HS_FLAG_SOM_LEFTMOST;
    flags[2] |= HS_FLAG_CASELESS;
    flags[2] |= HS_FLAG_SOM_LEFTMOST;

    int i = 0;
    char *ptr = "abcdefbcdaAabcddde";
    char *ptr1 = "abcdefbcdAaabcddde";

    printf("%s\n", ptr);

    hs_database_t *database;
    hs_compile_error_t *compile_err;

    hs_error_t err = hs_compile_multi(pt, flags, ids, 3, HS_MODE_BLOCK, NULL, &database, &compile_err);
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: compile %s : %s\n", pt[compile_err->expression], compile_err->message);
        hs_free_compile_error(compile_err);
    }

    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(database);
        return -1;
    }

    err = hs_scan(database, ptr, 18, 0, scratch, onMatch, "ptr");
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan. Exiting.\n");
        hs_free_scratch(scratch);
        hs_free_database(database);
        return -1;
    }

    err = hs_scan(database, ptr1, 18, 0, scratch, onMatch, "ptr1");
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan. Exiting.\n");
        hs_free_scratch(scratch);
        hs_free_database(database);
        return -1;
    }

    hs_free_scratch(scratch);
    hs_free_database(database);

    return 0;
}
