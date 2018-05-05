#include <iostream>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>

#include "kraken/kclient.hpp"

using namespace std;
using namespace kraken;
//------------------------------------------------------------------------------

typedef struct client_data{
  KInput in;
  KClient client;
} client_data;

client_data CD;
mutex mtxCD;

void update_1_min( string _asset )
{
  time_t last_timestamp = 0;
  KOrderBook kob;
  try{
    while(1){
      #ifdef KAPI_DEBUG_
      cout << _asset << " ORDERBOOK updated at " << last_timestamp << endl;
      #endif
      mtxCD.lock();
      CD.client.orderbook(_asset,to_string(last_timestamp),kob);
      mtxCD.unlock();
      this_thread::sleep_for(chrono::milliseconds(60*1000));// 60seconds
    }
  }
  catch(exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
  catch(...) {
    cerr << "Unknow exception." << endl;
  }
}

int main(int argc, char **argv) 
{ 
   curl_global_init(CURL_GLOBAL_ALL);
   cout << "starting 1 min thread..." << endl;
   thread one_min_thread(update_1_min,"ETHUSD");
   
   try {  
     //**************************************************
     
     //**************************************************
     /*cout << endl;
     cout << "TRADES" << endl;
     std::vector<KTrade> ktrades;
     cout << client.trades("ETHUSD","0",ktrades) << endl;
     */
     //*************************************************
     /*cout << endl;
     cout << "SPREAD" << endl;
     KSpreadStorage kss;
     cout << client.spread("ETHUSD","0", kss) << endl;
     */
     //************************************************
     /*cout << endl;
     cout << "ASSETS" << endl;
     KAssetsMap kam;
     cout << client.update_assets(kam) << endl;
     */
     //************************************************
     /*cout << endl;
     cout << "OHLC" << endl;
     KOHLCStorage kohlcs;
     cout << client.OHLC("ETHUSD","0","1",kohlcs) << endl;
     */
     //************************************************
     /*cout << endl;
     cout << "SERVER TIME" << endl;
     std::time_t stime;
     cout << client.server_time(stime) << endl;
     */
   }
   catch(exception& e) {
      cerr << "Error: " << e.what() << endl;
   }
   catch(...) {
      cerr << "Unknow exception." << endl;
   }
   cin.get();
   curl_global_cleanup();
   return 0;
}
 
