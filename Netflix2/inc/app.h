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

void app_add_to_watched_list(App* app, int internalMovieId);

Dllist app_recommend_movies(const App* app);