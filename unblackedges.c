/*
 *     unblackedges.c
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01), 
 *     2/13/22
 *     
 *     iii
 *
 *     This is the Unblackedges file used to remove all black edge pixels and 
 *     their neighbors from a PBM image.
 *
 */


#include <assert.h>
#include "bit2.h"
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <stdbool.h>
#include "stack.h"

/*****************************************************************
 *                  Data formatting functions
 *****************************************************************/

/**********print_pbm_map********
 *
 * apply function for mapping in row major, prints out contents
 * Inputs: col, row, Bit2_T array, curr value int, cl pointer
 * Return: none
 * 
 * Expects:
 *      *p1 to be the end of the row
 * Notes:
 *      
 ************************/
void print_pbm_map(int i, int j, Bit2_T a, int b, void *p1) {
        (void)j;
        (void)a;
        if (i % 70 == 0 || i == *((int *)p1) ) {
                printf("\n");
        }
        printf("%d", b);
}

/**********print_pbm********
 *
 * function that prints out the pbm
 * Inputs: Pnmrdr variable and a Bit2 array of the original pbm
 * Return: none
 * 
 * Expects:
 *      opened Pnmrdr, filled Bit2 array
 * Notes:
 *      calls map row major to print everything out
 *      throws CRE if rdr or unblacked are null
 ************************/
void print_pbm(Pnmrdr_T rdr, Bit2_T unblacked) {
        
        assert(rdr);
        assert(unblacked);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        
        printf("P1\n%u %u", data.width, data.height);
        int x = Bit2_width(unblacked);
        void *y = &x;
        Bit2_map_row_major(unblacked, print_pbm_map, y);
}

/**********makeUnblackArray********
 *
 * fills a Bit2 array with the contents of a pbm
 * Inputs: an open Pnmrdr variable to get contents from
 * Return: A Bit2_T
 * 
 * Expects:
 *      opened Pnmrdr
 * Notes:
 *      stores elements in row major
 *      throws CRE if rdr is null
 ************************/
Bit2_T makeUnblackArray(Pnmrdr_T rdr) {

        assert(rdr);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        Bit2_T unblackArray = Bit2_new(data.width, data.height);

        int currCol;
        int currRow;
        int curr;
        int length = data.width * data.height;

        /*For loop sets the row and col indices for each bit read by the 
          PNMRDR and adds them to the Bit2*/
        for(int i = 0; i < length; i++) {
                currRow = i / data.width;
                if (currRow == 0) {
                        currCol = i;
                } else {
                        currCol = (i % data.width);
                }
                curr = Pnmrdr_get(rdr);
                assert(curr == 0 || curr == 1);
                Bit2_put(unblackArray, currCol, currRow, curr);
        }
        return unblackArray;
}

/**********checkandAdd********
 *
 * Checks if the neighbor is a valid index and if neighbor is black , 
 * neighbor is a black edge and is added to the stack
 * Return: none
 * 
 * Expects:
 *     int col/row and valid array and valid stack (which is asserted in the
 *     function that calls this helper)
 * Notes:
 *     Just a helper function for processBit to remove a repetitive process
 ************************/
void checkandAdd (int col, int row, Bit2_T array, Stack_T stack) {
        int width = Bit2_width(array);
        int height = Bit2_height(array);

        if (col < width && col >= 0) {
                if (row  < height && row > 0) {
                        if (Bit2_get(array, col, row) == 1) {
                                int *n = malloc(2 * sizeof(*n));
                                assert(n);
                                n[0] = col; n[1] = row;
                                void* nptr = n;
                                Stack_push(stack, nptr);
                        }
                }
        }
}

/**********processBit********
 *
 * given a col and row check all neighbors to see if they contain black pixels
 * Inputs: col, row, Bit2_T array, Stack_T stack
 * Return: none
 * 
 * Expects:
 *     nonnull Bit2 array and a stack
 * Notes:
 *     checks all neighbors if the supplied pixel is black 
 *     
 *     if neighbors dimensions are inbouds, mallocs a two integer array to hold
 *     the row and the collumn
 *
 *     throws CRE if row or cullumn is out of bounds
 *      
 ************************/
void processBit (int col, int row, Bit2_T array, Stack_T stack) {
        
        int width = Bit2_width(array);
        int height = Bit2_height(array);
        assert(col >= 0 && col < width);
        assert(row >= 0 && row < height);
        int val = Bit2_get(array, col, row);

        /*If bit is black, change bit to white and check neighbors*/
        if (val == 1) {
                Bit2_put(array, col, row, 0);
                checkandAdd(col + 1, row, array, stack);
                checkandAdd(col - 1, row, array, stack);
                checkandAdd(col, row + 1, array, stack);
                checkandAdd(col, row - 1, array, stack);
        }
}


/**********unblacktheArray********
 *
 * removes black edges from a pb
 * Inputs: Bit2 arr
 * Return: none
 * 
 * Expects:
 *     nonnull Bit2 array
 * Notes:
 *     First checks 4 outer edges, then checks neighbors and calls upon
 *     processBit
 *     
 * Expects:
 *      nonnull Bit2_T array
 *
 *      throws CRE if stack is null
 *      
 ************************/
void unblacktheArray(Bit2_T array) {
        
        int height = Bit2_height(array);
        int width = Bit2_width(array);

        Stack_T currentedges;
        currentedges = Stack_new();
        assert(currentedges);

        /*Looping over first/last column edges*/
        for (int i = 0; i < height; i++) {
                processBit (0, i, array, currentedges);
                processBit (width - 1, i, array, currentedges);
        }
        /*Looping over first/last row edges*/
        for (int i = 0; i < width; i++) {
                processBit (i, 0, array, currentedges);
                processBit (i, height - 1, array, currentedges);
        }

        /*Looping over neighbors until no more black edges. Similar to how
          DFS works */
        while (Stack_empty(currentedges) == 0) {
                void* elemptr = Stack_pop(currentedges);
                int* currBit = (int*) elemptr;
                processBit(currBit[0], currBit[1], array, currentedges);
                free((int*) elemptr);
        }
        Stack_free(&currentedges);
}

/*****************************************************************
 *                  Data verification functions
 *****************************************************************/

/**********check_pbm********
 *
 * makes sure that we have taken in a valid pbm
 * Inputs: Pnmrdr variable 
 * Return: true if pbm is valid, false if not
 * 
 * Expects:
 *      opened Pnmrdr
 * Notes:
 *      
 ************************/
bool check_pbm(Pnmrdr_T rdr){
        assert(rdr);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);

        if (data.width > 0 && data.height > 0 && data.denominator < 2){
                return true;
        } else {
                return false;
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
        assert(check_pbm(rdr));

        Bit2_T imageArr = makeUnblackArray(rdr);
        assert(imageArr);

        unblacktheArray(imageArr);
        print_pbm(rdr, imageArr);

        Bit2_free(&imageArr);
        Pnmrdr_free(&rdr);
        fclose(fp);
        exit(0);
}