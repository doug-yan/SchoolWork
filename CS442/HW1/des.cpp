#include <iostream>
#include <bitset>
#include <string>

using namespace std;

void initPermutation(int input[], int output[]);
void getSubkeys(int originalKey[], int subkeys[][48]);
void permutationChoice1(int input[], int output[]);
void splitKPlus(int key[], int left[], int right[]);
void leftShift(int array[]);
void shiftSubKeyParts(int left[], int right[], int leftResults[][28], int rightResults[][28]);
void permute2Table(int input[], int output[]);
void permutationChoice2(int left[][28], int right[][28], int subkeys[][48]);
void encodeBlock(int message[], int keys[][48], int ciphertext[]);
void splitPlaintext(int input[], int left[], int right[]);
void expand(int original[], int expanded[]);
void exclusiveor(int message[], int key[]);
void substitute(int input[]);
void finalPermutation(int input[], int output[]);


int main(){
  int plaintextBinary[64] = {0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1,};
  int initPlainTextPermutation[64];
  int keyBinary[64] = {0,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1};
  int subKeys[16][48];
  int encodedBlock[64];
  
  //Perform initial Permuation
  initPermutation(plaintextBinary, initPlainTextPermutation);
  
  //Get subkeys
  getSubkeys(keyBinary, subKeys);

  //Encode block
  encodeBlock(initPlainTextPermutation, subKeys, encodedBlock);
  
  //Perform final permutation
  
  return 0;
}

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
  splitKPlus(kplus, c0, d0);
  shiftSubKeyParts(c0, d0, leftsubkeys, rightsubkeys);
  permutationChoice2(leftsubkeys, rightsubkeys, subkeys);
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

void splitKPlus(int key[], int left[], int right[]) {
  for(int i=0; i<28; i++)
    left[i] = key[i];
  
  for(int i=28; i<56; i++)
    right[i-28] = key[i];
}

void leftShift(int array[]) {
  int temp=array[0];
  for(int i=1; i<28; i++) {
    array[i-1]=array[i];
  }
  array[27] = temp;
}

void shiftSubKeyParts(int left[], int right[], int leftResults[][28], int rightResults[][28]){
  for(int i=0; i<2; i++) {
    leftShift(left);
    leftShift(right);
    for(int k=0; k<28; k++) {
      leftResults[i][k] = left[k];
      rightResults[i][k] = right[k];
    }
  }
  
  for(int i=2; i<8; i++) {
    leftShift(left);
    leftShift(left);
    leftShift(right);
    leftShift(right);
    for(int k=0; k<28; k++) {
      leftResults[i][k] = left[k];
      rightResults[i][k] = right[k];
    }
  }
  
  leftShift(left);
  leftShift(right);
  for(int k=0; k<28; k++) {
    leftResults[8][k] = left[k];
    rightResults[8][k] = right[k];
  }
  
  for(int i=9; i<15; i++) {
    leftShift(left);
    leftShift(left);
    leftShift(right);
    leftShift(right);
    
    for(int k=0; k<28; k++) {
      leftResults[i][k] = left[k];
      rightResults[i][k] = right[k];
    }
  }
  
  leftShift(left);
  leftShift(right);
  for(int k=0; k<28; k++) {
    leftResults[15][k] = left[k];
    rightResults[15][k] = right[k];
  }
}

void permutationChoice2(int left[][28], int right[][28], int subkeys[][48]){
    int tempsubkey[16][56];
    int fullPermute[16][56];
    
    //combine cn and dn
    for(int i=0; i<16; i++) {
      for(int k=0; k<28; k++) {
        tempsubkey[i][k] = left[i][k];
        tempsubkey[i][k+28] = right[i][k];
      }
    }
    
    //use permutation table
    for(int i=0; i<16; i++)
      permute2Table(tempsubkey[i], fullPermute[i]);
    
    //get first 48 bits for key
    for(int i=0; i<16; i++) {
      for(int k=0; k<48; k++) {
        subkeys[i][k] = fullPermute[i][k];
      }
    }
}

void permute2Table(int input[], int permute[]) {
    permute[0]=input[13];
    permute[1]=input[16];
    permute[2]=input[10];
    permute[3]=input[23];
    permute[4]=input[0];
    permute[5]=input[4];
    permute[6]=input[2];
    permute[7]=input[27];
    permute[8]=input[14];
    permute[9]=input[5];
    permute[10]=input[20];
    permute[11]=input[9];
    permute[12]=input[22];
    permute[13]=input[18];
    permute[14]=input[11];
    permute[15]=input[3];
    permute[16]=input[25];
    permute[17]=input[7];
    permute[18]=input[15];
    permute[19]=input[6];
    permute[20]=input[26];
    permute[21]=input[19];
    permute[22]=input[12];
    permute[23]=input[1];
    permute[24]=input[40];
    permute[25]=input[51];
    permute[26]=input[30];
    permute[27]=input[36];
    permute[28]=input[46];
    permute[29]=input[54];
    permute[30]=input[29];
    permute[31]=input[39];
    permute[32]=input[50];
    permute[33]=input[46];
    permute[34]=input[32];
    permute[35]=input[47];
    permute[36]=input[43];
    permute[37]=input[48];
    permute[38]=input[38];
    permute[39]=input[55];
    permute[40]=input[33];
    permute[41]=input[52];
    permute[42]=input[45];
    permute[43]=input[41];
    permute[44]=input[49];
    permute[45]=input[35];
    permute[46]=input[28];
    permute[47]=input[31];
}

void encodeBlock(int message[], int keys[][48], int ciphertext[]) {
  int left[32], right[32], templeft[32], expandright[48], finalright[32];
  splitPlainText(message, left, right);
  
  for(int i=0; i<16; i++) {
    //get left
    for(int k=0; k<32; k++) {
      templeft[k] = left[k];
      left[k] = right[k];
    }
    
    //get right
    expand(right, expandright);
    exclusiveor(expandright, keys[i]);
    substitute(expandright);
  }
}

void expand(int original[], int expanded[]) {
    expanded[0]=original[31];
    expanded[1]=original[0];
    expanded[2]=original[1];
    expanded[3]=original[2];
    expanded[4]=original[3];
    expanded[5]=original[4];
    expanded[6]=original[3];
    expanded[7]=original[4];
    expanded[8]=original[5];
    expanded[9]=original[6];
    expanded[10]=original[7];
    expanded[11]=original[8];
    expanded[12]=original[7];
    expanded[13]=original[8];
    expanded[14]=original[9];
    expanded[15]=original[10];
    expanded[16]=original[11];
    expanded[17]=original[12];
    expanded[18]=original[11];
    expanded[19]=original[12];
    expanded[20]=original[13];
    expanded[21]=original[14];
    expanded[22]=original[15];
    expanded[23]=original[16];
    expanded[24]=original[15];
    expanded[25]=original[16];
    expanded[26]=original[17];
    expanded[27]=original[18];
    expanded[28]=original[19];
    expanded[29]=original[20];
    expanded[30]=original[19];
    expanded[31]=original[20];
    expanded[32]=original[21];
    expanded[33]=original[22];
    expanded[34]=original[23];
    expanded[35]=original[24];
    expanded[36]=original[23];
    expanded[37]=original[24];
    expanded[38]=original[25];
    expanded[39]=original[26];
    expanded[40]=original[27];
    expanded[41]=original[28];
    expanded[42]=original[27];
    expanded[43]=original[28];
    expanded[44]=original[29];
    expanded[45]=original[30];
    expanded[46]=original[31];
    expanded[47]=original[0];
}

void exclusiveor(int message[], int key[]) {
  for(int i=0; i<48; i++){
    if(message[i] != key[i]) {
      message[i] = 1
    }
    
    else
      message[i] = 0;
  }
}

void substitute(int input[]) {
  
}

void splitPlaintext(int input[], int left[], int right[]){
  for(int i = 0; i < 32; i++)
    left[i] = input[i];
    
  for(int i = 32; i < 64; i++)
    right[i-32] = input[i];
  
}
