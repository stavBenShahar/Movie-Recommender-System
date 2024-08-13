#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>
#include <math.h>
#include <limits>
#define INVALID_VECTOR_ERROR "Vectors arent the same size."
using std::pair;
typedef std::vector<double> features_vector;

class RSUser;
class RecommenderSystem
{
 public:

  RecommenderSystem();
  //explicit RecommenderSystem()
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year, const features_vector &
  features);

  /**
   * a function that calculates the movie with highest score based on
   * movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score
   * based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf
   * procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k) const;

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  /**
   * Prints the movies in the RS by sorted order (according to the <
   * operator).
   * @param stream
   * @param RS - The Recommender System that contains the movies.
   * @return The ostream.
   */
  friend ostream &operator<< (ostream &stream, const RecommenderSystem &RS);

 private:

  //compare function for the map <system movies>.

  struct compare_func
  {
      bool operator()(const sp_movie& m1,const sp_movie& m2)const{
        return *m1 < *m2;
      }
  };
  // The map that holds all the movies that were given from the movies file.
  std::map<sp_movie, features_vector, compare_func>system_movies;
  //--------------------------------------------------------------------------/
  //                        helper functions                                 /
  //------------------------------------------------------------------------/
  /**
   * Create unordered map that is similar to the user's map but its ranks are
   * normalized.
   * @param user
   * @return a normalized unordered-map that is used to create the user's
   * preference vector.
   */
  std::unordered_map<sp_movie, double> get_normalized_uo_map (const RSUser
                                                              &user) const;
  /**
   * Creates a preference vector of a given user
   * @param normalized_uo_map - Similar to user's map but its ranks are
   * normalized
   * @return A preference vector that is used to find a similar vector of a
   * movie the user hasn't seen before.
   */
  features_vector get_pref_vector (std::unordered_map<sp_movie, double> &
  normalized_uo_map) const;

  double dot_product (const features_vector &, const features_vector &) const;
  double norm (const features_vector &) const;
  /**
   * Used the dot_product and norm in order to find the similarity between two
   * given vectors.
   * @return The angle between the input vectors.
   */
  double calculate_angle (const features_vector &, const
  features_vector &) const;
  /**
   * Create a vector with k (a given number) sp_movies, those movies are the
   * most similar to the input_movie.
   * @param input_movie
   * @param amount_of_movies
   * @param user
   * @return
   */
  std::vector<sp_movie> get_k_similar_movies (sp_movie input_movie, int
  amount_of_movies, const RSUser &user) const;
  /**
   * Create a new vector that is the sum of 2 given vectors.
   * @param v1
   * @param v2
   * @return
   */
  features_vector sum_of_vectors (const features_vector &v1,
                                  const
                                  features_vector &v2) const;
  /**
   * Create a new vectors that is the multiplication of a given vector and a
   * given scalar.
   * @param v1
   * @param scalar
   * @return
   */
  features_vector vec_multiply_by_scalar (const features_vector &v1,
                                          double scalar) const;
  bool is_feature_vector_valid(const std::vector<double>& v1);
};

#endif //RECOMMENDERSYSTEM_H
