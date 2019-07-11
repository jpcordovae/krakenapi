#ifndef KASSETS_HPP_
#define KASSETS_HPP_

#include <string>
#include <map>
#include <iostream>
#include <vector> 
#include "../libjson/libjson.h"

using namespace std;

namespace kraken {
  
  //--------------------------------------------------------------------
  // deals with Kraken assets:
  struct KAsset {
    string name;
    string altname;
    string aclass;
    int decimals;
    int display_decimals;
    // construct a KAssets from an JSONNode:
    KAsset(const JSONNode& node) {
      name = libjson::to_std_string(node.name());
      altname = node["altname"].as_string();
      aclass = node["aclass"].as_string();
      decimals = node["decimals"].as_int();
      display_decimals = node["display_decimals"].as_int();
    }
  };//struct KAsset
  
  //--------------------------------------------------------------------
  // prints a KAsset:
  std::ostream& operator<<(std::ostream& os, const KAsset& a);

  //--------------------------------------------------------------------
  // deals with Kraken assets:
  struct KAssetPair {
    string name;
    string altname;
    string aclass_base;
    string base;
    string aclass_quote; 
    string quote;
    string lot;
    int pair_decimals;
    int lot_decimals;
    int lot_multiplier;
    // leverage ...
    string fees; 
    string fee_volume_currency;
    int margin_call; 
    int margin_stop;
    // construct a KAssets from an JSONNode:
    KAssetPair(const JSONNode& node) {
      name = libjson::to_std_string( node.name() );
      altname = libjson::to_std_string( node["altname"].as_string() );
      aclass_base = libjson::to_std_string( node["aclass_base"].as_string() );
      base = libjson::to_std_string( node["base"].as_string() );
      aclass_quote = libjson::to_std_string( node["aclass_quote"].as_string() );
      quote = libjson::to_std_string( node["quote"].as_string() );
      lot = libjson::to_std_string( node["lot"].as_string() );
      pair_decimals = node["pair_decimals"].as_int();
      lot_decimals = node["lot_decimals"].as_int();
      lot_multiplier = node["lot_multiplier"].as_int();
      fees = libjson::to_std_string( node["fees"].as_string() );
      fee_volume_currency = libjson::to_std_string( node["fee_volume_currency"].as_string() );
      margin_call = node["margin_call"].as_int();
      margin_stop = node["margin_stop"].as_int();
    };
  };// END KAssetPair

  typedef std::vector<KAssetPair> KAssetPairs;

  //--------------------------------------------------------------
  // prints a KAsset_pair:
  std::ostream& operator<<(std::ostream& os, const KAssetPair& a);
  std::ostream& operator<<(std::ostream& os, const KAssetPairs& kaps);
};//namespace kraken

#endif
