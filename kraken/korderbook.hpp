#ifndef KORDERBOOK_H_
#define KORDERBOOK_H_

#include <string>

#include "korder.hpp"
#include <iostream>
#include <vector>
#include "kutils.hpp"

namespace kraken
{
  struct KOrderBook
  {
    std::string pair;
    KOrders lBids;
    KOrders lAsks;
    
    //default constructor
    KOrderBook(){}
    
    // from pair JSONNode
    KOrderBook(JSONNode node) {
      if(node.empty()) return;
      std::string sTmp;
      for(JSONNode::iterator it=node.begin();it != node.end(); ++it){
	sTmp = libjson::to_std_string(it->name());
	if(sTmp.compare("lAsks")==0) {
	  lAsks.push_back(*it);
	}else
	  if(sTmp.compare("lBids")==0){
	    lBids.push_back(*it);
	  }
      }
      //pair = libjson::as_std_string(node[0].as_string());
    }
  };
  
  typedef std::vector<KOrderBook> KOBStorage;
  
  void clear_korderbook(KOrderBook &_kob);
  
  std::ostream& operator<<(std::ostream &oss,KOrderBook &kob);
  std::ofstream& operator<<(std::ofstream &ofs, const KOrderBook &kob);
  std::ifstream& operator>>(std::ifstream &ifs, KOrderBook &kob);

  std::ofstream& operator<<(std::ofstream &ofs, const KOBStorage &kobs);
  std::ifstream& operator>>(std::ifstream &ifs, KOBStorage &kobs);
  
};

#endif
