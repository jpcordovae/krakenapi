#ifndef _KRAKEN_KCLIENT_HPP_
#define _KRAKEN_KCLIENT_HPP_

#include <map>
#include <string>
#include <vector>
#include <curl/curl.h>

#include "ktrade.hpp"
#include "korderbook.hpp"
#include "kassets.hpp"
#include "kspread.hpp"
#include "kohlc.hpp"

//------------------------------------------------------------------------------

namespace kraken {
  
  //------------------------------------------------------------------------------
  // helper type to make requests
  typedef std::map<std::string,std::string> KInput;
  typedef std::map<std::string,KAsset> KAssetsMap;
  //------------------------------------------------------------------------------
  
  class KClient {
  public:  
    
    // constructor with all explicit parameters
    KClient(const std::string& key, const std::string& secret, 
	    const std::string& url, const std::string& version);
    
    // default API base URL and API version
    KClient(const std::string& key, const std::string& secret);
    
    // constructor with empty API key and API secret
    KClient();
    
    // distructor
    ~KClient();
    
    // makes public method to kraken.com 
    std::string public_method(const std::string& method,
			      const KInput& input) const;
    
    // makes private method to kraken.com
    std::string private_method(const std::string& method,
			       const KInput& input) const;
    
    // returns recent Kraken recent trade data
    std::string trades(const std::string& pair,
		       const std::string& since,
		       std::vector<KTrade>& output);
    
    std::string orderbook(const std::string &pair,
			  const std::string &since,
			  KOrderBook &kob);
    
    std::string update_server_time();
    
    std::string update_assets(KAssetsMap &kam);
    
    void print_assets(const KAssetsMap &kam);
    
    // return last
    std::string spread(const std::string &pair,
		       const std::string &since,
		       KSpreadStorage &output);
    
    std::string OHLC(const std::string &pair, const std::string &since,
		     const std::string &interval, KOHLCStorage &ohlcs);
    
  private:
    // init CURL and other stuffs 
    void init();
    
    // TODO: gather common commands from public_method and 
    // private_method in a single method: curl_perfom
    
    // create signature for private requests
    std::string signature(const std::string& path,
			  const std::string& nonce,
			  const std::string& postdata) const;
    
    // CURL writefunction callback
    static size_t write_cb(char* ptr, size_t size, 
			   size_t nmemb, void* userdata);
    
    std::string key_;     // API key
    std::string secret_;  // API secret
    std::string url_;     // API base URL
    std::string version_; // API version
    CURL*  curl_;         // CURL handle
    
    // disallow copying
    KClient(const KClient&);
    KClient& operator=(const KClient&);
    
    //server time and difference from now()
    std::time_t local_time;  // local time at update_server_time() moment
    std::time_t utc_time;    // UTC timeat update_server_time() moment
    std::time_t server_time; // server time
    
  };
  
  //------------------------------------------------------------------------------
  // helper functions to initialize and terminate Kraker API library.
  // KClient uses CURL, the latter has a no thread-safe function called 
  // curl_global_init(). 
  
  void initialize();
  void terminate();
  
  //------------------------------------------------------------------------------
  
}; // namespace Kraken

//------------------------------------------------------------------------------

#endif

