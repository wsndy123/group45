#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define uint_8 unsigned char
#define uint_32 unsigned int

LARGE_INTEGER nFreq;
LARGE_INTEGER nstart;
LARGE_INTEGER nend;
double tottime = 0;

#define Loading QueryPerformanceFrequency(&nFreq);
#define Start QueryPerformanceCounter(&nstart);
#define End QueryPerformanceCounter(&nend);
#define Add tottime += ((double) (nend.QuadPart - nstart.QuadPart) / (double) nFreq.QuadPart);

uint_8 message_buffer[64] = {0};
uint_32 IV[8] = {
	0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e
}; 
uint_32 T[64] = {
	0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
	0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a
};

#define rotate_left(a, k) ((a << k) & 0xFFFFFFFF) | ((a & 0xFFFFFFFF) >> (32 - k))
#define P_0(X) (X ^ (rotate_left(X, 9)) ^ (rotate_left(X, 17)))
#define P_1(X) (X ^ (rotate_left(X, 15)) ^ (rotate_left(X, 23)))
#define FF(X, Y, Z, j) (0 <= j && j < 16) ? (X ^ Y ^ Z) : ((16 <= j && j < 64) ? (X & Y) | (X & Z) | (Y & Z) : 0)
#define GG(X, Y, Z, j) (0 <= j && j < 16) ? (X ^ Y ^ Z) : ((16 <= j && j < 64) ? (X & Y) | ((~X) & Z) : 0)

int CF(uint_8 *arr) {
	uint_32 W[68];
	uint_32 W_1[64];
	uint_32 j;
	uint_32 A, B, C, D, E, F, G, H;
	uint_32 SS1, SS2, TT1, TT2;
	for (j = 0; j < 16; j++)
		W[j] = arr[j * 4 + 0] << 24 | arr[j * 4 + 1] << 16 | arr[j * 4 + 2] << 8 | arr[j * 4 + 3];
	/*W[0] = arr[0] << 24 | arr[1] << 16 | arr[2] << 8 | arr[3];
	W[1] = arr[4] << 24 | arr[5] << 16 | arr[6] << 8 | arr[7];
	W[2] = arr[8] << 24 | arr[9] << 16 | arr[10] << 8 | arr[11];
	W[3] = arr[12] << 24 | arr[13] << 16 | arr[14] << 8 | arr[15];
	W[4] = arr[16] << 24 | arr[17] << 16 | arr[18] << 8 | arr[19];
	W[5] = arr[20] << 24 | arr[21] << 16 | arr[22] << 8 | arr[23];
	W[6] = arr[24] << 24 | arr[25] << 16 | arr[26] << 8 | arr[27];
	W[7] = arr[28] << 24 | arr[29] << 16 | arr[30] << 8 | arr[31];
	W[8] = arr[32] << 24 | arr[33] << 16 | arr[34] << 8 | arr[35];
	W[9] = arr[36] << 24 | arr[37] << 16 | arr[38] << 8 | arr[39];
	W[10] = arr[40] << 24 | arr[41] << 16 | arr[42] << 8 | arr[43];
	W[11] = arr[44] << 24 | arr[45] << 16 | arr[46] << 8 | arr[47];
	W[12] = arr[48] << 24 | arr[49] << 16 | arr[50] << 8 | arr[51];
	W[13] = arr[52] << 24 | arr[53] << 16 | arr[54] << 8 | arr[55];
	W[14] = arr[56] << 24 | arr[57] << 16 | arr[58] << 8 | arr[59];
	W[15] = arr[60] << 24 | arr[61] << 16 | arr[62] << 8 | arr[63];*/
	
	for (j = 16; j < 68; j += 2) {
		W[j] = P_1(W[j - 16] ^ W[j - 9] ^ (rotate_left(W[j - 3], 15))) ^ (rotate_left(W[j - 13], 7)) ^ W[j - 6];
		W[j + 1] = P_1(W[j - 15] ^ W[j - 8] ^ (rotate_left(W[j - 2], 15))) ^ (rotate_left(W[j - 12], 7)) ^ W[j - 5];
	}
		
	
	for (j = 0; j < 64; j++) W_1[j] = W[j] ^ W[j + 4];
	A = IV[0];
	B = IV[1];
	C = IV[2];
	D = IV[3];
	E = IV[4];
	F = IV[5];
	G = IV[6];
	H = IV[7];
	for (j = 0; j < 64; j++) {
		SS1 = rotate_left(((rotate_left(A, 12)) + E + (rotate_left(T[j], j))) & 0xFFFFFFFF, 7);
		SS2 = SS1 ^ (rotate_left(A, 12));
		TT1 = (FF(A, B, C, j) + D + SS2 + W_1[j]) & 0xFFFFFFFF;
		TT2 = (GG(E, F, G, j) + H + SS1 + W[j]) & 0xFFFFFFFF;
		D = C;
		C = rotate_left(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = rotate_left(F, 19);
		F = E;
		E = P_0(TT2);
	}
	IV[0] = (A ^ IV[0]);
	IV[1] = (B ^ IV[1]);
	IV[2] = (C ^ IV[2]);
	IV[3] = (D ^ IV[3]);
	IV[4] = (E ^ IV[4]);
	IV[5] = (F ^ IV[5]);
	IV[6] = (G ^ IV[6]);
	IV[7] = (H ^ IV[7]);
	return 1;
}


void Block(uint_8 *msg, uint_8 msglen) {
	int i;
	int left = 0;
	unsigned long long total = 0;
	for(i = 0; i < msglen / 64; i++) {
		memcpy(message_buffer, msg + i * 64, 64);
		CF(message_buffer);
	}
	total = msglen * 8;
	left = msglen%64;
	memset(&message_buffer[left], 0, 64 - left);
	memcpy(message_buffer, msg + i * 64, left);
	message_buffer[left] = 0x80;
	if(left <= 55) {
		for (i = 0; i < 8; i++)
			message_buffer[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
		CF(message_buffer);
	} else {
		CF(message_buffer);
		memset(message_buffer, 0, 64);
		for (i = 0; i < 8; i++)
			message_buffer[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
		CF(message_buffer);
	}
}

uint_32* SM3(uint_8 *msg, uint_32 msglen) {
	Block(msg,msglen);
	return IV;
}

int main() {
	uint_8 s[] = "bcbcabcabcabcabcabcabcabcabcabcabcaabcbcabcabcabcabcabcabcabcabc";
	Loading
	Start
	SM3(s, 64);
	End
	Add
	printf("%.6lf", tottime*1000000); 
	return 0;
}
