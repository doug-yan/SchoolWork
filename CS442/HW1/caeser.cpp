#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;

char alphabet[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
					 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
					  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
					  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


void encrypt(string plaintext, int key);
void decrypt(string ciphertext);
int getsize(string str);
int getAlphabetPlace(char input);
int getNewPlaceEncrypt(int place, int key);
int getNewPlaceDecrypt(int place, int key);

int main(){
  int input, key;
  string plaintext, ciphertext;
  
  cout << "Enter 1 for encryption, 2 for decryption, 0 for quit: ";
  cin >> input;
  
  while(input != 0){
	  if(input == 1)
	  	encrypt(plaintext, key);

	  else if(input == 2)
	  	decrypt(ciphertext);

	  cout << "Enter 1 for encryption, 2 for decryption, 0 for quit: ";
  	  cin >> input;
	}

  return 0;
}

void encrypt(string plaintext, int key) {
	int size, place, newplace;

	cout << "Enter plaintext: ";
	ws(cin);
	getline(cin, plaintext);
	cout << "Enter key: ";
	cin >> key;
	key = key % 26;

	size = getsize(plaintext);

	for(int i=0; i<size; i++) {
		if(plaintext[i] != ' ' && isalpha(plaintext[i]) ){
			place = getAlphabetPlace(plaintext[i]);
			newplace = getNewPlaceEncrypt(place, key);
			plaintext[i] = alphabet[newplace];
		}
	}
	cout << "Here is the ciphertext: " << plaintext << endl;
}

void decrypt(string ciphertext){
	int size, place, newplace;
	cout << "Enter cyphertext: ";
	ws(cin);
	getline(cin, ciphertext);
	
	size = getsize(ciphertext);
	string deciphered = ciphertext;
	cout << "deciphered: " << deciphered << endl;

	for(int i=0; i<52; i++){
		cout << "Testing key size of multiple of " << i << endl;
		cout << "Decryption: ";
		for(int k=0; k<size; k++){
			place = getAlphabetPlace(ciphertext[k]);
			newplace = getNewPlaceDecrypt(place, i);
			deciphered[k] = alphabet[newplace];
		}
		cout << deciphered << endl;
	}
}

int getsize(string str) {
	int size = 0;
	while(str[size] != '\0')
		size++;
	
	return size;
}

int getAlphabetPlace(char input){
	for(int i = 0; i<52; i++) {
		if(input == alphabet[i])
			return i;
	}
	return 0;
}

int getNewPlaceEncrypt(int place, int key) {
	if(place-key < 0)
		return place-key+52;
	return place-key;
}

int getNewPlaceDecrypt(int place, int key) {
	if(place+key > 52)
		return place+key-52;
	return place+key;
}