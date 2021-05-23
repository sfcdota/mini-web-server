//
// Created by sfcdo on 08.06.2021.
//

#include "MessageValidator.hpp"


/* Scheme of HTTP message in terms of rfc-7230 & subject restrictions
 * HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]

 * start-line   = request-line / status-line
 * ----------------------------------------
 * request-line = method SP request-target SP HTTP-version CRLF

 * method = token
 * token  = 1*tchar
 * tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA

 * request-target-target = origin-form
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
 * quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
 * comment = "(" *( ctext / quoted-pair / comment ) ")"
 * ctext = HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text


 * message-body = *OCTET
 */








bool MessageValidator::ValidRequest(std::string & request) {
  msg_pos = 0;
  message = request;
  status_code = 200;
  return ValidRequestLine(message, msg_pos) && ValidHeadersBlock(message, msg_pos) && ValidBody(message, msg_pos);
}


bool MessageValidator::ValidRequestLine(const std::string &msg, size_t &pos) {
  return ValidMethod(msg, pos) && ValidTarget(msg, pos) && ValidVersion(msg, pos);
}
bool MessageValidator::ValidMethod(const std::string &msg, size_t &pos) {
  if (!istchar(msg[pos]))
    return false;
  for(; istchar(msg[pos]); pos++);
  return msg[pos] == ' ';
}
bool MessageValidator::ValidTarget(const std::string &msg, size_t &pos) {
  pos++; //skip SP
  if (msg[pos] != '/')
    return false;
  while (msg[pos] == '/') {
    pos++;
    while (ispchar(msg[pos]))
      if (ispctencoded(msg, pos))
        pos += 2;
      else
        pos++;
  }
  if (msg[pos] == '?')
    while(isquery(msg[pos++]));
  return msg[pos++] == ' ';
}

bool MessageValidator::ValidVersion(const std::string &msg, size_t &pos) {
  if (msg.length() > pos + 14 && !msg.compare(pos,  14, "%x48.54.54.50/"))
    pos += 14;
  else if (msg.length() > pos + 5 && !msg.compare(pos,  5, "HTTP/"))
    pos += 5;
  else
    return false;
  return isdigit(msg[pos++]) && msg[pos++] == '.' && isdigit(msg[pos++]);
}
bool MessageValidator::ValidHeadersBlock(const std::string &msg, size_t &pos) {
  if (!iscrlf(msg, pos))
    return false;
  pos += 2; //skip CRLF from request line
  if (msg[pos] == ' ')
    return false;
  while(!iscrlf(msg, pos)) { // headers block
    if (!ValidHeader(msg, pos))
      return false;
    pos += 2;
  }
  return pos <= msg.length();
}

bool MessageValidator::ValidHeader(const std::string &msg, size_t &pos) {
  while (!iscrlf(msg, pos)) { // header-field
    if (!ValidFieldName(msg, pos))
      return false;
    for (; isows(msg[pos]); pos++); // first OWS
    if (!ValidFieldValue(msg, pos))
      return false;
    for (; isows(msg[pos]); pos++); // second OWS
  }
  return iscrlf(msg, pos);
}


bool MessageValidator::ValidFieldName(const std::string &msg, size_t &pos) {
  if (!istchar(msg[pos]))
    return false;
  for(; istchar(msg[pos]); pos++); // field-name
  if (msg[pos++] != ':') {
    status_code = 400;
    return false;
  }
  return true;
}
bool MessageValidator::ValidFieldValue(const std::string &msg, size_t &pos) {
  for(; isvchar(msg[pos]) || isobsfold(msg, pos);) { //field-value
    if (isvchar(msg[pos])) {
      if (msg[pos] == '\"') {
        if (!ValidQuotedString(msg, pos))
          return false;
      } else if (msg[pos] == '(') {
        if (!ValidComment(msg, pos))
          return false;
      }
      else if (!ValidFieldContent(msg, pos))
        return false;
    } else { //obs-fold
      pos += 2;
      for (; isows(msg[pos]); pos++); // 1*( SP / HTAB)
    }
  }
  return true;
}


bool MessageValidator::ValidQuotedString(const std::string &msg, size_t &pos) {
  pos++;
  for(; isqdtext(msg[pos]) || isquotedpair(msg, pos); )
    if (isquotedpair(msg, pos))
      pos += 2;
    else pos++;
  if (msg[pos] != '\"')
    return false;
  pos++;
  return true;
}

bool MessageValidator::ValidComment(const std::string &msg, size_t &pos) {
  size_t brace_count = 1;
  while (brace_count) {
    pos++;
    for (; isctext(msg[pos]); pos++);
    if (msg[pos] == '(')
      brace_count++;
    else if (msg[pos] == ')')
      brace_count--;
    else
      return false;
  }
  pos++;
  return true;
}

bool MessageValidator::ValidFieldContent(const std::string &msg, size_t &pos) {
  pos++;
  if (isows(msg[pos])) { //if at least one 1*( SP / HTAB )
    for (; isows(msg[pos]); pos++); // 1*( SP / HTAB)
    if (!isvchar(msg[pos]))
      return false;
  }
  return true;
}

MessageValidator::MessageValidator() {

}
MessageValidator::~MessageValidator() {

}
bool MessageValidator::ValidResponse(std::string &response) {
  msg_pos = 0;
  message = response;
  return ValidStatusLine(message, msg_pos) && ValidHeadersBlock(message, msg_pos) && ValidBody(message, msg_pos);
}

bool MessageValidator::ValidStatusLine(const std::string &msg, size_t &pos) {
  if (!ValidVersion(msg, pos) || msg[pos] != ' ')
    return false;
  pos++; // skip SP after HTTP version
  if (!isstatuscode(msg, pos))
    return false;
  pos += 3;
  if (msg[pos] != ' ')
    return false;
  pos++;
  for(; isreasonphrase(msg[pos]); pos++);
  return true;
}

bool MessageValidator::ValidBody(const std::string &msg, size_t &pos) {
  return true;
}

size_t MessageValidator::GetStatusCode() const {
  return status_code;
}
