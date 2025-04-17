#include <sodium.h>

#include "crypto.h"


void crypto_init(int crypto_chosen){
    switch(crypto_chosen){
        case 1:
            if(crypto_data.key[0]=='\0'){
                crypto_aead_chacha20poly1305_keygen(crypto_data.key);
            }
            randombytes_buf(crypto_data.nonce, sizeof crypto_data.nonce);
            break;

        default:
            exit(EXIT_FAILURE);
        }
}

void chacha20_encrypt(void){
    

    crypto_aead_chacha20poly1305_encrypt(crypto_data.ciphertext, &crypto_data.ciphertext_len,
            crypto_data.plaintext, MSG_CHAR_LIMIT,
            NULL,NULL,
            NULL, crypto_data.nonce, crypto_data.key);
}

void chacha20_decrypt(void){

    unsigned char decrypted[MSG_CHAR_LIMIT];
    unsigned long long decrypted_len;
    crypto_aead_chacha20poly1305_decrypt(decrypted, &decrypted_len,
                NULL,
                crypto_data.ciphertext, crypto_data.ciphertext_len,
                NULL,
                NULL,
                crypto_data.nonce, crypto_data.key);


    crypto_data.plaintext=decrypted;

}

void aes_encrypt(){
}

void aes_decrypt(){}
