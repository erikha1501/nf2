#include "../inc/app.h"
#include "../inc/shared/csv_reader_c.h"

#include <stdlib.h>
#include <assert.h>

App* app_create()
{
    return malloc(sizeof(App));
}

void app_drop(App* app)
{
    database_drop(app->database);
    jrb_free_tree(app->watched_list);
    free(app);
}


void callback(const MovieInfo* movieInfo, void* opaque)
{
    Database* database = (Database*)opaque;
    database_add_movie(database, movieInfo);
}

void app_init(App* app)
{
    app->database = database_create();
    
    const char* path = "./data/netflix_titles2.csv";
    read_csv(path, callback, app->database);

    printf("Generating similarity index...\n");
    database_generate_similarity_map(app->database);
    printf("Done.\n");

    app->watched_list = make_jrb();
}

void app_add_to_watched_list(App* app, int internalMovieId)
{
    if (internalMovieId >= 0 && internalMovieId < app->database->movies_count)
    {
        JRB watched_movie = NULL;

        if ((watched_movie = jrb_find_int(app->watched_list, internalMovieId)) != NULL)
        {
            printf("You have already watched this movie");
            return;
        }

        jrb_insert_int(app->watched_list, internalMovieId, new_jval_i(0));
    }
    else
    {
        printf("Invalid movie id\n");
    }
}

void recommended_movies_add(JRB similarMovieTree, const SimilarityList* similarMovies, int* similarCount)
{
    for (int i = 0; i < similarMovies->list_size; i++)
    {
        const SimilarityEntry* entry = &similarMovies->list[i];

        JRB score_node = NULL;
        if ((score_node = jrb_find_int(similarMovieTree, entry->movie->internal_id)) == NULL)
        {
            score_node = jrb_insert_int(similarMovieTree, entry->movie->internal_id, new_jval_i(0));

            *similarCount += 1;
        }

        score_node->val.i += entry->score;
    }
}

extern JRB jval_as_jrb(Jval jval);

extern int jval_score_comparer(Jval* a, Jval* b);

Dllist app_recommend_movies(const App* app)
{
    if (jrb_empty(app->watched_list))
    {
        printf("You haven't watched any movies yet\n");
        return NULL;
    }

    int similar_movies_count = 0;
    JRB similar_movie_tree = make_jrb();

    // Merge lists of similar movies
    JRB watched_movie = NULL;
    jrb_traverse(watched_movie, app->watched_list)
    {
        int watched_movie_id = watched_movie->key.i;
        const SimilarityList* similarity_list = database_get_similarity_list(app->database, watched_movie_id);

        recommended_movies_add(similar_movie_tree, similarity_list, &similar_movies_count);
    }


    // Populate a list of pointers into each score_node
    Jval* similar_movies_array = malloc(sizeof(Jval) * similar_movies_count);
    int similar_movie_index = 0;

    JRB score_node = NULL;
    jrb_traverse(score_node, similar_movie_tree)
    {
        similar_movies_array[similar_movie_index++] = new_jval_v(score_node);
    }

    assert(similar_movie_index == similar_movies_count);

    // Sort list of pointers
    qsort_jval(similar_movies_array, 0, similar_movie_index - 1, jval_score_comparer);

    // Extract top 10 movies with highest score
    Dllist result = new_dllist();
    similar_movies_count = similar_movies_count < 10 ? similar_movies_count : 10;

    for (int i = 0; i < similar_movies_count; i++)
    {
        JRB score_node = jval_as_jrb(similar_movies_array[i]);
        dll_append(result, new_jval_v(&app->database->movies[score_node->key.i]));
    }

    free(similar_movies_array);
    jrb_free_tree(similar_movie_tree);

    return result;
}