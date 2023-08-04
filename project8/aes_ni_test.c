#define _CRT_SECURE_NO_WARNINGS 
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "aes_ni.h"

LARGE_INTEGER nFreq;
LARGE_INTEGER nstart;
LARGE_INTEGER nend;
double tottime = 0;

#define Loading QueryPerformanceFrequency(&nFreq)
#define Start QueryPerformanceCounter(&nstart)
#define End QueryPerformanceCounter(&nend)
#define Add tottime += ((double) (nend.QuadPart - nstart.QuadPart) / (double) nFreq.QuadPart)

//#define AES128
#define AES192
//#define AES256

#ifndef LENGTH
#define LENGTH 16
#endif

ALIGN16 uint8_t AES128_TEST_KEY[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
ALIGN16 uint8_t AES_TEST_VECTOR[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                     0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                                     0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
                                     0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                                     0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
                                     0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                                     0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
                                     0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};

extern int AES_set_encrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);
extern int AES_set_decrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);
extern void AES_ECB_encrypt(const unsigned char* in, //pointer to the PLAINTEXT
    unsigned char* out,      //pointer to the CIPHERTEXT buffer
    unsigned long length,    //text length in bytes
    const char* key,         //pointer to the expanded key schedule
    int number_of_rounds);
void AES_ECB_decrypt(const unsigned char* in, //pointer to the CIPHERTEXT
    unsigned char* out,      //pointer to the DECRYPTED TEXT buffer
    unsigned long length,    //text length in bytes
    const char* key,         //pointer to the expanded key schedule
    int number_of_rounds);

int main()
{
  unsigned int eax, ebx, ecx, edx;
  AES_KEY key;
  AES_KEY decrypt_key;
  uint8_t *PLAINTEXT, *buffer;
  uint8_t *CIPHERTEXT;
  uint8_t *DECRYPTEDTEXT;
  uint8_t *EXPECTED_CIPHERTEXT;
  uint8_t *CIPHER_KEY;
  int i, j, outlen;
  int key_length;
  
  CIPHER_KEY = AES128_TEST_KEY;
  key_length = 128;
  PLAINTEXT = (uint8_t*)malloc(LENGTH);
  CIPHERTEXT = (uint8_t*)malloc(LENGTH);
  DECRYPTEDTEXT = (uint8_t*)malloc(LENGTH);
  buffer = (uint8_t*)malloc(LENGTH);
  buffer = "abcdabcdabcdabcd";
  AES_set_encrypt_key(CIPHER_KEY, key_length, &key);
  AES_set_decrypt_key(CIPHER_KEY, key_length, &decrypt_key);
  Loading;
  Start;
  AES_ECB_encrypt(buffer, CIPHERTEXT, LENGTH, key.KEY, key.nr);
  AES_ECB_decrypt(CIPHERTEXT, DECRYPTEDTEXT, LENGTH, decrypt_key.KEY, decrypt_key.nr);
  End;
  Add;
  printf("加密的明文是 %s\n", buffer);
  printf("加密后密文是 %s\n", CIPHERTEXT);
  printf("解密后明文是 %s\n", DECRYPTEDTEXT);
  int flag = 1;
  for (i = 0; i < LENGTH; i++) if (buffer[i] != DECRYPTEDTEXT[i]) flag = 0;
  if (flag) {
      puts("比较正确，AES加解密成功！\n");
  }
  else {
      puts("比较错误，AES加解密失败！\n");
  }
  printf("所用时间为 %0.6lf ms", tottime*1000);
  return 0;
}