#ifndef KSPREAD_HPP_
#define KSPREAD_HPP_

#include <ostream>
#include <vector>
#include <string>
#include "../libjson/libjson.h"
#include <ctime>

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
  };// struct KSpread

  typedef std::vector<KSpread> KSpreadStorage;

  std::ostream& operator<<(std::ostream &os, const KSpread &ks);
  //std::ostream& operator<<(std::ostream &os, const KSpreadStorage &kss);
  
};// namespace kraken

#endif
