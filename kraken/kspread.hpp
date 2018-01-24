#ifndef KSPREAD_HPP_
#define KSPREAD_HPP_

#include <chrono>

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
  
};// namespace kraken

#endif
