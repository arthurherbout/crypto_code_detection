#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;
uint sbox[256] =   {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
vector<vector<uint> > keys;
uint mat[4][4];
uint mixcols[4][4] = {{0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03},
                    {0x03, 0x01, 0x01, 0x02}};
uint substitute(uint x) {
        return sbox[x];
}
uint RC(int x) {
        uint RCVals[] = {0, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000,
        0x40000000, 0x80000000, 0x1B000000, 0x36000000};
        return RCVals[x];
}
uint g(uint temp, int round) {
        uint bitmask = 255 << 24;
        uint v0, v1, v2, v3;
        v0 = ((temp & bitmask) >> 24);
        bitmask >>= 8;
        v1 = ((temp & bitmask) >> 16);
        bitmask >>= 8;
        v2 = ((temp & bitmask) >> 8);
        bitmask >>= 8;
        v3 = ((temp & bitmask));
        v0 = substitute(v0);
v1 = substitute(v1);
        v2 = substitute(v2);
        v3 = substitute(v3);
        uint ans = (v1 << 24) | (v2 << 16) | (v3 << 8) | (v0);
        ans = ans ^ RC(round);
        return ans;
}
void keyGen(vector<uint> v, int round) {
        if(round > 10) {
                return;
        }
        uint w0, w1, w2, w3;
        w0 = v[0];
        w1 = v[1];
        w2 = v[2];
        w3 = v[3];
        uint t = g(w3, round);
        uint w4 = t ^ w0;
        uint w5 = w4 ^ w1;
        uint w6 = w5 ^ w2;
        uint w7 = w6 ^ w3;
        cout << "Key for round " << dec << round << endl;
        cout << hex << w4 << w5 << w6 << w7 << endl;
        v[0] = w4;
        v[1] = w5;
        v[2] = w6;
        v[3] = w7;
        keys.push_back(v);
        keyGen(v, round + 1);
}
void fillMat(vector<uint> plaintext) {
        int i;
        for(i = 0; i < 4; i++) {
                uint temp = plaintext[i];
                uint bitmask = 255 << 24;
                uint v0, v1, v2, v3;
                v0 = ((temp & bitmask) >> 24);
                bitmask >>= 8;
                v1 = ((temp & bitmask) >> 16);
 bitmask >>= 8;
                v2 = ((temp & bitmask) >> 8);
                bitmask >>= 8;
                v3 = ((temp & bitmask));
                mat[0][i] = v0;
                mat[1][i] = v1;
                mat[2][i] = v2;
                mat[3][i] = v3;
        }
}
void subbytes() {
        int i, j;
        for(i = 0; i < 4; i++) {
                for(j = 0; j < 4; j++) {
                        mat[i][j] = substitute(mat[i][j]);
                }
        }
}
void shiftrows() {
        int i, j;
        for(i = 0; i < 4; i++) {
                int shifts = i;
                vector<uint> temp;
                for(j = 0; j < i; j++) {
                        temp.push_back(mat[i][j]);
                }
                for(j = 0; j < 4; j++) {
                        if(j + shifts < 4) {
                                mat[i][j] = mat[i][j + shifts];
                        } else {
                                mat[i][j] = temp[j + shifts - 4];
                        }
                }
        }
}
void mixcolumns() {
        int i, j, k;
        uint tempmat[4][4];
        for(i = 0; i < 4; i++) {
 for(j = 0; j < 4; j++) {
                        tempmat[j][i] = 0;
                        for(k = 0; k < 4; k++) {
                                if(mixcols[i][k] == 0x01)
                                        tempmat[j][i] ^= mat[k][j];
                                else if(mixcols[i][k] == 0x02) {
                                        uint temp = 0;
                                        temp = mat[k][j] << 1;
                                        if(temp & 256) {
                                                temp ^= 0x11b;
                                        }
                                        tempmat[j][i] ^= temp;
                                } else if(mixcols[i][k] == 0x03) {
                                        uint temp = 0;
                                        temp = mat[k][j] << 1;
                                        if(temp & 256) {
                                                temp ^= 0x11b;
                                        }
                                        tempmat[j][i] ^= (temp ^ mat[k][j]);
                                }

                        }
                }
        }
        for(i = 0; i < 4; i++) {
                for(j = 0; j < 4; j++) {
                        mat[i][j] = tempmat[i][j];
                }
        }
}
vector<uint> addRoundKey(int round) {
        vector<uint> plaintext;
        int i, j;
        for(i = 0; i < 4; i++) {
                uint temp = 0;
                for(j = 0; j < 4; j++) {
                        temp <<= 8;
                        temp |= mat[j][i];
                }
                plaintext.push_back(temp);
        }
        for(i = 0; i < 4; i++) {
                plaintext[i] ^= keys[round][i];
        }
        return plaintext;
}
void print() {
        int i, j;
        for(i = 0; i < 4; i++) {
                for(j = 0; j < 4; j++) {
                        cout << mat[i][j] << " ";
                }
                cout << endl;
        }
        cout << endl;
}
int main() {     cout<<"hi";
        vector<uint> key, plaintext;
        key.push_back(0x2475a2b3);
        key.push_back(0x34755688);
        key.push_back(0x31e21200);
        key.push_back(0x13aa5487);
        plaintext.push_back(0x00041214);
        plaintext.push_back(0x12041200);
        plaintext.push_back(0x0c001311);
        plaintext.push_back(0x08231919);

        uint mat[4][4];
        cout << "Key for round 0" << endl;
        cout << hex << key[0] << key[1] << key[2] << key[3] << endl;
        keys.push_back(key);
        keyGen(key, 1);
        fillMat(plaintext);
        vector<uint> retPlaintext = addRoundKey(0);
        cout << "Plaintext after round 0" << endl;
        cout << hex << retPlaintext[0] << retPlaintext[1] << retPlaintext[2] << retPlaintext[3] << endl;
        retPlaintext[0] = 0x18645a8e;
        retPlaintext[1] = 0x0a68efb2;
        retPlaintext[2] = 0xb96ad710;
        retPlaintext[3] = 0xb5fb79d4;
        int i;
        for(i = 1; i < 10; i++)
        {
            fillMat(retPlaintext);
            subbytes();
            print();
            shiftrows();
            print();
            mixcolumns();
            print();
            retPlaintext = addRoundKey(i);
            cout << "Plain text after round " << i << endl;
            cout << hex << retPlaintext[0] << retPlaintext[1] << retPlaintext[2] << retPlaintext[3] << endl;
        }
        fillMat(retPlaintext);
        subbytes();
        shiftrows();
        retPlaintext = addRoundKey(10);
        cout << "Plaintext after round 10" << endl;
        cout << hex << retPlaintext[0] << retPlaintext[1] << retPlaintext[2] << retPlaintext[3] << endl;

        return 0;
}
