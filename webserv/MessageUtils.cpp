//
// Created by sfcdo on 05.06.2021.
//
#include <string>

static const char *vchardelimiters = "\"(),/:;<=>?@[\\]{}";


/// CRLF = \ r \ n
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 1] is CRLF
bool iscrlf(const std::string &s, size_t index) {
  return s[index] == '\r' && s[index + 1] == '\n';
}

/// obs-text = %x80-FF
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is obs-text
bool isobstext(int c) {
  return c >= '\x80' && c <= '\xFF';
}

/// VCHAR = Visible ASCII symbol
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is VCHAR
bool isvchar(int c) {
  return isprint(c) || isobstext(c);
}

/// OWS = *( SP / HTAB )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is OWS
bool isows(int c) {
  return c == ' ' || c == '\t';
}

/// HEX = «0» / «1» / «2» / «3» / «4» / «5» / «6» / «7» / «8» / «9» / «A» / «B» / «C» / «D» / «E» / «F»
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is HEX
bool ishex(int c) {
  return c > 47 && c < 58 || c > 64 && c < 71 || c > 94 && c < 103; // not sure about c > 94 && c < 103
}



/// obs-fold = CRLF 1*( SP / HTAB )
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 2] is obs-fold
bool isobsfold(const std::string &s, size_t index) {
  return iscrlf(s, index) && isows(s[index + 2]);
}

/// pct-encoded = "%" HEXDIG HEXDIG
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 2] is pct-encoded
bool ispctencoded(const std::string & s, size_t index) {
  return s[index] == '%' && ishex(s[index + 1]) && ishex(s[index + 2]);
}


static const char *pchar = "!$&'()*+,;=-._~:@%";
/// pchar = unreserved / pct-encoded / sub-delims / ":" / "@".
/// pct-encoded checked only with start as '%' symbol, need further call ispctencoded()
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is
bool ispchar(int c) {
  return isalnum(c) || strchr(pchar, c);
}

/// query = *( pchar / "/" / "?" )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is query
bool isquery(int c) {
  return ispchar(c) || c == '/' || c == '?';
}

static const char *tchar = "!#$%&'*+-.^_`|~";
/// tchar  = «!» / «#» / «$» / «%» / «&» / «’» / «*» / «+» / «-» / «.» / «^» / «_» / «‘» / «|» / «~» / DIGIT / ALPHA
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is tchar
bool istchar(int c) {
  return isalnum(c) || strchr(tchar, c);
}

/// reason-phrase = *( HTAB / SP / VCHAR / obs-text )
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is reason-phrase
bool isreasonphrase(int c) {
  return isows(c) || isvchar(c) || isobstext(c);
}

/// quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
/// \param string s to be checked at index msg_pos
/// \param int index, msg_pos to start check at
/// \return bool, indicates whether string at msg_pos range [msg_pos, msg_pos + 1] is quoted-pair
bool isquotedpair(const std::string &s, size_t index) {
  return s[index] == '\\' && isreasonphrase(s[index + 1]);
}


/// ctext = HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is ctext
bool isctext(int c) {
  return isows(c) || c >= '\x21' && c <= '\x27' || c >= '\x2A' && c <= '\x5B' ||
      c >= '\x5D' && c <= '\x7E' || isobstext(c);
}

/// qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
/// \param int c, a char symbol
/// \return bool, indicates whether parameter c is qdtext symbol
bool isqdtext(int c) {
  return c == '\t' || c == ' ' || c == '\x21' ||
      c >= '\x23' && c <= '\x5B' ||
      c >= '\x5D' && c <= '\x7E' || isobstext(c);
}

bool isstatuscode(const std::string &s, size_t index) {
  return  isdigit(s[index]) && isdigit(s[index + 1]) && isdigit(s[index + 2]);
}