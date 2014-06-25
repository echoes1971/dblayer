/*

qutexrlib QT-based XML-RPC library
Copyright (C) 2003  P. Oscar Boykin <boykin@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "qutexmlrpc/xrbase64.h"

/**
 * this code is adapted from Wei Dai's public domain base64.cpp
 */

const int XRBase64::MAX_LINE_LENGTH = 76;
const char XRBase64::vec[] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
;
const char XRBase64::padding = '=';

int XRBase64::convertToNumber(char inByte) {

    if (inByte >= 'A' && inByte <= 'Z')
        return (inByte - 'A');

    if (inByte >= 'a' && inByte <= 'z')
        return (inByte - 'a' + 26);

    if (inByte >= '0' && inByte <= '9')
        return (inByte - '0' + 52);

    if (inByte == '+')
        return (62);

    if (inByte == '/')
        return (63);

    return (-1);
}

QByteArray XRBase64::decode(QString ascii)
{
  uint j;
  int i;
  uint length = ascii.length();
  const char* data = ascii.toAscii().constData();
  //Make a buffer that can hold the maximum amount of
  //binary data that the ascii string can encode.  we
  //will truncate it later to the right size
  QByteArray result;
  result.resize((3*length)/4 + 1);
  uint bin_length = 0; //How many bytes have we decoded:
  
  unsigned char inBuf[4];
  memset(inBuf,0,4);
  int buf_pos = 0;
  for(j=0; j < length; j++) {
      i = convertToNumber(data[j]);
      if(i >= 0) {
          inBuf[buf_pos++] = (unsigned char)i;
	  if( buf_pos == 4) {
	      result[bin_length++] = (inBuf[0] << 2) | (inBuf[1] >> 4);
              result[bin_length++] = (inBuf[1] << 4) | (inBuf[2] >> 2);
              result[bin_length++] = (inBuf[2] << 6) | inBuf[3];
              buf_pos = 0;
              memset(inBuf,0,4);
	  }
      }
  }
  //We have 3 or less ascii chars to decode:
  //
  // 1 ascii char -> 6 bits, so we need at least
  // 2 chars to get more than 8 bits... and so on.
  if( buf_pos > 1) {
    result[bin_length++] = (inBuf[0] << 2) | (inBuf[1] >> 4);
  }
  if( buf_pos > 2) {
    result[bin_length++] = (inBuf[1] << 4) | (inBuf[2] >> 2);
  }
  //We don't need to check for buf_pos == 4, because,
  //we would handled it already (in the above loop).

  //Resize the buffer to be the correct length:
  result.resize(bin_length);
  return result;
}

QString XRBase64::encode(const QByteArray& bin)
{
  int i;
  QString result;
  //We write into the ascii encoded version into the following string:
  // 1 extra space for the '\n' and one more for the null ending.
  char one_line[MAX_LINE_LENGTH+2];
  int length = 0;
  //Initialize the buffer:
  memset(one_line,0,MAX_LINE_LENGTH+2);
  
  unsigned char b0,b1,b2;
  int buf_size = 0;
  for(i = 0; i < bin.size(); i+=3) {
     b0 = bin[i];
     if( bin.size() > i + 2) {
        b1 = bin[i+1];
        b2 = bin[i+2];
	buf_size = 3;
     }
     else if( bin.size() > i + 1) {
        b1 = bin[i+1];
	b2 = 0;
	buf_size = 2;
     }
     else {
        b1 = 0;
	b2 = 0;
	buf_size = 1;
     }
     one_line[length++] = vec[ (b0 & 0xFC) >> 2 ];
     one_line[length++] = vec[ ((b0 & 0x03) << 4) | (b1 >> 4) ];
     if( buf_size > 1) {
       one_line[length++] = vec[ ((b1 & 0x0F) << 2) | (b2 >> 6) ] ;
     }
     else {
       one_line[length++] = padding;
     }
     if( buf_size > 2) {
       one_line[length++] = vec[ (b2 & 0x3F) ];
     }
     else {
       one_line[length++] = padding;
     }
     
     //If after the next round we will be too long,
     //stop now
     if((length + 4) > MAX_LINE_LENGTH) {
         one_line[length] = '\n';
         result.append(one_line);
	 memset(one_line,0,MAX_LINE_LENGTH+2);
	 length = 0;
     }
  }
  //Add the last part of a line:
  result.append(one_line);
  return result;
}
