
#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}

Movie::Movie (const std::string &name, int year)
{
  this->_name = name;
  this->_year = year;
}

const std::string& Movie::get_name () const
{
  return this->_name;
}

int Movie::get_year () const
{
  return this->_year;
}

bool Movie::operator< (const Movie &rhs_movie) const
{
  if (rhs_movie.get_year() > (*this).get_year())
  {
    return true;
  }
  if (rhs_movie.get_year() == (*this).get_year())
  {
    return rhs_movie.get_name() > (*this).get_name();
  }
  return false;

}

ostream& operator<<(ostream &stream, const Movie& movie)
{
  cout << movie._name << " (" << movie._year << ")" <<endl;
  return stream;
}

