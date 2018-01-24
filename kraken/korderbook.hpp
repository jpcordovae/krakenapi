#ifndef KORDERBOOK_H_
#define KORDERBOOK_H_

#include <list>
#include <string>

#include "korder.hpp"

namespace kraken{

  struct korderbook{
    
    std::string pair;
    std::list<korder> lBids;
    std::list<korder> lAsks;

    double price;
    double volume;
    time_t time;
    std::string misc;

    //default constructor
    korderbook(): price(0), volume(0), time(0){}

    // from pair JSONNode
    korderbook(JSONNode node){

      if(node.empty()) return;
      
      pair = libjson::as_std_string(node[0].as_string());

    }

    
  };

};

#endif
