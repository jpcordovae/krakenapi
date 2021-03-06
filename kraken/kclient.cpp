#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cerrno>
#include <map>

#include <chrono>

#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>

#include "kclient.hpp"
#include "kassets.hpp"
#include "kspread.hpp"
#include "../libjson/libjson.h"

#define CURL_VERBOSE 0L //1L = enabled, 0L = disabled

//------------------------------------------------------------------------------

namespace kraken {
//------------------------------------------------------------------------------
// helper function to compute SHA256:
static std::vector<unsigned char> sha256(const std::string& data)
{
  std::vector<unsigned char> digest(SHA256_DIGEST_LENGTH);

  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  SHA256_Update(&ctx, data.c_str(), data.length());
  SHA256_Final(digest.data(), &ctx);
  
  return digest;
}
  
//------------------------------------------------------------------------------
// helper function to decode a base64 string to a vector of bytes:
static std::vector<unsigned char> b64_decode(const std::string& data) 
{
   BIO* b64 = BIO_new(BIO_f_base64());
   BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	
   BIO* bmem = BIO_new_mem_buf((void*)data.c_str(),data.length());
   bmem = BIO_push(b64, bmem);
   
   std::vector<unsigned char> output(data.length());
   int decoded_size = BIO_read(bmem, output.data(), output.size());
   BIO_free_all(bmem);

   if (decoded_size < 0)
      throw std::runtime_error("failed while decoding base64.");
   
   return output;
}

//------------------------------------------------------------------------------
// helper function to encode a vector of bytes to a base64 string:
static std::string b64_encode(const std::vector<unsigned char>& data) 
{
   BIO* b64 = BIO_new(BIO_f_base64());
   BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

   BIO* bmem = BIO_new(BIO_s_mem());
   b64 = BIO_push(b64, bmem);
   
   BIO_write(b64, data.data(), data.size());
   BIO_flush(b64);

   BUF_MEM* bptr = NULL;
   BIO_get_mem_ptr(b64, &bptr);
   
   std::string output(bptr->data, bptr->length);
   BIO_free_all(b64);

   return output;
}

//------------------------------------------------------------------------------
// helper function to hash with HMAC algorithm:
 static std::vector<unsigned char> 
 hmac_sha512(const std::vector<unsigned char>& data, 
	     const std::vector<unsigned char>& key)
{   
   unsigned int len = EVP_MAX_MD_SIZE;
   std::vector<unsigned char> digest(len);

   //HMAC_CTX *ctx;
   //HMAC_CTX_init(ctx);
   HMAC_CTX *ctx = HMAC_CTX_new();

   HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha512(), NULL);
   HMAC_Update(ctx, data.data(), data.size());
   HMAC_Final(ctx, digest.data(), &len);
   
   //HMAC_CTX_cleanup(ctx);
   HMAC_CTX_free(ctx);
   
   return digest;
}


//------------------------------------------------------------------------------
// builds a query string from KAPI::Input (a=1&b=2&...)
static std::string build_query(const KInput& input)
{
   std::ostringstream oss;
   KInput::const_iterator it = input.begin();
   for (; it != input.end(); ++it) {
      if (it != input.begin()) oss << '&';  // delimiter
      oss << it->first <<'='<< it->second;
   }

   return oss.str();
}

//------------------------------------------------------------------------------
// helper function to create a nonce:
static std::string create_nonce()
{
   std::ostringstream oss;

   timeval tp;
   if (gettimeofday(&tp, NULL) != 0) {
      oss << "gettimeofday() failed: " << strerror(errno); 
      throw std::runtime_error(oss.str());
   }
   else {
      // format output string 
      oss << std::setfill('0') 
	  << std::setw(10) << tp.tv_sec 
	  << std::setw(6)  << tp.tv_usec;
   }
   return oss.str();
}

//------------------------------------------------------------------------------
// constructor with all explicit parameters
KClient::KClient(const std::string& key, const std::string& secret, 
		 const std::string& url, const std::string& version)
  :key_(key), secret_(secret), url_(url), version_(version) 
{ 
   init(); 
}

//------------------------------------------------------------------------------
// default API base URL and API version
KClient::KClient(const std::string& key, const std::string& secret)
  :key_(key), secret_(secret), url_("https://api.kraken.com"), version_("0") 
{ 
   init(); 
}

//------------------------------------------------------------------------------
// constructor with empty API key and API secret
KClient::KClient() 
   :key_(""), secret_(""), url_("https://api.kraken.com"), version_("0") 
{ 
   init(); 
}

//------------------------------------------------------------------------------
// initializes libcurl:
void KClient::init()
{
   curl_ = curl_easy_init();
   if (curl_) {
      curl_easy_setopt(curl_, CURLOPT_VERBOSE, CURL_VERBOSE);
      curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
      curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 2L);
      curl_easy_setopt(curl_, CURLOPT_USERAGENT, "Kraken C++ API Client");
      curl_easy_setopt(curl_, CURLOPT_POST, 1L);
      // set callback function 
      curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, KClient::write_cb);
   }
   else {
      throw std::runtime_error("can't create curl handle");
   }
}

//------------------------------------------------------------------------------
// distructor:
KClient::~KClient() 
{
   curl_easy_cleanup(curl_);
}

//------------------------------------------------------------------------------
// returns message signature generated from a URI path, a nonce 
// and postdata, message signature is created as a follows:
// 
//   hmac_sha512(path + sha256(nonce + postdata), b64decode(secret)) 
//
// and the result is converted in a base64 string: 
std::string KClient::signature(const std::string& path, 
			       const std::string& nonce, 
			       const std::string& postdata) const
{
   // add path to data to encrypt
   std::vector<unsigned char> data(path.begin(), path.end());

   // concatenate nonce and postdata and compute SHA256
   std::vector<unsigned char> nonce_postdata = sha256(nonce + postdata);

   // concatenate path and nonce_postdata (path + sha256(nonce + postdata))
   data.insert(data.end(), nonce_postdata.begin(), nonce_postdata.end());

   // and compute HMAC
   return b64_encode( hmac_sha512(data, b64_decode(secret_)) );
}

//------------------------------------------------------------------------------
// CURL write function callback:
size_t KClient::write_cb(char* ptr, size_t size, size_t nmemb, void* userdata)
{
   std::string* response = reinterpret_cast<std::string*>(userdata);
   size_t real_size = size * nmemb;

   response->append(ptr, real_size);
   return real_size;
}

//------------------------------------------------------------------------------
// deals with public API methods:
std::string KClient::public_method(const std::string& method, 
				   const KInput& input) const
{
   // build method URL
   std::string path = "/" + version_ + "/public/" + method;
   std::string method_url = url_ + path;   
   curl_easy_setopt(curl_, CURLOPT_URL, method_url.c_str());

   // build postdata 
   std::string postdata = build_query(input);
   curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, postdata.c_str());

   // reset the http header
   curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, NULL);

   // where CURL write callback function stores the response
   std::string response;
   curl_easy_setopt(curl_, CURLOPT_WRITEDATA, static_cast<void*>(&response));
 
   // perform CURL request
   CURLcode result = curl_easy_perform(curl_);
   if (result != CURLE_OK) {
      std::ostringstream oss;  
      oss << "curl_easy_perform() failed: "<< curl_easy_strerror(result);
      throw std::runtime_error(oss.str());
   }

   return response;
}

//------------------------------------------------------------------------------
// deals with private API methods:
std::string KClient::private_method(const std::string& method, 
				    const KInput& input) const
{   
   // build method URL
   std::string path = "/" + version_ + "/private/" + method;
   std::string method_url = url_ + path;
   
   curl_easy_setopt(curl_, CURLOPT_URL, method_url.c_str());
   
   // create a nonce and and postdata 
   std::string nonce = create_nonce();
   std::string postdata = "nonce=" + nonce;

   // if 'input' is not empty generate other postdata
   if (!input.empty())
      postdata = postdata + "&" + build_query(input);
   
   curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, postdata.c_str());

   // add custom header
   curl_slist* chunk = NULL;

   std::string key_header =  "API-Key: "  + key_;
   std::string sign_header = "API-Sign: " + signature(path, nonce, postdata);

   chunk = curl_slist_append(chunk, key_header.c_str());
   chunk = curl_slist_append(chunk, sign_header.c_str());
   curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, chunk);
   
   // where CURL write callback function stores the response
   std::string response;
   curl_easy_setopt(curl_, CURLOPT_WRITEDATA, static_cast<void*>(&response));

   // perform CURL request
   CURLcode result = curl_easy_perform(curl_);

   // free the custom headers
   curl_slist_free_all(chunk);
  
   // check perform result
   if (result != CURLE_OK) {
	 std::ostringstream oss;
	 oss << "curl_easy_perform() failed: " << curl_easy_strerror(result);
	 throw std::runtime_error(oss.str());
   }
   
   return response;
}

std::string KClient::orderbook(const std::string &pair, KOrderBook &kb)
{
  // url options
  KInput ki;
  ki["pair"] = pair;

  json_string data = libjson::to_json_string(public_method("Depth",ki));
  JSONNode root = libjson::parse(data);

  // throw exception on error in JSON response
  if(!root["error"].empty()) {
    std::ostringstream oss;
    oss << "Kraken response contains errors: ";
    for(JSONNode::const_iterator it = root["error"].begin(); it != root["error"].end(); ++it) {
      oss << std::endl << " * " << libjson::to_std_string(it->as_string());
    }
    //throw std::runtime_error(oss.str());
	return pair;
  }
  
  // throw exception on empty json result
  if(root["result"].empty()){
    throw std::runtime_error("Kraken response empty");
  }
  
  JSONNode &result = root["result"];
  
  for(JSONNode::iterator it=result.begin(); it!=result.end(); ++it){
    kb.pair = libjson::to_std_string(it->name());
    for(JSONNode::iterator it2=it->begin(); it2 != it->end(); ++(it2)){
      for(JSONNode::iterator it3=it2->begin(); it3 != it2->end(); ++(it3)){
		if(std::string("asks").compare(it2->name())==0){
		  kb.lAsks.push_back(KOrder(*(it3)));
		}
		if(std::string("bids").compare(it2->name())==0){
		  kb.lBids.push_back(KOrder(*(it3)));
		}
      }
    }
  }
  return libjson::to_std_string(data); // pair name from server
}
  
//------------------------------------------------------------------------------
// downloads recent trade data:
std::string KClient::trades(const std::string& pair, 
			    const std::string& since,
			    std::vector<KTrade>& output)
{
  KInput ki;
  ki["pair"] = pair;
  ki["since"] = since;
  
  // download and parse data
  json_string data = libjson::to_json_string( public_method("Trades", ki) ); 
  JSONNode root = libjson::parse(data);
  
  // throw an exception if there are errors in the JSON response
  if (!root.at("error").empty()) {
	std::ostringstream oss;
	SafePrint{} << "Kraken response contains errors: ";
    
	// append errors to output string stream
	for (JSONNode::const_iterator it = root["error"].begin(); it != root["error"].end(); ++it) 
	  SafePrint{} << std::endl << " * " << libjson::to_std_string(it->as_string());
	
	return since;
  }
  
  // throw an exception if result is empty   
  if (root.at("result").empty()){
	SafePrint{} << "Kraken response doesn't contain result data";
    return since;
  }
  
  JSONNode &result = root["result"];
  JSONNode &result_pair = result[0];
  
  std::string last = libjson::to_std_string( result.at("last").as_string() );
  
  for (JSONNode::const_iterator it = result_pair.begin(); it != result_pair.end(); ++it){
	output.push_back(KTrade(*it));
  }

  return last;
}

//------------------------------------------------------------------------------
// update the server time, and the local and UTC time at the moment of the call
std::string KClient::server_time(std::time_t &stime){
  KInput in;
  // getting time from server
  json_string resp = libjson::to_json_string(public_method("Time",in));
  JSONNode root = libjson::parse(resp);
  
  if(!root.at("error").empty()){
	throw std::runtime_error("Kraken response doesn't contain result data.");
  }

  JSONNode &result = root["result"];
  JSONNode &timestamp = result["unixtime"];
  
  stime = (std::time_t)(timestamp.as_int());
  
  // for debug...
  /*std::cout << "Server UTC time: " << server_utc_time << std::endl;
    std::cout << "local time: " << local_time << std::endl;
    std::cout << "UTC time: " << utc_time << std::endl;*/
  
  return std::to_string(stime);
}

//------------------------------------------------------------------------------
// update the assets list
std::string KClient::update_assets(KAssetsMap &kam)
{
  KInput in;
  json_string resp = libjson::to_json_string(public_method("Assets",in));
  JSONNode root = libjson::parse(resp);
  
  if(!root.at("error").empty()){
	throw std::runtime_error("Kraken response doesn't contain assets data.");
  }
  
  JSONNode &result = root["result"];
  
  kam.clear();
  
  for(const JSONNode &node : result){
	kam.emplace(std::make_pair<std::string,KAsset>(libjson::to_std_string(node.name()),KAsset(node)));  
  }
  
  return libjson::to_std_string(resp);
}

std::string KClient::asset_pairs(KAssetPairs &kap)
{
  KInput in;
  json_string resp = libjson::to_json_string(public_method("AssetPairs",in));
  JSONNode root = libjson::parse(resp);
  
  if(!root.at("error").empty()){
	throw std::runtime_error("Kraken response doesn't contain asset pais data");
  }    
  
  JSONNode &result = root["result"];
  kap.clear();
  
  for(const JSONNode &node : result){
	kap.push_back(KAssetPair(node));
  }
  
  return libjson::to_std_string(resp);
}

//------------------------------------------------------------------------------
// print assets on cout fordebugin pourposes
void KClient::print_assets(const KAssetsMap &kam)
{
  for(std::map<std::string,KAsset>::const_iterator it=kam.begin(); it!=kam.end(); ++it){
	//std::cout << it->first << std::endl;
	std::cout << it->second << std::endl;
  }
}

//------------------------------------------------------------------------------
// get spreaddata for a pair
std::string KClient::spread(const std::string &pair, const std::string &since, KSpreadStorage &output)
{
  KInput in;
  in["pair"] = pair;
  in["since"] = since;
  json_string resp = libjson::to_json_string(public_method("Spread",in));
  JSONNode root = libjson::parse(resp);
  
  if(!root.at("error").empty()){
	throw std::runtime_error("Kraken response doesn't contain assets data.");
  }
  JSONNode result = root["result"];
  JSONNode pairnode = result[0];
  
  for(JSONNode::iterator it = pairnode.begin(); it!=pairnode.end(); ++it){
	output.push_back(KSpread(*it));
  }
  return libjson::to_std_string(result["last"].as_string());
}

std::string KClient::OHLC(const std::string &pair, const std::string &since,
						  const std::string &interval, KOHLCStorage &ohlcs)
{
  KInput in;
  in["pair"] = pair;
  in["since"] = since;
  in["interval"] = interval;
  try{
	json_string data = libjson::to_json_string(public_method("OHLC",in));
	JSONNode root = libjson::parse(data);
	
	if(!root.at("error").empty()){
	  std::string error_string = libjson::to_std_string(root.at("error").as_string());
	  cout << error_string << endl;
	  //throw std::runtime_error("Kraken response to public call OHLC don't contain data");
	  return since;
	}
	
	JSONNode result = root["result"];
	JSONNode ohlcnode = result[0];
    
	for(const JSONNode &node : ohlcnode){
	  ohlcs.push_back(KOHLC(node));
	}
	std::string last = libjson::to_std_string(result.at("last").as_string());
	return last;
  }
  catch(std::exception &e){
	std::cerr << "KCLIENT OHLC exception: " << e.what();
	return since;
  }
  return since; // juat for sake of avoid warnings
}

//------------------------------------------------------------------------------
// helper function to initialize Kraken API library's resources:
void initialize() 
{
  CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
  if (code != CURLE_OK) {
	std::ostringstream oss;
	oss << "curl_global_init() failed: " << curl_easy_strerror(code);
	throw std::runtime_error(oss.str());
  }
}

//------------------------------------------------------------------------------
// helper function to terminate Kraken API library's resources:
void terminate() 
{
  curl_global_cleanup();
}

//------------------------------------------------------------------------------

}; //namespace Kraken

//------------------------------------------------------------------------------
