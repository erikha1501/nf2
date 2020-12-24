#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "util_c.h"

    typedef enum MovieType
    {
        Movie,
        TVShow
    };

    typedef enum Genre
    {
        SciFi,
        Kids,
        Comedies,
        Standup,
        Fantasy,
        Crime,
        Spanish,
        International,
        Thrillers,
        Comedy,
        Docuseries,
        Science,
        Nature,
        Action,
        Adventure,
        Dramas,
        Cult,
        Indie,
        Romantic,
        Documentaries,
        Horror,
        Mysteries,
        British,
        Movies,
        Music,
        Reality,
        Anime,
        Teen,
        Sports,
        Spirituality,
        Korean,
        LGBTQ,
        Classic
    };

    typedef struct
    {
        int show_id;
        enum MovieType type;
        c_string_view title;

        int director_count;
        c_string_view* directors;

        int cast_count;
        c_string_view* casts;

        int release_year;
        int duration;
        long genreBitFields;
        c_string_view description;
    } MovieInfo;

    typedef void (*ReaderCallBack)(const MovieInfo*);

    void read_csv(const char* path, ReaderCallBack callback);

#ifdef __cplusplus
}
#endif

