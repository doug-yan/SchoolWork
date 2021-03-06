#include <iostream>
#include <string>

using namespace std;

int NUMZERO = 0, NUMONE = 0;

void putInArray(string message, string key, int messageArr[], int keyArr[]);
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
void exclusiveorkey(int message[], int key[]);
void exclusiveorleft(int message[], int key[]);
void fpermute(int input[], int output[]);
void substitute(int input[], int result[]);
int getRow(int array[]);
int getCol(int array[]);
void addToResult(int array[], int decimalNum, int counter);
void finalPermutation(int input[], int output[]);
void count(int input[]);
void seedVector(int oldVector[], int newVector[]);

int main(){
  string messagebin, keybin, vectorbin;
  int CYCLES = 16384; //2^20 bits divided by64 bits per round
  int plaintextBinary[64];
  int initVector[64];
  int initPlainTextPermutation[64];
  int keyBinary[64];
  int subKeys[16][48];
  int encodedBlock[64];
  int finalcipher[64];
  
  //Get data:
  cout << "Enter initialization vector in binary: ";
  ws(cin);
  getline(cin, messagebin);
  
  cout << "Enter key in binary: ";
  ws(cin);
  getline(cin, keybin);
  
  putInArray(messagebin, keybin, plaintextBinary, keyBinary);
  
  for(int i = 0; i < CYCLES; i++) {
    //Encrypt with the vector and the key
    //Perform initial Permuation
    initPermutation(plaintextBinary, initPlainTextPermutation);
    
    //Get subkeys
    getSubkeys(keyBinary, subKeys);
  
    //Encode block
    encodeBlock(initPlainTextPermutation, subKeys, encodedBlock);
    
    //Perform final permutation
    finalPermutation(encodedBlock, finalcipher);
    
    //count the number of 0s and 1s
    count(finalcipher);
    
    //Change the vector to the output of the previous output
    seedVector(finalcipher, plaintextBinary);
  }
  
  cout << "Number of 0's: " << NUMZERO << endl;
  cout << "Number of 1's: " << NUMONE << endl;
  
  return 0;
}

//Put input into array
void putInArray(string message, string key, int messageArr[], int keyArr[]) {
  for(int i=0; i<64; i++) {
    if(message[i] != ' ') {
      messageArr[i] = int(message[i])-48;
    }
    if(key[i] != ' ') {
      keyArr[i] = int(message[i])-48;
    }
  }
}

//Perform initial permutation according to table
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
  
  //Perform permutation choice 1 according to PC1 table
  permutationChoice1(originalKey, kplus);
  
  //Get c0 and d0
  splitKPlus(kplus, c0, d0);
  
  //Perform left shifts
  shiftSubKeyParts(c0, d0, leftsubkeys, rightsubkeys);
  
  //Perform permutation choice 2 according to PC2 table
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

//Must create own becuase not actually using bits so can't
//use bitwise operations
void leftShift(int array[]) {
  int temp=array[0];
  for(int i=1; i<28; i++) {
    array[i-1]=array[i];
  }
  array[27] = temp;
}

//Perform left shifts
void shiftSubKeyParts(int left[], int right[], int leftResults[][28], int rightResults[][28]){
  //First two rounds get one shift apiece
  for(int i=0; i<2; i++) {
    leftShift(left);
    leftShift(right);
    for(int k=0; k<28; k++) {
      leftResults[i][k] = left[k];
      rightResults[i][k] = right[k];
    }
  }
  
  //Third round to eighth round gets two shifts
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
  
  //Ninth round gets one shift
  leftShift(left);
  leftShift(right);
  for(int k=0; k<28; k++) {
    leftResults[8][k] = left[k];
    rightResults[8][k] = right[k];
  }
  
  //Tenth round to fifteenth round gets two shifts
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
  
  //Final, sixteenth round gets one shift
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
  int previousLeft[32], previousRight[32], currentLeft[32], currentRight[32], expandRight[48];
  splitPlaintext(message, previousLeft, previousRight);
  
  for(int i=0; i<16; i++) {
    //left
    for(int k=0; k<32; k++) {
      previousLeft[i] = currentLeft[i];
      currentLeft[k] = previousRight[k];
    }
    
    //right
    //expand previousRight (32 bits) to expandRight (48 bits)
    expand(previousRight, expandRight);
    
    //Xor expandright with current key
    exclusiveorkey(expandRight, keys[i]);
    
    //Use substitution tables to decrease expandRight(48 bits) to (32 bits)
    substitute(expandRight, previousRight);
    
    //Use permute table for f function
    fpermute(previousRight, currentRight);
    
    //Xor with the previous left 
    exclusiveorleft(currentRight, previousLeft);
    
    //Prime previousRight to be equal to the currentRight
    for(int k=0; k<32; k++) {
      previousRight[k] = currentRight[k];
    }
  }
  
  //Copy ciphertext in R16L16 order
  for(int i=0; i<32; i++){
    ciphertext[i] = currentRight[i];
    ciphertext[i+32] = currentLeft[i];
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

//Perform XOR with key. Need to write funciton because
//not using bitwise operations. 
void exclusiveorkey(int message[], int key[]) {
  for(int i=0; i<48; i++){
    if(message[i] != key[i]) {
      message[i] = 1;
    }
    
    else
      message[i] = 0;
  }
}

void exclusiveorleft(int message[], int key[]){
    for(int i=0; i<32; i++){
    if(message[i] != key[i]) {
      message[i] = 1;
    }
    
    else
      message[i] = 0;
  }
}

void fpermute(int input[], int output[]) {
    output[0]=input[15];
    output[1]=input[6];
    output[2]=input[19];
    output[3]=input[20];
    output[4]=input[28];
    output[5]=input[11];
    output[6]=input[27];
    output[7]=input[16];
    output[8]=input[0];
    output[9]=input[14];
    output[10]=input[22];
    output[11]=input[25];
    output[12]=input[4];
    output[13]=input[17];
    output[14]=input[30];
    output[15]=input[9];
    output[16]=input[1];
    output[17]=input[7];
    output[18]=input[23];
    output[19]=input[13];
    output[20]=input[31];
    output[21]=input[26];
    output[22]=input[2];
    output[23]=input[8];
    output[24]=input[18];
    output[25]=input[12];
    output[26]=input[29];
    output[27]=input[5];
    output[28]=input[21];
    output[29]=input[10];
    output[30]=input[3];
    output[31]=input[24];


}

void substitute(int input[], int result[]) {
  int sub1[6], sub2[6], sub3[6], sub4[6], sub5[6], sub6[6], sub7[6], sub8[6];
  int row, col, number;
  
  //Substitutuion tables
  int s1[4][16]=
  {
       14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
       0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
       4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
       15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
  };
 
  int s2[4][16]=
  {
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
  };
 
  int s3[4][16]=
  {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
  };
 
  int s4[4][16]=
  {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
  };
 
  int s5[4][16]=
  {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
  };
 
  int s6[4][16]=
  {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
  };
 
  int s7[4][16]=
  {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
  };
 
  int s8[4][16]=
  {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
  };
  
  for(int i=0; i<6; i++) {
    sub1[i] = input[i];
  }
  
  for(int i=6; i<12; i++) {
    sub2[i-6] = input[i];
  }
 
   for(int i=12; i<18; i++) {
      sub3[i-12] = input[i];
    }
   
   for(int i=18; i<24; i++) {
      sub4[i-18] = input[i];
    }
   
   for(int i=24; i<30; i++) {
      sub5[i-24] = input[i];
    }
   
   for(int i=30; i<36; i++) {
      sub6[i-30] = input[i];
    }
   
   for(int i=36; i<42; i++) {
      sub7[i-36] = input[i];
    }
   
   for(int i=42; i<48; i++) {
      sub8[i-42] = input[i];
    }
   
   row = getRow(sub1);
   col = getCol(sub1);
   number = s1[row][col];
   addToResult(result, number, 0);
   
   row = getRow(sub2);
   col = getCol(sub2);
   number = s2[row][col];
   addToResult(result, number, 4);
   
   row = getRow(sub3);
   col = getCol(sub3);
   number = s3[row][col];
   addToResult(result, number, 8);
   
   row = getRow(sub4);
   col = getCol(sub4);
   number = s4[row][col];
   addToResult(result, number, 12);
   
   row = getRow(sub5);
   col = getCol(sub5);
   number = s5[row][col];
   addToResult(result, number, 16);
   
   row = getRow(sub6);
   col = getCol(sub6);
   number = s6[row][col];
   addToResult(result, number, 20);
   
   row = getRow(sub7);
   col = getCol(sub7);
   number = s7[row][col];
   addToResult(result, number, 24);
   
   row = getRow(sub8);
   col = getCol(sub8);
   number = s8[row][col];
   addToResult(result, number, 28);
}

int getRow(int array[]) {
  int result = 0;
  if(array[0] == 1)
    result+=2;
  if(array[5] == 1)
    result++;
  return result;
}

int getCol(int array[]) {
  int result = 0;
  if(array[1] == 1)
    result+=8;
  if(array[2] == 1)
    result+=4;
  if(array[3] == 1)
    result+=2;
  if(array[4] == 1)
    result++;
  return result;
}

void addToResult(int array[], int decimalNum, int counter) {
  int a,b,c,d;
  d = decimalNum%2;
  decimalNum/=2;
  c = decimalNum%2;
  decimalNum/=2;
  b = decimalNum%2;
  decimalNum/=2;
  a = decimalNum%2;
  decimalNum/=2;
  
  array[counter] = a;
  array[counter+1] = b;
  array[counter+2] = c;
  array[counter+3] = d;
}

void splitPlaintext(int input[], int left[], int right[]){
  for(int i = 0; i < 32; i++)
    left[i] = input[i];
    
  for(int i = 32; i < 64; i++)
    right[i-32] = input[i];
  
}

void finalPermutation(int input[], int output[]){
  output[0]=input[39];
  output[1]=input[7];
  output[2]=input[47];
  output[3]=input[15];
  output[4]=input[55];
  output[5]=input[23];
  output[6]=input[63];
  output[7]=input[31];
  output[8]=input[38];
  output[9]=input[6];
  output[10]=input[46];
  output[11]=input[14];
  output[12]=input[54];
  output[13]=input[22];
  output[14]=input[62];
  output[15]=input[30];
  output[16]=input[37];
  output[17]=input[5];
  output[18]=input[45];
  output[19]=input[13];
  output[20]=input[53];
  output[21]=input[21];
  output[22]=input[62];
  output[23]=input[29];
  output[24]=input[36];
  output[25]=input[4];
  output[26]=input[44];
  output[27]=input[12];
  output[28]=input[52];
  output[29]=input[20];
  output[30]=input[60];
  output[31]=input[28];
  output[32]=input[35];
  output[33]=input[3];
  output[34]=input[43];
  output[35]=input[11];
  output[36]=input[51];
  output[37]=input[19];
  output[38]=input[59];
  output[39]=input[27];
  output[40]=input[34];
  output[41]=input[2];
  output[42]=input[42];
  output[43]=input[10];
  output[44]=input[50];
  output[45]=input[18];
  output[46]=input[58];
  output[47]=input[26];
  output[48]=input[33];
  output[49]=input[1];
  output[50]=input[41];
  output[51]=input[9];
  output[52]=input[49];
  output[53]=input[17];
  output[54]=input[57];
  output[55]=input[25];
  output[56]=input[32];
  output[57]=input[0];
  output[58]=input[40];
  output[59]=input[8];
  output[60]=input[48];
  output[61]=input[16];
  output[62]=input[56];
  output[63]=input[24];
}

void count(int input[]) {
  for(int i = 0; i < 64; i++) {
    if(input[i] == 0)
      NUMZERO++;
    else
      NUMONE++;
  }
}

void seedVector(int oldVector[], int newVector[]){
  for(int i = 0; i < 64; i++){
    newVector[i] = oldVector[i];
  }
}