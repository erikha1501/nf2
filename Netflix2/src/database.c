#define _CRT_SECURE_NO_WARNINGS

#include "../inc/database.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

GenreEntry* database_find_genre_entry(Database* database, enum Genre genre);

DirectorEntry* database_add_director_entry(Database* database, const c_string_view name);
CastEntry* database_add_cast_entry(Database* database, const c_string_view name);
MovieEntry* database_add_movie_entry(Database* database, const MovieInfo* movieInfo);

DirectorEntry* database_insert_director(Database* database, const c_string_view name);
CastEntry* database_insert_cast(Database* database, const c_string_view name);
MovieEntry* database_insert_movie(Database* database, const MovieInfo* movieInfo);

void database_init(Database* database)
{
    // Movies
    database->movies_capacity = 6300;
    database->movies_count = 0;
    database->movies = (MovieEntry*)malloc(sizeof(MovieEntry) * database->movies_capacity);

    // Directors
    database->directors_capacity = 8000;
    database->directors_count = 0;
    database->directors = (DirectorEntry*)malloc(sizeof(DirectorEntry) * database->directors_capacity);

    // Casts
    database->casts_capacity = 50000;
    database->casts_count = 0;
    database->casts = (CastEntry*)malloc(sizeof(CastEntry) * database->casts_capacity);

    // Genres
    database->genres_capacity = 35;
    database->genres_count = 0;
    database->genres = (GenreEntry*)malloc(sizeof(GenreEntry) * database->genres_capacity);

    // Populate genres list
    database->genres[0] = (GenreEntry){.internal_id = 0, .name = "SciFi", .movies = new_dllist()};
    database->genres[1] = (GenreEntry){.internal_id = 1, .name = "Kids", .movies = new_dllist()};
    database->genres[2] = (GenreEntry){.internal_id = 2, .name = "Comedies", .movies = new_dllist()};
    database->genres[3] = (GenreEntry){.internal_id = 3, .name = "Standup", .movies = new_dllist()};
    database->genres[4] = (GenreEntry){.internal_id = 4, .name = "Fantasy", .movies = new_dllist()};
    database->genres[5] = (GenreEntry){.internal_id = 5, .name = "Crime", .movies = new_dllist()};
    database->genres[6] = (GenreEntry){.internal_id = 6, .name = "Spanish", .movies = new_dllist()};
    database->genres[7] = (GenreEntry){.internal_id = 7, .name = "International", .movies = new_dllist()};
    database->genres[8] = (GenreEntry){.internal_id = 8, .name = "Thrillers", .movies = new_dllist()};
    database->genres[9] = (GenreEntry){.internal_id = 9, .name = "Comedy", .movies = new_dllist()};
    database->genres[10] = (GenreEntry){.internal_id = 10, .name = "Docuseries", .movies = new_dllist()};
    database->genres[11] = (GenreEntry){.internal_id = 11, .name = "Science", .movies = new_dllist()};
    database->genres[12] = (GenreEntry){.internal_id = 12, .name = "Nature", .movies = new_dllist()};
    database->genres[13] = (GenreEntry){.internal_id = 13, .name = "Action", .movies = new_dllist()};
    database->genres[14] = (GenreEntry){.internal_id = 14, .name = "Adventure", .movies = new_dllist()};
    database->genres[15] = (GenreEntry){.internal_id = 15, .name = "Dramas", .movies = new_dllist()};
    database->genres[16] = (GenreEntry){.internal_id = 16, .name = "Cult", .movies = new_dllist()};
    database->genres[17] = (GenreEntry){.internal_id = 17, .name = "Indie", .movies = new_dllist()};
    database->genres[18] = (GenreEntry){.internal_id = 18, .name = "Romantic", .movies = new_dllist()};
    database->genres[19] = (GenreEntry){.internal_id = 19, .name = "Documentaries", .movies = new_dllist()};
    database->genres[20] = (GenreEntry){.internal_id = 20, .name = "Horror", .movies = new_dllist()};
    database->genres[21] = (GenreEntry){.internal_id = 21, .name = "Mysteries", .movies = new_dllist()};
    database->genres[22] = (GenreEntry){.internal_id = 22, .name = "British", .movies = new_dllist()};
    database->genres[23] = (GenreEntry){.internal_id = 23, .name = "Movies", .movies = new_dllist()};
    database->genres[24] = (GenreEntry){.internal_id = 24, .name = "Music", .movies = new_dllist()};
    database->genres[25] = (GenreEntry){.internal_id = 25, .name = "Reality", .movies = new_dllist()};
    database->genres[26] = (GenreEntry){.internal_id = 26, .name = "Anime", .movies = new_dllist()};
    database->genres[27] = (GenreEntry){.internal_id = 27, .name = "Teen", .movies = new_dllist()};
    database->genres[28] = (GenreEntry){.internal_id = 28, .name = "Sports", .movies = new_dllist()};
    database->genres[29] = (GenreEntry){.internal_id = 29, .name = "Spirituality", .movies = new_dllist()};
    database->genres[30] = (GenreEntry){.internal_id = 30, .name = "Korean", .movies = new_dllist()};
    database->genres[31] = (GenreEntry){.internal_id = 31, .name = "LGBTQ", .movies = new_dllist()};
    database->genres[32] = (GenreEntry){.internal_id = 32, .name = "Classic", .movies = new_dllist()};
    database->genres_count = 33;

    // Initialize lookup trees
    database->movies_name_lookup = make_jrb();
    database->directors_name_lookup = make_jrb();
    database->casts_name_lookup = make_jrb();
    database->genres_name_lookup = make_jrb();

    // Populate genres lookup
    for (int i = 0; i < database->genres_count; i++)
    {
        jrb_insert_str(database->genres_name_lookup, database->genres[i].name, new_jval_v(database->genres + i));
    }

    // Initialize movies similarity map
    database->movies_similarity_map = NULL;
}

const char* string_view_dup(const c_string_view str)
{
    char* buf = (const char*)malloc(sizeof(char) * (str.size + 1));
    buf[str.size] = '\0';
    strncpy(buf, str.str, str.size);

    return buf;
}

const void string_view_copy(char* dest, int destSize, const c_string_view src)
{
    assert(src.size < destSize);
    dest[src.size] = '\0';
    strncpy(dest, src.str, src.size);
}

GenreEntry* jrb_as_genre(JRB node)
{
    if (node == NULL)
    {
        return NULL;
    }
    return (GenreEntry*)(node->val.v);
}
DirectorEntry* jrb_as_director(JRB node)
{
    if (node == NULL)
    {
        return NULL;
    }
    return (DirectorEntry*)(node->val.v);
}
CastEntry* jrb_as_cast(JRB node)
{
    if (node == NULL)
    {
        return NULL;
    }
    return (CastEntry*)(node->val.v);
}
MovieEntry* jrb_as_movie(JRB node)
{
    if (node == NULL)
    {
        return NULL;
    }
    return (MovieEntry*)(node->val.v);
}

GenreEntry* database_find_genre_entry(Database* database, enum Genre genre)
{
    // Genres list is already initialized with all genres.
    return &database->genres[genre];
}

DirectorEntry* database_add_director_entry(Database* database, const c_string_view name)
{
    char name_buf[256];
    string_view_copy(name_buf, sizeof(name_buf) / sizeof(char), name);

    DirectorEntry* directorEntry = jrb_as_director(jrb_find_str(database->directors_name_lookup, name_buf));

    if (directorEntry == NULL)
    {
        // Add new entry
        assert(database->directors_count < database->directors_capacity);

        int currentIndex = database->directors_count++;
        DirectorEntry* director = &database->directors[currentIndex];
        director->internal_id = currentIndex;
        director->name = string_view_dup(name);
        director->movies = new_dllist();

        // Update lookup
        jrb_insert_str(database->directors_name_lookup, director->name, new_jval_v(director));

        directorEntry = director;
    }

    return directorEntry;
}
CastEntry* database_add_cast_entry(Database* database, const c_string_view name)
{
    char name_buf[256];
    string_view_copy(name_buf, sizeof(name_buf) / sizeof(char), name);

    CastEntry* castEntry = jrb_as_cast(jrb_find_str(database->casts_name_lookup, name_buf));

    if (castEntry == NULL)
    {
        // Add new entry
        assert(database->casts_count < database->casts_capacity);

        int currentIndex = database->casts_count++;
        CastEntry* cast = &database->casts[currentIndex];
        cast->internal_id = currentIndex;
        cast->name = string_view_dup(name);
        cast->movies = new_dllist();

        // Update lookup
        jrb_insert_str(database->casts_name_lookup, cast->name, new_jval_v(cast));

        castEntry = cast;
    }

    return castEntry;
}
MovieEntry* database_add_movie_entry(Database* database, const MovieInfo* movieInfo)
{
    char title_buf[256];
    string_view_copy(title_buf, sizeof(title_buf) / sizeof(char), movieInfo->title);

    MovieEntry* movieEntry = jrb_as_movie(jrb_find_str(database->movies_name_lookup, title_buf));

    if (movieEntry == NULL)
    {
        // Add new entry
        assert(database->movies_count < database->movies_capacity);

        int currentIndex = database->movies_count++;
        MovieEntry* movie = &database->movies[currentIndex];
        movie->internal_id = currentIndex;
        movie->title = string_view_dup(movieInfo->title);

        // Update lookup
        jrb_insert_str(database->movies_name_lookup, movie->title, new_jval_v(movie));

        movieEntry = movie;
    }

    return movieEntry;
}

DirectorEntry* database_insert_director(Database* database, const c_string_view name)
{
    DirectorEntry* director = database_add_director_entry(database, name);

    // No more information to fill in
    return director;
}

CastEntry* database_insert_cast(Database* database, const c_string_view name)
{
    CastEntry* cast = database_add_cast_entry(database, name);

    // No more information to fill in
    return cast;
}

MovieEntry* database_insert_movie(Database* database, const MovieInfo* movieInfo)
{
    MovieEntry* movie = database_add_movie_entry(database, movieInfo);

    // Fill in information
    movie->show_id = movieInfo->show_id;
    movie->description = string_view_dup(movieInfo->description);
    movie->duration = movieInfo->duration;
    movie->release_year = movieInfo->release_year;
    movie->type = movieInfo->type;

    // Add genres
    movie->genre_count = movieInfo->genre_count;
    movie->genres = (GenreEntry**)malloc(sizeof(GenreEntry*) * movie->genre_count);
    for (int i = 0; i < movieInfo->genre_count; i++)
    {
        GenreEntry* genre = database_find_genre_entry(database, movieInfo->genres[i]);
        movie->genres[i] = genre;

        dll_append(genre->movies, new_jval_v(movie));
    }

    // Insert directors
    movie->director_count = movieInfo->director_count;
    movie->directors = (DirectorEntry**)malloc(sizeof(DirectorEntry*) * movie->director_count);
    for (int i = 0; i < movieInfo->director_count; i++)
    {
        DirectorEntry* insertedDirector = database_insert_director(database, movieInfo->directors[i]);
        movie->directors[i] = insertedDirector;

        dll_append(insertedDirector->movies, new_jval_v(movie));
    }

    // Insert casts
    movie->cast_count = movieInfo->cast_count;
    movie->casts = (CastEntry**)malloc(sizeof(CastEntry*) * movie->cast_count);
    for (int i = 0; i < movieInfo->cast_count; i++)
    {
        CastEntry* insertedCast = database_insert_cast(database, movieInfo->casts[i]);
        movie->casts[i] = insertedCast;

        dll_append(insertedCast->movies, new_jval_v(movie));
    }

    return movie;
}

// Public interface
Database* database_create()
{
    Database* database = (Database*)malloc(sizeof(Database));
    database_init(database);

    return database;
}

void database_drop(Database* database)
{
    // TODO
}

const MovieEntry* database_add_movie(Database* database, const MovieInfo* movieInfo)
{
    return database_insert_movie(database, movieInfo);
}

const GenreEntry* database_find_genre(Database* database, const char* name)
{
    return jrb_as_genre(jrb_find_str(database->genres_name_lookup, name));
}
const DirectorEntry* database_find_director(Database* database, const char* name)
{
    return jrb_as_director(jrb_find_str(database->directors_name_lookup, name));
}
const CastEntry* database_find_cast(Database* database, const char* name)
{
    return jrb_as_cast(jrb_find_str(database->casts_name_lookup, name));
}
const MovieEntry* database_find_movie(Database* database, const char* title)
{
    return jrb_as_movie(jrb_find_str(database->movies_name_lookup, title));
}

int substring_compare(Jval a, Jval b)
{
    const char* string_key = a.s;
    const char* string_node = b.s;

    if (strstr(string_node, string_key) != NULL)
    {
        return 0;
    }

    return strcmp(string_key, string_node);
}

const MovieEntry* database_find_movie_g(Database* database, const char* title)
{
    return jrb_as_movie(jrb_find_gen(database->movies_name_lookup, new_jval_s(title), substring_compare));
}

int c = 0;

void movie_update_similarity_score(JRB movieNode, int crossMovieId)
{
    JRB score_node = NULL;
    if ((score_node = jrb_find_int(movieNode, crossMovieId)) == NULL)
    {
        c += 1;
        // First encounter, insert new score node
        score_node = jrb_insert_int(movieNode, crossMovieId, new_jval_i(0));
    }

    score_node->val.i += 1;
}

void movie_calculate_similarity(const MovieEntry* movie, JRB movieNode)
{
    // Update score based on directors in common
    for (int i = 0; i < movie->director_count; i++)
    {
        const DirectorEntry* director = movie->directors[i];

        Dllist dll_cross_movie = NULL;
        dll_traverse(dll_cross_movie, director->movies)
        {
            const MovieEntry* cross_movie = (const MovieEntry*)(dll_cross_movie->val.v);

            if (cross_movie->internal_id != movie->internal_id)
            {
                movie_update_similarity_score(movieNode, cross_movie->internal_id);
            }
        }
    }

    // Update score based on casts in common
    for (int i = 0; i < movie->cast_count; i++)
    {
        const CastEntry* cast = movie->casts[i];

        Dllist dll_cross_movie = NULL;
        dll_traverse(dll_cross_movie, cast->movies)
        {
            const MovieEntry* cross_movie = (const MovieEntry*)(dll_cross_movie->val.v);

            if (cross_movie->internal_id != movie->internal_id)
            {
                movie_update_similarity_score(movieNode, cross_movie->internal_id);
            }
        }
    }

    // Update score based on genres in common
    for (int i = 0; i < movie->genre_count; i++)
    {
        const GenreEntry* genre = movie->genres[i];

        Dllist dll_cross_movie = NULL;
        dll_traverse(dll_cross_movie, genre->movies)
        {
            const MovieEntry* cross_movie = (const MovieEntry*)(dll_cross_movie->val.v);

            if (cross_movie->internal_id != movie->internal_id)
            {
                movie_update_similarity_score(movieNode, cross_movie->internal_id);
            }
        }
    }
}

JRB jval_as_jrb(Jval jval)
{
    return (JRB)(jval.v);
}

void database_generate_similarity_map(Database* database)
{
    assert(database->movies_similarity_map == NULL);

    database->movies_similarity_map = make_jrb();
    JRB similarity_map = database->movies_similarity_map;

    for (int i = 0; i < database->movies_count; i++)
    {
        const MovieEntry* movie = &database->movies[i];

        // Insert new entry into similarity map
        JRB movieNode = jrb_insert_int(similarity_map, movie->internal_id, new_jval_v(make_jrb()));
        movie_calculate_similarity(movie, jval_as_jrb(movieNode->val));
    }
}

void database_generate_similarity_map_1(Database* database, int internalMovieId)
{
    assert(database->movies_similarity_map == NULL);

    database->movies_similarity_map = make_jrb();
    JRB similarity_map = database->movies_similarity_map;


    const MovieEntry* movie = &database->movies[internalMovieId];

    // Insert new entry into similarity map
    JRB movieNode = jrb_insert_int(similarity_map, movie->internal_id, new_jval_v(make_jrb()));
    movie_calculate_similarity(movie, jval_as_jrb(movieNode->val));
}


JRB database_get_similarity_list(Database* database, int internalMovieId)
{
    assert(database->movies_similarity_map != NULL);

    JRB movie_node = jrb_find_int(database->movies_similarity_map, internalMovieId);
    if (movie_node == NULL)
    {
        return NULL;
    }

    return jval_as_jrb(movie_node->val);
}

