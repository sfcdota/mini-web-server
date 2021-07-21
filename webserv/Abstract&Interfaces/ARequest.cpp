//
// Created by sfcdo on 20.07.2021.
//

/*
 * Lifting the server siege...
Transactions:		       24490 hits
Availability:		      100.00 %
Elapsed time:		       91.75 secs
Data transferred:	        3.80 MB
Response time:		        0.09 secs
Transaction rate:	      266.92 trans/sec
Throughput:		        0.04 MB/sec
Concurrency:		       24.97
Successful transactions:         247
Failed transactions:	           0
Longest transaction:	        0.76
Shortest transaction:	        0.00
 */


#include "ARequest.hpp"
ARequest::ARequest(const ServerConfig &config)
    : server_config(config), closeOnEnd(false) {}
ARequest::ARequest(const ARequest &in): server_config(in.server_config) {
  *this = in;
}
ARequest &ARequest::operator=(const ARequest &in) {
  request_line = in.request_line;
  headers = in.headers;
  body = in.body;
  status_code = in.status_code;
  closeOnEnd = in.closeOnEnd;
  return *this;
}

ARequest::~ARequest() {}
