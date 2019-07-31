#include "kohlc.hpp"

namespace kraken {

  std::ostream& operator<<(std::ostream &os, const KOHLC &kohlc) {
    os << to_string(kohlc.ktime) << " ; "
       << kohlc.open << " ; "
       << kohlc.close << " ; "
       << kohlc.high << " ; "
       << kohlc.low << " ; "
       << kohlc.vwap << " ; "
       << kohlc.volume << " ; "
       << kohlc.count;
    return os;
  }

  std::ostream& operator<<(std::ostream &os, const KOHLCStorage &KS) {
    std::for_each(KS.begin(),KS.end(),[&os](const KOHLC &kohlcs){
        os << kohlcs << endl;
      });
    return os;
  }
};
