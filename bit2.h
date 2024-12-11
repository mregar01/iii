/*
 *     bit2.h
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01), 
 *     2/13/22
 *     
 *     iii
 *
 *     This is the header file for our Bit2 interface.
 *
 */


#ifndef BIT2_INCLUDE
#define BIT2_INCLUDED

typedef struct Bit2_T *Bit2_T;

/**********Bit2_new********
 *
 * Creates and returns a Bit2
 * Inputs: number of columns and number of rows
 * Return: A Bit2 of the designated dimensions to store bits
 * 
 * Expects:
 *
 *      Width and height to accurately reflect the dimensions of the 
 *      desired Bit2 and that they are greater than 0.
 *              Total number of elements stored in Bit2 doesn't 
 *              exceed width*height
 *
 * Notes:
 *
 ************************/
Bit2_T Bit2_new (int width, int height);

/**********Bit2_width********
 *
 * Find and return the width of a Bit2
 * Inputs: Bit2 to retrive width from
 * Return: Width of Bit2 as an int
 * Expects: Bit2 to be nonnull 
 *
 * Notes: Will throw a checked runtime error if the Bit2 is empty
 *
 ************************/
int Bit2_width(Bit2_T Bit2);

/**********Bit2_height********
 *
 * Find and return the height of a Bit2
 * Inputs: Bit2 to retrive height from
 * Return: Height of Bit2 as an int
 * Expects: Bit2 to be nonnull 
 *
 * Notes: 
 *
 ************************/
int Bit2_height(Bit2_T Bit2);

/**********Bit2_get********
 *
 * Finds the bit stored at the given col/row
 * Inputs: The Bit2, the col, and the row of the bit to be found
 * Return: The value of the bit at the given row/col
 * Expects
 *      The col/row parameters to be between 0 and the width/height of the
 *      Bit2 - 1.
 *              A nonnull Bit2
 *
 * Notes:
 *
 ************************/
int Bit2_get(Bit2_T Bit2, int col, int row);


/**********Bit2_put********
 *
 * places a binary value into a slot of the Bit2 and returns the value 
 * previously stored in the slot
 * Inputs: The Bit2 storing the elements, col and row to put element at, value
 *         of the element
 * Return: value previously in the slot 
 * Expects: 
 *      A nonnull Bit2
 *              in bounds col and row
 *      value either 0 or 1
 * Notes:
 *
 ************************/
int Bit2_put(Bit2_T Bit2, int col, int row, int val);


/**********Bit2_map_col_major********
 *
 * Applies a function onto the elements one by one in order of col major
 * Inputs: The Bit2 storing the elements, the function to apply, and a void
 *      pointer indicating the closure of the apply function
 * Return: nothing
 * Expects: 
 *      A nonnull Bit2
 *              Working apply function
 * Notes:
 *
 ************************/
void Bit2_map_col_major(Bit2_T Bit2, 
        void apply(int col, int row, Bit2_T Bit2, int curr, void *cl), 
                void *cl);


/**********Bit2_map_row_major********
 *
 * Applies a function onto the elements one by one in order of row major
 * Inputs: The Bit2 storing the elements, the function to apply, and a void
 *      pointer indicating the closure of the apply function
 * Return: nothing
 * Expects: 
 *      A nonnull Bit2
 *              Working apply function
 * Notes:
 *
 ************************/
void Bit2_map_row_major(Bit2_T Bit2, 
        void apply(int col, int row, Bit2_T Bit2, int curr, void *cl), 
                void *cl);


/**********Bit2_free********
 *
 * Frees up all space allocated by a Bit2
 * Inputs: pointer to an instance of a Bit2
 * Return: nothing
 * Expects: Bit2 to be nonnull and not freed already
 *
 * Notes: none
 *
 ************************/
void Bit2_free(Bit2_T *Bit2);

#undef Bit2_T
#endif