#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "bitrev.h"
#include "test_vectors.h"

/*
 * Independent reference: reverses bit i to bit (31 - i) one bit at a time.
 * Used to cross-check bitrev() against the expected values in test_vectors.h.
 * Owner: member C (1114405012).
 */
static uint32_t bitrev_naive(uint32_t x)
{
    uint32_t r = 0;
    for (int i = 0; i < 32; i++) {
        if ((x >> i) & 1u) {
            r |= (uint32_t)1u << (31 - i);
        }
    }
    return r;
}

int main(void)
{
    size_t total = BITREV_VECTOR_COUNT;
    size_t passed = 0;
    size_t failures = 0;

    printf("BITREV CP1 test suite (%zu vectors)\n", total);
    printf("%-4s %-12s %-12s %-12s %-6s %s\n",
           "#", "input", "expected", "actual", "naive", "result");

    for (size_t i = 0; i < total; i++) {
        uint32_t in  = bitrev_vectors[i].input;
        uint32_t exp = bitrev_vectors[i].expected;
        uint32_t got = bitrev(in);
        uint32_t nai = bitrev_naive(in);

        int ok = (got == exp) && (nai == exp);
        if (ok) {
            passed++;
        } else {
            failures++;
        }

        printf("%-4zu 0x%08" PRIX32 "   0x%08" PRIX32
               "   0x%08" PRIX32 "   0x%08" PRIX32 " %s  %s\n",
               i + 1, in, exp, got, nai, ok ? "PASS" : "FAIL",
               bitrev_vectors[i].note);
    }

    printf("\nTotal: %zu, Passed: %zu, Failed: %zu\n",
           total, passed, failures);
    return failures == 0 ? 0 : 1;
}
