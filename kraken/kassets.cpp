#include "kassets.hpp"

namespace kraken {

  //------------------------------------------------------------------------------
  // prints a KAsset:
  std::ostream& operator<<(std::ostream& os, const KAsset& a)
  {
    return os << '"' 
	      << a.name << "\",\""
	      << a.altname << "\",\""
	      << a.aclass << "\",\""
	      << a.decimals << "\",\""
	      << a.display_decimals 
	      << '"';
	      }
  
  //------------------------------------------------------------------------------
  // prints a KAsset_pair:
  std::ostream& operator<<(std::ostream& os, const KAsset_pair& a)
  {
    return os << '"' 
	      << a.name << "\",\""
	      << a.altname << "\",\""
	      << a.aclass_base << "\",\""
	      << a.base << "\",\""
	      << a.aclass_quote << "\",\""
	      << a.quote << "\",\""
	      << a.lot << "\",\""
	      << a.pair_decimals << "\",\""
	      << a.lot_decimals << "\",\""
	      << a.lot_multiplier << "\",\""
	      << a.fees << "\",\""
	      << a.fee_volume_currency << "\",\""
	      << a.margin_call << "\",\""
	      << a.margin_stop 
	      << '"';
	      }
  
};
