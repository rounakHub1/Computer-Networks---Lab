/*
Q.5 > Input a string of 0 and 1. In that string-
        a. Count number of 1's present in every alternate position starting from 1st postion.
            (e.g. positions 1,3,5,7,9 and so on)
        b. Count number of 1's present in every two alternate position starting from 2nd postion.
            (e.g. positions 2,3,6,7,10,11,14,15 and so on)
        c. Count number of 1's present in every four alternate position starting from 4th postion.
            (e.g. positions 4,5,6,7,12,13,14,15,20,21,22,23 and so on)

*/

#include <stdio.h>

int count_Alternate(char binary [], int length) {
    int num=0;
    for(int i=0; i<length; i+=2) {
        if(binary [i] == '1') {
            num++;
        }
    }
    return num;
}
int count_two_Alternate(char binary [], int length) {
    int num=0;
    for(int i=1; i<length; i+=4) {
        for(int j=i; (j<(i+2)) && (j<length); j++) {
            if(binary [j] == '1') {
                num++;
            }
        }
    }
    return num;
}
int count_four_Alternate(char binary [], int length) {
    int num=0;
    for(int i=3; i<length; i+=8) {
        for(int j=i; (j<(i+4)) && (j<length); j++) {
            if(binary [j] == '1') {
                num++;
            }
        }
    }
    return num;
}
int main() {
        char binary [100];
        printf("Enter the binary number : ");
        int i=0;
        while(i<100) {
            scanf("%c", &binary [i]);
            if(binary [i]=='\n')
                break;
            i++;
        }
        printf("No. of 1's present in every alternate position starting from 1st position is %d\n", count_Alternate(binary, i));
        printf("No. of 1's present in every two alternate position starting from 2nd position is %d\n", count_two_Alternate(binary, i));
        printf("No. of 1's present in every four alternate position starting from 4th position is %d\n", count_four_Alternate(binary, i));
        return 0;
}
