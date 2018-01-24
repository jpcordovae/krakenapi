#ifndef KOHLC_HPP_
#define KOHLC_HPP_

#include <iostream>
#include <vector>

namespace kraken{

  struct KOHLC{
    std::time_t time;
    float open;
    float high;
    float low;
    float close;
    float vwap;
    float volume;
    float count;
    
    KOHLC(const JSONNode &node){
      time = node[0].as_int();
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

  /*std::ostream &operator<<(std::ostream &os,const KOHLC &kohlc){
    return os << kohlc.time << " ; "
	      << kohlc.open << " ; "
	      << kohlc.close << " ; "
	      << kohlc.high << " ; "
	      << kohlc.low << " ; "
	      << kohlc.vwap << " ; "
	      << kohlc.volume << " ; "
	      << kohlc.count;
	      }*/
  
};

#endif
