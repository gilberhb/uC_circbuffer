/*
 * circbuffer.h
 *
 *  Created on: Mar 26, 2024
 *      Author: hunte
 */

#ifndef INC_CIRCBUFFER_H_
#define INC_CIRCBUFFER_H_

#include "stdint.h"

struct circbuf8
{
	uint8_t *start; //address of start of buffer
	uint32_t size;  //size of buffer in bytes
	uint8_t * volatile head;  //pointer to head, data is written to
	uint8_t * volatile tail;  //pointer to tail, data is read from
};

#define CIRCBUF_STATUS_OK 0
#define CIRCBUF_STATUS_ERR 1
#define CIRCBUF_STATUS_EMPTY 2
#define CIRCBUF_STATUS_OVERRUN 3
#define CIRCBUF_STATUS_NOTFOUND 4

int circbuf8_init(struct circbuf8 *hBuf, uint8_t *mem, uint32_t size);
int circbuf8_writeByte(struct circbuf8 *hBuf, uint8_t byte);
int circbuf8_writeString(struct circbuf8 *hBuf, const char *str);
int circbuf8_readByte(struct circbuf8 *hBuf, uint8_t *byte);
int circbuf8_readUntil(struct circbuf8 *hBuf, uint8_t *mem,
						uint32_t max_size, uint8_t c);

#endif /* INC_CIRCBUFFER_H_ */
