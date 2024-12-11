/*
 *     sudoku.c
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01), 
 *     2/13/22
 *     
 *     iii
 *
 *     This is the Sudoku file used to confirm if a 9x9 Sudoku solution in PGM
 *     format is valid.
 *
 */


#include <assert.h>
#include "uarray2.h"
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <stdbool.h>

/*****************************************************************
 *                  Data formatting functions
 *****************************************************************/

/**********makeSudokuArray********
 *
 * Creates and fills a UArray2 with the Sudoku PGM contents
 * Inputs: The PNM Reader reading the PNM
 * Return: A UArray2 with dimensions of 9x9 storing a solved sudoku puzzle
 * Expects:
 *      Values of the PNM to be between 1 and 9.
 *
 * Notes:
 *      Stores the contents in a column major order 
 ************************/
UArray2_T makeSudokuArray(Pnmrdr_T rdr) {

        UArray2_T sudokuArr = UArray2_new(9, 9, sizeof(unsigned));

        int currCol;
        int currRow;
        for(int i = 0; i < 81; i++) {
                currRow = i / UArray2_width(sudokuArr);
                if (currRow == 0) {
                        currCol = i;
                        
                } else {
                        currCol = (i % UArray2_width(sudokuArr));
                }
                unsigned curr = Pnmrdr_get(rdr);
                assert(curr > 0 && curr < 10);
                *((unsigned *)UArray2_at(sudokuArr, currCol, currRow)) = curr;
        }

        return sudokuArr;
}

/*****************************************************************
 *                  Data verification functions
 *****************************************************************/

/**********check_pgm********
 *
 * Verifies that the PGM header is of expected Sudoku values
 * Inputs: The PNM Reader reading the PNM
 * Return: A boolean indicating whether the PGM has the expected header
 * Expects:
 *      Width and Height to be 9 and 9 respectively
 *              Maxval to be 9
 *
 * Notes:
 *
 ************************/
bool check_pgm(Pnmrdr_T rdr){
        
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);

        if (data.width == 9 && data.height == 9 && data.denominator == 9){
                return true;
        } else {
                return false;
        }
}



/**********verifyLines********
 *
 * Goes through all columns or rows and makes sure that no rows have the same
 * character
 * Inputs: UArray2 and bool for whether to check file in row or column major
 * Return: none
 * Expects:
 *      Values of the PNM to be between 1 and 9
 *
 * Notes:
 *      creates a set by using a simple integer array, thena adds to an index
 *      when that number is found in the row/col 
 *              true is col major, false is row major
 ************************/
void verifyLines (UArray2_T array, bool major) {
        assert(array);
        unsigned curr;
        int set[10];
        for (int i = 0; i < 11; i++) {
                        set[i] = 0;
                }
        for (int i = 0; i < 9; i++) {

                for (int j = 0; j < 9; j++) {
                        if (major) {
                                curr = *((unsigned *)UArray2_at(array, i, j));
                        }
                        else {
                                curr = *((unsigned *)UArray2_at(array, j, i));
                        }
                        set[curr]++;

                        /*Duplicate has been found in a row/col, this Sudoku
                                solution is wrong.*/
                        if (set[curr] > 1) {
                                exit(1);
                        }
                }
                /*Clear out the set after checking each row/col*/
                for (int i = 0; i < 11; i++) {
                        
                        set[i] = 0;
                }
        }
}

/**********verifySquares********
 *
 * Goes through all 9 squares of the Sudoku and verifies that each square has
 * all 9 unique values
 * Inputs: UArray2 storing the Sudoku puzzle
 * Return:
 * Expects:
 *      Values of the PNM to be between 1 and 9.
 *
 * Notes:
 *
 ************************/
void verifySquares (UArray2_T array) {

        assert(array);
        unsigned curr;

        /*Initializing the set to count occurences of each digit in a square*/
        int set[10];
        for (int i = 0; i < 11; i++) {
                set[i] = 0;
        }
        /*The x and y loops represent the current square
                (x and y loop from 1 to 3, representing all 9 squares)*/
        for (int x = 1; x < 4; x++) {
                for (int y = 1; y < 4; y++) {
                        /*The i and j loops represent the current value in
                                the current square (i and j loop from 3*x - 3 
                                and 3*y - 3 respectively, representing all the 
                                square indices for their particular square)*/
                        for (int i = 3*x - 3; i < 3*x; i++) {
                                for (int j = 3*y - 3; j < 3*y; j++) {
                                        
                                        curr = *((unsigned *)UArray2_at(array, 
                                                                        i, j));
                                        set[curr] += 1;

                                        if (set[curr] > 1) {
                                                exit(1);
                                        }
                                }
                        }
                        /*Clear out the set after checking each square*/
                        for (int i = 0; i < 11; i++) {
                                set[i] = 0;
                        }
                }
        }

}

/*****************************************************************
 *                  Main function
 *****************************************************************/
int main (int argc, char *argv[]) {
        FILE *fp = NULL;
        Except_T wrong = {"Too many arguments\n"};
        Except_T cantopen = {"Can't open file\n"};
        
        if (argc == 1) {
                fp = stdin;
        }
        else if (argc == 2) {
                fp = fopen(argv[1], "r");
        }
        else {
                RAISE(wrong);
        }

        if (fp == NULL) {
                RAISE(cantopen);
        }

        Pnmrdr_T rdr = Pnmrdr_new(fp);
        assert(check_pgm(rdr));
        UArray2_T sudokuArr = makeSudokuArray(rdr);
        assert(sudokuArr);

        
        /*Calling the verify row/col/squares. If none of these cause exit 1, 
          the solution is valid*/
        verifyLines(sudokuArr, true);
        verifyLines(sudokuArr, false);
        verifySquares(sudokuArr);

        UArray2_free(&sudokuArr);
        Pnmrdr_free(&rdr);
        fclose(fp);
        exit(0);
}