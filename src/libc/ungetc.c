#include <stdio.h>
#include <stdint.h>
#include <fileioc.h>

__attribute__((__weak__))
int ungetc(int ch, FILE *__restrict stream) {
    // operation fails and stream is not affected
    if (ch == EOF) {
        return EOF;
    }
    // can't use ungetc here
    if (stream == NULL || stream == stdout || stream == stderr) {
        return EOF;
    }
    // todo: implement this possibly
    if (stream == stdin) {
        return EOF;
    }
    // test that we can go backwards
    uint16_t pos = ti_Tell(stream->slot);
    if (pos == 0) {
        return EOF;
    }
    const unsigned char *__restrict ptr = (const unsigned char *__restrict)ti_GetDataPtr(stream->slot);
    // we only support pushing back the current character onto the stream
    if ((*(ptr - 1) != (unsigned char)ch)) {
        return EOF;
    }
    int seek_status = ti_Seek(-1, SEEK_CUR, stream->slot);
    if (seek_status == EOF) {
        return EOF;
    }
    // we are no longer at the end of the file
    stream->eof = 0;
    return ch;
}
