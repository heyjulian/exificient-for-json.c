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
* \file 	EXIEncoder.h
* \brief 	EXI Encoder
*
*/

#ifndef EXI_exiForJson_ENCODER_H
#define EXI_exiForJson_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "EXITypes.h"
#include "exiForJsonEXIEncoder.h"

/**
 * \brief 	Initialize EXI encoder
 *
 *         	Resets & initializes the EXI encoder.
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       runtimeTable   	Runtime name-tables
 * \param       stringTable   	String table
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonInitEncoder(bitstream_t* stream, exi_state_t* state,
		exi_name_table_runtime_t runtimeTable, exi_value_table_t stringTable);

/**
 * \brief  	Reports the beginning of a set of XML events
 *
 *			see EXI StartDocument (SD) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeStartDocument(bitstream_t* stream,
		exi_state_t* state);

/**
 * \brief  	Reports the end of a set of XML events
 *
 *			see EXI EndDocument (ED) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeEndDocument(bitstream_t* stream,
		exi_state_t* state);

/**
 * \brief  	Supplies the start of a known element
 *
 *			Provides access to the namespace URI and local name of the start tag by providing qnameID.
 *			Note: Both, namespace URI and local name are known (schema-informed) or have been encoded before.
 *			see EXI StartElement (SE) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       qnameID   		qualified name ID
 * \return                  	Error-Code <> 0
 *
 */
int
exiexiForJsonEncodeStartElement(bitstream_t* stream,
		exi_state_t* state, uint16_t qnameID);

/**
 * \brief  	Supplies the start of an element where the namespace is known
 *
 *			Note: Namespace URI is known (schema-informed) or has been encoded before while local name is unknown.
 *			see EXI StartElement (SE) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       namespaceUriID  namespace URI ID
 * \param       localName  		local name as string
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeStartElementNS(bitstream_t* stream,
		exi_state_t* state, uint16_t namespaceUriID,
		exi_string_t* localName);

/**
 * \brief  	Supplies the start of an element where the namespace and the local name is unknown
 *
 *			Note: Neither namespace URI nor local name is known.
 *			see EXI StartElement (SE) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       namespaceUri  	namespace URI as string
 * \param       localName  		local name as string
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeStartElementGeneric(bitstream_t* stream,
		exi_state_t* state, exi_string_t* namespaceUri,
		exi_string_t* localName);

/**
 * \brief  	Supplies the end tag of an element
 *
 *			see EXI EndElement (EE) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeEndElement(bitstream_t* stream,
		exi_state_t* state);

/**
 * \brief  	Supplies characters value
 *
 *			Note: Special handling for list value.
 *			see EXI Characters (CH) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       val   			Characters value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeCharacters(bitstream_t* stream,
		exi_state_t* state, exi_value_t* val);

/**
 * \brief  	Supplies an attribute qname and value
 *
 *			Note: Special handling for list value.
 *			see EXI Attribute (AT) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       qnameID   		qualified name ID
 * \param       val   			Attribute value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeAttribute(bitstream_t* stream,
		exi_state_t* state, uint16_t qnameID, exi_value_t* val);




/**
 * \brief  	Supplies an attribute qname and value where the namespace is known
 *
 *			Note: Special handling for list value.
 *			see EXI Attribute (AT) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       namespaceUriID  Qualified namespace URI ID
 * \param       localName  		Qualified localname as String
 * \param       val   			Attribute value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeAttributeNS(bitstream_t* stream,
		exi_state_t* state, uint16_t namespaceUriID,
		exi_string_t* localName, exi_value_t* val);


/**
 * \brief  	Supplies an attribute qname and value where the namespace and the local name is unknown
 *
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       namespaceUri  	namespace URI as string
 * \param       localName  		local name as string
 * \param       val   			Attribute value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeAttributeGeneric(bitstream_t* stream,
		exi_state_t* state, exi_string_t* namespaceUri,
		exi_string_t* localName, exi_value_t* val);



/**
 * \brief  	Supplies an xsi:nil attribute
 *
 *			see EXI Attribute (AT) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       val   			Attribute value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeAttributeXsiNil(bitstream_t* stream,
		exi_state_t* state, exi_value_t* val);

/**
 * \brief  	Supplies an xsi:type attribute
 *
 *			see EXI Attribute (AT) event
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       val   			Attribute value
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeAttributeXsiType(bitstream_t* stream,
		exi_state_t* state, exi_value_t* val);

/**
 * \brief  	Supplies list value for characters and attribute event
 *
 *			List values are special given that first the number and type of entry needs to be passed (element/attribute event) and then one by one the actual values.
 *
 * \param       stream   		Output Stream
 * \param       state   		Codec state
 * \param       qnameID   		qualified name ID
 * \param       val   			one list value
 * \param       lt   			list type
 * \return                  	Error-Code <> 0
 *
 */
int exiexiForJsonEncodeListValue(bitstream_t* stream,
		exi_state_t* state, uint16_t qnameID, exi_value_t* val, exi_list_t lt);

#ifdef __cplusplus
}
#endif

#endif