#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>

int main(int argc, char** argv){
	GDBM_FILE dbf = gdbm_open("scoreDatabase.dat", 0, GDBM_READER, 0, NULL);
	datum key, data;
	for(key = gdbm_firstkey(dbf); key.dptr != NULL; key =gdbm_nextkey(dbf, key)){
	data =gdbm_fetch(dbf, key);
	if(data.dptr != NULL)
		printf("Entry found (%d bytes): %s %s.\n", data.dsize, key.dptr, data.dptr);
}
gdbm_close(dbf);
return EXIT_SUCCESS;

}
