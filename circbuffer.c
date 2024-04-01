/*
 * circbuffer.c
 *
 *  Created on: Mar 26, 2024
 *      Author: hunte
 */

#include "circbuffer.h"

//Increment a pointer and wrap to the beginning if necessary
uint8_t* _circbuf8_inc(struct circbuf8 *hBuf, uint8_t *p)
{
	p++;
	if (p >= hBuf->start + hBuf->size)
	{
		p = hBuf->start;
	}
	return p;
}

/*
 * @param hBuf, pointer to a circular buffer
 * @param mem, pointer to an area of memory to use for the buffer
 * @param size, size of the memory area mem which may be used
 */
int circbuf8_init(struct circbuf8 *hBuf, uint8_t *mem, uint32_t size)
{
	if (mem != 0)
	{
		hBuf->start = mem;
		hBuf->head = mem;
		hBuf->tail = mem;
		hBuf->size = size;
		return CIRCBUF_STATUS_OK;
	}
	else
	{
		return CIRCBUF_STATUS_ERR;
	}
}

/*
 * @param hBuf, pointer to a circular buffer
 * @param byte, a uint8_t value to write into the buffer
 */
int circbuf8_writeByte(struct circbuf8 *hBuf, uint8_t byte)
{
	int status = CIRCBUF_STATUS_OK;
	*(hBuf->head) = byte;
	hBuf->head = _circbuf8_inc(hBuf, hBuf->head);
	if (hBuf->head == hBuf->tail)
	{
		hBuf->tail = _circbuf8_inc(hBuf, hBuf->tail);
		status = CIRCBUF_STATUS_OVERRUN;
	}
	return status;
}

/*
 * @param hBuf, pointer to a circular buffer
 * @param str, a pointer to a null-terminated string
 * Note, it is assumed that the string is no longer than the buffer.
 */
int circbuf8_writeString(struct circbuf8 *hBuf, const char *str)
{
	int status = CIRCBUF_STATUS_OK;
	const char *c = str;
	int count = 0;
	while (*c != '\0')
	{
		circbuf8_writeByte(hBuf, *c);
		++c;
		++count;
		if (count == hBuf->size)
		{
			status = CIRCBUF_STATUS_ERR;
			break; //breaks out of the while loop
		}
	}
	return status;
}

/*
 * @param hBuf, pointer to a circular buffer
 * @param byte, a pointer to a byte where the result of the read will be stored
 */
int circbuf8_readByte(struct circbuf8 *hBuf, uint8_t *byte)
{
	if (hBuf->tail == hBuf->head) { //nothing to read!
		return CIRCBUF_STATUS_EMPTY;
	}
	else
	{
		*byte = *(hBuf->tail);
		hBuf->tail = _circbuf8_inc(hBuf, hBuf->tail);
		return CIRCBUF_STATUS_OK;
	}
}

int _circbuf8_peekUntil(struct circbuf8 *hBuf, char c)
{
	if (hBuf->tail == hBuf->head)
	{
		return CIRCBUF_STATUS_EMPTY;
	}
	uint8_t *b = hBuf->tail;
	while (b != hBuf->head)
	{
		if (*b == c)
			return CIRCBUF_STATUS_OK;
		else
			++b;
	}
	return CIRCBUF_STATUS_ERR;
}

/*
 * @param hBuf, pointer to a circular buffer
 * @param mem, a memory location in which to put the bytes which are read
 * @param max_size, the maximum number of bytes which can be put into array mem
 * @param c, the character to look for to stop reading
 * @ret the number of characters in the string that is read
 * The character c will be read, too. The string placed in mem will be null terminated.
 */
int circbuf8_readUntil(struct circbuf8 *hBuf, uint8_t *mem,
						uint32_t max_size, uint8_t c)
{
	if (max_size < 2 || mem == 0)
		return 0;
	if (hBuf->head == hBuf->tail)
		return 0;


	//first check ahead to see if there actually is a character c
	// if not, return a status message indicating as such
	if (_circbuf8_peekUntil(hBuf, c) != CIRCBUF_STATUS_OK)
		return 0;

	uint8_t *p = mem;
	int done = 0;

	while (!done)
	{
		int status = circbuf8_readByte(hBuf, p);
		if (status != CIRCBUF_STATUS_OK || *p == c)
		{
			done = 1;
		}
		++p;
		if (p - mem == max_size-1) {
			done = 1;
		}
	}
	*p = '\0';
	return p-mem;
}
