#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/types.h>
#include <string.h>
#include <stdio.h>

void handleErrors(void) {
    //ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;

    // Creează și inițializează contextul
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Inițializează operațiunea de criptare
    if (1 != EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv))
        handleErrors();

    // Criptează mesajul
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    // Finalizează criptarea
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Curăță contextul
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;

    int len;
    int plaintext_len;

    // Creează și inițializează contextul
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Inițializează operațiunea de decriptare
    if (1 != EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv))
        handleErrors();

    // Decriptează mesajul
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    // Finalizează decriptarea
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    // Curăță contextul
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void) {
    // Exemplu de cheie și IV (trebuie să fie de dimensiuni corecte)
    unsigned char key[32];
    unsigned char iv[12];

    // Generează cheie și IV random
    if (!RAND_bytes(key, sizeof(key))) handleErrors();
    if (!RAND_bytes(iv, sizeof(iv))) handleErrors();

    // Mesajul de criptat
    unsigned char *plaintext = (unsigned char *)"Mesaj secret";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    // Criptare
    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);

    // Decriptare
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);

    // Adăugare null-terminator
    decryptedtext[decryptedtext_len] = '\0';

    // Afișare rezultate
    printf("Text criptat este:\n");
    BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);
    printf("Text decriptat este:\n%s\n", decryptedtext);

    return 0;
}
