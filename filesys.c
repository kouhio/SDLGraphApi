/*!
 * \file	filesys.h
 * \brief	file system header file
 * \author	Lari Koskinen
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "filesys.h"
#include "dynamicPlatform.h"

#if (DEBUG == 1)
int displayPlatformErrors = 1;
int displayPlatformSuccess = 1;
int displayPlatformDebug = 1;
#else
int displayPlatformErrors = 0;
int displayPlatformSuccess = 0;
int displayPlatformDebug = 0;
#endif

/*!
 *	\brief		Open a directory handler to given path
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		DIR *
 *				Pointer to an open directory
 */
DIR *openDir(char *path) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(path != NULL) {
		return opendir(path);
	}
	return NULL;
}

/*!
 *	\brief		Close an open directory handler
 */
void closeDir(DIR *dir) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(dir != NULL) {
		closedir(dir);
		dir = NULL;
	}
}

/*!
 *	\brief		Get next filename from directory handler
 *
 *	\param		*dir
 *				Pointer to an open directory handler
 *
 *	\return		char *
 *				Next reserved filename or NULL
 */
char *getNextDirItem(DIR *dir) {
	struct dirent *namelist;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if((namelist = readdir(dir)) != NULL) {
		return namelist->d_name;
	}
	return NULL;
}

/*!
 *	\brief		Read a line from file
 *
 *	\param		*fd
 *				filepointer to an open file
 *
 *	\return		char *
 *				A read line frmo file or NULL
 */
char *readLine(FILE *fd) {
	static char buf[1024];
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(!feof(fd)) {
		memset(buf, 0, sizeof(buf));
		if(fgets(buf, sizeof(buf), fd) != NULL) {
			return buf;
		}
	}
	return NULL;
}

/*!
 *	\brief		Read data from file
 *
 *	\param		*fd
 *				filepointer to a open file
 *
 *	\param		bytes
 *				number of bytes to read
 *
 *	\return		char *
 *				read data or NULL
 */
char *readFile(FILE *fd, unsigned int bytes) {
	int status; 
	char *buf = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(!feof(fd)) {
		buf = (char *)malloc(bytes);
		if(buf != NULL) {
			if((status = fread(buf, 1, bytes, fd)) == bytes) {
				return buf;
			}
			if(displayPlatformErrors) {
				printf("%s -> read %d of %d bytes!\n", __FUNCTION__, status, bytes);
			}
			free(buf);
		}
	}
	return NULL;
}

/*!
 *	\brief		Get given filesize in bytes
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		int
 *				0 if nothing
 *				else a number of bytes in file
 */
/*unsigned long long getFileSize(char *path) {
	unsigned long long size = 0;
	FILE *fd;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
		
	if((fd = openFileRead(path)) != NULL) {
		fseek(fd, 0, SEEK_END);
		size = ftell(fd);
		closeFile(fd);
		return size;
	}

	return 0;
}*/

/*!
 *	\brief		Open a new file for writing or overwrite an existing one
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		FILE *
 *				Pointer to a newly opened file
 */
FILE *openFileWrite(char *path) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(path != NULL) {
		return fopen(path, "w");
	}
	return NULL;
}

/*!
 *	\brief		Open file for reading
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		FILE *
 *				Pointer to a newly opened file
 */
FILE *openFileRead(char *path) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(path != NULL) {
		return fopen(path, "r");
	}
	return NULL;
}

/*!
 *	\brief		Open file for appended writing
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		FILE *
 *				Pointer to a newly opened file
 */
FILE *openFileAppend(char *path) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(path != NULL) {
		return fopen(path, "a");
	}
	return NULL;
}

/*!
 *	\brief		Close filepointer
 *
 *	\param		*fd
 *				pointer to an open filehandler
 */
void closeFile(FILE *fd) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(fd != NULL) {
		fclose(fd);
	}
}

/*!
 *	\brief		Write a line of string data to file
 *
 *	\param		*fd
 *				Filepointer
 *
 *	\param		*data
 *				Data to be written to file pointer
 *
 *	\return		int	
 *				0 on success
 *				-1 on error
 */
int writeLine(FILE *fd, char *data) {
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(fprintf(fd, "%s\n", data) == (strlen(data) + strlen("\n"))) {
		return 0;
	}
	return -1;
}

/*!
 *	\brief		Write data to file
 *
 *	\param		*fd
 *				Filepointer
 *
 *	\param		*data
 *				Data to be written to file pointer
 *
 * 	\param		size
 * 				Number of bytes to be written
 *
 *	\return		int	
 *				0 on success
 *				-1 on error
 */
int writeFile(FILE *fd, char *data, unsigned int size) {
	unsigned int status;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if((status = fwrite((char *)data, 1, size, fd)) == size) {
		return 0;
	}
	if(displayPlatformErrors) {
		printf("%s -> failed. Wrote %d of %d\n", __FUNCTION__, status, size);
	}

	return -1;
}

/*!
 *	\brief		
 *
 *	\param		*path
 *				string path to file
 *
 *	\return 	int
 *				Return TYPE_DIRECTORY or TYPE_FILE, if given path
 *				is either of them.
 */
int getFileType(char *path) {
	//FILE *fd;
	DIR *dir;
	char *file;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	file = parseFilename(path);

	if(strlen(file) < 3) {
		if((!strncmp(file, ".", 1)) || (!strncmp(file, "..", 2))) {
			return TYPE_UNKNOWN;
		}
	} else if((!(strncmp(file, "./.", 3))) || (!(strncmp(file, "./..", 4))) ) {
		return TYPE_UNKNOWN;
	}

	if((dir = opendir(path)) != NULL) {
		closedir(dir);
		return TYPE_DIRECTORY;
	}/* else if((fd = openFileRead(path)) != NULL) {
		closeFile(fd);
		return TYPE_FILE;
	}
	return TYPE_UNKNOWN;*/
	return TYPE_FILE;
}

/*!
 *	\brief		Get number of items from a directory path
 *
 *	\param		*path
 *				string path to file
 *
 *	\param		type
 *				Type of the file wanted to be counted. if set as
 *				TYPE_UNKNOWN, will return directory and file count
 *				as one.
 *
 *	\return		int
 *				Number of files found in directory
 */
int getFileCount(char *path, int type) {
	int files = 0, dirs = 0, Filetype;
	DIR *dir;
	char workPath[1024], *file;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((dir = openDir(path)) != NULL) {
		while((file = getNextDirItem(dir)) != NULL) {
			memset(workPath, 0, sizeof(workPath));
			sprintf(workPath, "%s/%s", path, file);

			if((Filetype = getFileType(workPath)) != TYPE_UNKNOWN) {
				if(Filetype == TYPE_DIRECTORY) {
					dirs++;
				} else {
					files++;
				}
			}
		}
		closeDir(dir);
		return (type == TYPE_DIRECTORY)? dirs: (type == TYPE_FILE)? files: (dirs + files);
	}

	return -1;
}

/*!
 *	\brief		Get number of items with extension from a directory path
 *
 *	\param		*path
 *				string path to file
 *
 * 	\param		*ext
 * 				Wanted file extension
 *
 *	\return		int
 *				Number of files with extension found in directory
 */
int getExtensionFileCount(char *path, char *ext) {
	int files = 0;
	DIR *dir;
	char *file;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(ext == NULL) {
		return getFileCount(path, TYPE_UNKNOWN);
	}

	if((dir = openDir(path)) != NULL) {
		while((file = getNextDirItem(dir)) != NULL) {

			if((strstr(file, ext)) != NULL) {
				files++;
			}
		}
		closeDir(dir);
		return files;
	}

	return -1;
}

/*!
 *	\brief		Get a filename from full path string
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		char *
 *				parsed filename from path
 */
char *parseFilename(char *path) {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	for(i = strlen(path); i >= 0; i--) {
		if(path[i] == '/') {
			return &path[i+1];
		}
	}
	return NULL;
}

/*!
 *	\brief		Get file extension from filename
 *
 *	\param		*filename
 *				Filename where to strip the extension
 *
 *	\return 	char *
 *				extension stripped from given filename
 */
char *parseExtension(char *filename) {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	for(i = strlen(filename); i >= 0; i--) {
		if(filename[i] == '.') {
			return &filename[i];
		}
	}
	return NULL;
}

/*!
 *	\brief		get given directory file list structure
 *
 *	\param		*path
 *				string path to file
 *
 *	\return		struct fileDirectoryList *
 *				Pointer to reserved directory list structure
 */
struct fileDirectoryList *getFileList(char *path) {
	int fileCount = getFileCount(path, TYPE_UNKNOWN), sizeList, sizeTypes, Filetype;
	DIR *dir;
	char workPath[1024], *file;

	struct fileDirectoryList *list;
	struct fileTypeData *types;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if(fileCount > 0) {
		sizeList = sizeof(struct fileDirectoryList);
		sizeTypes = (sizeof(struct fileTypeData) * fileCount);

		if((dir = openDir(path)) != NULL) {
			if((list = (struct fileDirectoryList *)malloc(sizeList)) != NULL) {
				memset(list, 0, sizeList);
				if((types = (struct fileTypeData *)malloc(sizeTypes)) != NULL) {
					memset(types, 0, sizeTypes);
					list->file = types;

					while((file = getNextDirItem(dir)) != NULL) {
						memset(workPath, 0, sizeof(workPath));
						sprintf(workPath, "%s/%s", path, file);
						if((Filetype = getFileType(workPath)) != TYPE_UNKNOWN) {
							list->file[list->count].path = initializeText(workPath);
							list->file[list->count++].type = Filetype;
						}
					}
					return list;
				}
				free(list);
			}
		}
	}

	return NULL;
}

/*!
 *	\brief		Free existing filelist
 *
 *	\param		*list
 *				pointer to filelist
 */
void freeFileList(struct fileDirectoryList *list) {
	int i;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	if(list != NULL) {
		for(i = 0; i < list->count; i++) {
			if(list->file[i].path != NULL) {
				free(list->file[i].path);
			}
		}
		if(list->file != NULL) {
			free(list->file);
		}
		free(list);
	}
}

/*!
 *	\brief		Get current working directory path
 *
 *	\return		char *
 *				path to current working directory
 */
char *getCurrentDirectory() {
	static char path[1024];
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	return getcwd(path, sizeof(path));
	//return path;
}

/*!
 *	\brief		Check if file exists
 *
 * 	\param		*path
 * 				path to file to check for existance
 *
 *	\return 	int
 *				0 if exists
 *				other if doesn't
 */
int fileExists(char *path) {
	struct stat filename_buffer;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif
	return stat(path, &filename_buffer);
}

/*!
 * \brief	Parse first found item in given command-line option
 *
 * \param	argc
 * 			Number of arguments in table
 *
 * \param	*argv[]
 * 			Argument string table
 *
 * \param	*arguments
 * 			Wanted argument characters separated by ':'
 *
 * \param	*arg
 * 			Found argument character info
 *
 * \return	Pointer to wanted sata or NULL
 */
char *parseCommandLineData(int argc, char *argv[], char* arguments, int *arg) {
	int optch;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	while((optch = getopt(argc, argv, arguments)) != EOF) {
		*arg = optch;
		return optarg;
	}
	*arg = 0;
	return NULL;
}

/*!
 * \brief	Remove whitespace from the end of the string
 *
 * \param	*data
 * 			String data to be cleared
 *
 * \return	cleaned string
 */
char *removeEndWhiteSpace(char *data) {
	int len = strlen(data);
	while(len-- > 0) {
		switch(data[len]) {
			case '\n':
			case '\t':
			case ' ':
			case 13:
			case ';':
				data[len] = 0;
				break;
			default:
				return data;
		}
	}
	return data;
}

/*!
 *	\brief		Reserve memory for string and return pointer
 *
 *	\param		*text
 *				String that shall be reserved to memory
 *
 *	\return		char *
 *				Pointer to newly reserved text memory are or NULL
 */
char *initializeText(char *text) {
	int size = strlen(text);
	char *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((temp = (char *)malloc(size + 1)) != NULL) {
		memset(temp, 0, size + 1);
		memcpy(temp, text, size);
		return temp;
	}
	if(displayPlatformErrors) {
		printf("%s -> failed!\n", __FUNCTION__);
	}

	return NULL;
}

/*!
 *	\brief		Reserve memory for string with given size and return pointer
 *
 *	\param		*text
 *				String that shall be copied to the reserved memory, if smaller than given size
 *
 *  \param		size
 *  			Size of the memory-area to be reserved 
 *
 *	\return		char *
 *				Pointer to newly reserved text memory are or NULL
 */
char *initializeTextSize(char *text, unsigned int size) {
	char *temp = NULL;
#if (DEBUG == 1)
	printf("DEBUG: %s\n", __FUNCTION__);
#endif

	if((temp = (char *)malloc(size)) != NULL) {
		memset(temp, 0, size);
		if(text != NULL) {
			if(strlen(text) < size) {
				strcat(temp, text);
			}
		}
		return temp;
	}
	if(displayPlatformErrors) {
		printf("%s -> failed!\n", __FUNCTION__);
	}

	return NULL;
}

/*!
 * \brief	Get endianess status of current system
 *
 * \return	0 if little endian, 1 of big endian
 */
int amBigEndian(void) {
	long one= 1;
	return !(*((char *)(&one)));
}

/*!
 * \brief	Swap double value endianess
 *
 * \param	d
 * 			Double value to be endianess-swapped
 *
 * \return	Endianess swapped double value
 */
unsigned long long swapDouble(double d)
{
	unsigned long long a;
	unsigned char *dst = (unsigned char *)&a;
	unsigned char *src = (unsigned char *)&d;

	dst[0] = src[7];
	dst[1] = src[6];
	dst[2] = src[5];
	dst[3] = src[4];
	dst[4] = src[3];
	dst[5] = src[2];
	dst[6] = src[1];
	dst[7] = src[0];

	return a;
}

/*!
 * \brief	Swap swapped double value endianess
 *
 * \param	a
 * 			Swapped double value
 *
 * \return	Double value swapped from memory value
 */
double unswapDouble(unsigned long long a) 
{
	double d;
	unsigned char *src = (unsigned char *)&a;
	unsigned char *dst = (unsigned char *)&d;

	dst[0] = src[7];
	dst[1] = src[6];
	dst[2] = src[5];
	dst[3] = src[4];
	dst[4] = src[3];
	dst[5] = src[2];
	dst[6] = src[1];
	dst[7] = src[0];

	return d;
}

/*!
 * \brief	String to lowercase
 *
 * \param	*data
 * 			string that will be turned to lowercase
 *
 * \return	lowercased string
 */
char *toLower(char *data) {
	int i;
	for(i=0; i < strlen(data); i++) {
		data[i] = tolower(data[i]);
	}
	return data;
}

/*!
 * \brief	String to uppercase
 *
 * \param	*data
 * 			string that will be turned to uppercase
 *
 * \return	uppercased string
 */
char *toUpper(char *data) {
	int i;
	for(i=0; i < strlen(data); i++) {
		data[i] = toupper(data[i]);
	}
	return data;
}

/*!
 * \brief	Reads data from binary file and returns the whole chunk as one
 *
 * \param	*path
 * 			Path to the binary file to be read
 *
 * \param	*size
 * 			The size of the read data will be set to *size
 *
 * \return	Pointer to the read binary data
*/
unsigned char *readBinaryFile(char *path, int *size) {
	FILE *fd = NULL;
	unsigned char *data = NULL;
	int fileSize = 0, status = 0;

	if( (fd = openFileRead(path)) != NULL) {
		fseek(fd, 0, SEEK_END);
		fileSize = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		if( (data = malloc(fileSize)) != NULL) {
			memset( (unsigned char *)data,0,fileSize);
			status = fread((unsigned char *)data, 1, fileSize, fd);

			if(status != fileSize) {
				free(data);
				data = NULL;
			} else {
				*size = status;
			}
		}
		closeFile(fd);
	}
	if((displayPlatformErrors) && (data == NULL)) {
		printf("%s -> error: read failed! %s read %d bytes while target is %d bytes\n",__FUNCTION__, path, status, fileSize);
	}
	return data;
}

/*!
 * \brief	Alternate way to calculate filesize
 *
 * \param	*path
 * 			Path to the file wanted to be sized
 *
 * \return	Size of the given file
 */
unsigned long long getFileSize(char *path) {
	struct stat st;
	if(stat(path, &st)) {
		perror(path);
		return 0;
	}
	return st.st_size;
}

/*!
 * \brief	Calculates the size of files within given path
 *
 * \param	*path
 * 			Path that holds all files and directories to be included in the calculation
 *
 * \return	Size of path in bytes
 */
unsigned long long calculatePathSize(char *path) {
	int Filetype;
	unsigned long long fileSize = 0, siz = 0;
	DIR *dir;
	char workPath[1024], *file;
#if (DEBUG == 1)
	printf("DEBUG: %s %s\n", __FUNCTION__, path);
#endif

	if((dir = openDir(path)) != NULL) {
		while((file = getNextDirItem(dir)) != NULL) {
			memset(workPath, 0, sizeof(workPath));
			sprintf(workPath, "%s/%s", path, file);

			if((Filetype = getFileType(workPath)) != TYPE_UNKNOWN) {
				siz = getFileSize(workPath);
				fileSize += siz;
				if(displayPlatformErrors || displayPlatformDebug) {
					printf("%s -> %s fileSize:%lld total:%lld\n", __FUNCTION__, workPath, siz, fileSize);
				}
				if(Filetype == TYPE_DIRECTORY) {
					fileSize += calculatePathSize(workPath);
				}
			}
		}
		closeDir(dir);
	}

	return fileSize;
}

