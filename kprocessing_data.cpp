#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <memory>
#include <regex>

#include "kraken/kutils.hpp"
#include "kraken/kclient.hpp"
#include "kraken/gnuplot-iostream.h"

using namespace std;
using namespace kraken;

typedef struct client_data {
  KInput in;
  KClient client;
} client_data;

client_data CD;

std::vector<std::string> filter_string_vector(const std::string &filter, const std::vector<std::string> &vStr)
{
  std::vector<std::string> results;
  std::regex filter_rgx(filter);
  return results;
}

int main(int argc, char **argv)
{
  std::vector<std::string> vString;
  string path = "/home/jcordovaech/krakenapi/build/data/";
  get_files_from_directory(path,vString);

  /*std::for_each(vString.begin(),vString.end(),
    [&](const std::string &str){
    std::cout << str << std::endl;
    });*/

  std::list<std::string> trade_files;
  std::list<std::string> ohlc_files;
  std::list<std::string> spread_files;

  /// TRADES
  std::copy_if(vString.begin(),vString.end(), std::back_inserter(trade_files),
               [&](const std::string &str){
                 std::regex rgx("^trades_.*?");
                 return std::regex_match(str.begin(),str.end(),rgx);
               });

  trade_files.sort();
  std::cout << std::endl;
  std::cout << "TRADES" << std::endl;
  std::cout << "******" << std::endl;
  std::for_each(trade_files.begin(),trade_files.end(),[&](const std::string &str){
      std::cout << str << std::endl;
    });

  /// OHLC
  // filter files by name using regular expresions
  std::copy_if(vString.begin(),vString.end(), std::back_inserter(ohlc_files),
               [&](const std::string &str){
                 std::regex rgx("^ohlc_.*?");
                 return std::regex_match(str.begin(),str.end(),rgx);
               });

  ohlc_files.sort();
  std::cout << std::endl;
  std::cout << "OHLC" << std::endl;
  std::cout << "****" << std::endl;
  std::for_each(ohlc_files.begin(),ohlc_files.end(),[&](const std::string &str){
      std::cout << str << std::endl;
    });

  /// SPREAD
  // filtering files by name using regular expresions
  std::copy_if(vString.begin(),vString.end(), std::back_inserter(spread_files),
               [&](const std::string &str){
                 std::regex rgx("^spread_.*?");
                 return std::regex_match(str.begin(),str.end(),rgx);
               });

  spread_files.sort(); // sorting files
  std::cout << std::endl;
  std::cout << "SPREAD" << std::endl;
  std::cout << "******" << std::endl;
  std::for_each(spread_files.begin(),spread_files.end(),[&](const std::string &str){
      std::cout << str << std::endl;
    });

  ///////////////
  /// READING ///
  ///////////////
  // reading trade
  std::cout << "reading trade files " << std::endl;
  KTradeStorage kts;
  for(const std::string &str: trade_files){
    std::ifstream ifs(path+str,std::fstream::binary);
    std::cout << "reading file " << path + str << std::endl;
    if(!ifs.is_open()){
      std::cout << "Error opening ohlc file " << path + str << std::endl;
      continue;
    }
    while(!ifs.eof()) {
      ifs >> kts;
    }
    ifs.close();
  }


  // reading OHLC files
  std::cout << "reading OHLC files " << std::endl;
  KOHLCStorage kohlcs;
  for(const std::string &str: ohlc_files){
    std::ifstream ifs(path+str,std::fstream::binary);
    std::cout << "reading file " << path + str << std::endl;
    if(!ifs.is_open()){
      std::cout << "Error opening ohlc file " << path + str << std::endl;
      continue;
    }
    while(!ifs.eof()) {
      ifs >> kohlcs;
    }
    ifs.close();
  }

  // reading spread files
  std::cout << "reading spread files " << std::endl;
  KSpreadStorage kss;
  for(const std::string &str: spread_files){
    std::ifstream ifs(path+str,std::fstream::binary);
    std::cout << "reading file " << path + str << std::endl;
    if(!ifs.is_open()){
      std::cout << "Error opening spread file " << path + str << std::endl;
      continue;
    }
    while(!ifs.eof()) {
      ifs >> kss;
    }
    ifs.close();
  }

  std::cout << "total spreads : " << kss.size() << std::endl;

  /***************************************************/
  Gnuplot gp(">script.gp");
  // Create a script which can be manually fed into gnuplot later:
  //    Gnuplot gp(">script.gp");
  // Create script and also feed to gnuplot:
  //    Gnuplot gp("tee plot.gp | gnuplot -persist");
  // Or choose any of those options at runtime by setting the GNUPLOT_IOSTREAM_CMD
  // environment variable.

  // Gnuplot vectors (i.e. arrows) require four columns: (x,y,dx,dy)
  std::vector<boost::tuple<double, double, double, double> > pts_A;
  std::vector<boost::tuple<double, double, double, double> > points;
  // You can also use a separate container for each column, like so:
  std::vector<double> pts_B_x;
  std::vector<double> pts_B_y;
  std::vector<double> pts_B_dx;
  std::vector<double> pts_B_dy;

  // You could also use:
  //   std::vector<std::vector<double> >
  //   boost::tuple of four std::vector's
  //   std::vector of std::tuple (if you have C++11)
  //   arma::mat (with the Armadillo library)
  //   blitz::Array<blitz::TinyVector<double, 4>, 1> (with the Blitz++ library)
  // ... or anything of that sort

  for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
    double theta = alpha*2.0*3.14159;
    pts_A.push_back(boost::make_tuple(
                                      cos(theta),
                                      sin(theta),
                                      -cos(theta)*0.1,
                                      -sin(theta)*0.1
                                      ));

    pts_B_x .push_back( cos(theta)*0.8);
    pts_B_y .push_back( sin(theta)*0.8);
    pts_B_dx.push_back( sin(theta)*0.1);
    pts_B_dy.push_back(-cos(theta)*0.1);
  }

  // Don't forget to put "\n" at the end of each line!
  gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
  // '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
  gp << "plot '-' with vectors title 'pts_A', '-' with vectors title 'pts_B'\n";
  gp.send1d(pts_A);
  gp.send1d(boost::make_tuple(pts_B_x, pts_B_y, pts_B_dx, pts_B_dy));

#ifdef _WIN32
  // For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
  // the gnuplot window doesn't get closed.
  std::cout << "Press enter to exit." << std::endl;
  std::cin.get();
#endif
  /***************************************************/

  return EXIT_SUCCESS;
}
