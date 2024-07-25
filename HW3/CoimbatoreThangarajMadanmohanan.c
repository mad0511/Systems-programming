#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Function to transform a character for encryption or decryption
char transformCharacter(char ch, int offset, int isEncrypt) {
    if (isalpha(ch)) {
        char base = islower(ch) ? 'a' : 'A';
        if (isEncrypt) {
            ch = ((ch - base + offset) % 26) + base;  // Encryption
        } else {
            ch = ((ch - base - offset + 26) % 26) + base;  // Decryption
        }
    }
    return ch;
}

// Function to perform encryption or decryption
void cryptFile(const char *inputFilename, const char *outputFilename, int offset, int isEncrypt) {
    int offset2;
    
    if (isEncrypt) {
        offset2 = rand() % 26;  // Generate a random offset between 0 and 25
    }

    FILE *inputFile = fopen(inputFilename, "r");
    FILE *outputFile = fopen(outputFilename, "w");

    // Write random offset2 as a character code to the output file (for encryption)
    if (isEncrypt) {
        fprintf(outputFile, "%c\n", 'a' + offset2);
    }

    char ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        char transformedCh = transformCharacter(ch, offset, isEncrypt);
        fputc(transformedCh, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Offset 2 is %d\n", offset2);
    printf("File %s successfully.\n", isEncrypt ? "encrypted" : "decrypted");
}


int main() {
    char choice[10];
    char filename[100];
    char outputFilename[100];
    int offset;

    do {
        printf("Do you want to encrypt or decrypt a file? (encrypt/decrypt): ");
        scanf("%s", choice);
        printf("Enter the input file name: ");
        scanf("%s", filename);
        printf("Enter the offset: ");
        scanf("%d", &offset);
        printf("Enter the output file name: ");
        scanf("%s", outputFilename);

        if (strcmp(choice, "encrypt") == 0) {
            cryptFile(filename, outputFilename, offset, 1); // Encryption
        } else if (strcmp(choice, "decrypt") == 0) {
            cryptFile(filename, outputFilename, offset, 0); // Decryption
        } else {
            printf("Invalid choice. Please enter 'encrypt' or 'decrypt'.\n");
        }

        printf("Do you wish to continue? (yes/no): ");
        scanf("%s", choice);

    } while (strcmp(choice, "no") != 0 && strcmp(choice, "No") != 0 && strcmp(choice, "n") != 0 && strcmp(choice, "N") != 0);

    return 0;
}
