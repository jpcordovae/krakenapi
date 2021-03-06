#include <iostream>
#include <stdexcept>
#include <chrono>
#include <ctime>

#include "kraken/kclient.hpp"

using namespace std;
using namespace kraken;

//------------------------------------------------------------------------------

int main() 
{ 
   curl_global_init(CURL_GLOBAL_ALL);

   try {
     
     KInput in;
     KClient client;

     //**************************************************
     cout << endl;
     cout << "ORDERBOOK" << endl;
     KOrderBook kob;
     cout << client.orderbook("ETHUSD",kob) << endl;
     //**************************************************
     cout << endl;
     cout << "TRADES" << endl;
     std::vector<KTrade> ktrades;
     cout << client.trades("ETHUSD","0",ktrades) << endl;
     //*************************************************
     cout << endl;
     cout << "SPREAD" << endl;
     KSpreadStorage kss;
     cout << client.spread("ETHUSD","0", kss) << endl;
     //************************************************
     cout << endl;
     cout << "ASSETS" << endl;
     KAssetsMap kam;
     cout << client.update_assets(kam) << endl;
     //************************************************
     cout << endl;
     cout << "OHLC" << endl;
     KOHLCStorage kohlcs;
     client.OHLC("ETHUSD","0","1",kohlcs);
     cout << kohlcs << endl;
     //************************************************
     cout << endl;
     cout << "SERVER TIME" << endl;
     std::time_t stime;
     cout << client.server_time(stime) << endl;
     //************************************************
     cout << endl;
     cout << "ASSET PAIRS" << endl;
     KAssetPairs kaps;
     client.asset_pairs(kaps);
     cout << kaps << endl;

   }
   catch(exception& e) {
      cerr << "Error: " << e.what() << endl;
   }
   catch(...) {
      cerr << "Unknow exception." << endl;
   }

   curl_global_cleanup();

   return 0;
}
 
