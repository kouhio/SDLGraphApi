#ifndef __STRINGS_H__
#define __STRINGS_H__

#ifdef __cplusplus
	extern "C" {
#endif

enum TRIMSETTING {
	TRIM_BOTH = 0,
	TRIM_BEGINNING,
	TRIM_END,
};

char *parseFirstHexFromString(char *data);
char *parseFirstValueFromString(char *data);
char *parseDataToMark(char *data, char mark);
char *parseDataFromMarkToMark(char *data, char start, char end);
int parseFloat(char *data, float *ret);
int parseDouble(char *data, double *ret);
int parseInt(char *data, int *ret);
int parseUint(char *data, unsigned int *ret);
int parseLong(char *data, long *ret);
int parseUlong(char *data, unsigned long *ret);
int parseHex(char *data, unsigned long *ret);
char *trim(char *data, int setting, char extra);
int findDataFromFile(char *path, char *key, char *value, char *separator);

#ifdef __cplusplus
}
#endif

#endif // __STRING_H__

