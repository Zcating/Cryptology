#include "stdafx.h"
#include "Cipher.h"

/*AffineCipher*/
AffineCipher::AffineCipher()
{
	inverseFactor = 0;
	yie = 1;
	srand((unsigned int)time(nullptr));
	do
		factor = rand() % 26;
	while (Eculid(26, factor) != 1 && factor != 1);
	addend = rand() % 101;
	ExtendEculid(26, factor);
}

string AffineCipher::Encrypt(string text)
{
	string ciphertext;

	for (int i = 0; i < (int)(int)text.size(); i++) {
		int tempCipher = AlphabetToNum(text[i]);
		tempCipher = (tempCipher * factor + addend) % 26;
		ciphertext += NumToAlphabet(tempCipher);
	}
	return ciphertext;
}

string AffineCipher::Decipher(string ciphertext)
{
	string text;

	for (int i = 0; i < (int)ciphertext.size(); i++) {
		int tempCipher = AlphabetToNum(ciphertext[i]);
		tempCipher = (tempCipher - addend)*inverseFactor % 26;
		if (tempCipher < 0) tempCipher += 26;
		text += NumToAlphabet(tempCipher);
	}
	return string(text);
}

void AffineCipher::ShowFactor()
{
	cout << factor <<" " << addend << endl;
}

void AffineCipher::ExtendEculid(int x, int y)
{
	if (y == 0)
		return;
	ExtendEculid(y, x%y);
	int temp = yie;
	yie = inverseFactor;
	inverseFactor = temp - (x / y)*inverseFactor;
	return;
}
/**********************************************/


/*HillCipher*/
HillCipher::HillCipher() {
	maxDimensionSize = 2;
	//int arr[2][2] = { {11, 8}, {3, 7} };
	encryptMatrix = new int*[maxDimensionSize];
	decipherMatrix = new int*[maxDimensionSize];
	codesUnit = new int*;
	codesUnit[0] = new int[maxDimensionSize];
	for (int j = 0; j < maxDimensionSize; j++) {
		encryptMatrix[j] = new int[maxDimensionSize];
		decipherMatrix[j] = new int[maxDimensionSize];
	}
	srand((unsigned int)time(nullptr));
	do {
		for (int i = 0; i < maxDimensionSize*maxDimensionSize; i++)
			encryptMatrix[i / maxDimensionSize][i % maxDimensionSize] = rand() % 26;
			//encryptMatrix[i / maxDimensionSize][i % maxDimensionSize] = arr[i / maxDimensionSize][i % maxDimensionSize];
	} 
	while (det() != 1);
	Inverse(encryptMatrix);
	for (int i = 0; i < maxDimensionSize*maxDimensionSize; i++)
		decipherMatrix[i / maxDimensionSize][i % maxDimensionSize] = 
		Mod(decipherMatrix[i / maxDimensionSize][i % maxDimensionSize]);
}

string HillCipher::Encrypt(string text)
{
	string ciphertext;

	//如果文本长度小于加密矩阵的行数，则补上冗余项
	while ((int)text.size() % maxDimensionSize != 0)
		text += (rand() % 26) + 'a';
	
	for (int turn = 0; turn < (int)text.size(); turn += maxDimensionSize) {
		for (int i = 0; i < maxDimensionSize; i++)
			codesUnit[0][i] = AlphabetToNum(text[turn + i]);
		codesUnit = CodesUnit_Mul_EncryptMatrix();
		for (int i = 0; i < maxDimensionSize; i++)
			ciphertext += NumToAlphabet(codesUnit[0][i]);
	}
	return ciphertext;
}

string HillCipher::Decipher(string ciphertext)
{
	string text;
	for (int turn = 0; turn < (int)ciphertext.size(); turn += maxDimensionSize) {
		for (int i = 0; i < maxDimensionSize; i++)
			codesUnit[0][i] = AlphabetToNum(ciphertext[turn + i]);
		codesUnit = CodesUnit_Mul_DecipherMatrix();
		for (int i = 0; i < maxDimensionSize; i++)
			text += NumToAlphabet(codesUnit[0][i]);
	}
	return text;
}
matrix HillCipher::CodesUnit_Mul_EncryptMatrix()
{
	matrix mul;
	mul = new int*;
	mul[0] = new int[maxDimensionSize];
	mul[0][0] = (codesUnit[0][0] * encryptMatrix[0][0] + codesUnit[0][1] * encryptMatrix[1][0])%26;
	mul[0][1] = (codesUnit[0][0] * encryptMatrix[0][1] + codesUnit[0][1] * encryptMatrix[1][1])%26;
	return mul;
}

matrix HillCipher::CodesUnit_Mul_DecipherMatrix()
{
	matrix mul;
	mul = new int*;
	mul[0] = new int[maxDimensionSize];
	mul[0][0] = (codesUnit[0][0] * decipherMatrix[0][0] + codesUnit[0][1] * decipherMatrix[1][0])%26;
	mul[0][1] = (codesUnit[0][0] * decipherMatrix[0][1] + codesUnit[0][1] * decipherMatrix[1][1])%26;
	return mul;
}

void HillCipher::Inverse(matrix tempMatrix)
{
	matrix subMatrix = new int*[maxDimensionSize];
	for (int i = 0; i < maxDimensionSize; i++)
		subMatrix[i] = new int[maxDimensionSize];
	for (int i = 0; i < maxDimensionSize; i++)
		for (int j = 0; j < maxDimensionSize; j++)
			subMatrix[i][j] = tempMatrix[i][j];

	for (int i = 0; i < maxDimensionSize; i++)
		for (int j = 0; j < maxDimensionSize; j++)
			if (i != j) decipherMatrix[i][j] = 0;
			else decipherMatrix[i][j] = 1;

	//up-tangle
	for (int k = maxDimensionSize - 1; k> 0; k--) {
		for (int i = k - 1; i >= 0; i--) {
			int temp1 = tempMatrix[k][k];
			int temp2 = tempMatrix[i][k];
			for (int j = 0; j < maxDimensionSize; j++) {
				decipherMatrix[i][j] = temp1 * decipherMatrix[i][j] - temp2 * decipherMatrix[k][j];
				tempMatrix[i][j] = temp1 * tempMatrix[i][j] - temp2 * tempMatrix[k][j];
			}
		}
	}
	//down-tangle
	for (int k = 0; k < maxDimensionSize - 1; k++) {
		for (int i = k + 1; i < maxDimensionSize; i++) {
			int temp1 = tempMatrix[k][k];
			int temp2 = tempMatrix[i][k];
			for (int j = 0; j < maxDimensionSize; j++) {
				decipherMatrix[i][j] = temp1 * decipherMatrix[i][j] - temp2 * decipherMatrix[k][j];
				tempMatrix[i][j] = temp1 * tempMatrix[i][j] - temp2 * tempMatrix[k][j];
			}
		}
	}
	for (int i = 0; i < maxDimensionSize; i++) {
		int temp = 0;
		for (int j = 0; j < maxDimensionSize - 1; j++) {
			temp = Eculid(decipherMatrix[i][j], decipherMatrix[i][j + 1]);
		}
		for (int j = 0; j < maxDimensionSize; j++)
			decipherMatrix[i][j] /= abs(temp);
	}
	for (int i = 0; i < maxDimensionSize; i++)
		for (int j = 0; j < maxDimensionSize; j++)
			tempMatrix[i][j] = subMatrix[i][j];
	
	for (int i = 0; i < maxDimensionSize; i++)
		delete[] subMatrix[i];
	delete[] subMatrix;
}

void HillCipher::ExtendEculid(int x, int y, int &xi, int &yi)
{
	if (y == 0) {
		xi = 0;
		yi = 1;
		return;
	}
	ExtendEculid(y, x%y, xi, yi);
	int temp = yi;
	yi = xi;
	xi = temp - (x / y)*xi;
	return;
}
/**********************************************/


/*VernamCipher*/
VernamCipher::VernamCipher()
{
	srand((unsigned int)time(nullptr));
}

void VernamCipher::CreateCipherKey(int size)
{
	for (int i = 0; i < (int)size; i++)
		cipherKey += (rand() % 26) + 'a';
}

string VernamCipher::Encrypt(string text)
{
	string ciphertext;
	for (int i = 0; i < (int)(int)text.size(); i++)
		ciphertext += cipherKey[i%cipherKey.size()] ^ text[i];
	return ciphertext;
}

string VernamCipher::Decipher(string ciphertext)
{
	string text;
	for (int i = 0; i < (int)ciphertext.size(); i++)
		text += cipherKey[i] ^ ciphertext[i];
	return text;
}
/**********************************************/


/*PlayfairCipher*/
PlayfairCipher::PlayfairCipher(string encryptWord)
{
	encryptMatrix = new char*[5];
	for (int i = 0; i < 5; i++) 
		encryptMatrix[i] = new char[5];
	//pop the same letter in the encrypt word
	PopSameLetter(encryptWord);
	for (int i = 0; i < (int)encryptWord.size(); i++)
		encryptMatrix[i / 5][i % 5] = encryptWord[i];
	for (int i = encryptWord.size(),j = 0; i < 25; j++) {
		if ('a' + j == 'i')
			j++;
		if (encryptWord.find('a' + j) != string::npos)
			continue;
		encryptMatrix[i / 5 ][i % 5] = 'a' + j;
		i++;
	}
}

PlayfairCipher::~PlayfairCipher()
{
	for (int i = 0; i < 5; i++)
		delete[] encryptMatrix[i];
	delete[] encryptMatrix;
}

string PlayfairCipher::Encrypt(string text)
{
	string ciphertext;
	int x1, y1, x2, y2;
	if ((int)text.size() % 2) text += 'a';
	for (int i = 0; i < (int)text.size(); i += 2) {
		FindLetter(text[i], x1, y1);
		FindLetter(text[i + 1], x2, y2);
		ciphertext += Encrypt_2_Letters(x1, y1, x2, y2);
	}
	return ciphertext;
}

string PlayfairCipher::Decipher(string ciphertext)
{
	string text;	
	int x1, y1, x2, y2;
	for (int i = 0; i < (int)ciphertext.size(); i += 2) {
		FindLetter(ciphertext[i], x1, y1);
		FindLetter(ciphertext[i + 1], x2, y2);
		text += Decipher_2_Letters(x1, y1, x2, y2);
	}
	return text;

}

void PlayfairCipher::Show()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++)
			cout << encryptMatrix[i][j]<<' ';
		cout << endl;
	}
}

void PlayfairCipher::FindLetter(char letter, int &x, int &y)
{
	int i = 0;
	while (letter != (char)encryptMatrix[i / 5][i % 5]) i++;
	x = i / 5;
	y = i % 5;
}

void PlayfairCipher::PopSameLetter(string &encryptWord)
{
	for (int i = 0; i < (int)encryptWord.size(); i++) {
		int pos = encryptWord.substr(0, i).find(encryptWord[i]);
		if (pos != string::npos) {
			int j = pos;
			while (j++ != (int)encryptWord.size())
				encryptWord[j - 1] = encryptWord[j];
			encryptWord.pop_back();
			i--;
		}
		if (encryptWord[i] == 'i') encryptWord[i] = 'j';
	}
}

string PlayfairCipher::Encrypt_2_Letters(int x1, int y1, int x2, int y2)
{
	string cipher_2_Letters;
	if (x1 == x2){
		cipher_2_Letters += encryptMatrix[x1][(y1 + 1) % 5];
		cipher_2_Letters += encryptMatrix[x2][(y2 + 1) % 5];

	}
	else if (y1 == y2){
		cipher_2_Letters += encryptMatrix[(x1 + 1) % 5][y1];
		cipher_2_Letters += encryptMatrix[(x2 + 1) % 5][y2];
	}
	else{
		cipher_2_Letters += encryptMatrix[x1][y2];
		cipher_2_Letters += encryptMatrix[x2][y1];
	}
	return cipher_2_Letters;
}

string PlayfairCipher::Decipher_2_Letters(int x1, int y1, int x2, int y2)
{
	string plainword_2_Letters;
	if (x1 == x2) {
		plainword_2_Letters += encryptMatrix[x1][(y1 - 1 + 5) % 5];
		plainword_2_Letters += encryptMatrix[x2][(y2 - 1 + 5) % 5];

	}
	else if (y1 == y2) {
		plainword_2_Letters += encryptMatrix[(x1 - 1 + 5) % 5][y1];
		plainword_2_Letters += encryptMatrix[(x2 - 1 + 5) % 5][y2];
	}
	else {
		plainword_2_Letters += encryptMatrix[x1][y2];
		plainword_2_Letters += encryptMatrix[x2][y1];
	}
	return plainword_2_Letters;
}
/**********************************************/


/*VigenereCipher*/
VigenereCipher::VigenereCipher(string encryptWord)
{
	encryptKey = encryptWord;
}

string VigenereCipher::Encrypt(string text)
{
	string ciphertext;
	for (int i = 0; i < (int)text.size(); i++)
		ciphertext += NumToAlphabet((AlphabetToNum(text[i]) +
			AlphabetToNum(encryptKey[i % encryptKey.size()])) % 26);
	return string(ciphertext);
}

string VigenereCipher::Decipher(string ciphertext)
{
	string text;
	for (int i = 0; i < (int)ciphertext.size(); i++)
		text += NumToAlphabet((AlphabetToNum(ciphertext[i]) -
			AlphabetToNum(encryptKey[i % encryptKey.size()]) + 26) % 26);
	return string(text);
}
/**********************************************/