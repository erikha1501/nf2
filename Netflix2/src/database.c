#define _CRT_SECURE_NO_WARNINGS

#include "../inc/database.h"

#include <stdlib.h>
#include <assert.h>
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
    database->casts_capacity = 26000;
    database->casts_count = 0;
    database->casts = (CastEntry*)malloc(sizeof(CastEntry) * database->casts_capacity);

    // Genres
    database->genres_capacity = 35;
    database->genres_count = 0;
    database->genres = (GenreEntry*)malloc(sizeof(GenreEntry) * database->genres_capacity);

    // Populate genres list
    database->genres[0] = (GenreEntry){ .name = "SciFi", .movies = new_dllist() };
    database->genres[1] = (GenreEntry){ .name = "Kids", .movies = new_dllist() };
    database->genres[2] = (GenreEntry){ .name = "Comedies", .movies = new_dllist() };
    database->genres[3] = (GenreEntry){ .name = "Standup", .movies = new_dllist() };
    database->genres[4] = (GenreEntry){ .name = "Fantasy", .movies = new_dllist() };
    database->genres[5] = (GenreEntry){ .name = "Crime", .movies = new_dllist() };
    database->genres[6] = (GenreEntry){ .name = "Spanish", .movies = new_dllist() };
    database->genres[7] = (GenreEntry){ .name = "International", .movies = new_dllist() };
    database->genres[8] = (GenreEntry){ .name = "Thrillers", .movies = new_dllist() };
    database->genres[9] = (GenreEntry){ .name = "Comedy", .movies = new_dllist() };
    database->genres[10] = (GenreEntry){ .name = "Docuseries", .movies = new_dllist() };
    database->genres[11] = (GenreEntry){ .name = "Science", .movies = new_dllist() };
    database->genres[12] = (GenreEntry){ .name = "Nature", .movies = new_dllist() };
    database->genres[13] = (GenreEntry){ .name = "Action", .movies = new_dllist() };
    database->genres[14] = (GenreEntry){ .name = "Adventure", .movies = new_dllist() };
    database->genres[15] = (GenreEntry){ .name = "Dramas", .movies = new_dllist() };
    database->genres[16] = (GenreEntry){ .name = "Cult", .movies = new_dllist() };
    database->genres[17] = (GenreEntry){ .name = "Indie", .movies = new_dllist() };
    database->genres[18] = (GenreEntry){ .name = "Romantic", .movies = new_dllist() };
    database->genres[19] = (GenreEntry){ .name = "Documentaries", .movies = new_dllist() };
    database->genres[20] = (GenreEntry){ .name = "Horror", .movies = new_dllist() };
    database->genres[21] = (GenreEntry){ .name = "Mysteries", .movies = new_dllist() };
    database->genres[22] = (GenreEntry){ .name = "British", .movies = new_dllist() };
    database->genres[23] = (GenreEntry){ .name = "Movies", .movies = new_dllist() };
    database->genres[24] = (GenreEntry){ .name = "Music", .movies = new_dllist() };
    database->genres[25] = (GenreEntry){ .name = "Reality", .movies = new_dllist() };
    database->genres[26] = (GenreEntry){ .name = "Anime", .movies = new_dllist() };
    database->genres[27] = (GenreEntry){ .name = "Teen", .movies = new_dllist() };
    database->genres[28] = (GenreEntry){ .name = "Sports", .movies = new_dllist() };
    database->genres[29] = (GenreEntry){ .name = "Spirituality", .movies = new_dllist() };
    database->genres[30] = (GenreEntry){ .name = "Korean", .movies = new_dllist() };
    database->genres[31] = (GenreEntry){ .name = "LGBTQ", .movies = new_dllist() };
    database->genres[32] = (GenreEntry){ .name = "Classic", .movies = new_dllist() };
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
}

const char* string_view_dup(const c_string_view str)
{
    char* buf = (const char*)malloc(sizeof(char) * (str.size+1));
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

        DirectorEntry* director = &database->directors[database->directors_count++];
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

        CastEntry* cast = &database->casts[database->casts_count++];
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

        MovieEntry* movie = &database->movies[database->movies_count++];
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
    for (int i = 0; i < min(movieInfo->cast_count, 5); i++)
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