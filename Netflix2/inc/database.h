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
    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} DirectorEntry;

typedef struct _CastEntry
{
    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} CastEntry;

typedef struct _GenreEntry
{
    const char* name;
    // Value -> MovieEntry*
    Dllist movies;
} GenreEntry;

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
} Database;

Database* database_create();
void database_drop(Database* database);

const MovieEntry* database_add_movie(Database* database, const MovieInfo* movieInfo);

const GenreEntry* database_find_genre(Database* database, const char* name);
const DirectorEntry* database_find_director(Database* database, const char* name);
const CastEntry* database_find_cast(Database* database, const char* name);
const MovieEntry* database_find_movie(Database* database, const char* title);
