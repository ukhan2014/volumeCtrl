#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// function prototypes
static void check_calloc(char * buf, FILE * fp);
static char * save_header(const char * filename);

int main() {
	long data_size;
	char * header;
	char * data;
	const char * original_file = "test.txt";
	const char * processed_file = "processed.txt";

	//save header into buffer
	header = save_header(original_file);

	//read pcm data after header
	data = save_pcm_data(original_file);

	//scale bytes of pcm data
	//scale(data, "50");

	//create new file with header + scaled pcm data
	//create_processed_wav(header, data);

	return 0;
}

static int data_size(FILE * fp) {
    int size;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    return size;
}

/**
 *  accepts a file pointer, fast forwards file to data portion
 *  and returns the data in a buffer
 *
 */
static char * save_pcm_data(FILE * fp) {
    char * buf;
    
}

/**
 *  accepts a buffer and a file pointer, frees both
 *  then exits the program
 *
 */
static void check_calloc(char * buf, FILE * fp) {
	if (!buf) {
		fclose(fp);
		fputs("memory alloc failed", stderr);
		exit(1);
	}
}

/**
 *  accepts a filename and returns the wav file's header
 *  in a buffer
 *
 */
static char * save_header(const char * filename) {
	FILE * fp;
	char * buf;
	fp = fopen (filename , "rb");
	if( !fp ) perror(filename),exit(1);
	fseek(fp , 0 , SEEK_SET); 				// go to beginning of file
	buf = calloc(44, sizeof(char));				// allocate 44 bytes for header
	if (!buf) {
        goto on_error;
	}
	check_calloc(buf, fp);					// check for allocation success
	
	if(!fread((void *)buf, sizeof(char), 44 , fp)) {	// read first 44 bytes into buffer
		fclose(fp);
		free(buf);
		fputs("reading header failed", stderr);
		exit(1);
	}
	
	return buf;
	
	//printf("buffer contents: \n%s", buf);
on_error:
    fclose(fp);
    return 0;
}
