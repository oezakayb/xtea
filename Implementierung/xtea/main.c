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
        "Usage: %s file [options]   Encrypts file with predefined key\n"
        "   or: %s -h / --help   Show all options\n";

const char* help_msg =
        "Positional arguments:\n"
        "  file   The file to encrypt or decrypt\n"
        "\n"
        "Optional arguments:\n"
        "  -V X   Runs the chosen version of implementation\n"
        "       V0: Our Assembly implementation (default)\n"
        "       V1: Our C implementation\n"
        "       V2: Our C implementation with precalculated sum values\n"
        "       V3: Reference C code of Needham and Wheeler\n"
        "  -B X   The program will run for X times and the total runtime of the given version of implementation will be measured\n"
        "       and printed (default: X = 1)\n"
        "  -k X   A 128-bit integer key for encrypting and decrypting\n"
        "  -i X   The initialization vector (32-bit integer)\n"
        "  -o X   The output file\n"
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
    uint32_t keys[4] = {0X61737061,
                        0X73706173,
                        0X70617370,
                        0X61737061};
    uint32_t initVec = 0;
    FILE *output = NULL;

    const unsigned long [64] sums;
    sums[0] = 0;

    for (int i = 1; i < 64; ++i) {
        sums[i] = sums[i - 1] + 0x9E3779B9;
    }

    struct option long_options[] = {
            {"help",    0,              NULL, 'h'},
            {0,         0,                 0,  0 }
    };

    while((opt = getopt_long(argc, argv, "V:B:k:i:O:dhhelp", long_options, 0)) != -1) {
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
                break;
            case 'i':
                initVec = atoi(optarg);
                for(int i = 0; i < 4; i++){
                    //keys[i] = keys[i] & initVec + initVec;
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
    uint8_t* value = malloc(sizeof(*value) * (fileStat.st_size + 1)); //Consider the Null Byte
    value[fileStat.st_size] = '\0';
    size_t valueLen = fread(value, 1, fileStat.st_size, file);
    fclose(file);

    //valueLen >< 8 pad

    uint32_t values[2];

    for (int i = 0; i < 2; ++i) {
        uint8_t tempChar[5];
        tempChar[4] = '\0';
        strncpy(tempChar, &value[i*4], 4);
        uint32_t tempInt32 [4] = {tempChar[0], tempChar[1], tempChar[2], tempChar[3]};

        values[i] = (tempInt32[0] << 24) | (tempInt32[1] << 16) | (tempInt32[2] << 8) | (tempInt32[3]);
        printf("%d\n", values[i]);
    }

    //free(value);

    if(output == NULL){
        output = fopen("output.txt", "w");
    }

    switch(bbool){
        case true:
            clock_t t = clock();
            for (int i = 0; i < b; ++i) {
                define_version(v, d, values, keys);
            }
            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC;

            printf("It took %f seconds to encrypt %i times.\n", time_taken, b);
            break;

        case false:
            define_version(v, d, values, keys);
            break;

    }


    free(value);



    return EXIT_SUCCESS;
}

