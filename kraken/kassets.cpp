#include "kassets.hpp"
#include <algorithm>

namespace kraken {

  //------------------------------------------------------------------------------
  // prints a KAsset:
  std::ostream& operator<<(std::ostream& os, const KAsset& a)
  {
    return os << '\"' 
	      << a.name << "\",\""
	      << a.altname << "\",\""
	      << a.aclass << "\",\""
	      << std::to_string(a.decimals) << "\",\""
	      << std::to_string(a.display_decimals) 
	      << '\"';
	      }
  
  //------------------------------------------------------------------------------
  // prints a KAssetPair
  std::ostream& operator<<(std::ostream& os, const KAssetPair& a)
  {
    return os << a.name << ","
	      << a.altname << ","
	      << a.aclass_base << ","
	      << a.base << ","
	      << a.aclass_quote << ","
	      << a.quote << ","
	      << a.lot << ","
	      << std::to_string(a.pair_decimals) << ","
	      << std::to_string(a.lot_decimals) << ","
	      << std::to_string(a.lot_multiplier) << ","
	      << a.fees << ","
	      << a.fee_volume_currency << ","
	      << std::to_string(a.margin_call) << ","
	      << std::to_string(a.margin_stop);
  }

  //------------------------------------------------------------------------------
  // prints a KAssetPair
  std::ostream& operator<<(std::ostream& os, const KAssetPairs& kaps)
  {
    KAssetPairs::const_iterator it;
    for(it=kaps.begin();it!=kaps.end();++it){
      os << *it << endl;
    }
    return os;
  }
  
};
