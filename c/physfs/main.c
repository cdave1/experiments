#include <stdio.h>
#include <stdlib.h>
#include "physfs.h"

int main (int argc, char ** argv)
{
	PHYSFS_init(NULL);
	PHYSFS_addToSearchPath("myzip.zip", 1);
	if (PHYSFS_exists("test.txt"))
	{
		PHYSFS_file* myfile = PHYSFS_openRead("test.txt");
        char * contents;
        contents = (char *)calloc(1, 1+PHYSFS_fileLength(myfile));
        int length_read = PHYSFS_read(myfile, contents, 1, PHYSFS_fileLength(myfile));
		printf("%s (%d)\n", contents, length_read);
        PHYSFS_close(myfile);
	}
    PHYSFS_deinit();
    printf("done\n");
	return 0;
}
