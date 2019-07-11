#include "korderbook.hpp"
#include <algorithm>

namespace kraken {

void clear_korderbook(KOrderBook &_kob)
{
  _kob.pair.clear();
  _kob.lBids.clear();
  _kob.lAsks.clear();
}

std::ostream& operator<<(std::ostream &oss, KOrderBook &kob)
{
  oss << kob.pair << std::endl;
  oss << "BIDS" << std::endl;
  KOrders::iterator it;
  oss << "Bids" << std::endl;
  for(it=kob.lBids.begin();it!=kob.lBids.end();++it){
    oss << *it << std::endl;
  }
  oss << "Asks" << std::endl;
  for(it=kob.lAsks.begin();it!=kob.lAsks.end();++it){
    oss << *it << std::endl;
  }
  
  return oss;
}  
  
};
