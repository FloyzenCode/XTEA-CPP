#include <Windows.h>
#include <stdint.h>

using namespace std;

class XTEA_LOST {
public:
	uint32_t rol(uint32_t base, uint32_t shift)
	{
		uint32_t res;
		/* only 5 bits of shift are significant*/
		shift &= 0x1F;
		res = (base << shift) | (base >> (32 - shift));
		return res;
	}

	void xtea2_encipher(unsigned int num_rounds, uint32_t* v, uint32_t const* k) {
		unsigned int i;
		uint32_t a, b, c, d, sum = 0, t, delta = 0x9E3779B9;
		a = v[0];
		b = v[1] + k[0];
		c = v[2];
		d = v[3] + k[1];
		for (i = 0; i < num_rounds; i++) {
			a += ((b << 4) ^ (b >> 5)) + (d ^ sum) + rol(k[sum & 3], b);
			sum += delta;
			c += ((d << 4) ^ (d >> 5)) + (b ^ sum) + rol(k[(sum >> 11) & 3], d);
			t = a; a = b; b = c; c = d; d = t;
		}
		v[0] = a ^ k[2];
		v[1] = b;
		v[2] = c ^ k[3];
		v[3] = d;
	}

	void xtea2_decipher(unsigned int num_rounds, uint32_t* v, uint32_t const* k) {
		unsigned int i;
		uint32_t a, b, c, d, t, delta = 0x9E3779B9, sum = delta * num_rounds;
		d = v[3];
		c = v[2] ^ k[3];
		b = v[1];
		a = v[0] ^ k[2];
		for (i = 0; i < num_rounds; i++) {
			t = d; d = c; c = b; b = a; a = t;
			c -= ((d << 4) ^ (d >> 5)) + (b ^ sum) + rol(k[(sum >> 11) & 3], d);
			sum -= delta;
			a -= ((b << 4) ^ (b >> 5)) + (d ^ sum) + rol(k[sum & 3], b);
		}
		v[0] = a;
		v[1] = b - k[0];
		v[2] = c;
		v[3] = d - k[1];
	}
private:
}; //XTEA crypt;