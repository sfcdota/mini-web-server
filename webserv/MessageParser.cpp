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

/* Scheme of HTTP message in terms of rfc-7230 & subject restrictions
 * HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]
 * request-line = method SP request-target SP HTTP-version CRLF
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
Request MessageParser::ProcessHeaders(Request &request) {
  msg = request.buffer;
  std::map<std::string, std::string> request_line;
  std::map<std::string, std::string> headers;
  std::string body;
  size_t pointer = msg.find(' ');
  size_t begin = 0;
  size_t temp;
  request_line.insert(std::make_pair("method", msg.substr(begin, pointer - begin)));
  pointer = msg.find(' ', begin = pointer + 1);
  request_line.insert(std::make_pair("target", msg.substr(begin, pointer - begin)));
  pointer = msg.find("\r\n", begin = pointer + 1);
  request_line.insert(std::make_pair("version", msg.substr(begin, pointer - begin)));
  if ((pointer = msg.find("\r\n\r\n", begin = pointer + 2)) != std::string::npos) {
    while(begin < pointer) {
      temp = begin;
      headers.insert(std::make_pair(msg.substr(begin, (temp = msg.find(':', begin)) - begin),
                                    msg.substr(++temp, (begin = msg.find("\r\n", temp)) - temp)));
      begin += 2;
    }
    body = msg.substr(pointer + 4);
  }
  else
    body = msg.substr(pointer + 2);
  request.SetRequestLine(request_line);
  request.SetHeaders(headers);
  request.SetBody(body);
  return request;
}

void MessageParser::ParseBody(Request & request) {
  size_t pos = request.buffer.find("\r\n\r\n");
  size_t tmp;
  long length;
  if (pos == std::string::npos)
    return;
  for (size_t index = 0; (tmp = request.buffer.find("\r\n")) != pos;) {
    length = strtol(request.buffer.c_str(), NULL, 16);
    if (length < 0)
      request.SetFailed(400);
    index += tmp + 2 + length;
    request.body += request.buffer.substr(tmp + 2, index);
  }
  request.formed = true;
}



MessageParser::MessageParser(): remain(0) {}





