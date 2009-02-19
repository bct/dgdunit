#include <kernel/kernel.h>
#include <dgdunit.h>

#include <bigmap.h>

mapping map;

object bm;

void create()
{
  map = ([]);
  bm = clone_object(BIGMAP);
}

#include <type.h>

string repr(mixed value)
{
  string str;
  int i, sz;

  if(typeof(value) == T_STRING)
    return "'" + value + "'";
  else if(typeof(value) == T_NIL)
    return "nil";
  else if(typeof(value) == T_INT || typeof(value) == T_FLOAT)
    return (string)value;
  else if(typeof(value) == T_ARRAY) {
    str = "({";
    sz = sizeof(value);
    for (i = 0; i < sz; ++i)
      str += " " + repr(value[i]);

    str += " })";
    return str;
  }

  return "???";
}

int arrays_equal(mixed *a, mixed *b) {
  int i;
  int sz;

  sz = sizeof(a);

  if (sizeof(b) != sz)
    return FALSE;

  for(i = 0; i < sz; ++i) {
    if (typeof(a[i]) == typeof(b[i]) && typeof(a[i]) == T_ARRAY) {
      if(!arrays_equal(a[i], b[i]))
        return FALSE;
    } else if (a[i] != b[i]) {
      return FALSE;
    }
  }

  return TRUE;
}

void assert_equal(mixed expected, mixed actual)
{
  int succeed;

  if (typeof(expected) != typeof(actual))
    succeed = FALSE;
  else if (typeof(expected) == T_ARRAY)
    succeed = arrays_equal(expected, actual);
  else
    succeed = (actual == expected);

  if (!succeed)
    error("assert_equal failed, expected " + repr(expected) + ", got " + repr(actual));
}

void assert(int value)
{
  if(!value)
    error("assertion failed.");
}

void test_node()
{
  object node, node2, node3;
  node = new_object(BIGMAP_NODE);

  /* simple creation */
  node->set_key("foo");
  node->set_value("Hello world!");

  assert_equal(BM_RED, node->get_color());
  assert_equal(node, node->search("foo"));
  assert_equal(nil, node->search("bar"));

  /* change node's color to black since it's the root */
  node->set_color(BM_BLACK);
  assert_equal(BM_BLACK, node->get_color());

  /* simple replacement */
  node->insert("foo", "Goodbye...");

  assert_equal(BM_BLACK, node->get_color());
  assert_equal("Goodbye...", node->get_value());

  /* simple appendings */
  node2 = node->insert("bar", "greetings");
  node3 = node->insert("gaz", "");

  assert_equal(({node2, node3}), node->children());

  assert_equal(3, node2->get_color());
  assert_equal("greetings", node2->get_value());

  assert_equal(3, node3->get_color());
  assert_equal("", node3->get_value());

  assert_equal(node2, node->search("bar"));
  assert_equal(node3, node->search("gaz"));
}

void test_map()
{
  int i;
  string key;

  assert_equal(nil, bm->get("foo"));

  INITD->message("ouch.");

  for( i = 0; i < 65536; ++i) {
    key = (string) i;
    bm->insert(key, i);
    assert_equal(i, bm->get(key));
  }

  INITD->message("dumping");
  /* uncomment to dump a list of all keys to stderr */
  /*bm->get_root()->dump_keys();*/
}

void run()
{
  test_node();
  test_map();
}
