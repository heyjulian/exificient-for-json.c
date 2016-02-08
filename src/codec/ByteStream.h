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



/**
 * \file 	ByteStream.h
 * \brief 	Byte Stream utilities
 *
 */

#ifndef BYTE_STREAM_H
#define BYTE_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "EXITypes.h"

/**
 * \brief 		Write bytes to file
 *
 * \param       data	   		byte array
 * \param       len				length
 * \param       filename		File name
 * \return                  	Error-Code <> 0
 *
 */
int writeBytesToFile(uint8_t* data, uint16_t len, const char * filename);


/**
 * \brief 		Read bytes from file
 *
 * \param       filename   		File name
 * \param       data			byte array
 * \param       size		   	byte array size
 * \param       pos			   	byte array position
 * \return                  	Error-Code <> 0
 *
 */
int readBytesFromFile(const char * filename, uint8_t* data, uint16_t size, uint16_t* pos);

#ifdef __cplusplus
}
#endif

#endif /* BYTE_STREAM_H */