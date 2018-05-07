#ifndef KORDER_H_
#define KORDER_H_

#include <string>
#include <iostream>
#include <vector>

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

  std::ostream& operator<<(std::ostream &os,const KOrder &ko);
  
  typedef std::vector<KOrder> KOrders;
  
  std::vector<double> get_prices(KOrders &kos);
  std::vector<double> get_volumes(KOrders &kos);
  std::vector<time_t> get_times(KOrders &kos);
  
};

#endif // KORDER_H_
