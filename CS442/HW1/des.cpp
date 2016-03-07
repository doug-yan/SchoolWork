#include <iostream>
#include <bitset>
#include <string>

using namespace std;

void initPermutation(int input[], int output[]);
void permutationChoice1(int input[], int output[]);
void splitPlaintext(int input[], int left[], int right[]);
void finalPermutation(int input[], int output[]);

int main(){
  int plaintextBinary[64];
  int keyBinary[64];
  
  //Perform initial Permuation
  
  //Get subkeys
    //Perform permutation choice 1
    
    //Split into C0 and D0
    
    //Perform shifts
    
    //Concatenate and perform permutation choice 2
  
  //Encode block
  
  return 0;
}

//Algorithm courtesy of programmersheaven.com
void initPermutation(int input, int output[]) {
  int k=58;
  for(int i=0; i<32; i++) {
    output[i]=input[k-1];
    if(k-8>0)
      k-=8;
    else
      k+=58;
  }
  
  k=57;
  for(int i=32; i<64; i++) {
    output[i]=input[k-1];
    if(k-8>0)
      k-=8;
    else
      k+=58;
  }
  
}

void permutationChoice1(int input[], int output[]){
  int k=57;
  for(int i=0; i<28; i++){
    output[i]=input[k-1];
    if(k-8>0)
      k-=8;
    else
      k+=57;
  }
  
  k=63;
  for(int i=28; i<56; i++){
    output[i]=input[k-1];
    if(k-8>0)
      k-=8;
    else
      k+=55;
  }
  
  k=28;
  for(int i=52; i<56; i++){
    output[i]=input[k-1];
    k-=8;
  }
}

void splitPlaintext(int input[], int left[], int right[]){
  for(int i = 0; i << 32; i++)
    left[i] = input[i];
    
  for(int i = 32; i < 64; i++)
    right[i-32] = input[i];
  
}
