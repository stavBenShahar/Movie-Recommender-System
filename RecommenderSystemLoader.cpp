#include "RecommenderSystemLoader.h"

std::string get_all_file (const string &movies_file_path)
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
std::unique_ptr<RecommenderSystem> create_rs_from_movie_file_helper (const
                                             std::string &movies_file_path)
{
  RecommenderSystem recommender_system;
  std::istringstream file_stream (get_all_file (movies_file_path));
  std::string buffer;
  while (std::getline (file_stream, buffer))
  {
    std::istringstream curr_line_stream (buffer);
    //Includes movie name and year.
    std::string movie_info;
    curr_line_stream >> movie_info;
    std::string name = movie_info.substr (0, movie_info.find
                                                            (DELIMITERS));
    std::string year_str = movie_info.substr
        (name.length () + 1, movie_info.length () - name.length ());
    int year = std::stoi (year_str);
    features_vector current_movie_features;
    double feature;
    while (curr_line_stream >> feature)
    {

      if (feature > MAX_FEATURE_VALUE || feature < MIN_FEATURE_VALUE)
      {
        throw std::length_error (FEATURE_ERROR);
      }
      current_movie_features.push_back (feature);
    }
    current_movie_features.shrink_to_fit ();
    recommender_system.add_movie (name, year,current_movie_features);
  }
  return std::make_unique<RecommenderSystem> (recommender_system);
}



std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  try
  {
    std::unique_ptr<RecommenderSystem> rs = create_rs_from_movie_file_helper
        (movies_file_path);
    return rs;
  }
  catch (const std::runtime_error &invalid_file)
  {
    throw std::runtime_error (FILE_ERROR);
  }
  catch (const std::length_error &wrong_input)
  {
    throw std::length_error (FEATURE_ERROR);
  }
  catch (const std::bad_alloc &ba)
  {
    throw std::bad_alloc ();
  }
}

