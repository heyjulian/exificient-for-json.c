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
 * \file 	UCSString.h
 * \brief 	UCS String utilities
 *
 */

#ifndef UCS_STRING_H
#define UCS_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "EXITypes.h"

/**
 * \brief 		Converts characters to USC string
 *
 * \param       chars	   		Characters
 * \param       s				UCS String
 * \return                  	Error-Code <> 0
 *
 */
int toUCSString(char* chars, exi_string_t* s);


/**
 * \brief 		Converts String to ASCII characters
 *
 * 				Note: fails if string contains non ASCII characters.
 * 				Note: Make also sure that the char array is large enough.
 *
 * \param       string			UCS String
 * \param       string	   		Input String
 * \param       outASCII	   	ASCII Characters (out)
 * \return                  	Error-Code <> 0
 *
 */
int toASCIIString(exi_string_t* string, char* outASCII);



#ifdef __cplusplus
}
#endif

#endif

