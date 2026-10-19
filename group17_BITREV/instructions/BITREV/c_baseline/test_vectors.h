#ifndef TEST_VECTORS_H
#define TEST_VECTORS_H

#include <stdint.h>

/*
 * CP1 test vectors for BITREV.
 * Owner: member B (1114405042). Add rows here; test_bitrev.c runs them automatically.
 *
 * Rules (HOMEWORK 6.0): at least 10 vectors, at least 1 boundary value.
 * Expected values must be independently verified (hand-calc or reference).
 *
 * B-part verification (1114405042):
 * - 11 vectors, including boundary values 0x00000000, 0x00000001,
 *   0x80000000, 0xFFFFFFFF.
 * - Includes spec example set: 0x00000001, 0x0000000F, 0x12345678.
 * - All expected values verified against rd[i] = rs1[31-i];
 *   `make run` result: 11/11 passed, 0 failed.
 */
typedef struct {
    uint32_t input;
    uint32_t expected;
    const char *note;
} bitrev_vector_t;

static const bitrev_vector_t bitrev_vectors[] = {
    { 0x00000001u, 0x80000000u, "boundary: LSB -> MSB" },
    { 0x0000000Fu, 0xF0000000u, "low nibble -> high nibble" },
    { 0x12345678u, 0x1E6A2C48u, "spec example" },
    { 0x00000000u, 0x00000000u, "boundary: all zero" },
    { 0xFFFFFFFFu, 0xFFFFFFFFu, "boundary: self-symmetric" },
    { 0x80000000u, 0x00000001u, "boundary: MSB -> LSB" },
    { 0x0000FFFFu, 0xFFFF0000u, "swap halves" },
    { 0x00FF00FFu, 0xFF00FF00u, "alternating bytes" },
    { 0xAAAA5555u, 0xAAAA5555u, "self-symmetric pattern" },
    { 0xDEADBEEFu, 0xF77DB57Bu, "random word 1" },
    { 0xF0F0F0F0u, 0x0F0F0F0Fu, "nibble pattern" },
};

#define BITREV_VECTOR_COUNT \
    (sizeof(bitrev_vectors) / sizeof(bitrev_vectors[0]))

#endif /* TEST_VECTORS_H */
