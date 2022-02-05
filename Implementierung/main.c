#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>
#include "version_definer.h"

const char* usage_msg =
        "Usage: %s file [options]   Encrypts file. Try -h to see the options\n"
        "   or: %s -h / --help   Show all options\n";

const char* help_msg =
        "Positional arguments:\n"
        "  file   The file to encrypt or decrypt\n"
        "\n"
        "Optional arguments:\n"
        "  -V X   Runs the chosen version of implementation\n"
        "       V0: Our Assembler implementation (default)\n"
        "       V1: Our C implementation\n"
        "       V2: Our C implementation with precalculated sum values\n"
        "       V3: Reference C implementation of Needham and Wheeler\n"
        "  -B X   The program will run for X times and the total runtime of the given version of implementation will be measured\n"
        "       and printed\n"
        "  -k X   A 128-bit integer key for encrypting and decrypting\n"
        "  -i X   The initialization vector (32-bit integer)\n"
        "  -o X   The output file (default: output.txt)\n"
        "  -h / --help   Show help message (this text) and exit\n";

void print_usage(const char* progname) {
    fprintf(stderr, usage_msg, progname, progname);
}

void print_help(const char* progname) {
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}


int main(int argc, char** argv) {

    const char* progname = argv[0];

    if (argc == 1) {
        print_usage(progname);
        return EXIT_FAILURE;
    }


    int opt;
    int v = 0;
    int b;
    bool bbool = false;
    bool d = false;
    //keys = aspa spas pasp aspa
    uint32_t keys[4] = {0X61737061,
                        0X73706173,
                        0X70617370,
                        0X61737061};
    uint32_t initVec = 0;
    FILE *output = NULL;

    //precalculate sums for V2
    unsigned long sums [65];
    for (int i = 0; i < 65; ++i) {
        sums[i] = i * 0x9E3779B9;
    }

    struct option long_options[] = {
            {"help",    0,              NULL, 'h'},
            {0,         0,                 0,  0 }
    };

    while((opt = getopt_long(argc, argv, "V:B:k:i:o:dhhelp", long_options, 0)) != -1) {
        switch (opt) {
            case 'V':
                v = atoi(optarg);
                break;
            case 'B':
                bbool = true;
                b = atoi(optarg);
                if(b == 0){
                    fprintf(stderr, "Argument of B is 0 or not an integer.\n");
                    print_usage(progname);
                    return EXIT_FAILURE;
                }
                break;
            case 'd':
                d = true;
                break;
            case 'k':
                printf("Can't parse key right now. Predefined key will be used.\n");
                break;
            case 'i':
                initVec = atoi(optarg);
                for(int i = 0; i < 4; i++){
                    keys[i] = (keys[i] & initVec + initVec) & 0xFFFFFFFF;
                }
                break;
            case 'o':
                output = fopen(optarg, "w");
                if(output == NULL){
                    fprintf(stderr, "Output file could not be opened.\n");
                    print_usage(progname);
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                print_help(progname);
                return EXIT_SUCCESS;
            case 'help':
                print_help(progname);
                return EXIT_SUCCESS;
            default:
                print_usage(progname);
                return EXIT_FAILURE;
        }
    }

    FILE* file = fopen(argv[optind], "r");
    if(file == NULL){
        fprintf(stderr, "Input file could not be opened or it does not exist.\n");
        print_usage(progname);
        return EXIT_FAILURE;
    }

    struct stat fileStat;
    fstat(fileno(file), &fileStat);
    uint8_t* value = malloc(sizeof(*value) * (fileStat.st_size + 1));

    if(value == NULL){
        fprintf(stderr, "No free memory.\n");
        print_usage(progname);
        return EXIT_FAILURE;
    }

    value[fileStat.st_size] = '\0';
    size_t valueLen = fread(value, 1, fileStat.st_size, file);
    fclose(file);

    if(valueLen == 0){
        fprintf(stderr, "Message length was 0.\n");
        print_usage(progname);
        return EXIT_FAILURE;
    }

    unsigned int blockCount = valueLen / 8; //size of blocks with no pad
    uint8_t padBytes = (-valueLen) & 7; //size of padding bytes

    if(padBytes != 0) {
        uint8_t* valueTemp = malloc(sizeof(*valueTemp) * (valueLen + padBytes + 1));

        if(valueTemp == NULL){
            fprintf(stderr, "No free memory.\n");
            print_usage(progname);
            return EXIT_FAILURE;
        }

        valueTemp[valueLen + padBytes] = '\0';
        strncpy(valueTemp, value, valueLen);
        free(value);

        for (int i = valueLen; i < valueLen + padBytes; ++i) {
            valueTemp[i] = padBytes; //pad the block with PKCS#7
        }

        value = malloc(sizeof(*value) * (valueLen + padBytes + 1));

        if(value == NULL){
            fprintf(stderr, "No free memory.\n");
            print_usage(progname);
            return EXIT_FAILURE;
        }

        strncpy(value, valueTemp, valueLen + padBytes + 1);
        free(valueTemp);
        blockCount++;
    }

    uint32_t values[blockCount][2]; //2d array for all 2 * 4 blocks

    for (int k = 0; k < blockCount; ++k) {
        for (int i = 0; i < 2; ++i) { //parse the chars to uint32_t
            uint8_t tempChar[4];
            strncpy(tempChar, &value[k * 8 + i * 4], 4);
            uint32_t tempInt32 [4] = {tempChar[0], tempChar[1], tempChar[2], tempChar[3]}; //assign 8-bit int to 32-bit for shifting

            values[k][i] = (tempInt32[0] << 24) | (tempInt32[1] << 16) | (tempInt32[2] << 8) | (tempInt32[3]);
        }
    }

    free(value);

    if(output == NULL){
        output = fopen("output.txt", "w");
    }

    uint8_t* outputStr = malloc(sizeof(*outputStr) * (valueLen + padBytes + 1));

    if(outputStr == NULL){
        fprintf(stderr, "No free memory.\n");
        print_usage(progname);
        return EXIT_FAILURE;
    }

    outputStr[valueLen + padBytes] = '\0';
    uint8_t* charCounter = outputStr; //second pointer for concatting to outputStr but also not lose the array begin

    switch(bbool){
        case true:
        {clock_t t = clock();
            for (int i = 0; i < b; ++i) {
                charCounter = outputStr; //when run multiple times the string will be concatted to the end multiple times
                                        //by assigning ptr of outputStr to charCounter this won't happen
                for (int j = 0; j < blockCount; ++j) {
                    define_version(v, d, sums, values[j], keys);

                    for (int k = 0; k < 2; ++k) {
                        uint8_t tempChar[5];
                        tempChar[4] = '\n';
                        tempChar[0] = (values[j][k] >> 24) & 0x000000FF;
                        tempChar[1] = (values[j][k] >> 16) & 0x000000FF;
                        tempChar[2] = (values[j][k] >> 8) & 0x000000FF;
                        tempChar[3] = (values[j][k]) & 0x000000FF;
                        strncpy(charCounter, &tempChar[0], 4);
                        charCounter += 4;
                    }


                }
            }
            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC;

            if(d == false){
                printf("It took %f seconds to encrypt %i times.\n", time_taken, b);
            }else{
                printf("It took %f seconds to decrypt %i times.\n", time_taken, b);
            }

            break;
        }

        case false:
            for (int i = 0; i < blockCount; ++i) {
                define_version(v, d, sums, values[i], keys);
                for (int k = 0; k < 2; ++k) {
                    uint8_t tempChar[5];
                    tempChar[4] = '\n';
                    tempChar[0] = (values[i][k] >> 24) & 0x000000FF;
                    tempChar[1] = (values[i][k] >> 16) & 0x000000FF;
                    tempChar[2] = (values[i][k] >> 8) & 0x000000FF;
                    tempChar[3] = (values[i][k]) & 0x000000FF;
                    strncpy(charCounter, &tempChar[0], 4);
                    charCounter += 4;
                }
            }
            break;

    }

    if(d == true && (outputStr[strlen(outputStr) - 2] < 8 && outputStr[strlen(outputStr) - 2] > 0)){
        //if there should be a padding while encrypting obviously the padding bytes also get encrypted. when decrypted
        //chars corresponding to padding size will be displayed. to prevent this, we tried to strip the padding when
        //the encrypted message is written to output file. however, if the encrypted message without the padding bytes
        //is decrypted, the program pads the message with unencrypted padding bytes which causes the last block to get
        //incorrectly decrypted. our solution was not stripping the padding bytes and after decryption of the message
        // with padding bytes searching for a sequence of the same bytes -which are between 0 and 8 both
        // exclusive- that starts from the end to the 7th from the end byte and strip it. although it's theoretically
        //incorrect, since the message itself can include a sequence of the same aforementioned bytes at the end,
        //practically this cannot be the case for human-written messages, because not all machines can print those chars

        uint8_t counter = 0;
        size_t outputLen = strlen(outputStr);
        uint8_t lastChar = outputStr[outputLen - 1];
        if(lastChar == outputStr[outputLen - 2]){
            counter += 2;
        }

        for (int i = outputLen - 3; i > outputLen - 8; i--) {
            if(outputStr[i] != lastChar){
                break;
            }
            counter++;
        }


        uint8_t* outputTemp = malloc(sizeof(*outputTemp) * (valueLen - counter));

        if(outputTemp == NULL){
            fprintf(stderr, "No free memory.\n");
            print_usage(progname);
            return EXIT_FAILURE;
        }

        outputTemp[valueLen - counter - 1] = '\0';
        strncpy(outputTemp, &outputStr[0], valueLen - counter);
        free(outputStr);
        outputStr = outputTemp;
        free(outputTemp);
    }

    fwrite(outputStr, sizeof(*outputStr), strlen(outputStr), output);

    return EXIT_SUCCESS;
}

