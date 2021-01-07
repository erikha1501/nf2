#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/app.h"

App* app;

void showMenu()
{
    printf("\n__________________MENU___________________\n");
    printf("Choose an option to perform:\n");
    printf("1. Search for movie by title\n");
    printf("2. Search for cast name\n");
    printf("3. Search for movies featuring cast\n");
    printf("4. Add movie to watched list\n");
    printf("5. Show watched list\n");
    printf("6. Clear watched list\n");
    printf("7. Recommend movie from watch list\n");
    printf("0. Exit\n");
}

void searchTitle()
{
    char keyword[100];
    printf("Enter search keyword: ");
    scanf(" %[^\n]s", keyword);
    
    Dllist movies_list = database_find_movies(app->database, keyword);
    
    Dllist movie_dll = NULL;

    dll_traverse(movie_dll, movies_list)
    {
        const MovieEntry* movie = (const MovieEntry*)(movie_dll->val.v);
        printMovie(movie);
    }

    free_dllist(movies_list);
}

void searchCastName()
{
    char keyword[100];
    printf("Enter search keyword: ");
    scanf(" %[^\n]s", keyword);

    Dllist casts_list = database_find_casts(app->database, keyword);

    Dllist cast_dll = NULL;

    dll_traverse(cast_dll, casts_list)
    {
        const CastEntry* cast = (const CastEntry*)(cast_dll->val.v);
        printCast(cast);
    }

    free_dllist(casts_list);
}

void searchCastMovies()
{
    printf("Enter actor/actress id: ");

    int cast_id = -1;
    scanf(" %d", &cast_id);
    
    app_get_cast_movies(app, cast_id);
}

void addWatchList()
{
    printf("Enter movie id: ");

    int movie_id = -1;
    scanf(" %d", &movie_id);
    
    app_add_to_watched_list(app, movie_id);
}

void showWatchList()
{
    app_show_watched_list(app);
}

void clearWatchList()
{
    app_clear_watched_list(app);
    printf("Watched list cleared.");
}

void recommendMovies()
{
    Dllist recommended_movies = app_recommend_movies(app);

    if (recommended_movies != NULL)
    {
        printf("Recommended movies:\n");

        Dllist movie_dll = NULL;
        dll_traverse(movie_dll, recommended_movies)
        {
            const MovieEntry* movie = (const MovieEntry*)(movie_dll->val.v);
            printMovie(movie);
        }

        free_dllist(recommended_movies);
    }
}

int main()
{
    app = app_create();
    app_init(app);

    int running = 1;
    while (running)
    {
        showMenu();
        
        int choice = -1;
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            searchTitle();
            break;
        case 2:
            searchCastName();
            break;
        case 3:
            searchCastMovies();
            break;
        case 4:
            addWatchList();
            break;
        case 5:
            showWatchList();
            break;
        case 6:
            clearWatchList();
            break;
        case 7:
            recommendMovies();
            break;
        default:
            running = 0;
            break;
        }
    }

    app_drop(app);
}