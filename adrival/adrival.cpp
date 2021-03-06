#include <cstdlib>

extern void test_series();
extern void test_podplus();
extern void test_any();
extern void test_optional();
extern void test_simpletuple();

extern void test_functiontrait();
extern void test_memberfuntrait();

extern void test_event();
extern void test_multiplevent();
extern void test_semaphore();
extern void test_safequeue();
extern void test_canstealqueue();
extern void test_lockfreequeue();

extern void test_futuretask();
extern void test_taskpool();
extern void test_thread();

extern void test_obectpool();
extern void test_smartptr();

using testfun = void(*)();
testfun tests[] = {
	test_series,//0
	test_podplus,//1
	test_any,//2
	test_optional,//3
	test_simpletuple,//4
	test_functiontrait,//5
	test_memberfuntrait,//6
	test_event,//7
	test_multiplevent,//8
	test_semaphore,//9
	test_safequeue,//10
	test_canstealqueue,//11
	test_lockfreequeue,//12
	test_futuretask,//13
	test_taskpool,//14
	test_thread,//15
	test_obectpool,//16
	test_smartptr,//17
};

int main(int argc,char* argv[])
{
	if (2 == argc)
	{
		int index = atoi(argv[1]);
		tests[index]();
	}
	return 0;
}