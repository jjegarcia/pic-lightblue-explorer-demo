#include <stdbool.h>

#ifndef _Main_H
#define _Main_H

bool pushed = false;
bool sendSpiReadRequest = false;

void send_spi_read(void);
void service_push(void);
#endif