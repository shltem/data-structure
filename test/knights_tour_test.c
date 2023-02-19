#include <stdio.h>
#include "knights_tour.h"

int main()
{
    unsigned char path[100] = {0};
    int index = 0;
    printf("KnightsTour STATUS: %d\n",KnightsTour(2,3, path));

    for(index = 0; index < 64; ++index)
    {
        printf("position at index %d = %d\n",index, path[index]);
    }    

    printf("WarnsdorffKnights STATUS: %d\n", WarnsdorffKnightsTour(3,3, path));
    
    for(index = 0; index < 64; ++index)
    {
        printf("position at index %d = %d\n",index, path[index]);
    }   

    return 0;
}