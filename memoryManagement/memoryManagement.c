
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
// Memory Management
// Developer: Isaiah DeBenedetto

int num;

int main(int argc, char const *argv[])
{
    // get number from user
    printf("\nPlease enter a number in between 0 and 4095: ");
    scanf("%d", &num);
    
    // check number to make sure it is valid and if it isn't then prompt the user again
    while (num < 0 && num > 4095) {
        printf("\nThe number entered is out of bounds.\nPlease enter another number between 0 - 4095: ");
        scanf("%d", &num);
    }
    
    // convert the number to binary formatting
    printf("\nNumber in Binary:");
    convertBinary(num);
    printf("\n");
    
    // convert the number to hexadecimal formatting
    printf("Number in Hexadecimal: %032x\n", num);
    
    // covert the number into the final result by adding an x to properly format
    int result = ((num << 16) & 0x0FFFF0000) | 0x07FF;
    
    // results
    printf("Result in Binary: ");
    convertBinary(result);
    printf("\n");
    printf("Result in Hexadecimal: %032x\n", result);
    printf("Result in Decimal: %d\n", result);
  return 0;
}

// method to convert the numner to binary 32 bit format
int convertBinary(int num){
    // loop through 32 to times to properly conver to 32 bit binary
    for (int i = 31; i >= 0; i--){
        // shift the number
        int numShifted = num >> i;
        // if the number that is shifted is equal to 1 then print 1, if any other number than print 0
        if (numShifted & 1){
            printf("1");
        } else {
            printf("0");
        }
    }
}
