#ifndef CRYPTO_H
#define CRYPTO_H

#include <sodium.h>

#define MSG_CHAR_LIMIT 256

struct crypto_data{
    int crypto_type;
    unsigned char* plaintext[MSG_CHAR_LIMIT];
    unsigned long long plaintext_len;
    unsigned char* ciphertext[MSG_CHAR_LIMIT + crypto_aead_chacha20poly1305_ABYTES];
    unsigned long long ciphertext_len;
    unsigned char* nonce[crypto_aead_chacha20poly1305_NPUBBYTES];
    unsigned char* key[crypto_aead_chacha20poly1305_KEYBYTES];
}crypto_data;

void crypto_init(int crypto_chosen); 
void chacha20_encrypt(void);
void chacha20_decrypt(void);


#endif
