#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/database.h"
#include "../inc/shared/csv_reader_c.h"

Database* database;
const int movie_max = 10000;
int movie_count = 0;

void callback(const MovieInfo* movieInfo)
{
    if (movie_count <= movie_max)
    {
        database_add_movie(database, movieInfo);

        movie_count++;
    }
}

int main()
{
    const char* path = "./data/netflix_titles2.csv";

    database = database_create();

    read_csv(path, callback);

    MovieEntry* a = database_find_movie(database, "#realityhigh");
    CastEntry* c = a->casts[0];

    printf("%s", c->name);

    database_drop(database);
}