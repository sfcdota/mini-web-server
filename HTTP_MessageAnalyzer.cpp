//
// Created by sfcdo on 04.06.2021.
//

#include "HTTP_MessageAnalyzer.hpp"

HTTP_MessageAnalyzer::HTTP_MessageAnalyzer(const std::string & message, const bool isRequest)
    : msg(message), isRequest(isRequest), end(message.length()), begin(0) {}
HTTP_MessageAnalyzer::~HTTP_MessageAnalyzer() {}

/* Scheme of HTTP msg in terms of rfc-7230 & subject restrictions
 * HTTP-msg = start-line *( header-field CRLF ) CRLF [ msg-body ]

 * start-line   = request-line / status-line
 * ----------------------------------------
 * request-line = method SP request-target SP HTTP-version CRLF

 * method = token
 * token  = 1*tchar
 * tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA

 * request-target = origin-form
 * origin-form    = absolute-path [ "?" query ]
 * absolute-path  = 1*( «/» segment )
 * query = *( pchar / "/" / "?" )
 * segment        = *pchar
 * pchar          = unreserved / pct-encoded / sub-delims / ":" / "@"
 * unreserved     = ALPHA / DIGIT / "-" / "." / "_" / "~"
 * pct-encoded    = "%" HEXDIG HEXDIG
 * sub-delims     = "!" / "$" / "&" / "’" / "(" / ")" / "*" / "+" / "," / ";" / "="

 * HTTP-version = HTTP-name "/" DIGIT "." DIGIT
 * HTTP-name    = %x48.54.54.50 ; "HTTP", case-sensitive

 * status-line  = HTTP-version SP status-code SP reason-phrase CRLF
 * status-code = 3DIGIT
 * reason-phrase = *( HTAB / SP / VCHAR / obs-text )
 * ----------------------------------------


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
 *
 * quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
 * qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
 * comment = "(" *( ctext / quoted-pair / comment ) ")"
 * ctext = HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text


 * msg-body = *OCTET
 */
bool HTTP_MessageAnalyzer::IsValidRequest(std::string &request) {
  bool isValid = true;
  char c = '\x41';
  return ValidRequestLine() && ValidHeaders() && ValidBody();
}
bool HTTP_MessageAnalyzer::ValidBody() {
  return true;
}
bool HTTP_MessageAnalyzer::ValidRequestLine() {
  return ValidMethod() && ValidTarget() && ValidVersion();
}
bool HTTP_MessageAnalyzer::ValidMethod() {
  if (!istchar(msg[begin]))
    return false;
  for(; istchar(msg[begin]) && begin < end; begin++);
  return at_least_one != begin && msg[begin++] == ' ';
}
bool HTTP_MessageAnalyzer::ValidTarget() {
  if (msg[begin] != '/')
    return false;
  while (msg[begin] == '/' && begin < end) {
    begin++;
    while(ispchar(msg[begin]) && begin < end)
      if (msg[begin++] == '%') {
        if (ishex(msg[begin++]) && ishex(msg[begin++]));
        else
          return false;
      }
  }
  if (msg[begin] == '?')
    while(isquery(msg[begin++]) && begin < end);
  return msg[begin++] == ' ';
}
bool HTTP_MessageAnalyzer::ValidVersion() {
  if (!msg.compare(begin, begin + 14, "%x48.54.54.50/"))
    begin += 14;
  else if (!msg.compare(begin, begin + 5, "HTTP/"))
    begin += 5;
  else
    return false;
  return isdigit(msg[begin++]) && msg[begin++] == '.' && isdigit(msg[begin++]) &&
          msg[begin++] == '\r' && msg[begin++] == '\n';
}
bool HTTP_MessageAnalyzer::ValidHeaders() {
  if (msg[begin] == ' ')
    return false;
  while(msg[begin] != '\r' && msg[begin + 1] != '\n' && begin < end) { // headers block
    while (msg[begin] != '\r' && msg[begin + 1] != '\n' && begin < end) { // header-field
      if (!istchar(msg[begin]))
        return false;
      for(; istchar(msg[begin]) && begin < end; begin++); // field-name
      if (msg[begin++] != ':')
        return false;
      for (; isows(msg[begin]) && begin < end; begin++); // first OWS
      for(; ((isgraph(msg[begin]) || msg[begin] >= '\x80' && msg[begin] <= '\xFF')
            || (msg[begin] == '\r' && msg[begin] == '\n' && (msg[begin] == ' ' || msg[begin] == '\t')))
            || begin < end;) { // field-value = *(field-content / obs-fold)
        if (isgraph(msg[begin]) || msg[begin] >= '\x80' && msg[begin] <= '\xFF') { // field-content
          begin++;
///
/// check field value for quoted string and etc!!!!!!!!!!!!!!!!!!!!
///
          if (isows(msg[begin])) { //if at least one 1*( SP / HTAB )
            for (; isows(msg[begin]) && begin < end; begin++); // 1*( SP / HTAB)
            if (isgraph(msg[begin]) || msg[begin] >= '\x80' && msg[begin] <= '\xFF')
              begin++;
          }
        } else { //obs-fold
          begin += 2;
          if (!isows(msg[begin]))
            return false;
          for (; isows(msg[begin]) && begin < end; begin++); // 1*( SP / HTAB)
        }
      }
      for (; isows(msg[begin]) && begin < end; begin++); // second OWS
    }
    begin += 2;
  }
  return true;
}


bool HTTP_MessageAnalyzer::isows(int c) {
  return c == ' ' || c == '\t';
}

/// pchar without pct-encoded check
/// \param c
/// \return
bool HTTP_MessageAnalyzer::ispchar(int c) {
  return isalnum(c) && strchr(pchar, c);
//  return c > 37 && c < 47 || c > 47 && c < 60 ||
//      c > 63 && c < 91 || c > 94 && c < 123 ||
//      c == 33 || c == 35 || c == 61 || c == 126;
}
bool HTTP_MessageAnalyzer::isquery(int c) {
  return ispchar(c) || c == '/' || c == '?';
}

/// tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA
/// \param c
/// \return
bool HTTP_MessageAnalyzer::istchar(int c) {
  return isalnum(c) && strchr(tchar, c);
}

bool HTTP_MessageAnalyzer::ishex(int c) {
  return c > 47 && c < 58 || c > 64 && c < 71 || c > 94 && c < 103; // not sure about c > 94 && c < 103
}

bool HTTP_MessageAnalyzer::ispctencoded() {
  return msg[begin] == '%' && ishex(msg[begin + 1]) && ishex(msg[begin + 2]);
}
HTTP_MessageAnalyzer::HTTP_MessageAnalyzer(): end(), at_least_one(), isRequest(), begin() {

}


