#include "olus2000.h"

HELLO I AM OLUS2000 AND I WROTE THIS ENTRY. SO MAKE YOUR GUESSES WITH THAT IN MIND.
YOU DON/*'*/T BELIEVE ME? BUT WHY? HAVE I LIED TO YOU BEFORE? PLEASE,
JUST DRAG MY NICK TO THE APPROPRIATE PLACE. YOU WOULDN/*'*/T WANT TO SEE ME SAD WOULD YOU?
YOU STILL DONT BELIEVE ME? FINE... MY FULL NAME IS ALEKSANDER SABAK. I STUDIED
COMPUTER SCIENCE IN WARSAW UNIVERSITY OF TECHNOLOGY. I LOVE PICO8, ESOLANGS, AND
CONCATANATIVE PROGRAMMING. UMM... OH YEAH AND ALSO TOKI PONA. UHH...
IM FROM POLAND. YOU PROBABLY KNOW IT ALREADY, I JUST WANTED TO POINT IT OUT...
YOU NOW BELIEVE ME, RIGHT? :PLEADING_FACE: I SWEAR, THIS CODE REALLY DOES BELONG TO ME,
I AM NOT IMPERSONATING HIM. AND BY HIM, I MEAN ME OF COURSE. IVE GIVEN A LOT OF PERSONAL
DETAILS SO YOU ARE CONVINCED, R-RIGHT? HAVE A NICE CODE GUESSING

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if(!file)
    {
        fprintf(stderr, "Cannot open \"%s\"\n", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate memory to hold the file contents
    char* buffer = (char*)malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    // Read the entire file into the buffer
    if (fread(buffer, 1, file_size, file) != file_size) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        free(buffer);
        return 1;
    }
    fclose(file);
    if(buffer[file_size - 1] == '\n') buffer[file_size - 1] = '\0';

    char* result = entry(buffer);
    printf("%s\n", result);

    free(result);
    free(buffer);
    return 0;
}
