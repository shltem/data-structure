#include <time.h>/*time*/
#include <stdlib.h>/*qsort*/
#include <assert.h>/*assert*/
#include <stdio.h>/*printf*/
#include "knights_tour.h"
#include "bitarray.h"
#define FIVE_MINUTS (300)
#define INDEX_TABLE (64)
#define POSSIBLE_MOVE (8)
#define SUCCSESS (0)
#define FAIL (-1)
#define POSITION_TO_INDEX(x, y) ((y * POSSIBLE_MOVE + x))



typedef struct index
{
    int move;
    int valid_options;    
}index_t;

/*===============================function decleration ========================*/

static int KnightsTourRec(int position, unsigned char *path, bitarray_t visted_index, time_t start_time);
static int KnightsTourHeristicRec(int position, unsigned char *path, bitarray_t visted_index);
static void InitLutTable();
static int IndexPositionHelper(int index, int x_add, int y_add);
static void InitCountPossibleMoves(int index, bitarray_t visted_index, index_t *heuristic_moves);
int ComperMove(const void *data1, const void *data2);

/*============================================================================*/

/*========================== hash table ======================================*/

int possible_moves[INDEX_TABLE][POSSIBLE_MOVE];

/*============================================================================*/

/*================= knights tour function =====================================*/

int KnightsTour(unsigned char x_pos, unsigned y_pos, unsigned char *path)
{
    int status = SUCCSESS;    
    bitarray_t visted_index = 0;
    time_t start_time = 0;
    int position = POSITION_TO_INDEX(x_pos, y_pos);
    
    assert(NULL != path);

    InitLutTable();
    path[0] = position;
    visted_index = BitArraySetBit(visted_index, position, 1);
    start_time = time(NULL);
    status = KnightsTourRec(position, path, visted_index, start_time);

    return status;
}

int WarnsdorffKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path)
{
    int status = SUCCSESS; 
    bitarray_t visted_index = 0;
    int position = POSITION_TO_INDEX(x_pos, y_pos);
    assert(NULL != path);
    
    InitLutTable();
    
    path[0] = position;
    visted_index = BitArraySetBit(visted_index, position, 1);
    status = KnightsTourHeristicRec(position, path, visted_index);
    
    return status;
}

static int KnightsTourRec(int position, unsigned char *path, bitarray_t visted_index, time_t start_time)
{
    size_t counter = 0;
    int status = SUCCSESS;
    int num_move = 0;
    int current_index = 0;
   
    counter = BitArrayCountOn(visted_index);
    
    if(time(NULL) - start_time > FIVE_MINUTS)
    {
        printf("Time's up\n");
        return FAIL;
    }
    
    if(INDEX_TABLE == counter)
    {
        return SUCCSESS;
    }

    for(num_move = 0; num_move < POSSIBLE_MOVE; ++num_move)
    {
        current_index = possible_moves[position][num_move];
        if((FAIL != current_index) && (0 == BitArrayGetVal(visted_index, current_index)))
        {
            path[counter] = current_index;
            position = current_index;
            visted_index = BitArraySetBit(visted_index, position, 1);
            status =  KnightsTourRec(position, path, visted_index, start_time);    
            if(FAIL == status)
            {
                path[counter] = 0;
                visted_index = BitArraySetOff(visted_index, position);
            }
            else
            {
                return status;
            } 
        } 
    }

    return FAIL;  
}

static int KnightsTourHeristicRec(int position, unsigned char *path, bitarray_t visted_index)
{
    size_t counter = 0;
    int status = 0;
    int num_move = 0;
    int current_index = 0;
    index_t heuristic_moves[8] = {0};
    counter = BitArrayCountOn(visted_index);
    
    if(INDEX_TABLE == counter)
    {
        return SUCCSESS;
    }
    
    InitCountPossibleMoves(position, visted_index, heuristic_moves);
    qsort(heuristic_moves, POSSIBLE_MOVE, sizeof(index_t), ComperMove);
    
    for(num_move = 0; num_move < POSSIBLE_MOVE; ++num_move)
    {
        current_index = possible_moves[position][heuristic_moves[num_move].move];
        if((FAIL != current_index) && (0 == BitArrayGetVal(visted_index, current_index)))
        {
            path[counter] = current_index;
            position = current_index;
            visted_index = BitArraySetBit(visted_index, position, 1);
            status =  KnightsTourHeristicRec(position, path, visted_index);
            if(FAIL == status)
            {
                path[counter] = 0;
                visted_index = BitArraySetOff(visted_index, position);
            }
            else
            {
                return status;
            }
        }
    }

    return FAIL;  
}

/*============================================================================*/


/*========================= Init functions and tables ========================*/


static void InitLutTable()
{
    int index = 0;

    for(index = 0; index < INDEX_TABLE; ++index)
    {
        possible_moves[index][0] = IndexPositionHelper(index, +1, +2);
        possible_moves[index][1] = IndexPositionHelper(index, +1, -2);
        possible_moves[index][2] = IndexPositionHelper(index, -1, +2);
        possible_moves[index][3] = IndexPositionHelper(index, -1, -2);
        possible_moves[index][4] = IndexPositionHelper(index, +2, -1);
        possible_moves[index][5] = IndexPositionHelper(index, +2, +1);
        possible_moves[index][6] = IndexPositionHelper(index, -2, +1);
        possible_moves[index][7] = IndexPositionHelper(index, -2, -1);
    }

}

static int IndexPositionHelper(int index, int x_add, int y_add)
{ 
    int return_value = 0;
    int x = index % POSSIBLE_MOVE;
    int y = index / POSSIBLE_MOVE;
  
   if(((x + x_add) >= 0) && ((x + x_add) < 8) && ((y + y_add) >= 0) && ((y + y_add) < 8))
   {
        return_value =  (y + y_add) * 8 + (x + x_add);
   }
   else
   {
        return_value = FAIL;
   }

   return return_value;
}

static void InitCountPossibleMoves(int index, bitarray_t visted_index, index_t *heuristic_moves)
{
    int i = 0;
    int j = 0;
    int current_mov_index = 0;
    int current_check_index = 0;

    for(i = 0; i < POSSIBLE_MOVE; ++i)
    {
        heuristic_moves[i].move = i;
        current_mov_index = possible_moves[index][i];

        if((FAIL == current_mov_index || 0 != BitArrayGetVal(visted_index, current_mov_index)))
        {
            heuristic_moves[i].valid_options = POSSIBLE_MOVE;
            continue;
        }
        for(j = 0; j < POSSIBLE_MOVE; ++j)
        {    
            current_check_index = possible_moves[current_mov_index][j];

            if(FAIL != current_check_index && 0 == BitArrayGetVal(visted_index, current_check_index))
            {
                ++heuristic_moves[i].valid_options;
            }
        }    
    }

}

/*============================================================================*/

int ComperMove(const void *data1, const void *data2)
{
    return (((index_t *)data1)->valid_options - ((index_t *)data2)->valid_options);
}

/*============================================================================*/

