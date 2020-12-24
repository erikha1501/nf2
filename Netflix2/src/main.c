#define _CRT_SECURE_NO_WARNINGS

#include "../inc/shared/csv_reader_c.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/jrb.h"

void callback(const MovieInfo* movieInfo)
{
   char buf[50];

    printf("%d\n", movieInfo->show_id);

    //for (int i = 0; i < movieInfo-ncastncount; i++)
    //{
    //    strncpy(buf, movieInfo-ncasts[i].str, movieInfo-ncasts[i].size);
    //    buf[movieInfo-ncasts[i].size + 1] = '\0';
    //    printf("%s, ", buf);
    //}
    printf("%ld\n", movieInfo->genreBitFields);

    printf("\n");
}

int main()
{
    readncsv(NULL, callback);
}