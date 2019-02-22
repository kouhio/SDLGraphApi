#ifndef __FILESYS_H__
#define __FILESYS_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include <dirent.h>

/*!*
 * \brief	Types of files that can be used
 *
 * \enum	file_types_t
 *
 * \var		file_types_t::TYPE_UNKNOWN
 * 			Indicates either no or both filetypes
 *
 * \var		file_types_t::TYPE_FILE
 * 			Indicates a file type
 *
 * \var		file_types_t::TYPE_DIRECTORY
 * 			Indicates a directory
 */
enum file_types_t {
	TYPE_UNKNOWN=0,
	TYPE_FILE,
	TYPE_DIRECTORY,
};

/*!*
 * \brief	Filetype information handler structure
 */
struct fileTypeData {
	/// Path to a file
	char *path;
	/// Filetype
	int type;
};

/*!*
 * \brief	Main file list handler
 */
struct fileDirectoryList {
	/// Filetype structure list
	struct fileTypeData *file;
	/// Number of files in the structure
	int count;
};

char *readLine(FILE *fd);
char *readFile(FILE *fd, unsigned int bytes);
FILE *openFileRead(char *path);
FILE *openFileWrite(char *path);
FILE *openFileAppend(char *path);
void closeFile(FILE *fd);
int writeLine(FILE *fd, char *data);
int writeFile(FILE *fd, char *data, unsigned int size);

unsigned char *readBinaryFile(char *path, int *size);

int getFileType(char *path);
int getFileCount(char *path, int type);
int getExtensionFileCount(char *path, char *ext);
char *parseFilename(char *path);
char *parseExtension(char *filename);

struct fileDirectoryList *getFileList(char *path);
void freeFileList(struct fileDirectoryList *list);

unsigned long long getFileSize(char *path);
char *getCurrentDirectory();
int fileExists(char *path);
unsigned long long calculatePathSize(char *path);

void closeDir(DIR *dir);
DIR *openDir(char *path);
char *getNextDirItem(DIR *dir);

char *parseCommandLineData(int argc, char *argv[], char* arguments, int *arg);
char *removeEndWhiteSpace(char *data);

char *initializeText(char *text);
char *initializeTextSize(char *text, unsigned int size);

/// A flag to set external program to print al the erros, that the lib might give out
extern int displayPlatformErrors;
/// A flag to set external program to print all the successes, that lib might give out
extern int displayPlatformSuccess;
/// A flag to set external program to print all the debugging code from platform initialization
extern int displayPlatformDebug;

int amBigEndian(void);

unsigned long long swapDouble(double d);
double unswapDouble(unsigned long long a);

char *toUpper(char *data);
char *toLower(char *data);

/// Switch 16-bit values endianness
#define ByteSwap16(n)  ( ((((unsigned int) n) << 8) & 0xFF00) |  ((((unsigned int) n) >> 8) & 0x00FF) )
/// Switch 32-bit values endianness
#define ByteSwap32(n) ( ((((unsigned long) n) << 24) & 0xFF000000) | ((((unsigned long) n) <<  8) & 0x00FF0000) | \
		  	  ((((unsigned long) n) >>  8) & 0x0000FF00) | ((((unsigned long) n) >> 24) & 0x000000FF) )

/// Macro to make easy switch-cases that have values between two values
#define VAL_BETWEEN(X,Y) X...Y


#ifdef __cplusplus
	}
#endif

#endif // __FILESYS_H__


