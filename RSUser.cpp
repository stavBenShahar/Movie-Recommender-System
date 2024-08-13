#include "RSUser.h"
#include "RecommenderSystem.h"

RSUser::RSUser (string name, rank_map ranks,
               std::shared_ptr<RecommenderSystem>
    rs)
    :
    _name (name), _ranks (ranks), _rs (rs)
{

}

string RSUser::get_name () const
{
  return this->_name;
}

const rank_map & RSUser::get_ranks () const
{
  return this->_ranks;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const features_vector &features,
                              double rate)
{
    if (rate > MAX_RANK_VALUE || rate < MIN_RANK_VALUE)
    {
      throw std::length_error (RANK_ERROR);
    }
    sp_movie new_movie = this->_rs->add_movie (name, year, features);
    std::pair<sp_movie, double> new_rated_movie (new_movie, rate);
    this->_ranks.insert (new_rated_movie);
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _rs->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _rs->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year,
                                               int k) const
{
  sp_movie movie_to_predict = std::make_shared<Movie> (Movie (name, year));
  return _rs->predict_movie_score (*this, movie_to_predict, k);
}

ostream &operator<< (ostream &stream, const RSUser &user)
{
  cout << "name: " << user.get_name () << endl;
  cout << *user._rs;
  return stream;
}
