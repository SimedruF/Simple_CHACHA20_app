#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/types.h>
#include <string.h>
#include <stdio.h>

void handleErrors(void) {
    //ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) 
{
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    // Create and init the context 
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Init the encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv))
        handleErrors();

    // encrypt the message
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    // final encryption 
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Free context
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) 
{
    EVP_CIPHER_CTX *ctx;

    int len;
    int plaintext_len;

    // Create and init the context 
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Init the encryption
    if (1 != EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv))
        handleErrors();

    // Decrypt the mesage
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    // final decrypt
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    // Free context
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void) {
    // Key and IV the dimensions must be corect
    unsigned char key[32];
    unsigned char iv[12];

    // generate random the key and IV  
    if (!RAND_bytes(key, sizeof(key))) handleErrors();
    if (!RAND_bytes(iv, sizeof(iv))) handleErrors();

    // Secret mesaje
    unsigned char *plaintext = (unsigned char *)"Secret message for NSA ..";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    // encrypt
    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);

    // Decrypt
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);

    // add a null-terminator
    decryptedtext[decryptedtext_len] = '\0';

    // Rezult
    printf("Encrypted Text :\n");
    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);
    printf("Decrypted Text:\n%s\n", decryptedtext);

    return 0;
}
