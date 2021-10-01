/*
   Copyright 2021 Simon (psyomn) Symeonidis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "client.h"

static int send_buffer(struct client *client, uint8_t *buffer, size_t len) {
  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = client->port;
  addr.sin_addr.s_addr = inet_addr(client->host);

  int sock = 0;
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    return -1;
  }

  if (sendto(sock, buffer, len, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("sendto");
    return -1;
  }

  close(sock);

  return 0;
}

int packetdraw_client_send_message(struct client *client) {
  uint8_t buffer[512] = {0};

  switch (client->type) {
    case PACKETDRAW_MSGTYPE_PIXEL:
      {
        struct message mess = {0};
        mess.discriminator = PACKETDRAW_MSGTYPE_PIXEL;
        mess.as.pixel.x = htonl(client->x);
        mess.as.pixel.y = htonl(client->y);
        mess.as.pixel.rgb[0] = client->r;
        mess.as.pixel.rgb[1] = client->g;
        mess.as.pixel.rgb[2] = client->b;
        memcpy(buffer, &mess, sizeof(mess));
      }
      break;
    case PACKETDRAW_MSGTYPE_LINE:
      {
        struct message mess = {0};
        mess.discriminator = PACKETDRAW_MSGTYPE_LINE;
        mess.as.line.x = htonl(client->x);
        mess.as.line.y = htonl(client->y);
        mess.as.line.to_x = htonl(client->w);
        mess.as.line.to_y = htonl(client->h);
        mess.as.line.rgb[0] = client->r;
        mess.as.line.rgb[1] = client->g;
        mess.as.line.rgb[2] = client->b;
        memcpy(buffer, &mess, sizeof(mess));
      }
      break;
    case PACKETDRAW_MSGTYPE_RECTANGLE:
      {
        struct message mess = {0};
        mess.discriminator = PACKETDRAW_MSGTYPE_RECTANGLE;
        mess.as.rectangle.x = htonl(client->x);
        mess.as.rectangle.y = htonl(client->y);
        mess.as.rectangle.to_x = htonl(client->w);
        mess.as.rectangle.to_y = htonl(client->h);
        mess.as.rectangle.rgb[0] = client->r;
        mess.as.rectangle.rgb[1] = client->g;
        mess.as.rectangle.rgb[2] = client->b;
        memcpy(buffer, &mess, sizeof(mess));
      }
      break;
    default:
      fprintf(stderr, "unknown message type\n");
      return -1;
  }

  if (send_buffer(client, buffer, sizeof(buffer)) == -1) {
    fprintf(stderr, "problem sending buffer\n");
    return -1;
  }

  return 0;
}

