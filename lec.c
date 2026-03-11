#include "lec.h"
#include <string.h>

/* ---------------------------------------------------------
   Internal mutation logic: the core of LEC evolution.
   - XOR table[pointer] with (cipherBase XOR low byte of index)
   - Move pointer forward by (cipherBase + 1)
   - Increment internal index
--------------------------------------------------------- */
static void lec_mutate(LECContext* ctx, uint8_t cipherBase)
{
    ctx->table[ctx->pointer] ^= (cipherBase ^ (uint8_t)(ctx->internal_index & 0xFF));
    ctx->pointer = (ctx->pointer + cipherBase + 1) % LEC_TABLE_SIZE;
    ctx->internal_index++;
}

/* ---------------------------------------------------------
   FNV-1a 32-bit hash for table synchronization
--------------------------------------------------------- */
uint32_t lec_get_fingerprint(const LECContext* ctx)
{
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < LEC_TABLE_SIZE; i++) {
        hash ^= ctx->table[i];
        hash *= 16777619u;
    }
    return hash;
}

/* ---------------------------------------------------------
   Initialize evolving table using seed, jump, and mask.
   Matches the C# implementation exactly.
--------------------------------------------------------- */
int lec_init(LECContext* ctx, uint64_t seed, int jump, uint8_t mask)
{
    if (!ctx) return -1;

    for (size_t i = 0; i < LEC_TABLE_SIZE; i++) {
        uint64_t val = ((uint64_t)i ^ seed) * (uint64_t)(jump + (i % 257));
        uint64_t v2 = (val ^ (val >> 13)) ^ mask;
        ctx->table[i] = (uint8_t)(v2 & 0xFF);
    }

    lec_reset(ctx);
    return 0;
}

/* ---------------------------------------------------------
   Reset pointer and index (table remains mutated)
--------------------------------------------------------- */
void lec_reset(LECContext* ctx)
{
    if (!ctx) return;
    ctx->pointer = 0;
    ctx->internal_index = 0;
}

/* ---------------------------------------------------------
   Encrypt buffer (cipherBase = ciphertext)
--------------------------------------------------------- */
int lec_encrypt_buffer(LECContext* ctx,
    const uint8_t* in, uint8_t* out, size_t len)
{
    if (!ctx || !in || !out) return -1;

    for (size_t i = 0; i < len; i++) {
        uint8_t key = ctx->table[(ctx->pointer +
            (size_t)(ctx->internal_index % LEC_TABLE_SIZE))
            % LEC_TABLE_SIZE];

        out[i] = in[i] ^ key;

        /* Mutate using ciphertext */
        lec_mutate(ctx, out[i]);
    }
    return 0;
}

/* ---------------------------------------------------------
   Decrypt buffer (cipherBase = ciphertext input)
--------------------------------------------------------- */
int lec_decrypt_buffer(LECContext* ctx,
    const uint8_t* in, uint8_t* out, size_t len)
{
    if (!ctx || !in || !out) return -1;

    for (size_t i = 0; i < len; i++) {
        uint8_t key = ctx->table[(ctx->pointer +
            (size_t)(ctx->internal_index % LEC_TABLE_SIZE))
            % LEC_TABLE_SIZE];

        out[i] = in[i] ^ key;

        /* Mutate using ciphertext (input) */
        lec_mutate(ctx, in[i]);
    }
    return 0;
}