#include "RSUsersLoader.h"

std::string get_all_file_content (const string &movies_file_path)
{
  std::ifstream file (movies_file_path);
  if (!file)
  {
    throw std::runtime_error (FILE_ERROR);
  }
  file.seekg (0, std::ifstream::end);
  long int size = (long int) file.tellg ();
  file.seekg (0, std::ifstream::beg);
  char *buffer = new char[size + 1];
  file.read (buffer, size);
  file.close ();
  buffer[size] = '\0';
  std::string result = buffer;
  delete[] buffer;
  return result;
}


double convert_rank_to_double (std::string rank)
{
  if (rank == "NA")
  {
    return 0.0;
  }
  double new_rank =  std::stod (rank);
  if (new_rank > MAX_RANK_VALUE || new_rank < MIN_RANK_VALUE)
  {
    throw std::length_error(RANK_ERROR);
  }
  return new_rank;


}

/**
 * The "create_user_from_line" needs to know what movie it's checking
 * right now so the return vector from this function does exactly that.
 * @param buffer -A string of the first line of the user file
 * @return A vector that contains all the movies in the RS
 */
std::vector<Movie> create_vector_of_movies (std::string &buffer)
{
  std::istringstream stream (buffer);
  std::vector<Movie> result;

  std::string movie_info;
  while (stream >> movie_info)
  {
    std::string name = movie_info.substr
                                  (0, movie_info.find (DELIMITERS));
    std::string year_str = movie_info.substr
        (name.length () + 1, movie_info.length () - name.length ());
    int year = std::stoi (year_str);
    result.emplace_back (name, year);
  }
  result.shrink_to_fit ();
  return result;
}

/**
 * Create from a given line a user and his rank map.
 * @param stream A stream of a line in the user file.
 * @param rs - The Recommender System
 * @param movie_v - A vector of all the Movies.
 * @return A pair of the user's name and its rank map.
 */
std::pair<std::string, rank_map> create_user_from_line (std::istringstream &
stream, const std::shared_ptr<RecommenderSystem> &rs,
std::vector<Movie> &movie_v)
{

  std::string name;
  stream >> name;
  //Not starting as a double to check if user ranked it or NA.
  std::string rank_str;
  int i = 0;
  rank_map user_ranking (0, sp_movie_hash, sp_movie_equal);
  //Create ranking system of current user.
  while (stream >> rank_str)
  {
    try{
      double rank = convert_rank_to_double (rank_str);
      //It means the user didn't rank the current movie.
      if (rank == 0.0)
      {
        i++;
        continue;
      }
      sp_movie curr_movie_ptr = rs->get_movie (movie_v[i].get_name (),
                                               movie_v[i].get_year ());
      std::pair<sp_movie, double> curr_movie (curr_movie_ptr, rank);
      user_ranking.insert (curr_movie);
      i++;
    }
    catch(const std::length_error &invalid_len)
    {
      throw std::length_error(RANK_ERROR);
    }

  }
  std::pair<std::string, rank_map> result (name, user_ranking);
  return result;
}



std::vector<RSUser>
RSUsersLoader::create_users_from_file (const string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> rs)
                                       noexcept (false)
{
  try
  {
    std::vector<RSUser> users_vect;
    std::istringstream file_stream (get_all_file_content (users_file_path));

    //create a vector of all the movies so the helper func can see what movie
    //its checking at any given moment.
    std::string movies_buffer;
    std::getline (file_stream, movies_buffer);
    std::vector<Movie> movie_v = create_vector_of_movies (movies_buffer);
    std::shared_ptr<RecommenderSystem> rs_ptr = std::move (rs);
    std::string current_line;
    while (std::getline (file_stream, current_line))
    {
      std::istringstream current_line_stream (current_line);

      //Helper function that creates the user's name and his rank map.
      std::pair<std::string, rank_map> curr_user = create_user_from_line
          (current_line_stream, rs_ptr, movie_v);

      //Calls the RSUser constructor and inserts it into the user vector.
      users_vect.emplace_back(curr_user.first, curr_user.second,
                                                                rs_ptr);
    }
    users_vect.shrink_to_fit ();
    return users_vect;
  }
  catch (const std::runtime_error &ex)
  {
    throw std::runtime_error (FILE_ERROR);
  }
  catch (const std::length_error &wrong_input)
  {
    throw std::length_error (RANK_ERROR);
  }
}


