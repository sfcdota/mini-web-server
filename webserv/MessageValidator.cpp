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








bool MessageValidator::ValidHeaders(const std::string & request) {
  msg_pos = 0;
  message = request;
  status_code = 200;
  out_temp = 0;
  return ValidRequestLine(message, msg_pos) && ValidHeadersBlock(message, msg_pos);
}


bool MessageValidator::ValidRequestLine(const std::string &msg, size_t &pos) {
  return ValidMethod(msg, pos) && ValidTarget(msg, pos) && ValidVersion(msg, pos);
}
bool MessageValidator::ValidMethod(const std::string &msg, size_t &pos) {
  return expression_test(msg, pos, &istchar, status_code, 0, 1, 1) && msg[pos] == ' ';
}
bool MessageValidator::ValidTarget(const std::string &msg, size_t &pos) {
  pos++; //skip SP
  if (msg[pos] != '/')
    return false;
  while (msg[pos] == '/') {
    pos++;
    while (expression_test(msg, pos, &ispchar, status_code)
           || expression_test(msg, pos, &ispctencoded, status_code, 0, 2));
  }
  if (msg[pos] == '?')
    expression_test(msg, pos, &isquery, status_code);
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
    if (!expression_test(msg, pos, &ValidFieldName, status_code, 0, 0, 1, 1))
      return false;
//    if (!ValidFieldName(msg, pos))
//      return false;
    for (; isows(msg[pos]); pos++); // first OWS
    if (!ValidFieldValue(msg, pos))
      return false;
    for (; isows(msg[pos]); pos++); // second OWS
  }
  return iscrlf(msg, pos);
}


bool MessageValidator::ValidFieldName(const std::string &msg, size_t &pos) {
  out_temp = pos;
  return expression_test(msg, pos, &istchar, status_code, 0, 1, 1)
    && expression_test(msg, pos, &isdots, status_code, 400, 1, 1, 1);
}
bool MessageValidator::ValidFieldValue(const std::string &msg, size_t &pos) {
  if (!strncmp(&msg[out_temp], "Accept-Charset", 14))
    return CheckAcceptCharsets(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Accept-Language", 15))
    return CheckAcceptLanguage(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Allow", 5))
    return CheckAllow(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Authorization", 13))
    return CheckAuthorization(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Content-Language", 16))
    return CheckContentLanguage(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Content-Length", 14))
    return CheckContentLength(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Content-Location", 16))
    return CheckContentLocation(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Content-Type", 12))
    return CheckContentType(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Date", 4))
    return CheckDate(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Host", 4))
    return CheckHost(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Last-Modified", 13))
    return CheckLastModified(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Location", 8))
    return CheckLocation(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Referer", 7))
    return CheckReferer(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Retry-After", 11))
    return CheckRetryAfter(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Server", 6))
    return CheckServer(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "Transfer-Encoding", 17))
    return CheckTransferEncoding(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "User-Agent", 10))
    return CheckUserAgent(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else if (!strncmp(&msg[out_temp], "WWW-Authenticate", 16))
    return CheckWwwAuthenticate(msg, pos) && ValidDefaultFieldValue(msg, pos);
  else
    return ValidDefaultFieldValue(msg, pos);
}

bool MessageValidator::ValidDefaultFieldValue(const std::string &msg, size_t &pos) {
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

MessageValidator::MessageValidator() { status_code = 0; }
MessageValidator::~MessageValidator() {

}
bool MessageValidator::ValidResponse(const std::string &response) { //NEED TO FIX WITH RESPONSE CLASS
  msg_pos = 0;
  message = response;
  return ValidStatusLine(message, msg_pos) && ValidHeadersBlock(message, msg_pos) && ValidBody(message);
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

bool MessageValidator::ValidBody(const std::string &msg) {
  return true;
}

size_t MessageValidator::GetStatusCode() const {
  return status_code;
}

template<class ret, class kek>
bool MessageValidator::exp_test_call(const std::string &s, size_t &index,
                                     ret (kek::*f)(int, int)) {
  return (this->*f)(s[index], 0);
}

template<class ret, class kek, class T1, class T2>
bool MessageValidator::exp_test_call(const std::string &s, size_t &index,
                                     ret (kek::*f)(T1, T2)) {
  return (this->*f)(s, index);
}

template<class ret>
bool MessageValidator::exp_test_call(const std::string &s, size_t &index,
                                     ret (*f)(int, int)) {
  return f(s[index], 0);
}

template<class ret, class T1, class T2>
bool MessageValidator::exp_test_call(const std::string &s, size_t &index,
                                     ret (*f)(T1, T2)) {
  return f(s, index);
}



template<class ret, class kek, class T1, class T2>
bool MessageValidator::expression_test(const std::string & s, size_t & index,
                                       ret (kek::*f)(T1, T2), size_t status_code, size_t step, size_t min, size_t max) {
  size_t counter = 0;
  for(; counter < max && exp_test_call(s, index, f); ++counter, index += step);
  if (counter < min || counter > max || (!min && !counter)) {
    if (status_code)
      this->status_code = status_code;
    return false;
  }
  return true;
}


template<class ret, class T1, class T2>
bool MessageValidator::expression_test(const std::string & s, size_t & index,
                                       ret (*f)(T1, T2), size_t & out_status_code_ref, size_t status_code, size_t step, size_t min, size_t max) {
  size_t counter = 0;
  for(; counter < max && exp_test_call(s, index, f); ++counter, index += step);
  if (counter < min || counter > max || (!min && !counter)) {
    if (status_code)
      out_status_code_ref = status_code;
    return false;
  }
  return true;
}


/// Accept-Charset = 1#( ( charset / "*" ) [ weight ] )
/// Accept-Charset = *( «,» OWS ) ( ( charset / «*» ) [ weight ] ) *( OWS «,» [ OWS ( ( charset / «*» ) [ weight ] ) ] )
/// charset = token
/// weight = OWS «;» OWS «q=» qvalue
/// qvalue = ( «0» [ «.» *3DIGIT ] ) / ( «1» [ «.» *3″0″ ] )
/// \param header_map
/// \return
bool MessageValidator::CheckAcceptCharsets(const std::string &msg, size_t &pos) {
//  return expression_test_abnf(msg, pos, expression_test(msg, pos, &MessageValidator::ValidDefaultFieldValue));
  return true;
}


/// Accept-Language = *( «,» OWS ) ( language-range [ weight ] ) *( OWS «,» [ OWS ( language-range [ weight ] ) ] )
/// language-range   = (1*8ALPHA *("-" 1*8alphanum)) / "*"                        | at least 1 and at most 8 ALPHA
/// \param header_map
/// \return
bool MessageValidator::CheckAcceptLanguage(const std::string &msg, size_t &pos) {
  return true;
}

/// Allow = #method
/// #method => [ ( "," / method ) *( OWS "," [ OWS method ] ) ]
/// \param header_map
/// \return
bool MessageValidator::CheckAllow(const std::string &msg, size_t &pos) {
  return true;
}

/// Authorization = credentials
/// credentials = auth-scheme [ 1*SP ( token68 / [ ( «,» / auth-param ) *( OWS «,» [ OWS auth-param ] ) ] ) ]
/// auth-scheme = token
/// token68 = 1*( ALPHA / DIGIT / «-» / «.» / «_» / «~» / «+» / «/» ) *»=»
/// auth-param = token BWS «=» BWS ( token / quoted-string )
/// BWS = OWS
/// \param header_map
/// \return
bool MessageValidator::CheckAuthorization(const std::string &msg, size_t &pos) {
  return true;
}

/// Content-Language = 1#language-tag
/// language-tag = langtag
/// langtag = language[«-» script][«-» region]*(«-» variant)*(«-» extension)[«-» privateuse]
/// https://efim360.ru/rfc-5646-tegi-dlya-identifikatsii-yazykov/
/// \param header_map
/// \return
bool MessageValidator::CheckContentLanguage(const std::string &msg, size_t &pos) {
  return true;
}

/// Content-Length = 1*DIGIT
/// \param header_map
/// \return
bool MessageValidator::CheckContentLength(const std::string &msg, size_t &pos) {
  return true;
}

/// Content-Location = absolute-URI / partial-URI
/// absolute-URI = scheme ":" hier-part [ "?" query ]
/// scheme = ALPHA *( ALPHA / DIGIT / «+» / «-» / «.» )
/// hier-part = "//" authority path-abempty (авторитетный путь)/ path-absolute (абсолютный путь)
///       / path-rootless (путь без корней)/ path-empty (пустой путь)
/// authority = [ userinfo «@» ] host [ «:» port ]
/// userinfo = *( unreserved / pct-encoded / sub-delims / «:» )
/// host = IP-literal / IPv4address / reg-name

/*
IPv6address =                6( h16 ":" ) ls32
/                       "::" 5( h16 ":" ) ls32
/               [ h16 ] "::" 4( h16 ":" ) ls32
/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
/ [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32
/ [ *4( h16 ":" ) h16 ] "::" ls32
/ [ *5( h16 ":" ) h16 ] "::" h16
/ [ *6( h16 ":" ) h16 ] "::"

ls32 = ( h16 ":" h16 ) / IPv4address; (наименее значимый 32-битный адрес)

h16 = 1*4HEXDIG; (16 бит адреса представлены в шестнадцатеричном формате)

IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

dec-octet = DIGIT ; 0-9
/ %x31-39 DIGIT ; 10-99
/ "1" 2DIGIT ; 100-199
/ "2" %x30-34 DIGIT ; 200-249
/ "25" %x30-35 ; 250-255

 */


/// reg-name = *( unreserved / pct-encoded / sub-delims )
/// IP-literal = «[» ( IPv6address / IPvFuture ) «]»
/// IPvFuture = «v» 1*HEXDIG «.» 1*( unreserved / sub-delims / «:» )
/// path-abempty  = *( "/" segment )
/// path-absolute = "/" [ segment-nz *( "/" segment ) ]
/// path-rootless = segment-nz *( "/" segment )
/// segment       = *pchar
/// segment-nz    = 1*pchar
/// path-empty    = 0<pchar>
/// query = *( pchar / "/" / "?" )
/// \param header_map
/// \return
bool MessageValidator::CheckContentLocation(const std::string &msg, size_t &pos) {
  return true;
}

/// Content-Type = media-type
/// media-type = type "/" subtype *( OWS ";" OWS parameter )
/// type = token
/// subtype = token
/// parameter = token "=" ( token / quoted-string )
/// \param header_map
/// \return
bool MessageValidator::CheckContentType(const std::string &msg, size_t &pos) {
  return true;
}

/// Date = HTTP-date
/// HTTP-date = IMF-fixdate / obs-date
/// IMF-fixdate = day-name «,» SP date1 SP time-of-day SP GMT
/*

day-name = %x4D.6F.6E ; Mon
/ %x54.75.65 ; Tue
/ %x57.65.64 ; Wed
/ %x54.68.75 ; Thu
/ %x46.72.69 ; Fri
/ %x53.61.74 ; Sat
/ %x53.75.6E ; Sun

 */
/// date1 = day SP month SP year
/// day = 2DIGIT
/*

month = %x4A.61.6E ; Jan
/ %x46.65.62 ; Feb
/ %x4D.61.72 ; Mar
/ %x41.70.72 ; Apr
/ %x4D.61.79 ; May
/ %x4A.75.6E ; Jun
/ %x4A.75.6C ; Jul
/ %x41.75.67 ; Aug
/ %x53.65.70 ; Sep
/ %x4F.63.74 ; Oct
/ %x4E.6F.76 ; Nov
/ %x44.65.63 ; Dec

 */
/// year = 4DIGIT
/// time-of-day = hour «:» minute «:» second
/// hour = 2DIGIT
/// minute = 2DIGIT
/// second = 2DIGIT
/// GMT = %x47.4D.54 ; GMT
/// \param header_map
/// \return
bool MessageValidator::CheckDate(const std::string &msg, size_t &pos) {
  return true;
}

/// Host = uri-host [ «:» port ]
/// host = IP-literal / IPv4address / reg-name
/// port = *DIGIT                 | pri etom po scheme http default = 80, po https = 443
/// \param header_map
/// \return
bool MessageValidator::CheckHost(const std::string &msg, size_t &pos) {
  return true;
}

/// Last-Modified = HTTP-date
/// \param header_map
/// \return
bool MessageValidator::CheckLastModified(const std::string &msg, size_t &pos) {
  return true;
}

/// Location = URI-reference
/// URI-reference = URI / relative-ref
/// URI            = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
/// relative-ref   = relative-part [ "?" query ] [ "#" fragment ]

/*

relative-part  = "//" authority path-abempty
               / path-absolute
               / path-noscheme
               / path-empty

 */
/// fragment      = *( pchar / "/" / "?" )
/// \param header_map
/// \return
bool MessageValidator::CheckLocation(const std::string &msg, size_t &pos) {
  return true;
}

/// Referer = absolute-URI / partial-URI
/// \param header_map
/// \return
bool MessageValidator::CheckReferer(const std::string &msg, size_t &pos) {
  return true;
}

/// Retry-After = HTTP-date / delay-seconds
/// delay-seconds = 1*DIGIT
/// \param header_map
/// \return
bool MessageValidator::CheckRetryAfter(const std::string &msg, size_t &pos) {
  return true;
}

/// Server = product *( RWS ( product / comment ) )
/// product = token [ «/» product-version ]
/// product-version = token
/// comment = "(" *( ctext / quoted-pair / comment ) ")"
/// \param header_map
/// \return
bool MessageValidator::CheckServer(const std::string &msg, size_t &pos) {
  return true;
}

/// Transfer-Encoding = 1#transfer-coding
/// Transfer-Encoding = *( «,» OWS ) transfer-coding *( OWS «,» [ OWS transfer-coding ] )
/// transfer-coding = «chunked» / «compress» / «deflate» / «gzip» / transfer-extension
/// transfer-extension = token *( OWS «;» OWS transfer-parameter )
/// transfer-parameter = token BWS «=» BWS ( token / quoted-string )
/// \param header_map
/// \return
bool MessageValidator::CheckTransferEncoding(const std::string &msg, size_t &pos) {
  return true;
}

/// User-Agent = product *( RWS ( product / comment ) )
/// \param header_map
/// \return
bool MessageValidator::CheckUserAgent(const std::string &msg, size_t &pos) {
  return true;
}

/// WWW-Authenticate = 1#challenge
/// challenge = auth-scheme [ 1*SP ( token68 / [ ( «,» / auth-param ) *( OWS «,» [ OWS auth-param ] ) ] ) ]
/// auth-scheme = token
/// auth-param = token BWS «=» BWS ( token / quoted-string )
/// \param header_map
/// \return
bool MessageValidator::CheckWwwAuthenticate(const std::string &msg, size_t &pos) {
  return true;
}



static const char *vchardelimiters = "\"(),/:;<=>?@[\\]{}";


/// CRLF = \ r \ n
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 1] is CRLF
bool MessageValidator::iscrlf(const std::string &s, size_t index) {
  return s[index] == '\r' && s[index + 1] == '\n';
}

/// obs-text = %x80-FF
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is obs-text
bool MessageValidator::isobstext(int c, int) {
  return c >= '\x80' && c <= '\xFF';
}

/// VCHAR = Visible ASCII symbol
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is VCHAR
bool MessageValidator::isvchar(int c, int) {
  return isprint(c) || isobstext(c);
}

/// OWS = *( SP / HTAB )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is OWS
bool MessageValidator::isows(int c, int) {
  return c == ' ' || c == '\t';
}

/// HEX = «0» / «1» / «2» / «3» / «4» / «5» / «6» / «7» / «8» / «9» / «A» / «B» / «C» / «D» / «E» / «F»
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is HEX
bool MessageValidator::ishex(int c, int) {
  return (c > 47 && c < 58) || (c > 64 && c < 71) || (c > 94 && c < 103); // not sure about c > 94 && c < 103
}



/// obs-fold = CRLF 1*( SP / HTAB )
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 2] is obs-fold
bool MessageValidator::isobsfold(const std::string &s, size_t & index) {
  return iscrlf(s, index) && isows(s[index + 2]);
}

/// pct-encoded = "%" HEXDIG HEXDIG
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 2] is pct-encoded
bool MessageValidator::ispctencoded(const std::string & s, size_t & index) {
  return s[index] == '%' && ishex(s[index + 1]) && ishex(s[index + 2]);
}


static const char *pchar = "!$&'()*+,;=-._~:@";
/// pchar = unreserved / pct-encoded / sub-delims / ":" / "@".
/// pct-encoded checked only with start as '%' symbol, need further call ispctencoded()
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is
bool MessageValidator::ispchar(int c, int) {
  return isalnum(c) || strchr(pchar, c);
}

/// query = *( pchar / "/" / "?" )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is query
bool MessageValidator::isquery(int c, int) {
  return ispchar(c) || c == '/' || c == '?';
}

static const char *tchar = "!#$%&'*+-.^_`|~";
/// tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is tchar
bool MessageValidator::istchar(int c, int) {
  return isalnum(c) || strchr(tchar, c);
}

/// reason-phrase = *( HTAB / SP / VCHAR / obs-text )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is reason-phrase
bool MessageValidator::isreasonphrase(int c, int) {
  return isows(c) || isvchar(c) || isobstext(c);
}

/// quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 1] is quoted-pair
bool MessageValidator::isquotedpair(const std::string &s, size_t & index) {
  return s[index] == '\\' && isreasonphrase(s[index + 1]);
}


/// ctext = HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is ctext
bool MessageValidator::isctext(int c, int) {
  return isows(c) || (c >= '\x21' && c <= '\x27') || (c >= '\x2A' && c <= '\x5B') ||
      (c >= '\x5D' && c <= '\x7E') || isobstext(c);
}

/// qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is qdtext symbol
bool MessageValidator::isqdtext(int c, int) {
  return c == '\t' || c == ' ' || c == '\x21' ||
      (c >= '\x23' && c <= '\x5B') ||
      (c >= '\x5D' && c <= '\x7E') || isobstext(c);
}

bool MessageValidator::isstatuscode(const std::string &s, size_t & index) {
  return  isdigit(s[index]) && isdigit(s[index + 1]) && isdigit(s[index + 2]);
}

bool MessageValidator::isdots(int c, int) {
  return c == ':';
}


//template<class T1, class T2>
//bool MessageValidator::expression_test_abnf(const std::string &s,
//                                            size_t &index,
//                                            bool (MessageValidator::*f)(T1, T2),
//                                            size_t status_code,
//                                            size_t step,
//                                            size_t min,
//                                            size_t max) {
//  size_t counter = 0;
//  bool found = false;
//  for(; )
//  return test;
//}
