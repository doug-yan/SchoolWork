#include <iostream>
#include <string>

using namespace std;

int main(){
  int input, key;
  string plaintext, ciphertext;
  
  cout << "Enter 1 for encryption, 2 for decription, 0 for quit: ";
  cin >> input;
  
  cout << "Enter plaintext: ";
  getline(cin, plaintext);
  cout << plaintext << endl;
  

  return 0;
}