//
// Created by sfcdo on 02.07.2021.
//

#include "HeaderValidator.hpp"

/// Accept-Charset = 1#( ( charset / "*" ) [ weight ] )
/// Accept-Charset = *( «,» OWS ) ( ( charset / «*» ) [ weight ] ) *( OWS «,» [ OWS ( ( charset / «*» ) [ weight ] ) ] )
/// charset = token
/// weight = OWS «;» OWS «q=» qvalue
/// qvalue = ( «0» [ «.» *3DIGIT ] ) / ( «1» [ «.» *3″0″ ] )
/// \param header_map
/// \return
bool HeaderValidator::CheckAcceptCharsets(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Accept-Language = *( «,» OWS ) ( language-range [ weight ] ) *( OWS «,» [ OWS ( language-range [ weight ] ) ] )
/// language-range   = (1*8ALPHA *("-" 1*8alphanum)) / "*"                        | at least 1 and at most 8 ALPHA
/// \param header_map
/// \return
bool HeaderValidator::CheckAcceptLanguage(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Allow = #method
/// #method => [ ( "," / method ) *( OWS "," [ OWS method ] ) ]
/// \param header_map
/// \return
bool HeaderValidator::CheckAllow(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Authorization = credentials
/// credentials = auth-scheme [ 1*SP ( token68 / [ ( «,» / auth-param ) *( OWS «,» [ OWS auth-param ] ) ] ) ]
/// auth-scheme = token
/// token68 = 1*( ALPHA / DIGIT / «-» / «.» / «_» / «~» / «+» / «/» ) *»=»
/// auth-param = token BWS «=» BWS ( token / quoted-string )
/// BWS = OWS
/// \param header_map
/// \return
bool HeaderValidator::CheckAuthorization(std::map<std::string, std::string> &header_map) {
  return false;
}


/// Content-Language = 1#language-tag
/// language-tag = langtag
/// langtag = language[«-» script][«-» region]*(«-» variant)*(«-» extension)[«-» privateuse]
/// https://efim360.ru/rfc-5646-tegi-dlya-identifikatsii-yazykov/
/// \param header_map
/// \return
bool HeaderValidator::CheckContentLanguage(std::map<std::string, std::string> &header_map) {
  return false;
}


/// Content-Length = 1*DIGIT
/// \param header_map
/// \return
bool HeaderValidator::CheckContentLength(std::map<std::string, std::string> &header_map) {
  return false;
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
bool HeaderValidator::CheckContentLocation(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Content-Type = media-type
/// media-type = type "/" subtype *( OWS ";" OWS parameter )
/// type = token
/// subtype = token
/// parameter = token "=" ( token / quoted-string )
/// \param header_map
/// \return
bool HeaderValidator::CheckContentType(std::map<std::string, std::string> &header_map) {
  return false;
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
bool HeaderValidator::CheckDate(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Host = uri-host [ «:» port ]
/// host = IP-literal / IPv4address / reg-name
/// port = *DIGIT                 | pri etom po scheme http default = 80, po https = 443
/// \param header_map
/// \return
bool HeaderValidator::CheckHost(std::map<std::string, std::string> &header_map) {
  return false;
}


/// Last-Modified = HTTP-date
/// \param header_map
/// \return
bool HeaderValidator::CheckLastModified(std::map<std::string, std::string> &header_map) {
  return false;
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
bool HeaderValidator::CheckLocation(std::map<std::string, std::string> &header_map) {
  return false;
}


/// Referer = absolute-URI / partial-URI
/// \param header_map
/// \return
bool HeaderValidator::CheckReferer(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Retry-After = HTTP-date / delay-seconds
/// delay-seconds = 1*DIGIT
/// \param header_map
/// \return
bool HeaderValidator::CheckRetryAfter(std::map<std::string, std::string> &header_map) {
  return false;
}


/// Server = product *( RWS ( product / comment ) )
/// product = token [ «/» product-version ]
/// product-version = token
/// comment = "(" *( ctext / quoted-pair / comment ) ")"
/// \param header_map
/// \return
bool HeaderValidator::CheckServer(std::map<std::string, std::string> &header_map) {
  return false;
}

/// Transfer-Encoding = 1#transfer-coding
/// Transfer-Encoding = *( «,» OWS ) transfer-coding *( OWS «,» [ OWS transfer-coding ] )
/// transfer-coding = «chunked» / «compress» / «deflate» / «gzip» / transfer-extension
/// transfer-extension = token *( OWS «;» OWS transfer-parameter )
/// transfer-parameter = token BWS «=» BWS ( token / quoted-string )
/// \param header_map
/// \return
bool HeaderValidator::CheckTransferEncoding(std::map<std::string, std::string> &header_map) {
  return false;
}

/// User-Agent = product *( RWS ( product / comment ) )
/// \param header_map
/// \return
bool HeaderValidator::CheckUserAgent(std::map<std::string, std::string> &header_map) {
  return false;
}

/// WWW-Authenticate = 1#challenge
/// challenge = auth-scheme [ 1*SP ( token68 / [ ( «,» / auth-param ) *( OWS «,» [ OWS auth-param ] ) ] ) ]
/// auth-scheme = token
/// auth-param = token BWS «=» BWS ( token / quoted-string )
/// \param header_map
/// \return
bool HeaderValidator::CheckWwwAuthenticate(std::map<std::string, std::string> &header_map) {
  return false;
}
