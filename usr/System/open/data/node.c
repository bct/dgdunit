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

/* getters */

int get_color()
{
  return color;
}

string get_key() {
  return key;
}

object get_parent() {
  return parent;
}

mixed get_value()
{
  return value;
}

/* setters */

void set_color(int c) {
  color = c;
}

void set_key(string k) {
  key = k;
}

void set_parent(object p) {
  parent = p;
}

void set_value(mixed v)
{
  value = v;
}

/* actual useful stuff begins here */

object search(string k) {
  if(k == key)
    return this_object();
  else if (k < key && left != nil)
    return left->search(k);
  else if (right != nil)
    return right->search(k);
}

object new_node(string k, mixed v) {
  object node;

  node = new_object(BIGMAP_NODE);
  node->set_key(k);
  node->set_value(v);

  return node;
}

object *children() {
  return ({left, right});
}

void recolor() {
  object gp, uncle;

  if(!parent) {
    /* we're at the root node, it should be black */
    color = BM_BLACK;
    return;
  }

  if(parent->get_color() == BM_BLACK)
    return;             /* black node has a new red child, all is well */

  gp = parent->get_parent();

  if(gp) /* find uncle */
    uncle = (gp->children() - ({parent}))[0];

  if(uncle && uncle->get_color() == BM_RED) {
    /* repaint node's parent and uncle black */
    parent->set_color(BM_BLACK);
    uncle->set_color(BM_BLACK);
    /* grandparent must now be red */
    gp->set_color(BM_RED);
    /* this may have broken our nice set of properties, recolor grandparent */
    gp->recolor();
    return;
  }

  /* if we get here, we know our parent is red and our uncle is black
   * we need to rotate this node left or right, depending on its placement
   * and it's parent's placement.                                           */
}

object _append(string k, mixed v) {
  object node;
  node = new_node(k, v);

  node->set_parent(this_object());
  node->recolor();

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
    if (left)
    {
      /* recurse */
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
    if (right)
    {
      /* recurse */
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
