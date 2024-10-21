#include<assert.h>
#include<windows.h>

int main()
{
int ret = system("make");
assert(ret != -1);
ret = system("./interaction -t result.txt < test.txt");
assert(ret != -1);
system("diff -y -w test.txt answer.txt");
return 0;
}