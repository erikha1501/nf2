#pragma once
#include "../inc/dllist.h"
#include "../inc/jrb.h"
#include "../inc/shared/csv_reader_c.h"

struct MovieEntry;
struct DirectorEntry;
struct CastEntry;
struct GenreEntry;
struct Database;

typedef struct _MovieEntry
{
    int internal_id;

    const char* title;
    int show_id;
    enum MovieType type;
    int release_year;
    int duration;

    const char* description;

    int director_count;
    struct DirectorEntry** directors;

    int cast_count;
    struct CastEntry** casts;

    int genre_count;
    struct GenreEntry** genres;
} MovieEntry;

typedef struct _DirectorEntry
{
    int internal_id;

    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} DirectorEntry;

typedef struct _CastEntry
{
    int internal_id;

    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} CastEntry;

typedef struct _GenreEntry
{
    int internal_id;

    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} GenreEntry;


typedef struct _SimilarityEntry
{
    int score;
    MovieEntry* movie;
} SimilarityEntry;

typedef struct _SimilarityList
{
    int list_size;
    SimilarityEntry list[10];
} SimilarityList;

typedef struct _Database
{
    int movies_capacity;
    int movies_count;
    MovieEntry* movies;

    int directors_capacity;
    int directors_count;
    DirectorEntry* directors;

    int casts_capacity;
    int casts_count;
    CastEntry* casts;

    int genres_capacity;
    int genres_count;
    GenreEntry* genres;

    // Key -> name, value -> MovieEntry*
    JRB movies_name_lookup;
    // Key -> name, value -> DirectorEntry*
    JRB directors_name_lookup;
    // Key -> name, value -> CastEntry*
    JRB casts_name_lookup;
    // Key -> name, value -> GenreEntry*
    JRB genres_name_lookup;

    SimilarityList* movies_similarity;
} Database;

Database* database_create();
void database_drop(Database* database);

void database_generate_similarity_map(Database* database);

const MovieEntry* database_add_movie(Database* database, const MovieInfo* movieInfo);

const GenreEntry* database_find_genre(Database* database, const char* name);
const DirectorEntry* database_find_director(Database* database, const char* name);
const CastEntry* database_find_cast(Database* database, const char* name);
const MovieEntry* database_find_movie(Database* database, const char* title);
const MovieEntry* database_find_movie_g(Database* database, const char* title);

Dllist database_find_movies(Database* database, const char* keyword);
Dllist database_find_directors(Database* database, const char* keyword);
Dllist database_find_casts(Database* database, const char* keyword);

const SimilarityList* database_get_similarity_list(Database* database, int internalMovieId);