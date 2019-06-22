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

int main()
{
	test_series();
	test_podplus();
	test_any();
	test_optional();
	test_simpletuple();

	test_functiontrait();
	test_memberfuntrait();

	test_event();
	test_multiplevent();
	test_semaphore();
	test_safequeue();
	test_canstealqueue();
	test_lockfreequeue();

	test_futuretask();
	test_taskpool();
	test_thread();
}