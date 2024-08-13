#include "RecommenderSystem.h"

RecommenderSystem::RecommenderSystem ()
{
}

sp_movie RecommenderSystem::add_movie (const std::string &name,
                                       int year,
                                       const features_vector &features)
{
  if (!is_feature_vector_valid (features))
  {
    throw std::length_error (FEATURE_ERROR);
  }
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  this->system_movies[new_movie] = features;
  return new_movie;

}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  //Unordered map with the movies the user saw and the ranks are normalized.
  std::unordered_map<sp_movie, double> normalized_uo_map =
      get_normalized_uo_map (user);

  features_vector preference_vec = get_pref_vector (normalized_uo_map);
  sp_movie result;
  double biggest_angle = -INFINITY;
  rank_map user_rankings = user.get_ranks ();
  for (const auto &pair: system_movies)
  {
    //User rated the movie
    if (user_rankings.find (pair.first) != user.get_ranks ().end ())
    {
      continue;
    }
    else
    {
      double curr_angle = calculate_angle (preference_vec, pair.second);
      if (curr_angle > biggest_angle)
      {
        biggest_angle = curr_angle;
        result = pair.first;
      }
    }
  }

  return result;
}

double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie,
                                               int k) const
{
  std::vector<sp_movie> k_similar_movies = get_k_similar_movies (movie, k,
                                                                 user);

  features_vector given_movie_features = this->system_movies.at (movie);
  double numerator_result = 0.0;
  double denominator_result = 0.0;
  rank_map user_ranks = user.get_ranks ();
  for (int i = 0; i < k; i++)
  {
    double curr_rank = user_ranks[k_similar_movies[i]];
    double curr_angle = calculate_angle (given_movie_features,
                                         this->system_movies.at
                                             (k_similar_movies[i]));
    numerator_result += curr_angle * curr_rank;
    denominator_result += curr_angle;
  }
  return numerator_result / denominator_result;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie temp = std::make_shared<Movie>(name, year);
  auto result = system_movies.find(temp);
  if (result == system_movies.end())
  {
    return nullptr;
  }
  return result->first;
}

ostream &operator<< (ostream &stream, const RecommenderSystem &RS)
{
  for (const auto &movie: RS.system_movies)
  {
    stream << *movie.first;
  }
  return stream;
}

std::unordered_map<sp_movie, double>
RecommenderSystem::get_normalized_uo_map (const RSUser &user) const
{
  size_t amount_of_ranked_movies = user.get_ranks ().size ();
  double sum_of_ranked_movies = 0.0;
  for (const auto &pair: user.get_ranks ())
  {
    sum_of_ranked_movies += pair.second;
  }
  double normalizing_scalar = sum_of_ranked_movies / amount_of_ranked_movies;
  std::unordered_map<sp_movie, double> result;
  for (const auto &pair: user.get_ranks ())
  {
    std::pair<sp_movie, double> new_pair (pair.first, pair.second -
                                                      normalizing_scalar);
    result.insert (new_pair);
  }
  return result;
}

features_vector RecommenderSystem::get_pref_vector (std::unordered_map<
    sp_movie, double> &normalized_uo_map)
const
{
  features_vector preference_vec (system_movies.begin ()->second.size (), 0);
  for (const auto &rank: system_movies)
  {
    auto &current_pair = *normalized_uo_map.find (rank.first);
    if (normalized_uo_map.find (rank.first) != normalized_uo_map.end ())
    {
      features_vector temp = vec_multiply_by_scalar (rank.second,
                                                     current_pair.second);
      preference_vec = sum_of_vectors (preference_vec, temp);
    }
  }
  return preference_vec;
}
features_vector RecommenderSystem::sum_of_vectors (const features_vector &v1,
                                                   const features_vector &v2)
const
{
  if (v1.size () == v2.size ())
  {
    features_vector result_vector (v1.size (), 0);
    for (unsigned int i = 0; i < v1.size (); i++)
    {
      result_vector[i] = v1[i] + v2[i];

    }
    return result_vector;
  }
  throw std::length_error (INVALID_VECTOR_ERROR);

}

double RecommenderSystem::dot_product (const features_vector &v1,
                                       const features_vector &v2) const
{
  if (v1.size () == v2.size ())
  {
    double result = 0.0;
    for (unsigned int i = 0; i < v1.size (); i++)
    {
      result += (v1[i] * v2[i]);
    }
    return result;
  }
  throw std::length_error (INVALID_VECTOR_ERROR);
}

double RecommenderSystem::norm (const features_vector &v1) const
{
  return sqrt (dot_product (v1, v1));
}

std::vector<sp_movie> RecommenderSystem::get_k_similar_movies
    (sp_movie input_movie, int amount_of_movies, const RSUser &user) const
{
  //Map of all the movies the user rated, Key is the angle of the vector
  //with the given movie vector and the value is the pointer to the rated
  // movie.
  std::map<double, sp_movie> ranked_movies_by_user;
  //Vector of the features of the given movie.
  features_vector given_movie_features = this->system_movies.find
      (input_movie)->second;
  //Create map by calculating the angles of the rated movies.
  for (const auto &curr_movie: user.get_ranks ())
  {
    features_vector curr_movie_features = system_movies.at (curr_movie.first);
    double curr_angle = calculate_angle (given_movie_features,
                                         curr_movie_features);
    ranked_movies_by_user[curr_angle] = curr_movie.first;
  }
  std::vector<sp_movie> result;
  std::map<double, sp_movie>::iterator iterator = ranked_movies_by_user.end ();
  int k = 0;
  //Take K elements whose angle with the given vector is the biggest by
  //looping from the end of the map.

  while (k != amount_of_movies)
  {
    iterator--;
    result.push_back (iterator->second);
    k++;
  }
  result.shrink_to_fit ();
  return result;
}

double RecommenderSystem::calculate_angle (const features_vector &v1,
                                           const features_vector &v2) const
{
  double numerator = dot_product (v1, v2);
  double denominator = norm (v1) * norm (v2);
  return numerator / denominator;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  double highest_rank = -INFINITY;
  sp_movie result = nullptr;
  rank_map user_rankings = user.get_ranks ();
  for (const auto &curr_movie: this->system_movies)
  {
    //To check if the user already seen the movie
    if (user_rankings.find (curr_movie.first) != user.get_ranks ().end ())
    {
      continue;
    }
    double curr_predicted_rank = predict_movie_score
                                             (user,curr_movie.first, k);
    if (curr_predicted_rank > highest_rank)
    {
      highest_rank = curr_predicted_rank;
      result = curr_movie.first;
    }
  }
  return result;
}

features_vector
RecommenderSystem::vec_multiply_by_scalar (const features_vector &v1,
                                           double scalar) const
{
  features_vector result_vect (v1.size (), 0);
  for (unsigned int i = 0; i < v1.size (); i++)
  {

    result_vect[i] = v1[i] * scalar;
  }
  return result_vect;
}
bool RecommenderSystem::is_feature_vector_valid (const vector<double> &v1)
{
  for (size_t i = 0;i < v1.size();i++)
  {
    if (v1[i] > MAX_FEATURE_VALUE || v1[i] < MIN_FEATURE_VALUE )
    {
      return false;
    }
  }
  return true;
}
