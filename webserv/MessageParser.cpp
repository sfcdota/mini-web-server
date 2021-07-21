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
void MessageParser::ProcessHeaders(Request &request) {
  msg = request.GetRequestBuffer();
  std::string token;
  size_t tmp = 0;
  size_t pointer;
  size_t end_of_header_value;
  size_t end_of_header;
  std::string value;
  std::map<std::string, std::string> headers;
  size_t tmp_end = msg.find("\r\n");
  for (int i = 0; i < 3 ; i++) {
    pointer = msg.find(' ', tmp);
    headers.insert(std::make_pair(request_line_fields[i], msg.substr(tmp, pointer < tmp_end ? pointer - tmp : tmp_end - tmp)));
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
     while (MessageValidator::isows(msg[end_of_header_value]))
       end_of_header_value--;
     headers.insert(std::make_pair(token, msg.substr(pointer, end_of_header_value - pointer)));
     tmp = end_of_header + 2;
   }
   request.SetHeaders(headers);
}

void MessageParser::ParseBody(Request & request) {
  const std::string & buffer = request.GetRequestBuffer();
  size_t pos = buffer.find("0\r\n\r\n");
  size_t tmp;
  long length;
  const char* debug = buffer.c_str();
  if (pos == std::string::npos)
    return;
  for (size_t index = 0; (tmp = buffer.find("\r\n", index)) < pos;) {
    length = strtol(&buffer.c_str()[index], NULL, 16);
    if (length < 0) {
      std::cout << "LENGTH OF CHUNKED PART DATA WAS BELOW ZERO. BODY PARSING ABORTED!" << std::endl;
      request.SetFailed(400);
      break;
    }
    if (length > buffer.length() - index) {
      std::cout << "CHUNKED LENGTH WRONG SIZE ENCOUTERED. INPUT DATA WAS FEWER SYMBOLS" << std::endl;
      request.SetFailed(400);
      break;
    }
//    std::cout << "GETTING CHUNKED DATA PART. PART SIZE = " << length;
    request.AppendRequestBody(buffer.substr(tmp + 2,  length));
    index = tmp + 2 + length;
//    std::cout << "  chunked length = " << length  << std::endl;
    if (buffer.find("\r\n", index) != index) {
      std::cout << "NO CRLF AFTER CHUNKED DATA BLOCK. REQUEST FAILED" << std::endl;
      request.SetFailed(400);
      break;
    }
    else
      index += 2;
//    std::cout << request.buffer.length() - index << " data remains " << std::endl;
  }
  std::cout << "PARSE OF CHUNKED BODY SUCCESSFULLY ENDED WITH BODY LENGTH = " << request.GetBody().length() << std::endl;
  request.SetFormed(true);
}







