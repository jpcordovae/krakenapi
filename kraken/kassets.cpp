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

  std::ofstream& operator<<(std::ofstream& ofs, const KAssetPair &kap)
  {
    ofs << kap.name << kap.altname << kap.aclass_base << kap.base
        << kap.aclass_base << kap.quote << kap.lot;
    ofs.write((char*)&kap.pair_decimals,sizeof(int));
    ofs.write((char*)&kap.lot_decimals,sizeof(int));
    ofs.write((char*)&kap.lot_multiplier,sizeof(int));
    ofs << kap.fees << kap.fee_volume_currency;
    ofs.write((char*)&kap.margin_call,sizeof(int));
    ofs.write((char*)&kap.margin_stop,sizeof(int));
    return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KAssetPair &kap)
  {
    ifs >> kap.name >> kap.altname >> kap.aclass_base >> kap.base
        >> kap.aclass_base >> kap.quote >> kap.lot;
    ifs.read((char*)&kap.pair_decimals,sizeof(int));
    ifs.read((char*)&kap.lot_decimals,sizeof(int));
    ifs.read((char*)&kap.lot_multiplier,sizeof(int));
    ifs >> kap.fees >> kap.fee_volume_currency;
    ifs.read((char*)&kap.margin_call,sizeof(int));
    ifs.read((char*)&kap.margin_stop,sizeof(int));
    return ifs;
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KAssetPairs &kaps)
  {
    size_t size = kaps.size();
    ofs.write((char*)&size,sizeof(size_t));
    for(size_t k=0;k<kaps.size();k++){
      ofs << kaps[k];
    }
    return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KAssetPairs &kaps)
  {
    size_t size;
    ifs.read((char*)&size,sizeof(size_t));
    for(size_t k=0;k<size;k++){
      ifs >> kaps[k];
    }
    return ifs;
  }

};
