#include <kernel/kernel.h>
#include <dgdunit.h>

#define BIGMAP "~System/open/obj/bigmap"
#define BIGMAP_NODE "~System/open/obj/node"

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
  if(typeof(value) == T_STRING)
    return "'" + value + "'";
  else if(typeof(value) == T_NIL)
    return "nil";
  else if(typeof(value) == T_INT || typeof(value) == T_FLOAT)
    return (string)value;

  return "???";
}

void assert_equal(mixed expected, mixed actual)
{
  if(actual != expected) {
    error("assert_equal failed, expected " + repr(expected) + ", got " + repr(actual));
  }
}

void assert(int value)
{
  if(!value)
    error("assertion failed.");
}

void test_node()
{
  object node, node2, node3;
  node = clone_object(BIGMAP_NODE);

  /* simple creation */
  node->set_key("foo");
  node->set_value("Hello world!");

  assert_equal(node, node->search("foo"));
  assert_equal(nil, node->search("bar"));

  /* simple replacement */
  node->insert("foo", "Goodbye...");

  assert_equal("Goodbye...", node->get_value());

  /* simple appendings */
  node2 = node->insert("bar", "greetings");
  node3 = node->insert("gaz", "");

  assert_equal("greetings", node2->get_value());
  assert_equal("", node3->get_value());

  assert_equal(node2, node->search("bar"));
  assert_equal(node3, node->search("gaz"));
}

void test_map()
{
  int i;

  assert_equal(nil, bm->get("foo"));

  for( i = 0; i < 1024; ++i) {
    bm->insert((string)i, i);
    assert_equal(i, bm->get((string)i));
  }
}

void run()
{
  test_node();
  test_map();
}
