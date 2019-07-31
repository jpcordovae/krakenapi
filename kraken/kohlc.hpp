#ifndef KOHLC_HPP_
#define KOHLC_HPP_

#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <string>
#include "../libjson/libjson.h"
#include "kutils.hpp"

using namespace std;

namespace kraken{

  struct KOHLC{
    time_t ktime;
    float open;
    float high;
    float low;
    float close;
    float vwap;
    float volume;
    float count;

	KOHLC() = default;
	
    KOHLC(const JSONNode &node) {
      ktime = (time_t)(node[0].as_int());
      open = node[1].as_float();
      high = node[2].as_float();
      low  = node[3].as_float();
      close = node[4].as_float();
      vwap = node[5].as_float();
      volume = node[6].as_float();
      count = node[7].as_float();
    }
  };
  
  typedef std::vector<KOHLC> KOHLCStorage;
  // KOHLC operatorrs
  std::ostream& operator<<(std::ostream &os, const KOHLC &kohlc);
  std::ofstream& operator<<(std::ofstream &ifs,const KOHLC &kohlc);
  std::ifstream& operator>>(std::ifstream &ifs, KOHLC &kohlc);
  
  // KOHLCStorage operators
  std::ostream& operator<<(std::ostream &os, const KOHLCStorage &KS);
  std::ofstream& operator<<(std::ofstream &ofs, const KOHLCStorage & kohlcs);
  std::ifstream& operator>>(std::ifstream &ifs, const KOHLCStorage & kohlcs);
  
};

#endif
