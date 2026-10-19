#ifndef BITREV_H
#define BITREV_H

#include <stdint.h>

/*
 * BITREV 語意 (HOMEWORK.md 6.5):
 *   rd[i] = rs1[31 - i], i = 0 .. 31
 *
 * 一元運算,將 32-bit 輸入位元反轉。純位元重排,與符號無關,不溢位。
 * 性質: bitrev(bitrev(x)) == x
 */
uint32_t bitrev(uint32_t x);

#endif /* BITREV_H */
