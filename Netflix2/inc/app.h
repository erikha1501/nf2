#pragma once

#include "database.h"

typedef struct _App
{
    Database* database;

    JRB watched_list;
} App;

App* app_create();
void app_drop(App* app);

void app_init(App* app);

void printMovie(const MovieEntry* movie);
void printCast(const CastEntry* cast);

void app_add_to_watched_list(App* app, int internalMovieId);
void app_show_watched_list(App* app);
void app_clear_watched_list(App* app);
void app_get_cast_movies(App* app, int cast_id);

Dllist app_recommend_movies(const App* app);