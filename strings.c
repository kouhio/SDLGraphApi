/*!
 * \file	strings.h
 * \brief	Various string handling and parsing functions
 * \author	Lari Koskinen
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include "filesys.h"
#include "dynamicPlatform.h"
#include "strings.h"

/*!
 * \brief	Parse first found HEX-value from given string 
 *
 * \param	*data
 * 			String which holds the hex-data
 *
 * \return	Parsed hex-string from given string
 */
char *parseFirstHexFromString(char *data) {
	int i, found = 0, pos = 0;
	static char parsed[1028];
	memset(parsed, 0, sizeof(parsed));

	for(i = 0; i < strlen(data); i++) {
		if(!isdigit(data[i])) {
			/// Areas of A-F and a-f
			if(((data[i] >= 65) && (data[i] <= 70)) || ((data[i] >= 97) && (data[i] <= 102)) || (data[i] == 'x')) {
				parsed[pos++] = data[i];
			}
			else if(found) {
				break;
			}
		}
		else {
			parsed[pos++] = data[i];
			found = 1;
		}
	}
	return parsed;
}

/*!
 * \brief	Parse first found value from given string 
 *
 * \param	*data
 * 			String which holds the hex-data
 *
 * \return	Parsed hex-string from given string
 */
char *parseFirstValueFromString(char *data) {
	int i, found = 0, pos = 0;
	static char parsed[1028];
	memset(parsed, 0, sizeof(parsed));

	for(i = 0; i < strlen(data); i++) {
		if(!isdigit(data[i])) {
			if((data[i] == '-') || (data[i] == '.')) {
				parsed[pos++] = data[i];
			}
			else if((data[i] == 'e') && ((isdigit(data[i-1])) && ((isdigit(data[i+1]) || (data[i+1] == '-'))))) {
				parsed[pos++] = data[i];
			}
			else if(found) {
				break;
			}
		}
		else {
			parsed[pos++] = data[i];
			found = 1;
		}
	}
	return parsed;
}

/*!
 * \brief	Parse data that is before the given char
 *
 * \param	*data
 * 			String that withold the wanted data
 *
 * \param	mark
 * 			String ending character
 *
 * \return	Newly parsed string
 */
char *parseDataToMark(char *data, char mark) {
	int i=0;
	static char parsed[1028];
	memset(parsed, 0, sizeof(parsed));

	while(i < strlen(data)) {
		if(data[i] != mark) {
			parsed[i] = data[i];
		}
		else {
			break;
		}
		i++;
	}
	return parsed;
}

/*!
 * \brief	Parse data that is between the two given characters
 *
 * \param	*data
 * 			String that withold the wanted data
 *
 * \param	start
 * 			String starting character
 *
 * \param	end
 * 			String ending character
 *
 * \return	Newly parsed string
 */
char *parseDataFromMarkToMark(char *data, char start, char end) {
	int i = 0, found = 0, index = 0;
	static char parsed[1028];
	memset(parsed, 0, sizeof(parsed));

	while(i < strlen(data)) {
		if(!found) {
			if(data[i] == start) {
				found = 1;
			}
		}
		else if(found) {
			if(data[i] != end) {
				parsed[index++] = data[i++];
				continue;
			}
			break;
		}
		i++;
	}
	return parsed;
}

/*!
 * \brief	Parse value from HEX-string
 *
 * \param	*data
 * 			HEX value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseHex(char *data, unsigned long *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			if(!strncmp(data, "0x", 2)) {
				data += 2;
			}
			sscanf(data, "%lX", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse int value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseInt(char *data, int *ret) {
	if(data != NULL) {
		if(isdigit(data[0]) || (data[0] == '-')) {
			sscanf(data, "%d", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse unsigned int value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseUint(char *data, unsigned int *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			sscanf(data, "%u", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse long value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseLong(char *data, long *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			sscanf(data, "%ld", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse unsigned long value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseUlong(char *data, unsigned long *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			sscanf(data, "%lu", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse float value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseFloat(char *data, float *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			sscanf(data, "%f", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Parse float value from string
 *
 * \param	*data
 * 			Value as string
 *
 * \param	*ret
 * 			Pointer where the parsed value will be set
 *
 * \return	0 on success, -1 on failure
 */
int parseDouble(char *data, double *ret) {
	if(data != NULL) {
		if(isdigit(data[0])) {
			sscanf(data, "%lf", ret);
			return 0;
		}
	}
	return -1;
}

/*!
 * \brief	Get a nulling character if appropriate or return given input character
 *
 * \param	input
 * 			input character to compare to whitespace characters
 *
 * \param	remove
 * 			extra character that will be removed, if input is given character. Use 0, if nothing extra needed
 *
 * \return	0 if input char is withing parameters, otherwise input character
 */
char clearOrSkip(char input, char remove) {
	if((input == ' ') || (input == 10) || (input == 13) || (input == '\t') || (input == remove)) {
		return 0;
	}
	return input;
}

/*!
 * \brief	Remove excess characters from strings end
 *
 * \param	data
 * 			String to be cleaned
 *
 * \param	extra
 * 			Extra character that is not defined as whitespace
 *
 * \return	String without whitespace
 */
char *trimFromEnd(char *data, char extra) {
	int i;
	for(i = strlen(data); i > 0; i--) {
		if((data[i] = clearOrSkip(data[i], extra)) != 0) {
			break;
		}
	}
	return data;
}

/*!
 * \brief	Trim string whitespace from beginning of string
 *
 * \param	data
 * 			String to be trimmed
 *
 * \param	extra
 * 			Extra character that is not defined as whitespace
 *
 * \return	Trimmed string
 */
char *trimFromBeginning(char *data, char extra) {
	int i;
	for(i=0; i < strlen(data); i++) {
		if((data[i] = clearOrSkip(data[i], extra)) != 0) {
			return &data[i];
		}
	}
	return data;
}

/*!
 * \brief	remove unnecessary whitespace from given string
 *
 * \param	data
 * 			String to be trimmed
 *
 * \param	setting
 * 			Direction of trimming (TRIM_BOTH / TRIM_BEGINNING / TRIM_END)
 *
 * \param	extra
 * 			Extra character that is not defined as whitespace but is wanted to be removed
 */
char *trim(char *data, int setting, char extra) {
	switch (setting) {
		case TRIM_BOTH:
			data = trimFromEnd(data, extra);
			data = trimFromBeginning(data, extra);
		break;

		case TRIM_BEGINNING:
			data = trimFromBeginning(data, extra);
		break;

		case TRIM_END:
			data = trimFromEnd(data, extra);
		break;
	}
	return data;
}

/*!
 * \brief	Find given data from given filepath
 *
 * \param	*path
 * 			Path to the wanted file / information
 *
 * \param	*key
 * 			Key-string which will be searched throughout the given path
 *
 * \param	*value
 * 			Pointer where the found data is set, if found properly
 *
 * \param	*separator
 * 			String that will be used as separator in the data. If no separator is wanted, give NULL instead.
 *
 * \return	-1 on error. 0 on success.
 */
int findDataFromFile(char *path, char *key, char *value, char *separator) {
	char *data = NULL, *wanted = NULL;
	FILE *fd;

	if((fd = openFileRead(path)) != NULL) {
		while((data = readLine(fd)) != NULL) {
			if(strstr(key, data) != NULL) {
				if(separator != NULL) {
					if((wanted = strstr(separator, data)) != NULL) {
						wanted = trim(++wanted, TRIM_BOTH, 0);
					} else {
						continue;
					}
				} else {
					wanted = data;
				}
				value = trim(wanted, TRIM_BOTH, 0);
				closeFile(fd);
				if(value != NULL) {
					return 0;
				}
			}
		}
	}
	closeFile(fd);

	return -1;
}

