#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "util_c.h"

    enum MovieType
    {
        Movie,
        TVShow
    };

    enum Genre
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
        const c_string_view* directors;

        int cast_count;
        const c_string_view* casts;

        int genre_count;
        const enum Genre* genres;

        int release_year;
        int duration;
        
        c_string_view description;
    } MovieInfo;

    typedef void (*ReaderCallBack)(const MovieInfo*);

    void read_csv(const char* path, ReaderCallBack callback);

#ifdef __cplusplus
}
#endif

