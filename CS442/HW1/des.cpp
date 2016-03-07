#include <iostream>
#include <bitset>
#include <string>

using namespace std;

void initPermutation(int input[], int output[]);
void getSubkeys(int originalKey[], int subkeys[][48]);
void permutationChoice1(int input[], int output[]);
void splitPlaintext(int input[], int left[], int right[]);
void finalPermutation(int input[], int output[]);

int main(){
  int plaintextBinary[64];
  int initPlainTextPermutation[64];
  int keyBinary[64] = {0,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1};
  int keyPlus[56];
  int subKeys[16][48];
  
  //Perform initial Permuation
  initPermutation(plaintextBinary, initPlainTextPermutation);
  
  //Get subkeys
  getSubkeys(keyBinary, subKeys);
  
  //Encode block
  
  //Perform final permutation
  
  return 0;
}

//Algorithm courtesy of programmersheaven.com
void initPermutation(int input[], int output[]) {
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

void getSubkeys(int originalKey[], int subkeys[][48]) {
  int kplus[56];
  int c0[28];
  int d0[28];
  int leftsubkeys[16][28];
  int rightsubkeys[16][28];
  
  permutationChoice1(originalKey, kplus);
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
  for(int i=28; i<52; i++){
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
  for(int i = 0; i < 32; i++)
    left[i] = input[i];
    
  for(int i = 32; i < 64; i++)
    right[i-32] = input[i];
  
}
