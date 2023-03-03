#include <stdint.h>
#include <Windows.h>
#include <iostream>
#define _STD_ using namespace std

_STD_;

class _config {
public:
	void _cfg(void) {
		SetConsoleTextAttribute(floy, color);
		SetConsoleTitle(title);
		setlocale(NULL, _Command);
		system(_sys);
		WND.hIcon = (HICON)LoadImage(NULL, TEXT("encryption.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	}

private:
	WORD color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
	LPCWSTR title = L"XTEA C++";
	HANDLE floy = GetStdHandle(STD_OUTPUT_HANDLE);
	const CHAR* _Command = "";
	const CHAR* _sys = "chcp 1251 > null";
	WNDCLASS WND;
}; _config Q;

unsigned int key[4] = { 
	0xFDA5,0xD54E,0xFC00,0xB55A 
}; // encryption key

#define BLOCK_SIZE 8

void xtea_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;
	for (i = 0; i < num_rounds; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
	}
	v[0] = v0; v[1] = v1;


}
void xtea_decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], delta = 0x9E3779B9, sum = delta * num_rounds;
	for (i = 0; i < num_rounds; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
	}
	v[0] = v0; v[1] = v1;
}
void StringCrypt(char* inout, int len, bool encrypt)
{
	for (int i = 0; i < len / BLOCK_SIZE; i++)
	{
		if (encrypt)
			xtea_encipher(32, (uint32_t*)(inout + (i * BLOCK_SIZE)), key);
		else
			xtea_decipher(32, (uint32_t*)(inout + (i * BLOCK_SIZE)), key);
	}
	if (len % BLOCK_SIZE != 0)
	{
		int mod = len % BLOCK_SIZE;
		int offset = (len / BLOCK_SIZE) * BLOCK_SIZE;
		char data[BLOCK_SIZE];
		memcpy(data, inout + offset, mod);

		if (encrypt)
			xtea_encipher(32, (uint32_t*)data, key);
		else
			xtea_decipher(32, (uint32_t*)data, key);

		memcpy(inout + offset, data, mod);
	}
}

int main()
{
	Q._cfg();
	char str[32] = { 0 };
	cout << "Enter a text to crypt: ";
	cin >> str;
	cout << endl;
	int len = sizeof(str);

	StringCrypt(str, len, true);
	int i2[32];
	cout.unsetf(ios::dec);
	cout.setf(ios::hex);
	cout << "Encrypted string: ";

	for (int i = 0; i < len; i++) {
		i2[i] = (unsigned char)str[i];
		cout << i2[i];
	}

	cout << endl;

	StringCrypt(str, len, false);

	cout << "Decrypted string: ";
	cout << str << endl;
	cin.get();
	return NULL;
}

/*
	v � �������� ����� ��������� �� ���� ���� �� 32 ����
	k � ���� ��������� �� ������ 32-������ ����
	num_rounds � ����� ������ ��������� (������������� 32)
	num_rounds ������ ���� ���������� ��� ���������� � �������������,
	���� num_rounds==0 �� �� ����������, �� ������������� ����������� �� �����
	��������� �� ��������� � ������������ �����:

	������������ ��� uint32_t ���������� ����, ��� �� ������ ����� ������ 32 ���� � ������� �� long
	(�������������� � ������������ ����), ������� ����� ��������� 64 ���� (�������� � ��������� 64-������ ��������)
	�������� ��� �� ���������� ��� const
	� �������� ���� ������� ���������� ������ � ���������� ��� v0 � v1, � ������ ����������� 
	��� ��������� ��� ������� �����������
*/