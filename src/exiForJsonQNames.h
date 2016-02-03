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



#ifndef EXI_exiForJson_QNAMES_H
#define EXI_exiForJson_QNAMES_H

#include "EXITypes.h"

/**
 * \brief  	Returns efficient qname (namespaceURI & localName) IDs for qnameID
 *
 * \param       state		   	Codec state
 * \param       qnameID		   	Qualified name ID
 * \param       namespaceURI	QName namespace URI ID
 * \param       localPart		QName local name ID
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonGetEQName(exi_state_t* state, uint16_t qnameID, uint16_t* namespaceURI, uint16_t* localPart);


/**
 * \brief  	Returns qnameID for qname namespaceURI & localName
 *
 * \param       state		   	Codec state
 * \param       namespaceURI	QName namespace URI ID
 * \param       localPart		QName local name ID
 * \param       qnameID		   	QName ID (out)
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonGetQNameID(exi_state_t* state, uint16_t namespaceURI, uint16_t localPart, uint16_t* qnameID);


/**
 * \brief  	Add  efficient qname (namespaceURI & localName) IDs for qnameID
 *
 * \param       state		   	Codec state
 * \param       qnameID		   	Qualified name ID
 * \param       namespaceURI	QName namespace URI ID
 * \param       localPart		QName local name ID
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonAddEQName(exi_state_t* state, uint16_t* qnameID, uint16_t namespaceURI, uint16_t localPart);

#endif

