# How to use this code:

## Declare a circular buffer structure and a memory region to use (you get to pick the size)  
  
  struct circbuf8 buf;  
  #define CIRCBUF_SIZE 512  
  uint8_t buf_data[CIRCBUF_SIZE];  

## Initialize the buffer before using it:  
  int status = circbuf8_init(&buf, buf_data, CIRCBUF_SIZE);

## To write to the buffer, use the writeByte function:  
  int status;  
  char c = 'a';  
  status = circbuf8_writeByte(&buf, c);  
  
  
## To read from the buffer, use the readByte function:  
  int status;  
  char x;  
  status = circbuf8_readByte(&buf, &x);  
  
## To read strings with a known terminator, use the readUntil function:  
  int status;  
  char terminator = '\n';  
  char msg_rx[256]; //buffer to hold string data received  
  status = circbuf8_readUntil(&buf, msg_rx, 256, terminator);   

## To write null-terminated strings, use the writeString function:  
  const char *msg = "Hello, world!";  
  int status;  
  status = circbuf8_writeString(&buf, msg);  
  
  The writeString function will stop writing at the buffer size and return CIRCBUF_STATUS_ERR if the string is larger than the circular buffer.  
       
     
## Possible statuses are:  
  CIRCBUF_STATUS_OK - Success.  
  CIRCBUF_STATUS_ERR - The function call cannot be completed.  
  CIRCBUF_STATUS_EMPTY - Cannot read because the buffer is empty.
  CIRCBUF_STATUS_OVERRUN - Informs a writer that a byte written caused an overrun.  
  CIRCBUF_STATUS_NOTFOUND - No string was found with the indicated termiantor.

