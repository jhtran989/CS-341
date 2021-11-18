#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

/* cause of the error...MAKE SURE TO SET A DECENT STRING LENGTH */
#define MAX_STRING_LENGTH 50
#define MAX_NUM_LINES_INPUT 700
#define IDE_DEBUG false
#define PRINT_DEBUG false
#define FINAL_OUTPUT_DEBUG false
#define SKIP_INSTRUCTION_DEBUG false

/* TODO:
 * - remove white spaces (both sides) -- parse input
 * - print the input using the parsed arguments...don't use original input
 * since there's leading zeros...
 *
 * - fix LRU policy...maybe use linked list?
 * - too much work (no built-in library function...)
 * - just use a pointer with personal function to move everything around
 * - should update during hits as well...
 * */

/*
 * Cache Simulator
 *
 * Notes:
 * - Least Recently Used (LRU) -- replacement policy in cache for eviction
 * - cache lines will be used sequentially
 * - could of just stored number of sets/lines/blocks in the cache itself
 * without needing the cache parameters...
 * - lots of typedef for int types (i.e., unsigned long long)
 * - data load "L" vs. data store "S"
 * if store is a miss, then nothing happens after that (whereas a load would
 * result in fetching the data from main memory, disk. etc. and placing in
 * cache)
 * EDIT: scratch above point; they may have the same result...
 *
 *
 * John Tran
 * ID: 101821704
 */

typedef struct {
    int numHits;
    int numMisses;
    int numEvictions;
} cacheSummary;

/* enum for cache hit, miss */
enum cacheHitMiss {
    miss,
    hit
};

const char* HIT_MISS_STRINGS[] = {"miss", "hit"};

enum traceOperation {
    I = 'I',
    L = 'L',
    S = 'S',
    M = 'M'
};

/* define type for cache parts (looked at trace files and addresses in hex
 * had 64 bits, as said in lab spec. -- over the range of unsigned long) */
typedef unsigned long long rawCacheAddress;
typedef unsigned long long cacheTag; /* used for both the cache address and
 * cache line */
typedef unsigned long long cacheBlock; /* instead of storing the values, it
 * will store the addresses that are currently using the specified cache
 * line (below in struct cacheLine), assuming memory accesses don't cross block
 * boundaries*/

/* the "size" field as part of an operation in the trace files */
//typedef unsigned int operationSize;
typedef unsigned int uInt;

/* different parts of the cache address */
typedef unsigned long cacheBlockOffset;
typedef unsigned long cacheSetIndex;

typedef unsigned long long maskType;

typedef struct {
    enum traceOperation operation;
    rawCacheAddress rawAddress;
    int operationSize;
} traceInstruction;

typedef struct {
    rawCacheAddress rawAddress;
    cacheBlockOffset blockOffset;
    cacheSetIndex setIndex;
    cacheTag tag;
} cacheAddress;

typedef struct {
    int valid_bit;
    cacheTag tag;
    cacheBlock *block;
    /* might not be needed since we never "store" any real values, just
     * simulate the checking in the cache */

    uInt lineIndex;
    /* for LRU (least recently used) replacement policy where a lower uInt
     * means it was used earlier than a higher uInt (i.e., from 0, 1,...) */

    uInt numAddresses;
} cacheLine;

typedef struct {
    cacheLine *cacheLines;
    cacheLine *lruLine;
    uInt lruLineIndex;
    uInt numLinesInUse;
    uInt numLines;
    uInt *lineIndexOrder;
} cacheSet;

typedef struct {
    cacheSet *cacheSets;
    uInt numSets;
} entireCache;

/*
 * A struct that groups cache parameters together
 */
typedef struct {
    int m; /* assume 64-bit hexadecimal memory address */
    int s; /* number of bits for set uInt */
    int b; /* number of bits for block offset*/
    int E; /* associativity, or number of cache lines per set */
    int S; /* number of sets (S = 2^s) */
    int B; /* cache line block length in bytes (B = 2^b) */
    char *traceFile;
    bool verbose;
} cacheParameters;

typedef struct {
    enum cacheHitMiss hitMiss;
    bool eviction;
} cacheHitEvictionPair;

/*
 * Function definitions
 * - comment out personal printSummary() function
 */
//void printSummary(int hits, int misses, int evictions);
void printUsage(char** argv);
void printArgs(int argc, char** argv);
void printParsedTraceInstruction(traceInstruction instruction);
void printCacheAddress(cacheAddress address);
void printEntireCache(entireCache cache, cacheParameters parameters);
void printFinalOutput(char** finalOutput, int counterIndex);

#define PRINT_SEPARATOR

entireCache allocateEntireCache();
traceInstruction getTraceInstruction(char *rawTraceInstruction);
char* getTraceInstructionString(traceInstruction *parsedTraceInstruction);
void parseTraceFile(cacheParameters parameters, entireCache cache,
                    cacheSummary *summary);
void updateLRUCacheLineHit(cacheSet *set, cacheLine *currentCacheLine);
void updateLRUCacheLineEviction(cacheSet *set);
cacheAddress getCacheAddress(rawCacheAddress rawAddress,
                             cacheParameters parameters);
cacheHitEvictionPair loadStoreAddress(entireCache cache, cacheAddress address,
                                      cacheParameters parameters,
                                      int operationSize,
                                      cacheSummary *summary,
                                      traceInstruction *parsedInstruction);
cacheSummary initializeCacheSummary();

/*
 * Just for this test file to separate function definitions with actual
 * function implementations
 */
#define SEPARATOR

/*
 * Custom printSummary() function for debugging on local machine
 */
//void printSummary(int hits, int misses, int evictions) {
//    if (PRINT_DEBUG) {
//        printf("Summary:\n");
//    }
//
//    printf("hits: %d\n", hits);
//    printf("misses: %d\n", misses);
//    printf("evictions: %d\n", evictions);
//}

/*
 * Print usage info
 */
void printUsage(char** argv)
{
    printf("Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
    printf("Options:\n");
    printf("  -h              Print this help message with usage info.\n");
    printf("  -v              Optional verbose flag that displays trace info"
           ".\n");
    printf("  -s <s>          Number of set uInt bits (S = 2^s is the number "
           "of sets).\n");
    printf("  -E <E>          Associativity (number of lines per set).\n");
    printf("  -b <b>          Number of block bits (B = 2^b is the block size)"
           ".\n");
    printf("  -t <tracefile>  Name of the valgrind trace to replay.\n");
    printf("\nExamples:\n");
    printf("  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * Print list of args given for debugging purposes
 */
void printArgs(int argc, char** argv) {
    for (int i = 0; i < argc; i++) { // includes first argument (name of
        // executable)
        printf("%s\n", argv[i]);
    }
}

void printParsedTraceInstruction(traceInstruction instruction) {
    printf("Parsed Trace Instruction:\n");
    printf("operation: %c\n", instruction.operation);
    printf("cache address: 0x%llx\n", instruction.rawAddress);
    printf("operation size: %d\n", instruction.operationSize);
    printf("\n");
}

void printCacheAddress(cacheAddress address) {
    printf("address: 0x%llx\n", address.rawAddress);
    printf("block offset: %lu\n", address.blockOffset);
    printf("set index: %lu\n", address.setIndex);
    printf("tag: 0x%llx\n", address.tag);
    printf("\n");
}

void printEntireCache(entireCache cache, cacheParameters parameters) {
    printf("\n");

    printf("----------------------------------------\n");
    printf("Printing entire cache:\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < parameters.S; i++) {
        cacheSet currentSet = cache.cacheSets[i];
        uInt numLinesInUse = currentSet.numLinesInUse;

        //printf("current set successful\n");
        printf("----------------------------------------\n");
        printf("set index: %d\n", i);
        printf("num lines in use: %d\n", numLinesInUse);
        printf("line index order: ");

        if (numLinesInUse == 0) {
            printf("EMPTY");
        }

        for (int j = 0; j < numLinesInUse; j++) {
            printf("%u ", currentSet.lineIndexOrder[j]);
        }

        printf("\n");

        //FIXME
        printf("least recently used (LRU) line index: ");

        if (currentSet.lruLine == NULL) {
            printf("EMPTY");
        } else {
            printf("%d", currentSet.lruLine->lineIndex);
        }

        printf("\n");
        printf("----------------------------------------\n");

        for (int j = 0; j < parameters.E; j++) {
            //printf("current line...\n");
//            printf("size of cache lines: %lu\n", sizeof(currentSet.cacheLines));

            cacheLine *currentLine = &currentSet.cacheLines[j];

            //printf("current line successful\n");

            printf("line index: %u\n", currentLine->lineIndex);
            printf("line tag: 0x%llx\n", currentLine->tag);
            printf("line valid bit: %d\n", currentLine->valid_bit);
            printf("line num addresses: %u\n", currentLine->numAddresses);
            printf("line block(s): ");
            if (currentLine->block != NULL) {
                uInt numAddresses = currentLine->numAddresses; // no way to
                // get the operation size...
                for (int k = 0; k < numAddresses; ++k) {
                    cacheBlock currentBlock = currentLine->block[k];
                    printf("0x%llx ", currentBlock);
                }
            } else {
                printf("EMPTY");
            }

            printf("\n");
//            printf("\n");
        }

//        printf("\n");
    }

    printf("----------------------------------------\n");
}

void printCacheParameters(cacheParameters parameters) {
    printf("\n");
    printf("Printing cache parameters:\n");
    printf("m: %d\n", parameters.m);
    printf("E: %d\n", parameters.E);
    printf("b: %d\n", parameters.b);
    printf("B: %d\n", parameters.B);
    printf("s: %d\n", parameters.s);
    printf("S: %d\n", parameters.S);
    printf("trace file: %s\n", parameters.traceFile);
    printf("\n");
}

/*
 * Prints the final output with verbose option (-v)
 *
 * IMPORTANT: the last space after each line is done HERE
 */
void printFinalOutput(char** finalOutput, int counterIndex) {
    for (int i = 0; i < counterIndex; i++) {
        printf("%s \n", finalOutput[i]);
    }
}

#define PRINT_SEPARATOR

entireCache allocateEntireCache(cacheParameters parameters) {
    entireCache cache;
    cache.numSets = parameters.S;

    /* allocate memory with calloc since it also initializes the values */
    int numSets = parameters.S;
    cache.cacheSets = (cacheSet*) calloc(numSets, sizeof(cacheSet));

    int numLines = parameters.E;
    for (int i = 0; i < numSets; i++) {
        cache.cacheSets[i].cacheLines = (cacheLine*) calloc(
                numLines, sizeof(cacheLine));
        cacheSet *currentSet = &cache.cacheSets[i];
        currentSet->lruLine = NULL;
        currentSet->numLines = parameters.E;
        currentSet->lineIndexOrder = (uInt*) calloc(numLines, sizeof(uInt));

        /* result: ABSOLUTELY CANNOT USE REFERENCES/ALIASES WHEN ALLOCATING
         * MEMORY */
//        currentSet.cacheLines = (cacheLine*) calloc(
//                numLines, sizeof(cacheLine));

        cacheLine *currentLines = currentSet->cacheLines;

        for (int j = 0; j < numLines; j++) {
            cacheLine* currentLine = &currentLines[j];

            /* good initialization for checking later on */
            currentLine->block = NULL;
            currentLine->lineIndex = j;
        }

        //currentLines = calloc(numLines, sizeof(cacheLine));

        /* good initialization for checking later on */
//        for (int j = 0; j < numLines; j++) {
//            currentLines[j].block = NULL;
//        }
    }

    return cache;
}

/*
 * The "operation" field will be checked elsewhere (for the "I" operation
 * instruction load)
 */
traceInstruction getTraceInstruction(char *rawTraceInstruction) {
    traceInstruction parsedInstruction;

    if (rawTraceInstruction[0] == 'I') {
        parsedInstruction.operation = I;
        return parsedInstruction;
    } else {
        parsedInstruction.operation = (unsigned char) rawTraceInstruction[1];
    }

    char *tokenPtr = strtok(rawTraceInstruction + 3, ",");

    int argIndex = 0;
    while (tokenPtr != NULL) {
        if (argIndex == 0) {
            parsedInstruction.rawAddress = strtol(tokenPtr, NULL,
                                                  16);
        } else if (argIndex == 1) {
            parsedInstruction.operationSize = atoi(tokenPtr);
        }

        tokenPtr = strtok(NULL, ",");
        argIndex++;
    }

    if (PRINT_DEBUG) {
        printf("\ninput after token: %s\n", rawTraceInstruction);
    }

    return parsedInstruction;
}

/*
 * The "operation" field will be checked elsewhere (for the "I" operation
 * instruction load)
 */
char* getTraceInstructionString(traceInstruction *parsedTraceInstruction) {
    char* outputTraceInstruction = (char*) calloc(1, MAX_STRING_LENGTH);
    //int outputTracePtr = 0;

    /* first, set the first character to the type of operation */
//    outputTraceInstruction[outputTracePtr] = parsedTraceInstruction->operation;
//    outputTracePtr++;
//
//    outputTraceInstruction[outputTracePtr] = ' ';
//    outputTracePtr++;

    char operationString = parsedTraceInstruction->operation;

    strncat(outputTraceInstruction, &operationString, 1);
    strcat(outputTraceInstruction, " ");

    if (PRINT_DEBUG) {
        printf("after operation: %s\n", outputTraceInstruction);
    }

    /* sets the next characters (separated by a space from the operation) to
     * the address and the operation size, separated by a comma */
    /* converts the address and operation size to strings first */
    char* addressString = (char*) calloc(1, MAX_STRING_LENGTH);
    sprintf(addressString, "%llx", parsedTraceInstruction->rawAddress);

    if (PRINT_DEBUG) {
        printf("address string: 0x%s\n", addressString);
    }

    char* operationSizeString = (char*) calloc(1, MAX_STRING_LENGTH);
    sprintf(operationSizeString, "%d", parsedTraceInstruction->operationSize);

    if (PRINT_DEBUG) {
        printf("operation size string: %s\n", operationSizeString);
    }

    /* appends them to the final output string */
    /* MAKE sure DESTINATION string is CORRECT... */
    strcat(outputTraceInstruction, addressString);
    strcat(outputTraceInstruction, ",");
    strcat(outputTraceInstruction, operationSizeString);

    if (PRINT_DEBUG) {
        printf("after address/operation size: %s\n", outputTraceInstruction);
    }

    return outputTraceInstruction;
}

void parseTraceFile(cacheParameters parameters, entireCache cache,
                    cacheSummary *summary) {
    char *traceFilename = parameters.traceFile;

    //FIXME: go up directory for testing
    FILE *traceFile;
    if (IDE_DEBUG) {
        traceFile = fopen("../traces/yi.trace", "r");
    } else {
        traceFile = fopen(traceFilename, "r");
    }

    if (PRINT_DEBUG) {
        printf("Opening %s...\n", traceFilename);

        if (traceFile == NULL) {
            printf("ERROR: FILE NOT FOUND...\nEXITING...\n");
            exit(99);
        } else {
            printf("File found: %s\n", traceFilename);
        }
    }

    //char testInput[MAX_STRING_LENGTH];
    char *instructionInput = (char*) calloc(MAX_STRING_LENGTH, sizeof(char));

    /* concatenate strings...no spaces between*/
    //int finalOutputLength = 100;
    char **finalOutput = (char**) calloc(MAX_NUM_LINES_INPUT, sizeof(char*));
    char **newFinalOutput = (char**) calloc(MAX_NUM_LINES_INPUT,
                                            sizeof(char*));

    //int stringLength = 50;
    for (int i = 0; i < MAX_NUM_LINES_INPUT; i++) {
        finalOutput[i] = (char*) calloc(MAX_STRING_LENGTH, sizeof(char));
        newFinalOutput[i] = (char*) calloc(MAX_STRING_LENGTH, sizeof(char));
    }

    if (PRINT_DEBUG) {
        printf("Reading trace file...\n");
        printEntireCache(cache, parameters);
    }

    int counterIndex = 0;
    //int newCounterIndex = 0;
    /* dubious...does fgets() clears the pointer each time... */
    while (fgets(instructionInput, MAX_STRING_LENGTH, traceFile) != NULL) {
        //FIXME
        if (PRINT_DEBUG) {
            printf("in while loop...\n");
            printf("input line: %s\n", instructionInput);
        }

        char *copyInstructionInput = (char*) calloc(MAX_STRING_LENGTH,
                                                    sizeof(char));
        strcpy(copyInstructionInput, instructionInput);

        traceInstruction parsedInstruction = getTraceInstruction(
                copyInstructionInput);

        /* get the formatted string of the instruction input */
        char* outputInstructionString =
                getTraceInstructionString(&parsedInstruction);

        /* test the formatted input line */
        if (PRINT_DEBUG) {
            printf("after formatting: %s\n", outputInstructionString);
            printf("\n");
        }

        enum traceOperation operation = parsedInstruction.operation;

        if (operation != I) {
            if (PRINT_DEBUG) {
                printParsedTraceInstruction(parsedInstruction);
            }

            int operationSize = parsedInstruction.operationSize;

            if (PRINT_DEBUG) {
                printf("before get cache address parameters\n");
            }

            //TODO: continue implementation
            cacheAddress address = getCacheAddress(
                    parsedInstruction.rawAddress, parameters);

            if (PRINT_DEBUG) {
                printCacheAddress(address);
                printf("after get cache address parameters\n");
            }

            /* the data load (L) part of M */
            if (operation == M) {
                /* just in case as we will edit the parsed instruction --
                 * split into two parts */
                //traceInstruction copyParsedInstruction = parsedInstruction;
                parsedInstruction.operation = L;
            }

            cacheHitEvictionPair hitEvictionPair =
                    loadStoreAddress(cache, address, parameters,
                                     operationSize, summary,
                                     &parsedInstruction);

            enum cacheHitMiss hitMiss = hitEvictionPair.hitMiss;
            bool eviction = hitEvictionPair.eviction;

            if (PRINT_DEBUG) {
                printf("Final output:\n");
                printf("Instruction line (input): %d\n", counterIndex);
            }

            /* dubious...changes the actual start of the pointer...could be
             * fixed by decrementing again, hopefully... */
            /* remove the space in front of the instruction after parsing */
            instructionInput++;

            /* remove the new line ("\n") after the instruction in the trace
             * file by setting that character to the null character ("\0") */
            instructionInput[strlen(instructionInput) - 1] = '\0';

            if (PRINT_DEBUG) {
                printf("original input: %s ", instructionInput);
            }
            //finalOutput[counterIndex] = instructionInput;

            //TODO: maybe remove extra spaces on either side of input...
            strcat(finalOutput[counterIndex], instructionInput);
            strcat(finalOutput[counterIndex], " ");

            if (PRINT_DEBUG) {
                printf("%s", HIT_MISS_STRINGS[hitMiss]);
            }

            strcat(finalOutput[counterIndex], HIT_MISS_STRINGS[hitMiss]);

            /* edit a final output string */
            strcat(outputInstructionString, " ");
            strcat(outputInstructionString, HIT_MISS_STRINGS[hitMiss]);

            /* eviction now works with store */
            if (eviction && operation) {
                if (PRINT_DEBUG) {
                    printf(" eviction");
                }

                strcat(finalOutput[counterIndex], " eviction");

                /* edit a final output string */
                strcat(outputInstructionString, " eviction");
            }

            //TODO: maybe move into function (repeat from above)

            /* the data store (S) part of M */
            if (operation == M) {
                /* just in case as we will edit the parsed instruction --
                 * split into two parts */
                //traceInstruction copyParsedInstruction = parsedInstruction;
                parsedInstruction.operation = S;

                cacheAddress address = getCacheAddress(
                        parsedInstruction.rawAddress, parameters);
                cacheHitEvictionPair hitEvictionPair =
                        loadStoreAddress(cache, address, parameters,
                                         operationSize, summary,
                                         &parsedInstruction);

                enum cacheHitMiss hitMiss = hitEvictionPair.hitMiss;
                bool eviction = hitEvictionPair.eviction;

                if (PRINT_DEBUG) {
                    printf(" %s", HIT_MISS_STRINGS[hitMiss]);
                }

                strcat(finalOutput[counterIndex], " ");
                strcat(finalOutput[counterIndex], HIT_MISS_STRINGS[hitMiss]);

                /* edit a final output string */
                strcat(outputInstructionString, " ");
                strcat(outputInstructionString, HIT_MISS_STRINGS[hitMiss]);

                /* eviction now works with store */
                if (eviction) {
                    if (PRINT_DEBUG) {
                        printf(" eviction");
                    }

                    strcat(finalOutput[counterIndex], " eviction");

                    /* edit a final output string */
                    strcat(outputInstructionString, " eviction");
                }
            }

            /* move the string to the final output string BEFORE incrementing
             * counter index */
            strcpy(newFinalOutput[counterIndex], outputInstructionString);

            /* IMPORTANT decrement again */
            instructionInput--;

            /* ignores any 'I' instruction */
            counterIndex++;

            if (PRINT_DEBUG) {
                printf("\n\n");
            }
        } else {
            if (SKIP_INSTRUCTION_DEBUG) {
                if (PRINT_DEBUG) {
                    printf("Final output:\n");
                    printf("Instruction line (input): %d\n", counterIndex);
                }

                printf("Skipped 'I' instruction load\n");

                strcat(finalOutput[counterIndex],
                       "Skipped 'I' instruction load");
                //finalOutput[counterIndex] = "Skipped 'I' instruction load";

                /* ------------------- */

                /* edit a final output string */
                strcat(outputInstructionString, " ");
                strcat(outputInstructionString, "SKIPPED");

                /* move the string to the final output string BEFORE
                 * incrementing counter index */
                strcpy(newFinalOutput[counterIndex], outputInstructionString);

                /* IMPORTANT: increment counterIndex for debugging... */
                counterIndex++;
            }
        }

        /* prints out the corresponding line stored in the final output */
        if (FINAL_OUTPUT_DEBUG) {
            printf("counter index: %d\n", counterIndex);
            printf("(old) final output line: %s\n", finalOutput[counterIndex]);
            printf("(new) final output line: %s\n", outputInstructionString);

            /* check if memory is overwritten for previous entries... */
            printf("(old):\n");
            printFinalOutput(finalOutput, counterIndex);

            printf("(new):\n");
            printFinalOutput(newFinalOutput, counterIndex);
        }

        /* move up for final submission -- without 'I' instruction now... */
        /* includes the 'I' instruction */
        //counterIndex++;

        if (SKIP_INSTRUCTION_DEBUG) {
            printEntireCache(cache, parameters);
        } else {
            if (PRINT_DEBUG && (operation != I)) {
                printEntireCache(cache, parameters);
            }
        }
    }

    if (PRINT_DEBUG) {
        printf("Completed -- FINAL OUTPUT\n");
        printf("number of instructions: %d\n", counterIndex);
    }

    /* the final space AFTER each line is in the printFinalOutput() function */
    if (parameters.verbose) {
        if (PRINT_DEBUG) {
            printf("(old):\n");
            printFinalOutput(finalOutput, counterIndex);
        }

        if (PRINT_DEBUG) {
            printf("(new):\n");
        }
        printFinalOutput(newFinalOutput, counterIndex);
    }

    if (PRINT_DEBUG) {
        printf("\n");
    }
}

void updateLRUCacheLineHit(cacheSet *set, cacheLine *currentCacheLine) {
    /* need to update LRU line and line index order */
    uInt updateLineIndex = currentCacheLine->lineIndex;

    uInt numLinesInUse = set->numLinesInUse;

    uInt lineOrderIndex = -1;
    for (uInt i = 0; i < (numLinesInUse - 1); i++) {
        if (set->lineIndexOrder[i] == updateLineIndex) {
            lineOrderIndex = i;
            break;
        }
    }

    /* move all indices after updateLineIndex back by one so we can add
     * updateLineIndex at the end of the line index order */
    /* need to find where the line index is in the order array first...
     * from above*/
    for (uInt i = lineOrderIndex; i < (numLinesInUse - 1); i++) {
        set->lineIndexOrder[i] = set->lineIndexOrder[i + 1];
    }

    set->lineIndexOrder[numLinesInUse - 1] = updateLineIndex;

    if (PRINT_DEBUG) {
        printf("---------------------------------\n");
        printf("HIT section:\n");
        printf("num lines in use: %u\n", numLinesInUse);
        printf("update line index: %u\n", updateLineIndex);
        printf("---------------------------------\n");
    }

    /* set the NEW LRU line -- hit only if the set is NONEMPTY */
    set->lruLineIndex = set->lineIndexOrder[0];
    set->lruLine = &set->cacheLines[set->lruLineIndex];
}

void updateLRUCacheLineEviction(cacheSet *set) {
    //int numLines = parameters.E;
    uInt numLinesInUse = set->numLinesInUse;

    /* since cache lines are used sequentially, the next LRU line should the
     * one after the previous LRU line */
    /* EDIT: only when all the lines are not used up yet is above true, need
     * to shift the line order otherwise... */

//    uInt lruLineIndex = set->lruLineIndex;
//    set->lruLine = &set->cacheLines[(lruLineIndex + 1) % numLines];

    /* move all indices after updateLineIndex back by one so we can add
     * updateLineIndex at the end of the line index order */
    /* need to shift everything back one and move the LRU index to the very
     * end */
    uInt updateLineIndex = set->lineIndexOrder[0];

    for (uInt i = 0; i < (numLinesInUse - 1); i++) {
        set->lineIndexOrder[i] = set->lineIndexOrder[i + 1];
    }

    set->lineIndexOrder[numLinesInUse - 1] = updateLineIndex;

    if (PRINT_DEBUG) {
        printf("---------------------------------\n");
        printf("EVICTION section:\n");
        printf("num lines in use: %u\n", numLinesInUse);
        printf("update line index: %u\n", updateLineIndex);
        printf("---------------------------------\n");
    }

    /* set the NEW LRU line -- hit only if the set is NONEMPTY */
    set->lruLineIndex = set->lineIndexOrder[0];
    set->lruLine = &set->cacheLines[set->lruLineIndex];
}

cacheAddress getCacheAddress(rawCacheAddress rawAddress,
                             cacheParameters parameters) {
    cacheAddress finalAddress;

    int numBlockBits = parameters.b;
    maskType blockMask = (1 << numBlockBits) - 1;

    int numSetBits = parameters.s;
    maskType setMask = (1 << numSetBits) - 1;
    int setShift = numBlockBits;

    int numTagBits = parameters.m - (numBlockBits + numSetBits);
    maskType tagMask = (1 << numTagBits) - 1;
    int tagShift = numBlockBits + numSetBits;

    finalAddress.rawAddress = rawAddress;
    finalAddress.blockOffset = rawAddress & blockMask;
    finalAddress.setIndex = (rawAddress >> setShift) & setMask;
    finalAddress.tag = (rawAddress >> tagShift) & tagMask;

    return finalAddress;
}

/*
 * Need the type of instruction load to see if we need to check for eviction...
 */
cacheHitEvictionPair loadStoreAddress(entireCache cache, cacheAddress address,
                                      cacheParameters parameters, int operationSize,
                                      cacheSummary *summary,
                                      traceInstruction *parsedInstruction) {
    cacheSetIndex setIndex = address.setIndex;
    cacheTag tag = address.tag;
    cacheSet *set = &cache.cacheSets[setIndex];

    //enum traceOperation operation = parsedInstruction->operation;

    int numLines = parameters.E;
    enum cacheHitMiss hitMiss = miss;
    bool eviction = true;
    cacheLine *currentCacheLine;
    for (uInt i = 0; i < numLines; i++) {
        currentCacheLine = &set->cacheLines[i];

        /* maybe should have created constants, but 1 used for valid bit
         * when checking for hit */
        if (currentCacheLine->valid_bit == 1) {
            if (currentCacheLine->tag == tag) {
                /* now check through all the addresses stored at the
                 * specified cache line to see if it's already in the cache */
                uInt numAddresses = currentCacheLine->numAddresses;
                rawCacheAddress currentAddress = address.rawAddress;
                cacheBlock *blockAddresses = currentCacheLine->block;

                for (uInt j = 0; j < numAddresses; j++) {
                    if (currentAddress == blockAddresses[j]) {
                        hitMiss = hit;
                        eviction = false;
                        /* no eviction when there is a hit */
                        break;
                    }
                }

                if (hitMiss == hit) {
                    break;
                }
            }
        } else {
            /* no eviction when there are still unused lines (since valid
             * bit is 0) */
            eviction = false;
            break;
        }
    }

    if (hitMiss == hit) {
        summary->numHits++;

        updateLRUCacheLineHit(set, currentCacheLine);
    } else { // hitMiss == miss
        summary->numMisses++;

        /* only process eviction if the instruction is not a store, do
         * nothing otherwise... */
//        if (operation != S) {
//
//        }
        /* maybe no difference after all... */
        if (eviction) {
            summary->numEvictions++;

            /* need to implement Least Recently Used (LRU) replacement policy
             * for eviction */
            cacheLine *lruLine = set->lruLine;

//            printf("LRU cache line:\n");
//            printf("line index: %u\n", lruLine->lineIndex);
//            printf("line tag: %llu\n", lruLine->tag);
//            printf("line valid bit: %d\n", lruLine->valid_bit);
//            printf("line num addresses: %u\n", lruLine->numAddresses);
//            printf("line block(s): ");

            /* memory should be REALLOCATED */
            free(lruLine->block);

            /* request (operation) sizes don't matter... */
            //int numAddresses = parameters.B / operationSize;
            int numAddresses = parameters.B;
            lruLine->block = calloc(numAddresses, sizeof(cacheBlock));

            cacheBlock *lruBlock = lruLine->block;

            /* relabel addresses to the specified cache line */
            rawCacheAddress initialAddress = address.rawAddress
                                             - address.blockOffset;
            for (uInt i = 0; i < numAddresses; i++) {
                lruBlock[i] = initialAddress + i;
            }

            lruLine->tag = address.tag;
            lruLine->numAddresses = numAddresses;

            updateLRUCacheLineEviction(set);
        } else { /* maybe need to account store WITH eviction and other
 * instructions WITHOUT eviction separately -- don't do anything to store? */
            cacheLine *lineEdit;
            uInt numLinesInUse = set->numLinesInUse;

            lineEdit = &set->cacheLines[numLinesInUse];
            //lineEdit->lineIndex = numLinesInUse;

            //TODO: edit valid bit and block addresses

            /* request (operation) sizes don't matter... */
//            int numBytes = parameters.B;
//            int numAddresses = numBytes / operationSize;
            int numAddresses = parameters.B;
            lineEdit->block = calloc(numAddresses, sizeof(cacheBlock));

            cacheBlock *currentBlock = lineEdit->block;
            rawCacheAddress initialAddress = address.rawAddress
                                             - address.blockOffset;
            for (int i = 0; i < numAddresses; i++) {
                currentBlock[i] = initialAddress + i;
            }

            lineEdit->valid_bit = 1;
            lineEdit->tag = address.tag;
            lineEdit->numAddresses = numAddresses;

            /* should only set lru line at the beginning -- index 0 */
            /* sets the first line as the lru line if none of the lines were
             * used yet */
            if (numLinesInUse == 0) {
                set->lruLine = lineEdit;
                set->lruLineIndex = numLinesInUse;
            }

            /* need to update line index order for miss (just append to end,
             * and just use the number of lines in use) */
            set->lineIndexOrder[numLinesInUse] = numLinesInUse;

            set->numLinesInUse++;
        }
    }

    /* only when all lines have a valid bit of 1 and none of the tags
     * matched will eviction occur */
    cacheHitEvictionPair hitEvictionPair = {hitMiss, eviction};

    return hitEvictionPair;
}

cacheSummary initializeCacheSummary() {
    cacheSummary summary;

    summary.numHits = 0;
    summary.numMisses = 0;
    summary.numEvictions = 0;

    return summary;
}

//TODO: cacheBlock pointer that dynamically allocates memory with each
// operation (calloc and free...)

//TODO: allocate memory for POINTERS

//TODO: also valid bit is set to 0 if unused -- EASY check

//TODO: replace atoi() with strtol()...use atoi() for now
//TODO: check optarg

// IMPORTANT
//TODO: check "I" instructions below...

//TODO: set last character to '\0' to remove it

int main(int argc, char **argv) {
    if (PRINT_DEBUG) {
        //printSummary(1, 2, 3);
        printArgs(argc, argv);

        printf("-----------------------------\n");
        printf("Test Stuff:\n");
        printf("-----------------------------\n");

        /* test right shift with unsigned numbers...logical shift, as expected */
//    unsigned int a = UINT32_MAX;
//    printf("a before shift: %x\n", a);
//    printf("a after shift: %x\n", a >> 31);

        /* test if variables are initialized with some value...*/
//    int test;
//    printf("value of test: %d\n", test);

        /* test pointer arithmetic */
//    char *b = "abc";
//    printf("string before: %s\n", b);
//    printf("string after: %s\n", b + 1);

        /* test enums and values */
        /* result: can set enum to ANY value -- no errors... */
//    traceInstruction instruction;
//    instruction.operation = 'P';
//    printf("operation: %c\n", instruction.operation);
//    if (instruction.operation == I) {
//        printf("DONE...'I' works\n");
//    }

        /* test opening a file */
        /* result: the executable from CLion is NOT at the root */
//    FILE *file = fopen("../traces/yi.trace", "r");
//    if (file == NULL) {
//        printf("Test opening file FAILED\n");
//        exit(99);
//    } else {
//        printf("Test opening file SUCCEEDED\n");
//    }

        /* test strcat */
        /* result: used wrong function...strcpy */
        char* a = (char*) calloc(50, sizeof(char));
        strcat(a, "a");
        printf("test strcat: %s\n", a);
        strcat(a, "b");
        printf("test strcat: %s\n", a);
        strcat(a, "c");
        printf("test strcat: %s\n", a);

        /* test null pointer... */
//        char* testNullPtr;
//        char* otherNullPtr = testNullPtr;
//
//        otherNullPtr = calloc(1, sizeof(char));
//        if (testNullPtr == NULL) {
//
//        }

        /* from before, make sure EVERYTHING is a pointer -- reference */

        /* test if non pointer alias has the same address */
        /* result: it results in a copy (different addresses with room to fit all
         * data in the defined struct -- now, what about pointers?)...so
         * very big memory constraints */
        /* result 2: WORKS IF THE ORIGINAL WAS THE ONE ALLOCATED, but doesn't
         * work if the alias allocates first (some weird address if not
         * allocated, so no errors when printing out the address) */

//    entireCache testCache;
//    entireCache anotherTestCache = testCache;
        typedef struct {
            char charArray[256];
        } test;

        typedef struct {
            char* array;
        } test2;

        test testCache;
        test anotherTestCache = testCache;

        printf("address of test cache: %p\n", &testCache);
        printf("address of another test cache: %p\n", &anotherTestCache);

        test2 testCache2;
        //testCache2.array = calloc(256, sizeof(char));
        test2 anotherTestCache2 = testCache2;
        anotherTestCache2.array = calloc(256, sizeof(char));
        //testCache2 = anotherTestCache2;
        //test2 someTestCache2;

        printf("address of test cache2: %p\n", &testCache2);
        printf("address of another test cache2: %p\n", &anotherTestCache2);
        printf("check first element of the char*\n");
        printf("address of test cache2: %p\n", &testCache2.array[0]);
        printf("address of another test cache2: %p\n", &anotherTestCache2.array[0]);
        //printf("address of some test cache2: %p\n", &someTestCache2.array[0]);

        printf("-----------------------------\n");
    }

    cacheParameters parameters;

    int opt;
    int previousOpt;

    /* IMPORTANT: set m to 64-bit addresses */
    parameters.m = 64;

    /* IMPORTANT: initialize the verbose option to false */
    parameters.verbose = false;

    /* put ':' in the starting of the string so that program can distinguish
     * between '?' and ':'*/
    while((opt = getopt(argc, argv, ":hvs:E:b:t:")) != -1)
    {
        if (PRINT_DEBUG) {
            printf("Option input: %c\n", opt);
        }

        switch(opt)
        {
            case 'h':
                printUsage(argv);
                break;
            case 'v':
                /* set some sort of flag for verbose printing... */
                parameters.verbose = true;
                break;
            case 's':
                if (PRINT_DEBUG) {
                    printf("Option argument: %s\n", optarg);
                }

                int sBits = atoi(optarg);
                parameters.s = sBits;
                parameters.S = 1 << sBits;
                break;
            case 'E':
                if (PRINT_DEBUG) {
                    printf("Option argument: %s\n", optarg);
                }

                parameters.E = atoi(optarg);
                break;
            case 'b':
                if (PRINT_DEBUG) {
                    printf("Option argument: %s\n", optarg);
                }

                int bBits = atoi(optarg);
                parameters.b = bBits;
                parameters.B = 1 << bBits;
                break;
            case 't':
                if (PRINT_DEBUG) {
                    printf("Option argument: %s\n", optarg);
                }

                parameters.traceFile = optarg;
                break;
            case ':':
                printf("Error, option %c needs an argument\n", previousOpt);

                exit(99);
                break;
            case '?':
                printf("Error, unknown option %c\n", optopt);

                exit(99);
                break;
            default:
                printf("Help...this shouldn't happen.\n");

                exit(99);
                break;
        }

        previousOpt = opt;
    }

    if (PRINT_DEBUG) {
        printf("Finished parsing command line arguments\n");
        printCacheParameters(parameters);
    }

    //TODO: create entire cache and allocate memory, including cache summary
    // (num hits, misses, evictions)
    cacheSummary summary = initializeCacheSummary();
    entireCache cache = allocateEntireCache(parameters);

    parseTraceFile(parameters, cache, &summary);
    printSummary(summary.numHits, summary.numMisses, summary.numEvictions);

    if (PRINT_DEBUG) {
        printCacheParameters(parameters);
    }

    return 0;
}
