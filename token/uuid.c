//编译时加 -luuid
#include <uuid/uuid.h>
#include <stdio.h>
//生成UUID
void test(){
	char buf[64] = {0};
	uuid_t uu;
	uuid_generate(uu);
	uuid_generate_random(uu);
	uuid_unparse_upper(uu,buf);
	printf("%s\n",buf);
}

int main(){
	test();
}
