
#include "RSUsersLoader.h"
#include "RecommenderSystemLoader.h"

#define MOVIES_PATH_INDEX 1
#define USERS_PATH_INDEX 2
#include <cassert>



int main(int argc, char** argv)
{

  //-------------------------------------------------------------------------//
  //                           Loaders Test                                 //
  //-----------------------------------------------------------------------//
  std::unique_ptr<RecommenderSystem> RS =
      RecommenderSystemLoader::create_rs_from_movies_file
      (argv[MOVIES_PATH_INDEX]);
//  std::vector<RSUser> USERS = RSUsersLoader::create_users_from_file
//      (argv[USERS_PATH_INDEX], std::move(RS));
  std::shared_ptr<RecommenderSystem> rs = std::move(RS);
  std::cout << *rs;




  return 0;




}