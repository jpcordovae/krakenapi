#ifndef KORDER_H_
#define KORDER_H_

#include <string>
#include "../libjson/libjson.h"

namespace kraken{

  struct korder{

    enum Order_t { BUY='b', SELL='s' };
    
    double price;
    double volume;
    time_t time;

    Order_t otype;
    std::string misc;
    
    korder():price(0),volume(0),time(0){}

    korder(JSONNode node){

    }
    
  };

};

#endif // KORDER_H_
