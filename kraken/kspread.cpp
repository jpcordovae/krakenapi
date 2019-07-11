#include "kspread.hpp"

namespace kraken {

 std::ostream& operator<<(std::ostream &os, const KSpread &ks)
{
  os << std::to_string(ks.time)
     << " , " << ks.bid
     << " , " << ks.ask;
  return os;
}
  
   /*std::ostream& operator<<(std::ostream &os, const KSpreadStorage &kss)
{
  KSpreadStorage::iterator it;
  for(it=kss.begin();it!=kss.end();++it){
    os << *it << std::endl;
    }
  return os;
  }*/

};//namespace kraken
