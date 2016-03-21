#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// open file and read into buffer

long data_size;
char *buffer;
char *originalheader;

static char * readFile(const char* filename) {

	FILE *original;
	long data_size;
	char *buf;

	original = fopen ( filename , "rb" );
	if( !original ) perror(filename),exit(1);

	fseek( original , 0L , SEEK_END);
	data_size = ftell( original );
	rewind( original );
	
	//save the header
	originalheader = calloc( 1, 100 );
	if( !originalheader ) fclose(original),fputs("memory alloc failed",stderr),exit(1);
	if( 1!=fread( originalheader , 100, 1 , original) )
	  fclose(original),free(buf),fputs("entire read failed",stderr),exit(1);

	rewind( original);
	/* allocate memory for entire content */
	buf = calloc( 1, data_size+1 );
	if( !buf ) fclose(original),fputs("memory alloc failed",stderr),exit(1);
	/* copy the file into the buffer */
	if( 1!=fread( buf , data_size, 1 , original) )
	  fclose(original),free(buf),fputs("entire read failed",stderr),exit(1);

	return buf;
}

int main() {

	FILE *processed;
	int buffer_length = data_size;
	buffer = readFile( "test.wav");	

	printf("%s", buffer);

	int sampleCount = (( buffer_length - 100 ) / sizeof (int16_t)); 
	int16_t * samp  = (int16_t *) &buffer[100];
	float percent   = 0.6f;

	for (int i = 0; i < sampleCount; i++){
	    samp[i] -= (int16_t) (samp[i] * percent); // Should work +/- values
	}

	//write to an output file
	processed = fopen ("processed.wav", "wb");
	fwrite (originalheader, sizeof(char), sizeof(originalheader), processed);
	fwrite (buffer , sizeof(char), sizeof(buffer), processed);
	fclose (processed);

return 0;
}

// save file
