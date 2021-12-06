/*
 * Transpose Matrix
 *
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 *
 * John Tran
 * ID: 101821704
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_32_32_desc[] = "Optimized transpose for 32 x 32 (M = 32, N = 32)";
void trans_32_32(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    //int k;
    int blockRowIndex, blockColumnIndex; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 8;

    /* removed this below -- optimize for specific cases anyways */
    //int reducedMatrixSize = blockSize * (M / blockSize);

    bool diagonal = false;
    int diag_index;

    /* Blocking in block size of 8 x 8 */
    /* Changed from original i that indexes the entire matrix size to just
     * the block size */
    for (blockColumnIndex = 0; blockColumnIndex < M;
         blockColumnIndex += blockSize) {
        for (blockRowIndex = 0; blockRowIndex < M;
             blockRowIndex += blockSize) {
            for (i = blockRowIndex; i < blockRowIndex + blockSize; i++) {
                for (j = blockColumnIndex;
                     j < blockColumnIndex + blockSize; j++) {
                    if (i != j) {
                        B[j][i] = A[i][j];
//                        tmp = A[i][j];
//                        B[j][i] = tmp;
                    } else {
                        tmp = A[i][j];
                        diag_index = i;
                        diagonal = true;
                    }
                }

                /* instead of missing B[j][i], we leave it at the end of the
                 * row */
                if (diagonal) {
                    B[diag_index][diag_index] = tmp;
                    diagonal = false;
                }
            }
        }
    }

    if (is_transpose(M, N, A, B)) {
        printf("Success. Transpose of 64 x 64 worked.");
    } else {
        printf("ERROR...\n");
        printf("Transpose of 64 x 64 did not work...\n");
        printf("Exiting...\n");

        exit(99);
    }

//    for (i = 0; i < N; i++) {
//        for (j = 0; j < M; j++) {
//            tmp = A[i][j];
//            B[j][i] = tmp;
//        }
//    }
}

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_64_64_desc[] = "Optimized transpose for 64 x 64 (M = 64, N = 64)";
void trans_64_64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    //int k;
    int kk, jj; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 8;
    int reducedMatrixSize = blockSize * (M / blockSize);

    /* Blocking in block size of 8 x 8 */
    for (kk = 0; kk < reducedMatrixSize; kk += blockSize) {
        for (jj = 0; jj < reducedMatrixSize; jj += blockSize) {
            for (i = 0; i < M; i++) {
                for (j = jj; j < jj + blockSize; j++) {
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }

    if (is_transpose(M, N, A, B)) {
        printf("Success. Transpose of 64 x 64 worked.");
    } else {
        printf("ERROR...\n");
        printf("Transpose of 64 x 64 did not work...\n");
        printf("Exiting...\n");

        exit(99);
    }

//    for (i = 0; i < N; i++) {
//        for (j = 0; j < M; j++) {
//            tmp = A[i][j];
//            B[j][i] = tmp;
//        }
//    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans_64_64, trans_64_64_desc);
    registerTransFunction(trans_32_32, trans_32_32_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
