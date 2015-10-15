#pragma once
#ifndef _CLASSICALCIPHER_H
#define _CLASSICALCIPHER_H

//Base Cipher Class
class cipher {
public:
	cipher() {};
	~cipher() {};
	virtual string Encrypt(string text) = 0;
	virtual string Decipher(string ciphertext) = 0;
protected:
	int AlphabetToNum(char textChar) {
		if ('a' <= textChar && textChar <= 'z') {
			return textChar - 'a';
		}
		cout << "you may use some other symbol." << endl;
		exit(1);
	}
	char NumToAlphabet(int textNum) { return textNum + 'a'; };
	int Eculid(int x, int y) { return y == 0 ? x : Eculid(y, x%y); };
	int Mod(int temp) { return temp >= 0 ? temp % 26 : (temp % 26) + 26; };
};
/*******************************************************/

//Affine Cipher
class AffineCipher : public cipher {
public:
	AffineCipher();
	~AffineCipher() {};
	string Encrypt(string text);
	string Decipher(string ciphertext);
	void ShowFactor();
private:
	int factor;			//encrypt factor
	int addend;			//encrypt addend
	int inverseFactor;	
	int yie;			
	void ExtendEculid(int x, int y);
};
/*******************************************************/

//Hill Cipher
typedef int** matrix;
class HillCipher : public cipher {
public:
	HillCipher();
	~HillCipher() {};
	string Encrypt(string text);
	string Decipher(string ciphertext);
private:
	matrix encryptMatrix;
	matrix decipherMatrix;
	matrix codesUnit;
	int maxDimensionSize;
	matrix CodesUnit_Mul_EncryptMatrix();
	matrix CodesUnit_Mul_DecipherMatrix();
	int det() { return Mod(encryptMatrix[0][0] * encryptMatrix[1][1] - encryptMatrix[0][1] * encryptMatrix[1][0]); }
	void Inverse(matrix);
	void ExtendEculid(int x, int y, int &xi, int &yi);
};
/*******************************************************/

//Vernam Cipher
class VernamCipher : public cipher {
public:
	VernamCipher();
	~VernamCipher() {};
	void CreateCipherKey(int size);
	string Encrypt(string text);
	string Decipher(string ciphertext);
private:
	string cipherKey;
};
/*******************************************************/

//Playfair Cipher
typedef char** pfMatrix;
class PlayfairCipher : public cipher {
public:
	PlayfairCipher(string encryptWord = "helo");
	~PlayfairCipher();
	string Encrypt(string text);
	string Decipher(string ciphertext);
	void Show();
private:
	pfMatrix encryptMatrix;
	void FindLetter(char, int&, int&);
	void PopSameLetter(string &encrypWord);
	string Encrypt_2_Letters(int x1, int y1, int x2, int y2);
	string Decipher_2_Letters(int x1, int y1, int x2, int y2);
};

/*******************************************************/

//Vigenere Cipher
class VigenereCipher: public cipher {
public:
	VigenereCipher(string encryptWord = "hello");
	~VigenereCipher() {};
	string Encrypt(string text);
	string Decipher(string ciphertext);
private:
	string encryptKey;
};
/*******************************************************/
#endif 