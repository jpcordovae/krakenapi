#ifndef KORDERBOOK_H_
#define KORDERBOOK_H_

#include <list>
#include <string>

#include "korder.hpp"

namespace kraken{

  struct KOrderBook{
    
    std::string pair;
    std::list<KOrder> lBids;
    std::list<KOrder> lAsks;

    std::string misc;

    //default constructor
    KOrderBook(){}

    // from pair JSONNode
    KOrderBook(JSONNode node){
      if(node.empty()) return;
      std::string sTmp;
      for(JSONNode::iterator it=node.begin();it != node.end(); ++it){
	sTmp = libjson::to_std_string(it->name());
	if(sTmp.compare("lAsks")==0){
	  
	}else
	  if(sTmp.compare("lBids")==0){
	    
	  }
      }
      //pair = libjson::as_std_string(node[0].as_string());
    }
    
  };

};

#endif
