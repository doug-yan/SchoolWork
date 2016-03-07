#include <iostream>
#include <string>

using namespace std;

int main(){
  int input, key;
  string plaintext, ciphertext;
  
  cout << "Enter 1 for encryption, 2 for decription, 0 for quit: ";
  cin >> input;
  
  while(input!=0){
    if(input==1) {
      cout << "Enter plaintext: ";
      cin >> noskipws >> plaintext;
      cout << plaintext << endl;
    }
      
    cout << "Enter 1 for encryption, 2 for decription, 0 for quit: ";
    cin >> input;
  }
  return 0;
}