#include <iostream>
#include <string>
#include <cmath>
using namespace std;
//常数：
//初始值
int IV[8] = {
	0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,
	0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e
};
//常量
int Tj(int j)
{
	if (j >= 0 && j <= 15)
		return 0x79cc4519;
	else if (j >= 16 && j <= 63)
		return 0x7a879d8a;
}

//布尔函数：
int FF(int x, int y, int z, int j)
{
	if (j >= 0 && j <= 15)
		return x ^ y ^ z;
	else if (j >= 16 && j <= 63)
		return (x & y) | (x & z) | (y & z);
}

int GG(int x, int y, int z, int j)
{
	if (j >= 0 && j <= 15)
		return x ^ y ^ z;
	else if (j >= 16 && j <= 63)
		return (x & y) | (~x & z);
}

int P0(int x)
{
	return x ^ (x << 9) ^ (x << 17);
}

int P1(int x)
{
	return x ^ (x << 15) ^ (x << 23);
}


string extension(string str) {//消息扩展函数
	string res = str;//字符串类型存储前68位存储扩展字W值
	for (int i = 16; i < 68; i++) {//根据公式生成第17位到第68位的W值
		res += XOR(XOR(P1(XOR(XOR(res.substr((i - 16) * 8, 8), res.substr((i - 9) * 8, 8)), LeftShift(res.substr((i - 3) * 8, 8), 15))), LeftShift(res.substr((i - 13) * 8, 8), 7)), res.substr((i - 6) * 8, 8));
	}
	cout << "扩展后的消息：" << endl;
	cout << "W0,W1,……,W67的消息：" << endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << res.substr(i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}
	cout << res.substr(512, 8) << "  " << res.substr(520, 8) << "  " << res.substr(528, 8) << "  " << res.substr(536, 8) << endl;
	cout << endl;
	for (int i = 0; i < 64; i++) {//根据公式生成64位W'值
		res += XOR(res.substr(i * 8, 8), res.substr((i + 4) * 8, 8));
	}
	cout << "W0',W1',……,W63'的消息：" << endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << res.substr(544 + i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}
	cout << endl;
	return res;
}

string compress(string str1, string str2) {//消息压缩函数
	string IV = str2;
	string A = IV.substr(0, 8), B = IV.substr(8, 8), C = IV.substr(16, 8), D = IV.substr(24, 8), E = IV.substr(32, 8), F = IV.substr(40, 8), G = IV.substr(48, 8), H = IV.substr(56, 8);
	string SS1 = "", SS2 = "", TT1 = "", TT2 = "";
	cout << "迭代压缩中间值: " << endl;
	cout << "    A         B         C         D         E         F        G         H " << endl;
	cout << A << "  " << B << "  " << C << "  " << D << "  " << E << "  " << F << "  " << G << "  " << H << endl;
	for (int j = 0; j < 64; j++) {
		SS1 = LeftShift(ModAdd(ModAdd(LeftShift(A, 12), E), LeftShift(T(j), (j % 32))), 7);
		SS2 = XOR(SS1, LeftShift(A, 12));
		TT1 = ModAdd(ModAdd(ModAdd(FF(A, B, C, j), D), SS2), str1.substr((j + 68) * 8, 8));
		TT2 = ModAdd(ModAdd(ModAdd(GG(E, F, G, j), H), SS1), str1.substr(j * 8, 8));
		D = C;
		C = LeftShift(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = LeftShift(F, 19);
		F = E;
		E = P0(TT2);
		cout << A << "  " << B << "  " << C << "  " << D << "  " << E << "  " << F << "  " << G << "  " << H << endl;
	}
	string res = (A + B + C + D + E + F + G + H);
	cout << endl;
	return res;
}

string iteration(string str) {//迭代压缩函数实现
	int num = str.size() / 128;
	cout << "消息经过填充之后共有 " + to_string(num) + " 个消息分组。" << endl;
	cout << endl;
	string V = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {
		cout << "第 " << to_string(i + 1) << " 个消息分组：" << endl;
		cout << endl;
		B = str.substr(i * 128, 128);
		extensionB = extension(B);
		compressB = compress(extensionB, V);
		V = XOR(V, compressB);
	}
	return V;
}
