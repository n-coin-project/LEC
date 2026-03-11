#ifndef LEC_H
#define LEC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Fixed table size: 100 KB */
#define LEC_TABLE_SIZE (100 * 1024)

/* LEC internal context */
    typedef struct LECContext {
        uint8_t  table[LEC_TABLE_SIZE];   /* Evolving table */
        size_t   pointer;                 /* Current pointer position */
        uint64_t internal_index;          /* History-dependent index */
    } LECContext;

    /* Initialize the evolving table using seed, jump, and mask */
    int  lec_init(LECContext* ctx, uint64_t seed, int jump, uint8_t mask);

    /* Reset pointer and index (table remains mutated) */
    void lec_reset(LECContext* ctx);

    /* Encrypt buffer */
    int  lec_encrypt_buffer(LECContext* ctx,
        const uint8_t* in, uint8_t* out, size_t len);

    /* Decrypt buffer */
    int  lec_decrypt_buffer(LECContext* ctx,
        const uint8_t* in, uint8_t* out, size_t len);

    /* Compute 32-bit FNV-1a fingerprint of the table */
    uint32_t lec_get_fingerprint(const LECContext* ctx);

#ifdef __cplusplus
}
#endif

#endif /* LEC_H */