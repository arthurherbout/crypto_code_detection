#include "md5.h"

const uint32_t constants_t[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

const uint32_t constants_s[] = {
	 7, 12, 17, 22,
	 7, 12, 17, 22,
	 7, 12, 17, 22,
	 7, 12, 17, 22,
	 5,  9, 14, 20,
	 5,  9, 14, 20,
	 5,  9, 14, 20,
	 5,  9, 14, 20,
	 4, 11, 16, 23,
	 4, 11, 16, 23,
	 4, 11, 16, 23,
	 4, 11, 16, 23,
	 6, 10, 15, 21,
	 6, 10, 15, 21,
	 6, 10, 15, 21,
	 6, 10, 15, 21,
};

const uint32_t constants_k[] = {
	  0,  1,  2,  3,
	  4,  5,  6,  7,
	  8,  9, 10, 11,
	 12, 13, 14, 15,
	  1,  6, 11,  0,
	  5, 10, 15,  4,
	  9, 14,  3,  8,
	 13,  2,  7, 12,
	  5,  8, 11, 14,
	  1,  4,  7, 10,
	 13,  0,  3,  6,
	  9, 12, 15,  2,
	  0,  7, 14,  5,
	 12,  3, 10,  1,
	  8, 15,  6, 13,
	  4, 11,  2,  9,
};

static uint32_t func_f(uint32_t b, uint32_t c, uint32_t d) {
	return (b & c) | ((~b) & d);
}

static uint32_t func_g(uint32_t b, uint32_t c, uint32_t d) {
	return (d & b) | ((~d) & c);
}

static uint32_t func_h(uint32_t b, uint32_t c, uint32_t d) {
	return b ^ c ^ d;
}

static uint32_t func_i(uint32_t b, uint32_t c, uint32_t d) {
	return c ^ (b | (~d));
}

static void md5_step(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d,
	uint32_t* data_in_32, uint32_t j, uint32_t n, uint32_t (*f)(uint32_t, uint32_t, uint32_t)) {
		
	uint32_t shift = *a + f(*b, *c, *d) + data_in_32[n + constants_k[j]] + constants_t[j];
	*a = *b + ((shift << constants_s[j]) | (shift >> (32 - constants_s[j])));
	shift = *d;
	*d = *c;
	*c = *b;
	*b = *a;
	*a = shift;
}

char* md5_hash(const char* data) {

	// рассчет длины нового массива
	uint64_t data_len = strlen(data);
	uint64_t new_len = (data_len / 64 + (data_len % 64 > 55 ? 2 : 1)) * 16;

	// выравнивание и заполнение массива
	uint32_t* data_in_32 = (uint32_t*)calloc(new_len, sizeof(uint32_t));

	for (uint64_t i = 0; i < data_len; i++)
		data_in_32[i / 4] |= data[i] << ((i % 4) * 8);

	data_in_32[data_len / 4] |= 0x80 << (data_len % 4) * 8;
	data_in_32[new_len - 2] = data_len * 8;
	data_in_32[new_len - 1] = data_len * 8 << 32;

	// инициализация переменных
	uint32_t consts[] = {0x67452301, 0xEFCDAB89,  0x98BADCFE, 0x10325476};

	// основной цикл
	for (uint64_t i = 0; i < new_len / 16; i++) {

		uint32_t a = consts[0], b = consts[1], c = consts[2], d = consts[3];
		uint32_t n = i * 16;

		for (uint32_t j = 0; j < 16; j++)
			md5_step(&a, &b, &c, &d, data_in_32, j, n, func_f);

		for (uint32_t j = 16; j < 32; j++)
			md5_step(&a, &b, &c, &d, data_in_32, j, n, func_g);

		for (uint32_t j = 32; j < 48; j++)
			md5_step(&a, &b, &c, &d, data_in_32, j, n, func_h);

		for (uint32_t j = 48; j < 64; j++)
			md5_step(&a, &b, &c, &d, data_in_32, j, n, func_i);

		consts[0] += a;
		consts[1] += b;
		consts[2] += c;
		consts[3] += d;

	}

	// перевод в строку
	char* result = (char*)malloc(32 * sizeof(char));
	for (uint32_t i = 0; i < 4; i++)
		for (uint32_t j = 0, temp = (consts[i] >> 4) & 0xF; j < 8;
			j++, temp = (consts[i] >> (j % 2 == 0 ? (j + 1) * 4 : (j - 1) * 4)) & 0xF)
			result[i * 8 + j] = temp + (temp > 9 ? 87 : 48);

	free(data_in_32);
	return result;
}
