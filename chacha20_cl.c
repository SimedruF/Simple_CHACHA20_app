#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CHACHA20_BLOCKSIZE 64

// ChaCha20 quarter round
static inline void chacha20_quarterround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
    *a += *b;
    *d ^= *a;
    *d = ((*d) << 16) | ((*d) >> 16);
    *c += *d;
    *b ^= *c;
    *b = ((*b) << 12) | ((*b) >> 20);
    *a += *b;
    *d ^= *a;
    *d = ((*d) << 8) | ((*d) >> 24);
    *c += *d;
    *b ^= *c;
    *b = ((*b) << 7) | ((*b) >> 25);
}

// ChaCha20 block function
static void chacha20_block(uint32_t input[16], uint32_t output[16])
{
    uint32_t x[16];
    memcpy(x, input, sizeof(x));

    for (int i = 0; i < 10; i++)
    {
        chacha20_quarterround(&x[0], &x[4], &x[8], &x[12]);
        chacha20_quarterround(&x[1], &x[5], &x[9], &x[13]);
        chacha20_quarterround(&x[2], &x[6], &x[10], &x[14]);
        chacha20_quarterround(&x[3], &x[7], &x[11], &x[15]);
        chacha20_quarterround(&x[0], &x[5], &x[10], &x[15]);
        chacha20_quarterround(&x[1], &x[6], &x[11], &x[12]);
        chacha20_quarterround(&x[2], &x[7], &x[8], &x[13]);
        chacha20_quarterround(&x[3], &x[4], &x[9], &x[14]);
    }

    for (int i = 0; i < 16; i++)
    {
        output[i] = x[i] + input[i];
    }
}

// ChaCha20 encryption/decryption
void chacha20(uint8_t *output, const uint8_t *input, size_t len, const uint8_t key[32], const uint8_t nonce[12], uint32_t counter)
{
    uint32_t state[16] = {
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
        *(uint32_t *)key, *(uint32_t *)(key + 4), *(uint32_t *)(key + 8), *(uint32_t *)(key + 12),
        *(uint32_t *)(key + 16), *(uint32_t *)(key + 20), *(uint32_t *)(key + 24), *(uint32_t *)(key + 28),
        counter, *(uint32_t *)nonce, *(uint32_t *)(nonce + 4), *(uint32_t *)(nonce + 8)};

    while (len >= CHACHA20_BLOCKSIZE)
    {
        chacha20_block(state, (uint32_t *)output);
        state[12]++;
        input += CHACHA20_BLOCKSIZE;
        output += CHACHA20_BLOCKSIZE;
        len -= CHACHA20_BLOCKSIZE;
    }

    if (len > 0)
    {
        uint32_t block[16];
        chacha20_block(state, block);
        memcpy(output, block, len);
    }
}