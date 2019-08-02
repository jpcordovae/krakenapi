#ifndef _KRAKEN_KTRADE_HPP_
#define _KRAKEN_KTRADE_HPP_

#include <string>
#include <sstream>
#include <vector>
#include "../libjson/libjson.h"
#include "kutils.hpp"

//------------------------------------------------------------------------------

namespace kraken {

  //------------------------------------------------------------------------------
  // deals with recent trade data:
  struct KTrade {

    enum Otype_t { MARKET='m', LIMIT='l' };
    enum Order_t { BUY='b', SELL='s' };

    double price, volume;
    time_t time;
    Otype_t otype;
    Order_t order;
    std::string misc;

    // default ctor
    KTrade() :price(0), volume(0), time(0),
              otype(KTrade::MARKET),
              order(KTrade::BUY) { }

    // construct from a JSONNode
    KTrade(JSONNode node);
  };

  typedef std::vector<KTrade> KTradeStorage;

  //------------------------------------------------------------------------------
  // helper function to print a KTrade
  std::ostream& operator<<(std::ostream& os, const KTrade& kt);
  std::ostream& operator<<(std::ostream& os, const KTradeStorage &kts);

  //------------------------------------------------------------------------------
  // wrtie KTrade and KTradeStorage to a binary file
  std::ofstream& operator<<(std::ofstream &ofs, const KTrade& kt);
  std::ofstream& operator<<(std::ofstream &ofs, const KTradeStorage &kts);
  std::ifstream& operator>>(std::ifstream &ifs, KTrade &kt);
  std::ifstream& operator>>(std::ifstream &ifs, KTradeStorage &kts);

  //------------------------------------------------------------------------------
}; // namespace Kraken

//------------------------------------------------------------------------------

#endif
