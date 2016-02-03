/*
 * Copyright (C) 2007-2016 Siemens AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************
 *
 * @author Daniel.Peintner.EXT@siemens.com
 * @version 2016-02-03 
 * @contact Joerg.Heuer@siemens.com
 *
 * <p>Code generated by EXIdizer</p>
 * <p>Schema: schema-for-json.xsd</p>
 *
 *
 ********************************************************************/



#include "DecoderChannel.h"
#include "EXIOptions.h"
#include "BitInputStream.h"
#include "EXITypes.h"
#include "ErrorCodes.h"

#ifndef BYTE_DECODER_CHANNEL_C
#define BYTE_DECODER_CHANNEL_C


#if EXI_OPTION_ALIGNMENT == BYTE_ALIGNMENT

int decode(bitstream_t* stream, uint8_t* b) {
	int errn = 0;
	if ( (*stream->pos) < stream->size ) {
		*b = stream->data[(*stream->pos)++];
	} else {
		errn = EXI_ERROR_INPUT_STREAM_EOF;
	}
#if EXI_STREAM == FILE_STREAM
	*b = (uint8_t)(getc(stream->file));
	/* EOF cannot be used, 0xFF valid value */
	if ( feof(stream->file) || ferror(stream->file) ) {
		errn = EXI_ERROR_INPUT_STREAM_EOF;
	}
#endif /* EXI_STREAM == FILE_STREAM */

	return errn;
}

int decodeBoolean(bitstream_t* stream, int* b) {
	uint8_t bb;
	int errn = decode(stream, &bb);
	*b = (bb == 0) ? 0 : 1;
	return errn;
}

/**
 * Decodes and returns an n-bit unsigned integer using the minimum number of
 * bytes required for n bits.
 */
int decodeNBitUnsignedInteger(bitstream_t* stream, uint16_t nbits, uint32_t* uint32) {
	uint16_t bitsRead = 0;
	uint8_t b;
	int errn = 0;
	*uint32 = 0;

	while (errn == 0 && bitsRead < nbits) {
		errn = decode(stream, &b);
		*uint32 = *uint32 + (uint32_t)(b << bitsRead);
		bitsRead = (uint16_t)(bitsRead + 8);
	}

	return errn;
}

#endif

#endif

