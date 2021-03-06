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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "EXIforJSONDecoder.h"

#include "EXITypes.h"
#include "EXIforJSONQNames.h"
#include "StringNameTable.h"
#include "EXIforJSONEXIDecoder.h"
#include "ErrorCodes.h"

#include "MethodsBag.h"
#include "DecoderChannel.h"

#include "DynamicMemory.h"
#include "StringValueTable.h"


#ifndef EXI_FOR_JSON_DECODER_C
#define EXI_FOR_JSON_DECODER_C

/** EXI Debug mode */
#define EXI_DECODE_DEBUG 0

#if EXI_DECODE_DEBUG == 1
# define DEBUG_PRINTF(x) printf x
#else
# define DEBUG_PRINTF(x) do {} while (0)
#endif

/* String table memory setup */
static uint16_t numberOfLocalStringsDecode[EXI_EXIforJSONMAX_NUMBER_OF_QNAMES];

static exi_value_t val;

typedef enum {NONE, START_OBJECT, START_ARRAY, VALUE_STRING, VALUE_NUMBER, VALUE_FALSE, VALUE_TRUE, VALUE_NULL} json_events_t;

/* static void debugValue(exi_value_t* val, exi_value_table_t* valueTable, uint16_t qnameID); */

#define MAX_KEY_LENGTH 30

static json_events_t jsonEvent = NONE;
/* static uint16_t key = UINT16_MAX; */

static char skey[MAX_KEY_LENGTH];
static size_t skeylen = 0;


static size_t getInt64Length(int64_t number) {
	int i;
	int count = 1;  /* count starts at one because its the minimum amount of digits possible */
	if (number < 0) {
		number *= (-1);
		count++; /* minus sign */
	}

    /*
     * this loop will loop until the number "i" is bigger then "number"
     * if "i" is less then "number" multiply "i" by 10 and increase count
     * when the loop ends the number of count is the length of "number".
     */
	for(i = 10; i <= number; i*=10) {
	     count++;
	}
	return count;
}


static int writeString(char *json, size_t jlen, size_t* posJSON, exi_value_table_t* valueTable, uint16_t qnameID) {
	int errn = 0;
	int i;
	exi_string_t sv;

	if(val.type == EXI_DATATYPE_STRING) {
		json[(*posJSON)++] = '"';
		switch(val.str.type) {
		case EXI_STRING_VALUE_MISS:
			sv = val.str.miss;
			break;
		case EXI_STRING_VALUE_LOCAL_HIT:
			errn = exiGetLocalStringValue(valueTable, qnameID, val.str.localID, &sv);
			break;
		case EXI_STRING_VALUE_GLOBAL_HIT:
			errn = exiGetGlobalStringValue(valueTable, val.str.globalID, &sv);
			break;
		}
		if(errn == 0) {
			if( (sv.len + *posJSON) < jlen  ) {
				for (i = 0; i < sv.len; i++) {
					sprintf(&json[*posJSON], "%c", (char) sv.characters[i]);
					(*posJSON)++;
				}
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			json[(*posJSON)++] = '"';
		}
	} else {
		errn = EXIforJSON_ERROR_UNEXPECTED_STRING_TYPE;
	}

	return errn;
}

static int checkPendingEvent(char *json, size_t jlen, size_t* posJSON, exi_state_t* stateDecode) {
	int errn = 0;


	/* TODO check JSON string length */

	if(jsonEvent != NONE) {
		if(skeylen != 0) {
			/* write key: is already quoted such as "keyName" */
			if( (*posJSON + skeylen + 1) < jlen) {
				sprintf(&json[*posJSON], "%s", skey);
				*posJSON += skeylen;
				json[(*posJSON)++] = ':';
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
		}

		switch(jsonEvent) {
		case START_OBJECT:
			if( (*posJSON + 1) < jlen) {
				json[(*posJSON)++] = '{';
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			break;
		case START_ARRAY:
			if( (*posJSON + 1) < jlen) {
				json[(*posJSON)++] = '[';
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			break;
		case VALUE_STRING:
			errn = writeString(json, jlen, posJSON, stateDecode->stringTable, EXI_EXIforJSON_4_string);
			if( (*posJSON + 1) < jlen) {
				json[(*posJSON)++] = ',';
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			break;
		case VALUE_NUMBER:
			if(val.type == EXI_DATATYPE_FLOAT) {
				size_t remN = (jlen-*posJSON);
				
				size_t lm = getInt64Length(val.float_me.mantissa);
				size_t ll;

				if(val.float_me.exponent == 0) {
					ll = lm;
					if( (*posJSON + ll) < jlen) {
						sprintf(&json[*posJSON], "%ld", (long int) val.float_me.mantissa);
					} else {
						errn = EXIforJSON_ERROR_OUT_OF_STRING;
					}
				} else {
					size_t le = getInt64Length(val.float_me.exponent);
					ll = lm + 1 + le; /* E */
					if( (*posJSON + ll) < jlen) {
						sprintf(&json[*posJSON], "%ldE%d", (long int) val.float_me.mantissa, val.float_me.exponent);
					} else {
						errn = EXIforJSON_ERROR_OUT_OF_STRING;
					}
				}

				if(errn ==0 && ll >= 0 && ll < remN ) {
					(*posJSON) +=ll;
					if( (*posJSON + 1) < jlen) {
						json[(*posJSON)++] = ',';
					} else {
						errn = EXIforJSON_ERROR_OUT_OF_STRING;
					}
				} else {
					 errn = EXIforJSON_ERROR_OUT_OF_STRING;
				}
			} else {
				errn = EXIforJSON_ERROR_UNEXPECTED_NUMBER_TYPE;
			}
			break;
		case VALUE_FALSE:
		case VALUE_TRUE:
			if( (*posJSON + 6) < jlen) {
				if(val.type == EXI_DATATYPE_BOOLEAN) {
					if(val.boolean) {
						json[(*posJSON)++] = 't';
						json[(*posJSON)++] = 'r';
						json[(*posJSON)++] = 'u';
						json[(*posJSON)++] = 'e';
						json[(*posJSON)++] = ',';
					} else {
						json[(*posJSON)++] = 'f';
						json[(*posJSON)++] = 'a';
						json[(*posJSON)++] = 'l';
						json[(*posJSON)++] = 's';
						json[(*posJSON)++] = 'e';
						json[(*posJSON)++] = ',';
					}
				} else {
					errn = EXIforJSON_ERROR_UNEXPECTED_BOOLEAN_TYPE;
				}
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			break;
		case VALUE_NULL:
			if( (*posJSON + 5) < jlen) {
				json[(*posJSON)++] = 'n';
				json[(*posJSON)++] = 'u';
				json[(*posJSON)++] = 'l';
				json[(*posJSON)++] = 'l';
				json[(*posJSON)++] = ',';
			} else {
				errn = EXIforJSON_ERROR_OUT_OF_STRING;
			}
			break;
		default:
			errn = EXIforJSON_ERROR_UNEXPECTED_JSON_EVENT;
			break;
		}

		jsonEvent = NONE;
		skeylen = 0;
		/* value = null; */
	}


	return errn;
}

int decodeEXIforJSONsharedStrings(uint8_t* buffer, size_t blen, size_t* posDecode, char *json, size_t jlen, const char ** sharedStrings, size_t stlen) {
	int errn = 0, i;
	size_t posJSON = 0;
	int noEndOfDocument;

	bitstream_t iStream;

	exi_state_t stateDecode;
	exi_event_t event;

	uint16_t qnameID = 0; /* qname */

	exi_name_table_runtime_t runtimeTableDecode;
	exi_value_table_t stringTableDecode = { 0, EXI_EXIforJSONMAX_NUMBER_OF_QNAMES, numberOfLocalStringsDecode, NULL };

	/** string values */
	exi_value_string_table_t stringTableValuesDecode;
	stringTableValuesDecode.size = 0;
	stringTableValuesDecode.len = 0;
	stringTableValuesDecode.strs = 0;
	stringTableDecode.valueStringTable = &stringTableValuesDecode;

	/* BINARY memory setup */
	/*val.binary.len = 0;
	val.binary.size = 0;
	val.binary.data = NULL;*/

	/* STRING miss memory setup */
	val.str.type = EXI_STRING_VALUE_MISS;
	val.str.miss.len = 0;
	val.str.miss.size = 0;
	val.str.miss.characters = NULL;

	/* setup input stream */
	iStream.size = blen;
	iStream.data = buffer;
	iStream.pos = (uint16_t*)posDecode;

	noEndOfDocument = 1; /* true */

	/* init decoder (read header, set initial state) */
	/* init runtime table */
	errn = exiInitNameTableRuntime(&runtimeTableDecode);
	if (errn==0) {
		errn = exiEXIforJSONInitDecoder(&iStream, &stateDecode, &runtimeTableDecode, &stringTableDecode);

		if(errn == 0 && stlen > 0 && sharedStrings != NULL) {
			exi_string_t sv;
			for(i=0; i<stlen; i++) {
				sv.characters = (exi_string_character_t*)sharedStrings[i];
				sv.len = sv.size = strlen(sharedStrings[i]);
				errn = exiAddStringValue(stateDecode.stringTable, &sv, EXI_EXIforJSON_2_nil);
			}
		}
	}
	if (errn) {
		DEBUG_PRINTF(("[Init-Decode-ERROR] %d \n", errn));
		return errn;
	}


	DEBUG_PRINTF(("[DECODE] >>> EXI  >>> [ENCODE] \n"));

	do {
		errn = exiEXIforJSONDecodeNextEvent(&iStream, &stateDecode,
				&event);
		if (errn) {
			DEBUG_PRINTF(("[Decode-ERROR] %d \n", errn));
			return errn;
		}

		switch (event) {
		case EXI_EVENT_START_DOCUMENT:
			errn = exiEXIforJSONDecodeStartDocument(&iStream,
					&stateDecode);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-SD] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF((">> START_DOCUMENT \n"));
			break;
		case EXI_EVENT_END_DOCUMENT:
			errn = exiEXIforJSONDecodeEndDocument(&iStream,
					&stateDecode);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-ED] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF((">> END_DOCUMENT \n"));

			if(json[posJSON-1] == ',') {
				posJSON--;
			}
			json[posJSON] = '\0';

			/* signalize end of document */
			noEndOfDocument = 0; /* false */
			break;
		case EXI_EVENT_START_ELEMENT:
			errn = exiEXIforJSONDecodeStartElement(&iStream,
					&stateDecode, &qnameID);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-SE] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF((">> SE (%d) \n", qnameID));

			checkPendingEvent(json, jlen, &posJSON, &stateDecode);
			if(qnameID == EXI_EXIforJSON_4_map) {
				/* wait for possible key */
				jsonEvent = START_OBJECT;
			} else if(qnameID == EXI_EXIforJSON_4_array) {
				/* wait for possible key */
				jsonEvent = START_ARRAY;
			} else if(qnameID == EXI_EXIforJSON_4_string) {
				/* wait for possible key */
				jsonEvent = VALUE_STRING;
			} else if(qnameID == EXI_EXIforJSON_4_number) {
				/* wait for possible key */
				jsonEvent = VALUE_NUMBER;
			} else if(qnameID == EXI_EXIforJSON_4_boolean) {
				/* wait for possible key */
				jsonEvent = VALUE_FALSE;
			} else if(qnameID == EXI_EXIforJSON_4_null) {
				/* wait for possible key */
				jsonEvent = VALUE_NULL;
			} else {
				DEBUG_PRINTF(("[Decode-ERROR-SE] Not supported EXI element \n"));
				errn = 10;
				return errn;
			}
			break;
		case EXI_EVENT_END_ELEMENT:
			errn = exiEXIforJSONDecodeEndElement(&iStream,
					&stateDecode, &qnameID);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-EE] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF(("<< EE \n"));

			checkPendingEvent(json, jlen, &posJSON, &stateDecode);
			if(qnameID == EXI_EXIforJSON_4_map) {
				/* writeEnd } */
				if(json[posJSON-1] == ',') {
					posJSON--;
				}
				if( (posJSON + 2) < jlen) {
					json[posJSON++] = '}';
					json[posJSON++] = ',';
				} else {
					errn = EXIforJSON_ERROR_OUT_OF_STRING;
				}

			} else if(qnameID == EXI_EXIforJSON_4_array) {
				/* writeEnd ] */
				if(json[posJSON-1] == ',') {
					posJSON--;
				}
				if( (posJSON + 2) < jlen) {
					json[posJSON++] = ']';
					json[posJSON++] = ',';
				} else {
					errn = EXIforJSON_ERROR_OUT_OF_STRING;
				}
			}
			break;
		case EXI_EVENT_CHARACTERS:
			errn = exiEXIforJSONDecodeCharacters(&iStream,
					&stateDecode, &val);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-CH] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF((" CH: "));
			/* debugValue(&val, &stringTableDecode, stateDecode.elementStack[stateDecode.stackIndex]); */

			checkPendingEvent(json, jlen, &posJSON, &stateDecode);
			break;
		case EXI_EVENT_ATTRIBUTE:
			errn = exiEXIforJSONDecodeAttribute(&iStream, &stateDecode,
					&qnameID, &val);
			if (errn) {
				DEBUG_PRINTF(("[Decode-ERROR-AT] %d \n", errn));
				return errn;
			}
			DEBUG_PRINTF((" AT (%d): ", qnameID));
			/* debugValue(&val, &stringTableDecode, qnameID); */

			if(qnameID != EXI_EXIforJSON_0_key) {
				DEBUG_PRINTF(("[Decode-ERROR-AT] Not supported attribute other than key \n", errn));
				errn = 11;
				return errn;
			}

			errn = writeString(skey, MAX_KEY_LENGTH, &skeylen, stateDecode.stringTable, EXI_EXIforJSON_0_key);

			/* errn = getGlobalStringValueID(&stringTableDecode, &key); */
			break;
		default:
			/* ERROR */
			DEBUG_PRINTF(("[Unknown/Unexpected-Event] %d \n", event));
			return EXI_ERROR_UNKOWN_EVENT;
		}

	} while (noEndOfDocument);


	/* free memory if any */
	exiFreeDynamicStringMemory(&val.str.miss);
	/* exiFreeDynamicBinaryMemory(&val.binary);*/ /* binary not used */
	for(i=stlen; i<stringTableValuesDecode.len; i++) {
		exiFreeDynamicStringMemory(&stringTableValuesDecode.strs[i].str);
	}
	free(stringTableValuesDecode.strs);
	stringTableValuesDecode.strs = 0;

	return errn;
}

int decodeEXIforJSON(uint8_t* buffer, size_t blen, size_t* posDecode, char *json, size_t jlen) {
	return decodeEXIforJSONsharedStrings(buffer, blen, posDecode, json, jlen, NULL, 0);
}


#endif
