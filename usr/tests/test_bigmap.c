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
  int i, sz;

  sz = sizeof(a);

  if (sizeof(b) != sz)
    return FALSE;

  for(i = 0; i < sz; ++i) {
    if (typeof(a[i]) != typeof(b[i]))
      return FALSE;
    else if (typeof(a[i]) == T_ARRAY && !arrays_equal(a[i], b[i]))
      return FALSE;
    else if (a[i] != b[i])
      return FALSE;
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
  object node, node2, node3, node4, node5, node6;
  node = new_object(BIGMAP_NODE);

  /* simple creation */
  node->set_key("100");
  node->set_value("Hello world!");

  /* new nodes are red */
  assert_equal(BM_RED, node->get_color());
  assert_equal(node, node->search("100"));
  assert_equal(nil, node->search("101"));
  assert_equal(({nil, nil}), node->children());

  /* node is the root, so it should be black */
  node->recolor();
  assert_equal(BM_BLACK, node->get_color());

  /* simple replacement */
  node->insert("100", "Goodbye...");

  /* color shouldn't change */
  assert_equal(BM_BLACK, node->get_color());
  assert_equal("Goodbye...", node->get_value());

  /* simple appendings */
  node2 = node->insert("050", "greetings");
  node3 = node->insert("150", "");

  assert_equal(BM_BLACK, node->get_color());
  assert_equal(({node2, node3}), node->children());

  assert_equal(BM_RED, node2->get_color());
  assert_equal("greetings", node2->get_value());

  assert_equal(BM_RED, node3->get_color());
  assert_equal("", node3->get_value());

  assert_equal(node2, node->search("050"));
  assert_equal(node3, node->search("150"));

  /* append requiring fancier recoloration */
  /* red parent, red uncle */

  node4 = node->insert("025", "");
  assert_equal(BM_RED, node4->get_color());

  /* node4's parent and uncle have been recolored */
  assert_equal(node2, node4->get_parent());
  assert_equal(BM_BLACK, node2->get_color());
  assert_equal(BM_BLACK, node3->get_color());

  /* the root is still black */
  assert_equal(BM_BLACK, node->get_color());

  /* even fancier: red parent, black uncle */
  /* case 1: node is on the same side of parent as parent is of grandparent */
  node5 = node->insert("012", "");

  /* case 2: node and parent are on opposite sides of their parents */
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
