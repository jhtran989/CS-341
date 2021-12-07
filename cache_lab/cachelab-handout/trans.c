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

#define PRINT_DEBUG false

#define SEPARATOR

/* Function definitions/prototypes */
void trans(int M, int N, int A[N][M], int B[M][N]);
void trans_32_32(int M, int N, int A[N][M], int B[M][N]);
void trans_61_67(int M, int N, int A[N][M], int B[M][N]);
void trans_64_64_diag(int M, int N, int A[N][M], int B[M][N]);
void trans_64_64_L_diag(int M, int N, int A[N][M], int B[M][N]);
void trans_64_64_zigzag(int M, int N, int A[N][M], int B[M][N]);

#define SEPARATOR

//TODO: Make sure to comment out the is_transpose check for each function...

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 *
 * takes an input matrix A (N x M) and takes the matrix transpose, storing it
 * in the output matrix B (M x N)
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32) {
        trans_32_32(M, N, A, B);
    } else if (M == 61) {
        trans_61_67(M, N, A, B);
    } else if (M == 64) {
        trans_64_64_L_diag(M, N, A, B);
    } else {
        printf("ERROR...Unknown case for M = %d\n", M);
        printf("EXITING...\n");
        exit(99);
    }
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

#define SEPARATOR

/*
 * Diagonal local variable in 32 x 32 and 61 x 67
 *
 * As a result of the thrashing when the row indexing of A and B match, we
 * can store the diagonal element for the corresponding row and column of A
 * and B, respectively. Since the row (8 ints since B = 32 bytes) of B was
 * stored previously, setting the diagonal element for B would result in a
 * hit (otherwise, setting B[i][i] = A[i][i] would evict the A row to store
 * the corresponding B row).
 */

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

    if (PRINT_DEBUG) {
        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 64 x 64 worked.");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 64 x 64 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
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
char trans_61_67_desc[] = "Optimized transpose for 61 x 67 (M = 61, N = 67)";
void trans_61_67(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    //int k;
    int blockRowIndex, blockColumnIndex; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 16;

    /* removed this below -- optimize for specific cases anyways */
    //int reducedMatrixSize = blockSize * (M / blockSize);

    bool diagonal = false;
    int diag_index;

    /* Blocking in block size of 8 x 8 */
    /* Changed from original i that indexes the entire matrix size to just
     * the block size */
    /* Extra conditional inside the inner loops (i.e., i < N and j < M) are
     * needed since the block size does not divide the dimensions M and N
     * evenly (M = 61 and N = 67 with a block size of 16...) */
    for (blockColumnIndex = 0; blockColumnIndex < M;
         blockColumnIndex += blockSize) {
        for (blockRowIndex = 0; blockRowIndex < N;
             blockRowIndex += blockSize) {
            for (i = blockRowIndex; (i < blockRowIndex + blockSize) && (i < N);
                 i++) {
                for (j = blockColumnIndex;
                     (j < blockColumnIndex + blockSize) && (j < M); j++) {
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

    if (PRINT_DEBUG) {
        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 61 x 67 worked.\n");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 61 x 67 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
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
 *
 * took a lot of trial and error, but a block size of 4 produced the best
 * results
 *
 * Ordering for matrix B
 *  9 10 11 12
 *  6  7  8 13
 *  3  4  5 14
 *  0  1  2 15
 *
 *  -  -  >  |
 *  -  -  >  |
 *  -  -  >  |
 *  -  -  >  V
 *
 * Sections:
 *     3
 *     2     4
 *     1
 *     0
 *
 * Ordering for matrix A
 *   9  6  3  0
 *  10  7  4  1
 *  11  8  5  2
 *  12 13 14 15
 *
 *  |  |  |  |
 *  |  |  |  |
 *  V  V  V  V
 *  -  -  -  >
 *
 * Sections:
 *
 *  3  2  1  0
 *
 *      4
 *
 * Note: the sections given above for A and B will be used below to label
 * when each section of the transpose will be done (numbering is done
 * sequentially -- section 0 is done first, then section 1,...)
 *
 * this method stores the diagonal elements of A to prevent thrashing when
 * assigning the corresponding elements to B (applicable for diagonal blocks)
 * since the same row index of A and B will be stored in the same set...
 *
 * Diagonals of A
 * *
 *   *
 *     *
 *       *
 */
char trans_64_64_diag_desc[] = "Optimized transpose for 64 x 64 diagonal "
                               "(M = 64, N = 64)";
void trans_64_64_diag(int M, int N, int A[N][M], int B[M][N])
{
    //int i, j, tmp;
    //int k;
    int blockRowIndex, blockColumnIndex; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 4;

    /* removed this below -- optimize for specific cases anyways */
    //int reducedMatrixSize = blockSize * (M / blockSize);

    //bool diagonal = false;
    //int diag_index;

    int diag_value_0;
    int diag_value_1;
    int diag_value_2;
    int diag_value_3;

    /* Blocking in block size of 8 x 8 */
    /* Changed from original i that indexes the entire matrix size to just
     * the block size */
    for (blockRowIndex = 0; blockRowIndex < N;
         blockRowIndex += blockSize) {
        for (blockColumnIndex = 0; blockColumnIndex < M;
             blockColumnIndex += blockSize) {
            diag_value_0 = A[blockRowIndex][blockColumnIndex];
            diag_value_1 = A[blockRowIndex + 1][blockColumnIndex + 1];
            diag_value_2 = A[blockRowIndex + 2][blockColumnIndex + 2];
            diag_value_3 = A[blockRowIndex + 3][blockColumnIndex + 3];

            /* Section 0 */
            B[blockColumnIndex + 3][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 2] =
                    A[blockRowIndex + 2][blockColumnIndex + 3];

            /* Section 1 */
            B[blockColumnIndex + 2][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 2];
            B[blockColumnIndex + 2][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 2];
            B[blockColumnIndex + 2][blockRowIndex + 2] =
                    diag_value_2;
            /* diagonal value of A in row index 1 (not in L-shape) */
            //l_value_4 = A[blockRowIndex + 1][blockColumnIndex + 1];

            /* Section 2 */
            B[blockColumnIndex + 1][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 1];
            B[blockColumnIndex + 1][blockRowIndex + 1] = diag_value_1;
            B[blockColumnIndex + 1][blockRowIndex + 2] =
                    A[blockRowIndex + 2][blockColumnIndex + 1];

            /* Section 3 */
            B[blockColumnIndex][blockRowIndex] = diag_value_0;
            B[blockColumnIndex][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex];
            B[blockColumnIndex][blockRowIndex + 2] =
                    A[blockRowIndex + 2][blockColumnIndex + 0];

            /* Section 4 */
            B[blockColumnIndex][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex];
            B[blockColumnIndex + 1][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 1];
            B[blockColumnIndex + 2][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 2];


            B[blockColumnIndex + 3][blockRowIndex + 3] = diag_value_3;
        }
    }

    if (PRINT_DEBUG) {
//        printArrayA(A);
//        printArrayB(B);

        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 64 x 64 worked.\n");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 64 x 64 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
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
 *
 * took a lot of trial and error, but a block size of 4 produced the best
 * results
 *
 * Ordering for matrix B
 *  9 10 11 12
 *  6  7  8 13
 *  3  4  5 14
 *  0  1  2 15
 *
 *  -  -  >  |
 *  -  -  >  |
 *  -  -  >  |
 *  -  -  >  V
 *
 * Sections:
 *     3
 *     2     4
 *     1
 *     0
 *
 * Ordering for matrix A
 *   9  6  3  0
 *  10  7  4  1
 *  11  8  5  2
 *  12 13 14 15
 *
 *  |  |  |  |
 *  |  |  |  |
 *  V  V  V  V
 *  -  -  -  >
 *
 * Sections:
 *
 *  3  2  1  0
 *
 *      4
 *
 * Note: the sections given above for A and B will be used below to label
 * when each section of the transpose will be done (numbering is done
 * sequentially -- section 0 is done first, then section 1,...)
 *
 * this method stores the values along the L-shape of A (hugging column 0)
 * initially, and then grabs the diagonal elements of A as we proceed through
 *
 * L-shape of A (along column 0)
 * *
 * *
 * *  *  *
 *
 *
 * Diagonals of A
 * *
 *   *
 *     *
 *       *
 *
 * All values grabbed from A
 * *
 * *  *
 * *  *  *
 *         *
 *
 * Another note: luckily, managed to use all 12 local values allowed to save
 * some misses for this one...
 */
char trans_64_64_L_diag_desc[] = "Optimized transpose for 64 x 64 "
                                 "L-shape diagonal (M = 64, N = 64)";
void trans_64_64_L_diag(int M, int N, int A[N][M], int B[M][N])
{
    //int i, j, tmp;
    //int k;
    int blockRowIndex, blockColumnIndex; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 4;

    /* removed this below -- optimize for specific cases anyways */
    //int reducedMatrixSize = blockSize * (M / blockSize);

//    bool diagonal = false;
//    int diag_index;

    int l_value_0;
    int l_value_1;
    int l_value_2;
    int l_value_3;
    int l_value_4;
    int diagonal_value; /* holds the extra diagonal values not in the L-shape */

    /* Blocking in block size of 8 x 8 */
    /* Changed from original i that indexes the entire matrix size to just
     * the block size */
    for (blockRowIndex = 0; blockRowIndex < N;
         blockRowIndex += blockSize) {
        for (blockColumnIndex = 0; blockColumnIndex < M;
             blockColumnIndex += blockSize) {
            l_value_0 = A[blockRowIndex][blockColumnIndex];
            l_value_1 = A[blockRowIndex + 1][blockColumnIndex];

            /* diagonal value of A in row index 1 (not in L-shape) */
            /* same row of A to prevent a miss later on*/
            diagonal_value = A[blockRowIndex + 1][blockColumnIndex + 1];

            l_value_2 = A[blockRowIndex + 2][blockColumnIndex];
            l_value_3 = A[blockRowIndex + 2][blockColumnIndex + 1];
            l_value_4 = A[blockRowIndex + 2][blockColumnIndex + 2];

            /* Section 0 */
            B[blockColumnIndex + 3][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 2] =
                    A[blockRowIndex + 2][blockColumnIndex + 3];

            /* Section 1 */
            B[blockColumnIndex + 2][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 2];
            B[blockColumnIndex + 2][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 2];
            B[blockColumnIndex + 2][blockRowIndex + 2] = l_value_4;
            /* diagonal value of A in row index 1 (not in L-shape) */
            //l_value_4 = A[blockRowIndex + 1][blockColumnIndex + 1];

            /* Section 2 */
            B[blockColumnIndex + 1][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 1];
            B[blockColumnIndex + 1][blockRowIndex + 1] = diagonal_value;
            B[blockColumnIndex + 1][blockRowIndex + 2] = l_value_3;

            /* Section 3 */
            B[blockColumnIndex][blockRowIndex] = l_value_0;
            B[blockColumnIndex][blockRowIndex + 1] = l_value_1;
            B[blockColumnIndex][blockRowIndex + 2] = l_value_2;

            /* Section 4 */
            B[blockColumnIndex][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex];
            B[blockColumnIndex + 1][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 1];
            B[blockColumnIndex + 2][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 2];

            diagonal_value = A[blockRowIndex + 3][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 3] = diagonal_value;
        }
    }

    if (PRINT_DEBUG) {
//        printArrayA(A);
//        printArrayB(B);

        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 64 x 64 worked.\n");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 64 x 64 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
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
 *
 * took a lot of trial and error, but a block size of 4 produced the best
 * results
 *
 * Ordering for matrix B
 * 11 10  9 12
 *  6  7  8 13
 *  5  4  3 14
 *  0  1  2 15
 *
 *  <  -  -  |
 *  -  -  >  |
 *  <  -  -  |
 *  -  -  >  V
 *
 * Sections:
 *     3
 *     2     4
 *     1
 *     0
 *
 * Ordering for matrix A
 *  11  6  5  0
 *  10  7  4  1
 *   9  8  3  2
 *  12 13 14 15
 *
 *  ^  |  ^  |
 *  |  |  |  |
 *  |  V  |  V
 *  -  -  -  >
 *
 * Sections:
 *
 *  3  2  1  0
 *
 *      4
 *
 * Note: the sections given above for A and B will be used below to label
 * when each section of the transpose will be done (numbering is done
 * sequentially -- section 0 is done first, then section 1,...)
 *
 * This method is a slight modification to the L-shape method defined above.
 * Basically, the same values of A are fetched before going through the block
 * (same size of 4), but the pattern of going through the values of B and A
 * resemble a zig-zag pattern (rows and columns follow this pattern for B and
 * A, respectively).
 *
 * L-shape of A (along column 0)
 * *
 * *
 * *  *  *
 *
 *
 * Diagonals of A
 * *
 *   *
 *     *
 *       *
 *
 * All values grabbed from A
 * *
 * *  *
 * *  *  *
 *         *
 *
 * Another note: luckily, managed to use all 12 local values allowed to save
 * some misses for this one...
 */
char trans_64_64_zigzig_desc[] = "Optimized transpose for 64 x 64 "
                                 "zig-zag (M = 64, N = 64)";
void trans_64_64_zigzag(int M, int N, int A[N][M], int B[M][N])
{
    //int i, j, tmp;
    //int k;
    int blockRowIndex, blockColumnIndex; /* remove k indexing -- not needed for transpose */
    //int sum;
    int blockSize = 4;

    /* removed this below -- optimize for specific cases anyways */
    //int reducedMatrixSize = blockSize * (M / blockSize);

//    bool diagonal = false;
//    int diag_index;

    int l_value_0;
    int l_value_1;
    int l_value_2;
    int l_value_3;
    int l_value_4;

    /* holds the extra diagonal values not in the L-shape */
    //int diagonal_value;

    /* Blocking in block size of 8 x 8 */
    /* Changed from original i that indexes the entire matrix size to just
     * the block size */
    for (blockRowIndex = 0; blockRowIndex < N;
         blockRowIndex += blockSize) {
        for (blockColumnIndex = 0; blockColumnIndex < M;
             blockColumnIndex += blockSize) {
            l_value_0 = A[blockRowIndex][blockColumnIndex];
            l_value_1 = A[blockRowIndex + 1][blockColumnIndex];

            /* diagonal value of A in row index 1 (not in L-shape) */
            /* same row of A to prevent a miss later on*/
            //diagonal_value = A[blockRowIndex + 1][blockColumnIndex + 1];

            l_value_2 = A[blockRowIndex + 2][blockColumnIndex];
            l_value_3 = A[blockRowIndex + 2][blockColumnIndex + 1];
            l_value_4 = A[blockRowIndex + 2][blockColumnIndex + 2];

            /* Section 0 */
            B[blockColumnIndex + 3][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 2] =
                    A[blockRowIndex + 2][blockColumnIndex + 3];

            /* Section 1 */
            B[blockColumnIndex + 2][blockRowIndex + 2] = l_value_4;
            B[blockColumnIndex + 2][blockRowIndex + 1] =
                    A[blockRowIndex + 1][blockColumnIndex + 2];
            B[blockColumnIndex + 2][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 2];
            /* diagonal value of A in row index 1 (not in L-shape) */
            l_value_4 = A[blockRowIndex + 1][blockColumnIndex + 1];

            /* Section 2 */
            B[blockColumnIndex + 1][blockRowIndex] =
                    A[blockRowIndex][blockColumnIndex + 1];

            //B[blockColumnIndex + 1][blockRowIndex + 1] = diagonal_value;
            B[blockColumnIndex + 1][blockRowIndex + 1] = l_value_4;

            B[blockColumnIndex + 1][blockRowIndex + 2] = l_value_3;

            /* Section 3 */
            B[blockColumnIndex][blockRowIndex + 2] = l_value_2;
            B[blockColumnIndex][blockRowIndex + 1] = l_value_1;
            B[blockColumnIndex][blockRowIndex] = l_value_0;

            /* Section 4 */
            B[blockColumnIndex][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex];
            B[blockColumnIndex + 1][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 1];
            B[blockColumnIndex + 2][blockRowIndex + 3] =
                    A[blockRowIndex + 3][blockColumnIndex + 2];

            //diagonal_value = A[blockRowIndex + 3][blockColumnIndex + 3];
            //B[blockColumnIndex + 3][blockRowIndex + 3] = diagonal_value;
            l_value_0 = A[blockRowIndex + 3][blockColumnIndex + 3];
            B[blockColumnIndex + 3][blockRowIndex + 3] = l_value_0;
        }
    }

    if (PRINT_DEBUG) {
//        printArrayA(A);
//        printArrayB(B);

        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 64 x 64 worked.\n");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 64 x 64 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
    }

//    for (i = 0; i < N; i++) {
//        for (j = 0; j < M; j++) {
//            tmp = A[i][j];
//            B[j][i] = tmp;
//        }
//    }
}

//FIXME: Remove
char trans_64_64_default_desc[] = "Optimized transpose for 64 x 64 "
                                  "default (M = 64, N = 64)";
void trans_64_64_default(int M, int N, int A[N][M], int B[M][N]) {
    int blockSize;
    int r;
    int c;
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;

    blockSize = 4;
    for (r = 0; r < N; r += blockSize) {
        for (c = 0; c < M; c += blockSize) {
/*Elements in A[r][], A[r+1][], A[r+2][] are assigned to the variables for use throughout this loop
This is becuase we are only allowed to modify the second matrix B but not the matrix A */
            v0 = A[r][c];
            v1 = A[r + 1][c];
            v2 = A[r + 2][c];
            v3 = A[r + 2][c + 1];
            v4 = A[r + 2][c + 2];
//Elements in B[c+3][] are assigned
            B[c + 3][r] = A[r][c + 3];
            B[c + 3][r + 1] = A[r + 1][c + 3];
            B[c + 3][r + 2] = A[r + 2][c + 3];
//Elements in B[c+2][] are assigned
            B[c + 2][r] = A[r][c + 2];
            B[c + 2][r + 1] = A[r + 1][c + 2];
            B[c + 2][r + 2] = v4;
            v4 = A[r + 1][c + 1];
//Elements in B[c+1][] are assigned
            B[c + 1][r] = A[r][c + 1];
            B[c + 1][r + 1] = v4;
            B[c + 1][r + 2] = v3;
//Elements in B[c][] are assigned
            B[c][r] = v0;
            B[c][r + 1] = v1;
            B[c][r + 2] = v2;
//Elements in row A[r+3][] are assigned to the left out elements in B (where B has r+3)
            B[c][r + 3] = A[r + 3][c];
            B[c + 1][r + 3] = A[r + 3][c + 1];
            B[c + 2][r + 3] = A[r + 3][c + 2];
            v0 = A[r + 3][c + 3];
//Finally, elements in row B[c+3][] are assigned
            B[c + 3][r + 3] = v0;
        }
    }
}

//FIXME: Another remove
char trans_64_64_another_default_desc[] = "Optimized transpose for 64 x 64 "
                                          "another default (M = 64, N = 64)";
void trans_64_64_another_default(int M, int N, int A[N][M], int B[M][N]) {
    int t0, t1, t2, t3, t4, t5, t6, t7;
    for (int i = 0; i < N; i += 8) {
        for (int j = 0; j < M; j += 8) {
            for (int k = i; k < i + 4; k++) {
                t0 = A[k][j];
                t1 = A[k][j + 1];
                t2 = A[k][j + 2];
                t3 = A[k][j + 3];
                t4 = A[k][j + 4];
                t5 = A[k][j + 5];
                t6 = A[k][j + 6];
                t7 = A[k][j + 7];
                B[j][k] = t0;
                B[j + 1][k] = t1;
                B[j + 2][k] = t2;
                B[j + 3][k] = t3;
                B[j + 0][k + 4] = t7;
                B[j + 1][k + 4] = t6;
                B[j + 2][k + 4] = t5;
                B[j + 3][k + 4] = t4;
            }
            for (int h = 0; h < 4; h++) {
                t0 = A[i + 4][j + 3 - h];
                t1 = A[i + 5][j + 3 - h];
                t2 = A[i + 6][j + 3 - h];
                t3 = A[i + 7][j + 3 - h];
                t4 = A[i + 4][j + 4 + h];
                t5 = A[i + 5][j + 4 + h];
                t6 = A[i + 6][j + 4 + h];
                t7 = A[i + 7][j + 4 + h];
                B[j + 4 + h][i + 0] = B[j + 3 - h][i + 4];
                B[j + 4 + h][i + 1] = B[j + 3 - h][i + 5];
                B[j + 4 + h][i + 2] = B[j + 3 - h][i + 6];
                B[j + 4 + h][i + 3] = B[j + 3 - h][i + 7];
                B[j + 3 - h][i + 4] = t0;
                B[j + 3 - h][i + 5] = t1;
                B[j + 3 - h][i + 6] = t2;
                B[j + 3 - h][i + 7] = t3;
                B[j + 4 + h][i + 4] = t4;
                B[j + 4 + h][i + 5] = t5;
                B[j + 4 + h][i + 6] = t6;
                B[j + 4 + h][i + 7] = t7;
            }
        }
    }

    if (PRINT_DEBUG) {
//        printArrayA(A);
//        printArrayB(B);

        if (is_transpose(M, N, A, B)) {
            printf("Success. Transpose of 64 x 64 worked.\n");
        } else {
            printf("ERROR...\n");
            printf("Transpose of 64 x 64 did not work...\n");
            printf("Exiting...\n");

            exit(99);
        }
    }
}

#define SEPARATOR

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
    registerTransFunction(trans_32_32, trans_32_32_desc);
    registerTransFunction(trans_61_67, trans_61_67_desc);
    registerTransFunction(trans_64_64_diag, trans_64_64_diag_desc);
    registerTransFunction(trans_64_64_L_diag, trans_64_64_L_diag_desc);
    registerTransFunction(trans_64_64_zigzag, trans_64_64_zigzig_desc);

    //FIXME: remove
    //registerTransFunction(trans_64_64_default, trans_64_64_default_desc);
    registerTransFunction(trans_64_64_another_default,
                          trans_64_64_another_default_desc);
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
