#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define uint_8 unsigned char
#define uint_32 unsigned int

uint_8 s1[64];
uint_8 s2[64];

uint_8 message_buffer[64];
uint_32 IV[8];
uint_32 T[64];


int Check(uint_32 a, uint_32 b, int nbit) {
	int i;
	for(i = 0; i < nbit; i ++) {
		if((a & (1 << (31 - i))) == (b & (1 << (31 - i)))) continue;
		else return 0;
	}
	return 1;
}

uint_32 rotate_left(uint_32 a, uint_32 k) {
	k = k % 32;
	return ((a << k) & 0xFFFFFFFF) | ((a & 0xFFFFFFFF) >> (32 - k));
}

uint_32 P_0(uint_32 X) {
	return X ^ (rotate_left(X, 9)) ^ (rotate_left(X, 17));
}
uint_32 P_1(uint_32 X) {
	return X ^ (rotate_left(X, 15)) ^ (rotate_left(X, 23));
}

LARGE_INTEGER nFreq;
LARGE_INTEGER nstart;
LARGE_INTEGER nend;
double tottime = 0;

#define Loading QueryPerformanceFrequency(&nFreq);
#define Start QueryPerformanceCounter(&nstart);
#define End QueryPerformanceCounter(&nend);
#define Add tottime += ((double) (nend.QuadPart - nstart.QuadPart) / (double) nFreq.QuadPart);

uint_32 FF(X, Y, Z, j) {
	uint_32 ret = 0;
	if (0 <= j && j < 16) ret = X ^ Y ^ Z;
	else if (16 <= j && j < 64) ret = (X & Y) | (X & Z) | (Y & Z);
	return ret;
}

uint_32 GG(X, Y, Z, j) {
	uint_32 ret = 0;
	if (0 <= j && j < 16) ret = X ^ Y ^ Z;
	else if (16 <= j && j < 64) ret = (X & Y) | ((~X) & Z);
	return ret;
}

void init() {
	int i = 0;
	for(i = 0; i < 64; i ++) message_buffer[i] = 0;
	for (i = 0; i < 16; i++) T[i] = 0x79cc4519;
	for (i = 16; i < 64; i++) T[i] = 0x7a879d8a;
	IV[0] = 0x7380166f;
	IV[1] = 0x4914b2b9;
	IV[2] = 0x172442d7;
	IV[3] = 0xda8a0600;
	IV[4] = 0xa96f30bc;
	IV[5] = 0x163138aa;
	IV[6] = 0xe38dee4d;
	IV[7] = 0xb0fb0e4e;
}

int CF(uint_8 *arr) {
	uint_32 W[68];
	uint_32 W_1[64];
	uint_32 j;
	uint_32 A, B, C, D, E, F, G, H;
	uint_32 SS1, SS2, TT1, TT2;
	for (j = 0; j < 16; j++)
		W[j] = arr[j * 4 + 0] << 24 | arr[j * 4 + 1] << 16 | arr[j * 4 + 2] << 8 | arr[j * 4 + 3];
	for (j = 16; j < 68; j++)
		W[j] = P_1(W[j - 16] ^ W[j - 9] ^ (rotate_left(W[j - 3], 15))) ^ (rotate_left(W[j - 13], 7)) ^ W[j - 6];
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

	for(i = 0; i < msglen/64; i++) {
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

void TEST() {
	Loading
	srand(time(0) + 19260817);
	int nbit;
	for(nbit = 8; nbit <= 20; nbit ++) {
		int i, tot = 0;
		int flag = 0;
		Start
		tottime = 0;
		while(1) {
			for(i = 0; i < 4; i ++) {
				s1[i] = rand() % 256;
				s2[i] = rand() % 256;
			}
			if(s1[0] == s2[0] && s1[1] == s2[1] && s1[2] == s2[2] && s1[3] == s2[3]) continue;
			tot ++;
			init();
			uint_32 a = SM3(s1, 64)[0];
			init();
			uint_32 b = SM3(s2, 64)[0];
			if(Check(a, b, nbit)) break;
		}
		End
		Add
		printf("%dbitÅö×²ÕÒµ½£¡Ê±¼ä%.6lf\n", nbit, tottime);
	}
}

// 512bit -> 256bit
int main() {
	TEST();
	return 0;
}
