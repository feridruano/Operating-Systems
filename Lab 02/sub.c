/**
 * Name: Ferid Ruano
 * Lab/task: Lab 02
 * Date: 2/7/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help() {
    puts("USAGE:\n"
         "   sub [ -h | --fromChars -+toChars [-i inputFile] [-o outputFile] ]\n"
         "\n"
         "DESCRIPTION:\n"
         "   This utility copies text from an input stream to an output stream replacing every \n"
         "   instance of a specific character in fromChars with a corresponding (position-wise) \n"
         "   character from toChars. Any characters in fromChars and in toChars that do not \n"
         "   have corresponding counterparts in the other one are ignored. If an input file is \n"
         "   provided, the content is read from that file; otherwise, the standard input is used. \n"
         "   If an output file is provided, then the modified content is written to that file; \n"
         "   otherwise, the standard output is used.\n"
         "\n"
         "OPTIONS:\n"
         "   --(followed by a string without separating space)\n"
         "     indicates all characters that will be replaced in the processed text\n"
         "   -+(followed by a string without separating space) \n"
         "     indicates the characters that will be used to replace corresponding \n"
         "     (position-wise) characters from fromChars in the processed text\n"
         "   -i (followed by input file name) \n"
         "     use the provided file as an input stream instead of standard input \n"
         "   -o (followed by output file name) \n"
         "     use the provided file as an output stream instead of standard output \n"
         "   -h prints this help message");
}

void error(int type) {
    fclose(stdin);
    fclose(stdout);
    printf("hello");
    switch (type) {
        case 0:
            puts("ERROR: Missing Arguments.");
            break;
        case 1:
            puts("ERROR: Missing replacement character.");
            break;
        case 2:
            puts("ERROR: Extraneous replacement characters");
            return;
    }
    exit(2);
}

int main(int argc, char *args[]) {
    int fromCharIndex = -1;
    int toCharIndex = -1;

    // None or help command
    if (argc == 1 || (argc == 2 && (strcmp(args[1], "-h") == 0)))
        help();
    else {
        // Check commands
        for (int index = 1; index < argc; ++index) {

            // Record position or enable command
            switch (args[index][1]) {
                case '-':
                    fromCharIndex = index;
                    break;
                case '+':
                    toCharIndex = index;
                    break;
                case 'i':
                    freopen(args[index + 1], "r", stdin);
                    break;
                case 'o':
                    freopen(args[index + 1], "w+", stdout);
                    break;
            }
        }

        // Missing replacement arguments check
        if (fromCharIndex == -1 || toCharIndex == -1)
            error(0);

        // Lengths of replacement characters
        unsigned int numOfFromChars = strlen(args[fromCharIndex]);
        unsigned int numOfToChars = strlen(args[toCharIndex]);

        // Equal number of replacement character checks
        if (numOfFromChars > numOfToChars)
            error(1);

        if (numOfFromChars < numOfToChars)
            error(2);

        char getChar;
        int swap = 0; // A character has been swapped flag
        unsigned int length = strlen(args[fromCharIndex]);

        // Copy or replace characters
        while ((getChar = getc(stdin)) != EOF) {
            for (int index = 2; swap == 0 && index < length; ++index) {
                if (getChar == args[fromCharIndex][index]) {
                    putc(args[toCharIndex][index], stdout);
                    swap = 1;
                }
            }
            if (swap == 0)
                putc(getChar, stdout);
            swap = 0;
        }
    }
    return 0;
}