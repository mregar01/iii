/*
 *     bit2.c
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01), 
 *     2/13/22
 *     
 *     iii
 *
 *     This is the implementation file for our Bit2 interface.
 *
 */
#include <bit.h>
#include <assert.h>
#include "bit2.h"
#include <stdlib.h>
#include <stdio.h>

struct Bit2_T {
        int width; /* The number of rows to represent the elems, at least 1*/
        int height; /* The number of cols to represent the elems, at least 1*/
        Bit_T elems; /* 1D UArray to store the elems of 2D implementation */
};


Bit2_T Bit2_new (int width, int height) {
        int length = width * height;
        assert(width > 0);
        assert(height > 0);
        
        Bit_T ourArray = Bit_new(length);
        Bit2_T Bit2 = malloc(sizeof(struct Bit2_T));
        assert(Bit2);

        Bit2->width = width;
        Bit2->height = height;
        Bit2->elems = ourArray;
        return Bit2;
}

int Bit2_width(Bit2_T Bit2) {
        assert(Bit2);
        return Bit2->width;
}

int Bit2_height(Bit2_T Bit2) {
        assert(Bit2);
        return Bit2->height;
}

int Bit2_get(Bit2_T Bit2, int col, int row) {
        assert(Bit2);
        assert(col < Bit2_width(Bit2));
        assert(row < Bit2_height(Bit2));
        assert(col > -1 && row > -1);
        int index = (Bit2->width)*row + col;

        return Bit_get(Bit2->elems, index);
}

int Bit2_put(Bit2_T Bit2, int col, int row, int val) {
        assert(Bit2);
        assert(col < Bit2_width(Bit2));
        assert(row < Bit2_height(Bit2));
        assert(col > -1 && row > -1);
        assert(val == 0 || val == 1);
        int index = (Bit2->width)*row + col;
        int popped = Bit2_get(Bit2, col, row);
        Bit_put(Bit2->elems, index, val);
        return popped;
}

void Bit2_map_col_major(Bit2_T Bit2, 
void apply(int col, int row, Bit2_T Bit2, int curr, void *cl), void *cl) {
        
        assert(apply);
        assert(Bit2);
        assert(cl);
        int height = Bit2_height(Bit2);

        int currCol;
        int currRow;

        int length = Bit_length(Bit2->elems);
        for(int i = 0; i < length; i++) {
                if (height == 0){
                        currCol = 0;
                } else {
                        currCol = i / height;
                }
                if (((height) * currCol) == 0){
                        currRow = i;
                } else {
                        currRow = i % (height * currCol);
                }
                int currval = Bit2_get(Bit2, currCol, currRow);
                apply(currCol, currRow, Bit2, currval, cl);
        }
}

void Bit2_map_row_major(Bit2_T Bit2, 
void apply(int col, int row, Bit2_T Bit2, int curr, void *cl), void *cl) {
        
        assert(apply);
        assert(Bit2);
        assert(cl);

        int width = Bit2_width(Bit2);
        int height = Bit2_height(Bit2);

        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        int currval = Bit2_get(Bit2, j, i);
                        apply(j, i, Bit2, currval, cl);
                }
        }

}

void Bit2_free(Bit2_T *Bit2) {
        assert(Bit2 && *Bit2);

        Bit_free(&(*Bit2)->elems);
        free(*Bit2);
}
