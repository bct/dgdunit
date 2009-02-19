#include <bigmap.h>

object root;

object search(string key);

void create(int clone)
{
}

void insert(string key, mixed value)
{
  object node;

  if(root == nil)
  {
    root = new_object(BIGMAP_NODE);
    root->set_key(key);
    root->set_value(value);
    root->set_color(BM_BLACK);
    return;
  }

  root->insert(key, value);
}

/* get the node associated with a key */
object search(string key)
{
  if(root == nil)
    return nil;

  return root->search(key);
}

/* get the value associated with a key */
mixed get(string key)
{
  object node;

  node = search(key);

  if(node == nil)
    return nil;

  return node->get_value();
}

object get_root()
{
  return root;
}
