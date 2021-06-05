
#include "Request.hpp"

Request::Request(std::string &request)  {
  allowed_methods.push_back("GET");
  allowed_methods.push_back("POST");
  allowed_methods.push_back("DELETE");
  left = 0;
  right = 0;

}

/* Scheme of request-line in terms of rfc-7230
 * request-line = method SP request-target SP HTTP-version CRLF
 * SP = space symbol
 * CRLF = carriage return + line feed
 */
/// MUST BE ERROR CODE 400 ON FALSE RETURN
/// \param request
/// \return
bool Request::ValidParsedRequest(std::string & request) {
  const char *request_line_fields[] = {"method", "target", "version"}; //find a better place to put
  size_t token_count = 0;
  size_t token_right = 0;
  if ((right = request.find("\r\n")) != std::string::npos) {
    while (token_count < 3 && token_right <= right) { // must be exact 3 tokens and the 3rd exactly ends before \r\n
      if (request[token_right] == ' ' || token_right == right) {
        request_line.insert(std::pair<std::string, std::string>(request_line_fields[token_count],
                                                                request.substr(left, token_right - left)));
        left = token_right + 1; // skip space, at the 3rd token == index of \n (right + 1)
        token_count++;
      }
      token_right++;
    }
  }
  return token_count == 3 && left++ == right + 1 //move left to next block after condition
         && ValidMethod(request_line["method"]) && ValidTarget(request_line["target"]) &&
         ValidVersion(request_line["version"]);
}


/* Scheme of header part of http msg in terms of rfc-7230
 * HTTP-msg = start-line *( header-field CRLF ) CRLF [ msg-body ]
 * header-field = field-name ":" OWS field-value OWS
 * field-name = token
 * token  = 1*tchar
 * tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA
 * OWS = *( SP / HTAB )
 * field-value = *( field-content / obs-fold )
 * field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
 * field-vchar = VCHAR / obs-text
 * VCHAR = ASCII printable char
 * obs-fold = CRLF 1*( SP / HTAB )
 * obs-text = %x80-FF                   # x80 = 128, FF = 255, so any symbol in range 128-255
 */

bool Request::ValidParsedHeaders(std::string &request) {
  if (request[left] == ' ')
    return false; // decline msg as invalid (rfc-7230: after start-line must be header-field)
  size_t token_right = 0;
  if ((right = request.find("\r\n\r\n", left)) != std::string::npos) {
    while (token_right <= right) {
      token_right = request.find("\r\n", left);
    }
  }
  return true;
}

/* Scheme of method in terms of rfc-7230
 * method = token
 * token  = 1*tchar
 * tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA
 */
bool Request::ValidMethod(std::string & request) {
  for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++)
    if (request == *it)
      return true;
  return false;
}

/* Scheme of target in terms of rfc-7230 & rfc-3986 & subject restrictions (types of request-target forms)
 * request-target = origin-form
 * ---------------------
 * origin-form    = absolute-path [ "?" query ]
 * absolute-path  = 1*( «/» segment )
 * segment        = *pchar
 * pchar          = unreserved / pct-encoded / sub-delims / ":" / "@"
 * unreserved     = ALPHA / DIGIT / "-" / "." / "_" / "~"
 * pct-encoded    = "%" HEXDIG HEXDIG
 * HEXDIG         = DIGIT in base 16 (0123456789ABCDEF)
 * sub-delims     = "!" / "$" / "&" / "’" / "(" / ")" / "*" / "+" / "," / ";" / "="
*/
bool Request::ValidTarget(std::string &target) {
  return true;
}

/* Scheme of http version in terms of rfc-7230
 * HTTP-version = HTTP-name "/" DIGIT "." DIGIT
 * HTTP-name    = %x48.54.54.50 ; "HTTP", case-sensitive
 */
bool Request::ValidVersion(std::string &version) {
  if (version.compare(0, 5, "HTTP/"))
    return true;
  return true;
}
