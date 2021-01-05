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


    const MovieEntry* m = database_find_movie_g(database, "#reality");

    database_generate_similarity_map_1(database, m->internal_id);

    JRB s_list = database_get_similarity_list(database, m->internal_id);

    printf("%s\n", m->title);

    if (s_list != NULL)
    {
        JRB score_node;
        jrb_traverse(score_node, s_list)
        {
            const MovieEntry* movie = &database->movies[score_node->key.i];
            printf("Movie: %s - score: %d\n", movie->title, score_node->val.i);
        }
    }


    database_drop(database);
}