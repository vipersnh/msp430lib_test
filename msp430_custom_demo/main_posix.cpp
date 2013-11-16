
#include <stdio.h>
typedef unsigned char uint8_t;

class temp_Class
{
	public:
	void testClass_func(char *c)
	{
		*c = 0x0F;
	}
}temp_obj;


void testLocal_func(char *c)
{
	*c = 0xF0;
}

int main( void )
{
	char c[] = "Hello World;
	temp_obj.testClass_func(&c);
	c = 0xBA;
	testLocal_func(&c);
	while(1);
	return -1;
}










