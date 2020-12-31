#define _CRT_SECURE_NO_WARNINGS

#include "../inc/shared/csv_reader_c.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/jrb.h"
#include "../inc/dllist.h"

struct MovieEntry;
struct DirectorEntry;
struct CastEntry;
struct GenreEntry;
struct Database;

typedef struct _MovieEntry
{
    int show_id;
    enum MovieType type;
    int release_year;
    int duration;

    const char *description;

    int director_count;
    struct DirectorEntry **directors;

    int cast_count;
    struct CastEntry **casts;

    int genre_count;
    struct GenreEntry **genres;
} MovieEntry;

typedef struct _DirectorEntry
{
    const char *name;
    Dllist movies;
} DirectorEntry;

typedef struct _CastEntry
{
    const char *name;
    Dllist movies;
} CastEntry;

typedef struct _GenreEntry
{
    const char *name;
    Dllist movies;
} GenreEntry;

typedef struct _Database
{
    int movies_capacity;
    int movies_count;
    MovieEntry *movies;

    int directors_capacity;
    int directors_count;
    DirectorEntry *directors;

    int casts_capacity;
    int casts_count;
    CastEntry *casts;

    int genres_capacity;
    int genres_count;
    GenreEntry *genres;

    // Key -> name, value -> MovieEntry*
    JRB movies_name_lookup;
    // Key -> name, value -> DirectorEntry*
    JRB directors_name_lookup;
    // Key -> name, value -> CastEntry*
    JRB casts_name_lookup;
    // Key -> name, value -> GenreEntry*
    JRB genres_name_lookup;
} Database;

void database_init(Database *database)
{
    // Movies
    database->movies_capacity = 6300;
    database->movies_count = 0;
    database->movies = (MovieEntry *)malloc(sizeof(MovieEntry) * database->movies_capacity);

    // Directors
    database->directors_capacity = 200;
    database->directors_count = 0;
    database->directors = (DirectorEntry *)malloc(sizeof(DirectorEntry) * database->directors_capacity);

    // Casts
    database->casts_capacity = 400;
    database->casts_count = 0;
    database->casts = (CastEntry *)malloc(sizeof(CastEntry) * database->casts_capacity);

    // Genres
    database->genres_capacity = 35;
    database->genres_count = 0;
    database->genres = (GenreEntry *)malloc(sizeof(GenreEntry) * database->genres_capacity);

    // Populate genres list
    database->genres[0] = (GenreEntry){.name = "SciFi", .movies = new_dllist()};
    database->genres[1] = (GenreEntry){.name = "Kids", .movies = new_dllist()};
    database->genres[2] = (GenreEntry){.name = "Comedies", .movies = new_dllist()};
    database->genres[3] = (GenreEntry){.name = "Standup", .movies = new_dllist()};
    database->genres[4] = (GenreEntry){.name = "Fantasy", .movies = new_dllist()};
    database->genres[5] = (GenreEntry){.name = "Crime", .movies = new_dllist()};
    database->genres[6] = (GenreEntry){.name = "Spanish", .movies = new_dllist()};
    database->genres[7] = (GenreEntry){.name = "International", .movies = new_dllist()};
    database->genres[8] = (GenreEntry){.name = "Thrillers", .movies = new_dllist()};
    database->genres[9] = (GenreEntry){.name = "Comedy", .movies = new_dllist()};
    database->genres[10] = (GenreEntry){.name = "Docuseries", .movies = new_dllist()};
    database->genres[11] = (GenreEntry){.name = "Science", .movies = new_dllist()};
    database->genres[12] = (GenreEntry){.name = "Nature", .movies = new_dllist()};
    database->genres[13] = (GenreEntry){.name = "Action", .movies = new_dllist()};
    database->genres[14] = (GenreEntry){.name = "Adventure", .movies = new_dllist()};
    database->genres[15] = (GenreEntry){.name = "Dramas", .movies = new_dllist()};
    database->genres[16] = (GenreEntry){.name = "Cult", .movies = new_dllist()};
    database->genres[17] = (GenreEntry){.name = "Indie", .movies = new_dllist()};
    database->genres[18] = (GenreEntry){.name = "Romantic", .movies = new_dllist()};
    database->genres[19] = (GenreEntry){.name = "Documentaries", .movies = new_dllist()};
    database->genres[20] = (GenreEntry){.name = "Horror", .movies = new_dllist()};
    database->genres[21] = (GenreEntry){.name = "Mysteries", .movies = new_dllist()};
    database->genres[22] = (GenreEntry){.name = "British", .movies = new_dllist()};
    database->genres[23] = (GenreEntry){.name = "Movies", .movies = new_dllist()};
    database->genres[24] = (GenreEntry){.name = "Music", .movies = new_dllist()};
    database->genres[25] = (GenreEntry){.name = "Reality", .movies = new_dllist()};
    database->genres[26] = (GenreEntry){.name = "Anime", .movies = new_dllist()};
    database->genres[27] = (GenreEntry){.name = "Teen", .movies = new_dllist()};
    database->genres[28] = (GenreEntry){.name = "Sports", .movies = new_dllist()};
    database->genres[29] = (GenreEntry){.name = "Spirituality", .movies = new_dllist()};
    database->genres[30] = (GenreEntry){.name = "Korean", .movies = new_dllist()};
    database->genres[31] = (GenreEntry){.name = "LGBTQ", .movies = new_dllist()};
    database->genres[32] = (GenreEntry){.name = "Classic", .movies = new_dllist()};
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

const char *duplicate_string(const c_string_view str)
{
    const char *buf = (const char *)malloc(sizeof(char) * str.size);
    strncpy(buf, str.str, str.size);

    return buf;
}

DirectorEntry *database_insert_director(Database *database, const char *name);
CastEntry *database_insert_cast(Database *database, const char *name);
MovieEntry *database_insert_movie(Database *database, const MovieInfo *movieInfo);

DirectorEntry *database_insert_director(Database *database, const char *name)
{
}
CastEntry *database_insert_cast(Database *database, const char *name)
{
}

MovieEntry *database_insert_movie(Database *database, const MovieInfo *movieInfo)
{
    MovieEntry *movie = &database->movies[database->movies_count++];
    movie->show_id = movieInfo->show_id;
    movie->description = duplicate_string(movieInfo->description);
    movie->duration = movieInfo->duration;
    movie->release_year = movieInfo->release_year;
    movie->type = movieInfo->type;

    // Add genres
    movie->genre_count = movieInfo->genre_count;
    movie->genres = (GenreEntry **)malloc(sizeof(GenreEntry **) * movie->genre_count);
    for (int i = 0; i < movieInfo->genre_count; i++)
    {
        // Genre value is the index into database-genres
        movie->genres[i] = &database->genres[movieInfo->genres[i]];
    }

    // Insert directors
    movie->director_count = movieInfo->director_count;
    movie->directors = (DirectorEntry **)malloc(sizeof(DirectorEntry *) * movie->director_count);
    for (int i = 0; i < movieInfo->directors->size; i++)
    {
        DirectorEntry *insertedDirector = database_insert_director(database, duplicate_string(movieInfo->directors[i]));
        movie->directors[i] = insertedDirector;
    }

    // Insert casts
    movie->cast_count = movieInfo->cast_count;
    movie->casts = (CastEntry **)malloc(sizeof(CastEntry *) * movie->cast_count);
    for (int i = 0; i < movieInfo->casts->size; i++)
    {
        CastEntry *insertedCast = database_insert_cast(database, duplicate_string(movieInfo->casts[i]));
        movie->casts[i] = insertedCast;
    }
}

void callback(const MovieInfo *movieInfo)
{
    char buf[50];

    printf("%d\n", movieInfo->show_id);

    //for (int i = 0; i < movieInfo-ncastncount; i++)
    //{
    //    strncpy(buf, movieInfo-ncasts[i].str, movieInfo-ncasts[i].size);
    //    buf[movieInfo-ncasts[i].size + 1] = '\0';
    //    printf("%s, ", buf);
    //}

    printf("\n");
}

int main()
{
    const char *path = "./data/netflix_titles2.csv";
    read_csv(path, callback);
}