#ifndef KORDER_H_
#define KORDER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "../libjson/libjson.h"
#include "kutils.hpp"

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

  std::ostream& operator<<(std::ostream &os, const KOrder &ko);
  std::ofstream& operator<<(std::ofstream &ofs, const KOrder &ko);
  std::ifstream& operator>>(std::ifstream &ifs, KOrder &ko);

  typedef std::vector<KOrder> KOrders;

  std::ostream& operator<<(std::ostream &os, const KOrders &kos);
  std::ofstream& operator<<(std::ofstream &ofs, const KOrders &kos);
  std::ifstream& operator>>(std::ifstream &ifs, KOrders &kos);
  
  std::vector<double> get_prices(KOrders &kos);
  std::vector<double> get_volumes(KOrders &kos);
  std::vector<time_t> get_times(KOrders &kos);
  
};

#endif // KORDER_H_
