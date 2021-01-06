#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/app.h"

App* app;

void printMovie(const MovieEntry* movie)
{
    printf("[%d] %s (%d) - ", movie->internal_id, movie->title, movie->release_year);
    switch (movie->type)
    {
    case TVShow:
        printf("TV show");
        break;
    case Movie:
        printf("Movie");
        break;
    default:
        break;
    }

    printf("\n");
}

void printCast(const CastEntry* cast)
{
    printf("%s", cast->name);
    printf("\n");
}

void showMenu()
{
    printf("\n__________________MENU___________________\n");
    printf("Choose an option to perform:\n");
    printf("1. Search for movie by title\n");
    printf("2. Search for cast name\n");
    printf("3. Add movie to watch list\n");
    printf("4. Recommend movie from watch list\n");
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

void addWatchList()
{
    printf("Enter movie id: ");

    int movie_id = -1;
    scanf(" %d", &movie_id);
    
    app_add_to_watched_list(app, movie_id);
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
    }

    free_dllist(recommended_movies);
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
            addWatchList();
            break;
        case 4:
            recommendMovies();
            break;
        default:
            running = 0;
            break;
        }
    }

    app_drop(app);
}