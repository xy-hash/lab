#include <iostream>
#include <vector>
#include <cstring>
#include <string>
using namespace std;


typedef unsigned short uint16;
typedef unsigned int uint32;
uint32 lmove(uint32 val, unsigned int n)
{
    n = n % 32;
    uint32 size = sizeof(val) * 8;
    n = n % size;
    return (val >> (size - n) | (val << n)); //左移
    // return (val << (size - n) | (val >> n)); //右移
}

static unsigned int IV[8] =
{ 0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600,
 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e };

static unsigned char buf[64] = { 0 };

static unsigned int T[64] = { 0 };

void T_init()
{
    for (unsigned int i = 0; i < 16; i++)
        T[i] = 0x79cc4519;
    for (unsigned int i = 16; i < 64; i++)
        T[i] = 0x7a879d8a;
}

unsigned int FF(unsigned int x, unsigned int y, unsigned int z, unsigned int j)
{
    if (j <= 15 && j >= 0)
        return x ^ y ^ z;
    else if (j >= 16 && j <= 63)
        return (x & y) | (x & z) | (y & z);
    return 0;
}

unsigned int GG(unsigned int x, unsigned int y, unsigned int z, unsigned int j)
{
    if (j >= 0 && j <= 15)
        return x ^ y ^ z;
    else if (j >= 16 && j <= 63)
        return (x & y) | ((~x) & z);
    return 0;
}

unsigned int P0(unsigned int x)
{
    return x ^ lmove(x, 9) ^ lmove(x, 17);
}

unsigned int P1(unsigned int x)
{
    return x ^ lmove(x, 15) ^ lmove(x, 23);
}

unsigned int CF(unsigned char* arr)
{
    unsigned int W[68];
    unsigned int W_1[64];
    unsigned int j;
    unsigned int A, B, C, D, E, F, G, H;
    unsigned int SS1, SS2, TT1, TT2;
    for (j = 0; j < 16; j++)
        W[j] = arr[j * 4 + 0] << 24 | arr[j * 4 + 1] << 16 | arr[j * 4 + 2] << 8 | arr[j * 4 + 3];
    for (j = 16; j < 68; j++)
        W[j] = P1(W[j - 16] ^ W[j - 9] ^ (lmove(W[j - 3], 15))) ^ (lmove(W[j - 13], 7)) ^ W[j - 6];
    for (j = 0; j < 64; j++)
        W_1[j] = W[j] ^ W[j + 4];
    A = IV[0];
    B = IV[1];
    C = IV[2];
    D = IV[3];
    E = IV[4];
    F = IV[5];
    G = IV[6];
    H = IV[7];
    for (j = 0; j < 64; j++)
    {
        SS1 = lmove(((lmove(A, 12)) + E + (lmove(T[j], j))) & 0xFFFFFFFF, 7);
        SS2 = SS1 ^ (lmove(A, 12));
        TT1 = (FF(A, B, C, j) + D + SS2 + W_1[j]) & 0xFFFFFFFF;
        TT2 = (GG(E, F, G, j) + H + SS1 + W[j]) & 0xFFFFFFFF;
        D = C;
        C = lmove(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = lmove(F, 19);
        F = E;
        E = P0(TT2);
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

void Block(unsigned char* msg, unsigned int msglen)
{
    unsigned int i;
    unsigned int left = 0;
    unsigned long long total = 0;

    for (i = 0; i < msglen / 64; i++)
    {
        memcpy(buf, msg + i * 64, 64);
        CF(buf);
    }
    total = msglen * 8;
    left = msglen % 64;
    memset(&buf[left], 0, 64 - left);
    memcpy(buf, msg + i * 64, left);
    buf[left] = 0x80;
    if (left <= 55)
    {
        for (i = 0; i < 8; i++)
            buf[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
        CF(buf);
    }
    else
    {
        CF(buf);
        memset(buf, 0, 64);
        for (i = 0; i < 8; i++)
            buf[56 + i] = (total >> ((8 - 1 - i) * 8)) & 0xFF;
        CF(buf);
    }
}
void out_hex()
{
    unsigned int i = 0;
    for (i = 0; i < 8; i++)
    {
        printf("%08x ", IV[i]);
    }
    printf("\n");
}

unsigned int SM3(unsigned char* msg, unsigned int msglen, unsigned char* out_hash)
{
    T_init();
    Block(msg, msglen);
    out_hex();
    return 1;
}

int main(int argc, char* argv[])
{
    unsigned char mes[] = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    unsigned int meslen = strlen((const char*)mes);
    unsigned char hash[32] = { 0 };
    SM3(mes, meslen, hash);
}
