//Dynamically allocate an array of structs

int main(int argc, char *argv[])
{
typedef struct{
	int field1;
	char field2[7];
	} Astruct;

//Let's assume that we know we want argv[1] of these.

Astruct *ArrayPtr;
int StructCount;
StructCount = atoi(argv[1]);

ArrayPtr = (Astruct *)calloc( StructCount, sizeof(Astruct));
//ArrayPtr[i];

//Use GDB Backtrace to backtrace calls.

