#ifndef KORDER_H_
#define KORDER_H_

#include <string>
#include "../libjson/libjson.h"

namespace kraken{

  struct KOrder{

    enum Order_t { BUY='b', SELL='s' };
    
    double price;
    double volume;
    time_t time;

    Order_t otype;
    std::string misc;
    
    KOrder():price(0),volume(0),time(0){}

    KOrder(JSONNode node){
      price = node[0].as_float();
      volume = node[1].as_float();
      time = node[2].as_int();
    }
    
  };

};

#endif // KORDER_H_
