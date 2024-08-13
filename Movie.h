
#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#define HASH_START 17

#define MIN_FEATURE_VALUE 1
#define MAX_FEATURE_VALUE 10
#define MAX_RANK_VALUE 10
#define MIN_RANK_VALUE 1
#define FILE_ERROR "The given file is invalid."
#define FEATURE_ERROR "Invalid number was given as a feature of a movie."
#define RANK_ERROR "Invalid number was given as a rank of a movie."

using std::shared_ptr;
using std::string;
using std::ostream;
using std::cout;
using std::endl;
using std::min;

class Movie;

typedef shared_ptr<Movie> sp_movie;

/**
 * those declarations and typedefs are given to you and should be used in
 * the exercise
 */
typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);
std::size_t sp_movie_hash(const sp_movie& movie);
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

class Movie
{
 public:
    /**
     * constructor
     * @param name: name of movie
     * @param year: year it was ma
     * de
     */
    Movie(const std::string& name, int year);

    /**
     * returns the name of the movie
     * @return const ref to name of movie
     */
    const std::string& get_name() const;

    /**
     * returns the year the movie was made
     * @return year movie was made
     */
     int get_year() const;

	/**
     * operator< for two movies
     * @param rhs: right hand side
     * @param lhs: left hand side
     * @return returns true if (rhs.year) < lhs.year or
     * (rhs.year == lhs.year & rhs.name == lhs.name) else return false
     */
    bool operator<(const Movie& rhs_movie)const;

    /**
     * operator<< for movie
     * @param os ostream to output info with
     * @param movie movie to output
     */
    friend ostream& operator<<(ostream &stream, const Movie& movie);

 private:
  int _year;
  string _name;

};


#endif //MOVIE_H
