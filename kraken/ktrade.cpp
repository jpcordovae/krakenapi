#include <iomanip>
#include "ktrade.hpp"

//------------------------------------------------------------------------------

namespace kraken
{

  //------------------------------------------------------------------------------
  // construct from a JSONNode:
  KTrade::KTrade(JSONNode node)
  {
    price  = node[0].as_float();
    volume = node[1].as_float();
    std::istringstream( libjson::to_std_string(node[2].as_string()) ) >> time;

    order = static_cast<Order_t>
      ( libjson::to_std_string(node[3].as_string())[0] );
    otype = static_cast<Otype_t>
      ( libjson::to_std_string(node[4].as_string())[0] );

    misc = libjson::to_std_string(node[5].as_string());
  }

  //------------------------------------------------------------------------------
  // prints out a KTrade:
  std::ostream& operator<<(std::ostream& os, const KTrade& kt)
  {
    return os << '"' << kt.time << "\",\""
              << static_cast<char>(kt.order) << "\",\""
              << static_cast<char>(kt.otype) << "\",\""
              << std::fixed
              << std::setprecision(5) << kt.price << "\",\""
              << std::setprecision(9) << kt.volume << '"';
  }

  //-----------------------------------------------------------------------------

  std::ofstream& operator<<(std::ofstream &ofs, const KTrade& kt)
  {
    ofs.write((char*)&kt.price,sizeof(double));
    ofs.write((char*)&kt.volume,sizeof(double));
    ofs.write((char*)&kt.time,sizeof(time_t));
    ofs.write((char*)&kt.otype,sizeof(KTrade::Otype_t));
    ofs.write((char*)&kt.order,sizeof(KTrade::Order_t));
    ofs << kt.misc;
    return ofs;
  }

  //-----------------------------------------------------------------------------

  std::ofstream& operator<<(std::ofstream &ofs, const KTradeStorage &kts)
  {
    size_t size = kts.size();
    ofs.write((char*)&size,sizeof(size_t));
    for(size_t k = 0;k<kts.size();k++){
      ofs << kts[k];
    }
    return ofs;
  }


  //------------------------------------------------------------------------------

  std::ifstream& operator>>(std::ifstream &ifs, KTradeStorage &kts)
  {
    size_t size;
    ifs.read((char*)&size,sizeof(size_t));
    for(size_t k = 0;k<size;k++){
      ifs >> kts[k];
    }
    return ifs;
  }

  //----------------------------------------------------------------------------
  std::ifstream& operator>>(std::ifstream &ifs, KTrade &kt)
  {
    ifs.read((char*)&kt.price,sizeof(double));
    ifs.read((char*)&kt.volume,sizeof(double));
    ifs.read((char*)&kt.time,sizeof(time_t));
    ifs.read((char*)&kt.otype,sizeof(KTrade::Otype_t));
    ifs.read((char*)&kt.order,sizeof(KTrade::Order_t));
    ifs >> kt.misc;
    return ifs;
  }



}; // namespace Kraken
