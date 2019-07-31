#ifndef KSPREAD_HPP_
#define KSPREAD_HPP_

#include <ostream>
#include <vector>
#include <string>
#include "../libjson/libjson.h"
#include <ctime>
#include "kutils.hpp"

namespace kraken{

  struct KSpread{
    std::time_t time;
    float bid;
    float ask;

    KSpread(const JSONNode &node){
      time = node[0].as_int();
      bid = node[1].as_float();
      ask = node[2].as_float();
    }

    KSpread():time(0),bid(0.0f),ask(0.0f) { }
  };// struct KSpread

  typedef std::vector<KSpread> KSpreadStorage;

  std::ostream& operator<<(std::ostream &os, const KSpread &ks);

  // KSpread operators
  std::ofstream& operator<<(std::ofstream &ofs, const KSpread &ks);
  std::ifstream& operator>>(std::ifstream &ifs, KSpread &ks);

  // KSpreadStorage operators
  std::ofstream& operator<<(std::ofstream &ofs, const KSpreadStorage &kss);
  std::ifstream& operator>>(std::ifstream &ifs, KSpreadStorage &kss);

};// namespace kraken

#endif
