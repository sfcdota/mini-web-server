//
// Created by sfcdo on 04.06.2021.
//

#include "MessageParser.hpp"

enum MachineState {
  start_line,
  request_line,
  status_line,
  method,
  token,

  request_sp1,
  request_target,
  request_sp2,
  http_version,
  request_crlf,
  FINAL
};

MessageParser::~MessageParser() {}

/* Scheme of HTTP msg in terms of rfc-7230 & subject restrictions
 * HTTP-msg = start-line *( header-field CRLF ) CRLF [ msg-body ]

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


 * msg-body = *OCTET
 */
Request MessageParser::ProcessRequest(std::string &http_message) {
  bool isValid = true;
  char c = '\x41';
  msg = http_message;
  pos = 0;
  end = http_message.length();
  ProcessRequestLine();
  return ValidRequestLine() && ValidHeadersBlock() && ValidBody();
}


MessageParser::MessageParser(): end(), pos(), request_() {
  request_line_parse_mapping_.insert(std::make_pair("method", &MessageParser::ValidMethod));
  request_line_parse_mapping_.insert(std::make_pair("target", &MessageParser::ValidTarget));
  request_line_parse_mapping_.insert(std::make_pair("version", &MessageParser::ValidVersion));
}


std::map<std::string, std::string> &MessageParser::ParseRequestLine() {
  std::for_each(request_line_parse_mapping_.begin(), request_line_parse_mapping_.end(),
                &MessageParser::ParseRequestLineField);
}

void MessageParser::ParseRequestLineField(const std::pair<const std::string, bool (MessageParser::*)()>& pair) {
  begin = pos;
  if ((this->*(pair.second))())
    request_.AddRequestLineField(std::make_pair(pair.first, msg.substr(begin, pos - begin)));
}

std::map<std::string, std::string> &MessageParser::ParseHeadersBlock() {

}


std::pair<std::string, std::string> &MessageParser::ParseHeader() {
}


std::string &MessageParser::ParseBody() {
}
void MessageParser::ProcessMethod() {
  if (!ValidMethod())
    return;
  request_.
}
void MessageParser::ProcessTarget() {

}
void MessageParser::ProcessVersion() {

}
void MessageParser::ProcessRequestLine() {

}

bool MessageParser::ValidBody() {
  return true;
}


bool MessageParser::ValidRequestLine() {
  return ValidMethod() && ValidTarget() && ValidVersion();
}
bool MessageParser::ValidMethod() {
  if (!istchar(msg[pos]))
    return false;
  for(; istchar(msg[pos]); pos++);
  return msg[pos] == ' ';
}
bool MessageParser::ValidTarget() {
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
  return msg[pos] == ' ';
}

bool MessageParser::ValidVersion() {
  pos++; //skip SP
  if (!msg.compare(pos, pos + 14, "%x48.54.54.50/"))
    pos += 14;
  else if (!msg.compare(pos, pos + 5, "HTTP/"))
    pos += 5;
  else
    return false;
  return isdigit(msg[pos++]) && msg[pos++] == '.' && isdigit(msg[pos++]) && iscrlf(msg, pos);
//  pos++; //skip SP
//  if (!strncmp(&msg[pos], "%x48.54.54.50/", 14))
//    pos += 14;
//  else if (!strncmp(&msg[pos], "HTTP/", 5))
//    pos += 5;
//  else
//    return false;
//  return isdigit(msg[pos++]) && msg[pos++] == '.' && isdigit(msg[pos++]) && iscrlf(msg, pos);
}
bool MessageParser::ValidHeadersBlock() {
  pos += 2; //skip CRLF from request line
  if (msg[pos] == ' ')
    return false;
  while(!iscrlf(msg, pos)) { // headers block
    if (!ValidHeader())
      return false;
    pos += 2;
  }
  return pos <= end;
}

bool MessageParser::ValidHeader() {
  while (!iscrlf(msg, pos)) { // header-field
    if (!ValidFieldName())
      return false;
    for (; isows(msg[pos]); pos++); // first OWS
    if (!ValidFieldValue())
      return false;
    for (; isows(msg[pos]); pos++); // second OWS
  }
  return iscrlf(msg, pos);
}


bool MessageParser::ValidFieldName() {
  if (!istchar(msg[pos]))
    return false;
  for(; istchar(msg[pos]); pos++); // field-name
  if (msg[pos++] != ':')
    return false;
  return true;
}
bool MessageParser::ValidFieldValue() {
  for(; isvchar(msg[pos]) || isobsfold(msg, pos);) { //field-value
    if (isvchar(msg[pos])) {
      if (msg[pos] == '\"' && !ValidQuotedString() || msg[pos] == '(' && !ValidComment() || !ValidFieldContent())
          return false;
    } else { //obs-fold
      pos += 2;
      for (; isows(msg[pos]); pos++); // 1*( SP / HTAB)
    }
  }
  return true;
}

bool MessageParser::ValidQuotedString() {
  pos++;
  for(; isqdtext(msg[pos]) || isquotedpair(msg, pos); pos++);
  if (msg[pos] != '\"')
    return false;
  pos++;
  return true;
}
bool MessageParser::ValidComment() {
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

bool MessageParser::ValidFieldContent() {
  pos++;
  if (isows(msg[pos])) { //if at least one 1*( SP / HTAB )
    for (; isows(msg[pos]); pos++); // 1*( SP / HTAB)
    if (!isvchar(msg[pos]))
      return false;
  }
  return true;
}


