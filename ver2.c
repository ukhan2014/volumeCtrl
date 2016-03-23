#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HEADER_SIZE     (44)

// function prototypes
static void check_calloc(char * buf, FILE * fp);
static char * save_header(FILE * fp);
static int data_size(FILE * fp);
static char * save_pcm_data(FILE * fp);
static char * scale(char * buf, float percentage);
static int create_processed_wav(char * header, char * data);

static int d_size = 0;

int main() {
	FILE * fp = NULL;
	char * header = NULL;
	char * pcm_data = NULL;
	char * scaled_data = NULL;
	const char * original_file = "test2.bin";
    int ret_value = 0;
    
    // open file
	fp = fopen (original_file , "rb");
	if( !fp ) {
	    fputs("couldn't open file", stderr);
        ret_value = -1;
    }
    
	//save header into buffer
	header = save_header(fp);
	//printf("buffer contents: \n%s", header);

	//save pcm data after header
	pcm_data = save_pcm_data(fp);

	//scale bytes of pcm data
	scaled_data = scale(pcm_data, 0.5f);

	//create new file with header + scaled pcm data
	create_processed_wav(header, scaled_data);

	return ret_value;
}

/**
 *  accepts pointers to buffers containing the original header
 *  and the processed data, merges them into one chunk and
 *  puts it into a file called processed.wav.
 *  returns 0 on success, -1 on failure
 *
 */
static int create_processed_wav(char * header, char * data) {
	const char * processed_file = "processed.txt";
    // allocate memory for pcm data
    char * buf = calloc((HEADER_SIZE + d_size), sizeof(char));
	if(!buf) {
        return -1;
	}
	printf("header: \n%s\n\n", header);
	printf("data: \n%s\n\n", data);
	memcpy(buf, header, HEADER_SIZE);               // copy header
	memcpy((&buf[HEADER_SIZE]), data, d_size);  // copy data
	
	printf("total buf: \n%s\n\n", buf);
	
	FILE * fp = fopen (processed_file, "wb");
	fwrite(buf, sizeof(char), (HEADER_SIZE + d_size), fp);
	fclose(fp);
	
    return 0;
}

/**
 *  accepts a pointer to a buffer and percentage float value,
 *  multiplies the data in the buffer with the percentage
 *
 */
static char * scale(char * buf, float percentage) {
    int sampleCount = ((d_size - HEADER_SIZE) / sizeof(int16_t)); 
	int16_t * samp = (int16_t *) &buf[(HEADER_SIZE + 1)];
	for (int i = 0; i < sampleCount; i++){
	    // printf("sample before: %x \n", samp[i]);
	    samp[i] -= (int16_t) (samp[i] * percentage); // Should work +/- values
	    // printf("sample after: %x \n", samp[i]);
	}
	return (char *)samp;
}

/**
 *  accepts a file pointer to a wav file, returns the size of
 *  the data portion only
 *
 */
static int data_size(FILE * fp) {
    int size;
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    return (size - HEADER_SIZE);
}

/**
 *  accepts a file pointer, fast forwards file to data portion
 *  and returns the data in a buffer
 *
 */
static char * save_pcm_data(FILE * fp) {
    char * buf;
    d_size = data_size(fp);
    buf = calloc(d_size, sizeof(char));     // allocate memory for pcm data
	if(!buf) {
        return NULL;
	}
    rewind(fp);
    fseek(fp, (HEADER_SIZE + 1), SEEK_SET);
    if(!fread((void *)buf, sizeof(char), d_size, fp)) {	// copy data to buffer
	    fputs("reading pcm data failed", stderr);
        return NULL;
	}
	
	return buf;
}

/**
 *  accepts a filename and returns the wav file's header
 *  in a buffer. Returns NULL on failure.
 *
 */
static char * save_header(FILE * fp) {
	char * buf;
	fseek(fp , 0 , SEEK_SET);                    // go to beginning of file
	buf = calloc(HEADER_SIZE, sizeof(char));     // allocate memory for wav header
	if(!buf) {
        return NULL;
	}
	
	if(!fread((void *)buf, sizeof(char), HEADER_SIZE , fp)) {	// copy header to buffer
	    fputs("reading header failed", stderr);
        return NULL;
	}
	
	// printf("buffer contents: \n%s", buf);
	
	return buf;
}
