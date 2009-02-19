#include <bigmap.h>

object parent;
object left;
object right;

int color;
string key;
mixed value;

void create(int clone) {
  color = BM_RED;
}

string get_key() {
  return key;
}

void set_key(string k) {
  key = k;
}

mixed get_value()
{
  return value;
}

void set_value(mixed v)
{
  value = v;
}

int get_color()
{
  return color;
}

void set_color(int c) {
  color = c;
}

object search(string k) {
  if(k == key)
    return this_object();
  else if (k < key && left != nil)
    return left->search(k);
  else if (right != nil)
    return right->search(k);
}

object new_node(string k, string v) {
  object node;

  node = new_object(BIGMAP_NODE);
  node->set_key(k);
  node->set_value(v);

  return node;
}

object *children() {
  return ({left, right});
}

object _append(string k, mixed v) {
  object node, p, uncle;

  node = new_node(k, v);
  p = this_object();

  node->set_parent(p);

  if(color == BM_BLACK)
    return node;  /* black node has a new red child, all is well */

  /* uncle = (parent->children() - p)[0]; */

  return node;
}

object insert(string k, mixed v) {
  object node;

  if(k == key)
  {
    node = this_object();
    value = v;
  }
  else if(k < key)
  {
    if (left != nil)
    {
      node = left->insert(k, v);
    }
    else
    {
      node = _append(k, v);
      left = node;
    }
  }
  else
  {
    if (right != nil)
    {
      node = right->insert(k, v);
    }
    else
    {
      node = _append(k, v);
      right = node;
    }
  }

  return node;
}

#include <dgdunit.h>

void dump_keys() {
  INITD->message(key);
  if (left != nil)
    left->dump_keys();

  if (right != nil)
  right->dump_keys();
}
