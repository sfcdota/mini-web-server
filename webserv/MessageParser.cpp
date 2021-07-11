//
// Created by sfcdo on 04.06.2021.
//

#include <sstream>
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
  std::string token;
  size_t tmp = 0;
  size_t pointer;
  size_t end_of_header_value;
  size_t end_of_header;
  std::string value;
  size_t tmp_end = msg.find("\r\n");
  for (int i = 0; i < 3 ; i++) {
    pointer = msg.find(' ', tmp);
    request.request_line.insert(std::make_pair(request_line_fields[i], msg.substr(tmp, pointer < tmp_end ? pointer - tmp : tmp_end - tmp)));
    tmp = pointer + 1;
  }
  tmp = tmp_end + 2;
  tmp_end = msg.find("\r\n\r\n", tmp_end);
   for (; tmp < tmp_end ;) {
     pointer = msg.find(':', tmp);
     token = msg.substr(tmp, pointer - tmp);
     pointer++;
     pointer = msg.find_first_not_of(" \t", pointer);
     end_of_header = msg.find("\r\n", tmp);
     end_of_header_value = end_of_header;
     while (isows(msg[end_of_header_value]))
       end_of_header_value--;
     request.headers.insert(std::make_pair(token, msg.substr(pointer, end_of_header_value - pointer)));
     tmp = end_of_header + 2;
   }
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





