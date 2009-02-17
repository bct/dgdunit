#include <kernel/kernel.h>
#include <kernel/rsrc.h>
#include <kernel/access.h>

#define TEST_DIR "~tests/"

inherit rsrc API_RSRC;
inherit access API_ACCESS;

void message(string str);
void run_test(string name);
void run_tests();

static void create()
{
  /* initialize apis */
  rsrc::create();
  access::create();

  /* initialize test user */
  add_owner("tests");
  add_user("tests");

  compile_object("~System/open/obj/bigmap"); /* FIXME this shouldn't go here... */
  compile_object("~System/open/obj/node"); /* FIXME this shouldn't go here... */

  /* run tests */
  run_tests();

  message("finished, shutting down.");
  shutdown(); /* automatically shut down when tests are complete */
}

void run_test(string name)
{
  object test;
  string f;

  sscanf(name, "%s.c", name);
  message("compiling " + TEST_DIR + name);
  test = compile_object(TEST_DIR + name);

  message("running " + TEST_DIR + name);
  test->run();
}

void run_tests()
{
  string *tests;
  int i, sz;

  message("loading tests...");

  tests = get_dir(TEST_DIR + "/*.c")[0];
  sz = sizeof(tests);

  if (sz == 0) {
    message("no tests found in " + TEST_DIR);
    return;
  }

  for (i = 0; i < sz; ++i) {
    run_test(tests[i]);
  }
}

void message(string str)
{
  if(str == nil)
    str = "nil";
  DRIVER->message("dgdunit: " + str + "\n");
}
