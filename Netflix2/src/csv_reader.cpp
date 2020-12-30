#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <charconv>
// #include <Windows.h>

#include "../inc/csv.hpp"
#include "../inc/shared/csv_reader_c.h"
#include "../inc/shared/util_c.h"

namespace csv_reader
{
    using DefaultCSVReader = io::CSVReader<12, io::trim_chars<' '>, io::double_quote_escape<',', '"'>>;
    using GenreMap = std::unordered_map<std::string_view, Genre>;

    GenreMap genreMap{
        {"SciFi", SciFi},
        {"Kids", Kids},
        {"Comedies", Comedies},
        {"Standup", Standup},
        {"Fantasy", Fantasy},
        {"Crime", Crime},
        {"Spanish", Spanish},
        {"International", International},
        {"Thrillers", Thrillers},
        {"Comedy", Comedy},
        {"Docuseries", Docuseries},
        {"Science", Science},
        {"Nature", Nature},
        {"Action", Action},
        {"Adventure", Adventure},
        {"Dramas", Dramas},
        {"Cult", Cult},
        {"Indie", Indie},
        {"Romantic", Romantic},
        {"Documentaries", Documentaries},
        {"Horror", Horror},
        {"Mysteries", Mysteries},
        {"British", British},
        {"Movies", Movies},
        {"Music", Music},
        {"Reality", Reality},
        {"Anime", Anime},
        {"Teen", Teen},
        {"Sports", Sports},
        {"Spirituality", Spirituality},
        {"Korean", Korean},
        {"LGBTQ", LGBTQ},
        {"Classic", Classic} };

    struct MovieInfoEntry
    {
        int show_id;
        char* type;
        char* title;
        char* director;
        char* cast;
        char* country;
        char* date_added;
        int release_year;
        char* rating;
        char* duration;
        char* listed_in;
        char* description;
    };

    c_string_view to_c_string_view(const std::string_view string)
    {
        return c_string_view{ string.data(), (int)string.size() };
    }

    std::string_view trim(const std::string_view str)
    {
        size_t first = str.find_first_not_of(' ');
        if (std::string_view::npos == first)
        {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    std::vector<std::string_view> string_view_split(const std::string_view string, char seperator = ',')
    {
        int tokenStartIndex = 0;
        int commaIndex = 0;
        bool endOfString = false;

        std::vector<std::string_view> result{};

        while (!endOfString)
        {
            commaIndex = string.find(seperator, tokenStartIndex);

            if (commaIndex == std::string_view::npos)
            {
                endOfString = true;
            }

            std::string_view token = trim(string.substr(tokenStartIndex, commaIndex - tokenStartIndex));
            result.push_back(token);

            tokenStartIndex = commaIndex + 1;
        }

        return result;
    }

    int extract_duration(const std::string_view string)
    {
        int durationEndIndex = string.find(' ');

        int result = -1;
        std::from_chars(string.data(), string.data() + durationEndIndex - 1, result);

        return result;
    }

    void retrieve_genres(DefaultCSVReader& reader)
    {
        std::unordered_set<std::string> genreSet{};

        while (true)
        {
            MovieInfoEntry entry{};
            MovieInfo movieInfo{};

            try
            {
                bool result = reader.read_row(entry.show_id, entry.type, entry.title, entry.director,
                    entry.cast, entry.country, entry.date_added, entry.release_year,
                    entry.rating, entry.duration, entry.listed_in, entry.description);

                if (!result)
                {
                    break;
                }

                char* genreList = entry.listed_in;
                std::string_view genreListSV{ genreList };


            }
            catch (const std::exception&)
            {
            }
        }

        for (auto& item : genreSet)
        {
            std::cout << item << "\n";
        }
    }

    MovieInfo create_movie_info(
        int showId,
        const std::string_view type,
        const std::string_view title,
        const std::vector<std::string_view>& directors,
        const std::vector<std::string_view>& casts,
        int release_year,
        const std::string_view duration,
        const std::vector<std::string_view>& genres)
    {
        using namespace std::literals;

        MovieInfo result{};

        // Show id
        result.show_id = showId;

        // Type
        if (type == "Movie"sv)
        {
            result.type = MovieType::Movie;
        }
        else if (type == "TV Show"sv)
        {
            result.type = MovieType::TVShow;
        }

        // Title
        result.title = to_c_string_view(title);

        // Directors
        std::vector<c_string_view> directors_c{ directors.size() };
        result.director_count = directors.size();
        for (int i = 0; i < directors.size(); i++)
        {
            directors_c[i] = to_c_string_view(directors[i]);
        }
        result.directors = directors_c.data();

        // Casts
        std::vector<c_string_view> casts_c{ casts.size() };
        result.cast_count = casts.size();
        for (int i = 0; i < casts.size(); i++)
        {
            casts_c[i] = to_c_string_view(casts[i]);
        }
        result.casts = casts_c.data();

        // Release year
        result.release_year = release_year;

        // Duration
        result.duration = extract_duration(duration);

        // Genres
        std::vector<Genre> genres_c{ genres.size() };
        for (int i = 0; i < genres.size(); i++)
        {
            Genre g = genreMap[genres[i]];
            genres_c[i] = g;
        }
        result.cast_count = genres.size();
        result.genres = genres_c.data();

        return result;
    }

    int read_csv_internal(const char* path, ReaderCallBack callback)
    {
        DefaultCSVReader reader{ path };

        reader.read_header(io::ignore_extra_column, "show_id", "type", "title", "director", "cast", "country", "date_added", "release_year", "rating", "duration", "listed_in", "description");

        int count = 0;

        while (true)
        {
            MovieInfoEntry entry{};

            try
            {
                bool result = reader.read_row(entry.show_id, entry.type, entry.title, entry.director,
                    entry.cast, entry.country, entry.date_added, entry.release_year,
                    entry.rating, entry.duration, entry.listed_in, entry.description);

                if (!result)
                {
                    break;
                }

                int showId = entry.show_id;
                std::string_view type{ entry.type };
                std::string_view title{ entry.title };
                std::vector<std::string_view> directors = string_view_split(entry.director);
                std::vector<std::string_view> casts = string_view_split(entry.cast);
                int release_year = entry.release_year;
                std::string_view duration{ entry.duration };
                std::vector<std::string_view> genres = string_view_split(entry.listed_in);

                MovieInfo movieInfo = create_movie_info(showId, trim(type), trim(title), directors, casts, release_year, trim(duration), genres);
                callback(&movieInfo);

                count++;
            }
            catch (const std::exception&)
            {
            }
        }

        return count;
    }
}

void read_csv(const char* path, ReaderCallBack callback)
{
    // SetConsoleOutputCP(65001);
    csv_reader::read_csv_internal(path, callback);
}