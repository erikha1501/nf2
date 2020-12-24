#define _CRT_SECURE_NO_WARNINGS

#include "csv_reader_c.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/jrb.h"

JRB 

void callback(const MovieInfo* movieInfo)
{
    char buf[50];

    printf("%d\n", movieInfo->show_id);

    //for (int i = 0; i < movieInfo->cast_count; i++)
    //{
    //    strncpy(buf, movieInfo->casts[i].str, movieInfo->casts[i].size);
    //    buf[movieInfo->casts[i].size + 1] = '\0';
    //    printf("%s, ", buf);
    //}
    printf("%ld\n", movieInfo->genreBitFields);

    printf("\n");
}

int main()
{
    read_csv(NULL, callback);
}