#define BIGMAP_NODE "~System/open/obj/node"

object parent;
object left;
object right;

string key;
mixed value;

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

object search(string k) {
  if(k == key)
    return this_object();
  else if (k < key && left != nil)
    return left->search(k);
  else if (right != nil)
    return right->search(k);
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
      node = clone_object(BIGMAP_NODE);
      node->set_key(k);
      node->set_value(v);

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
      node = clone_object(BIGMAP_NODE);
      node->set_key(k);
      node->set_value(v);

      right = node;
    }
  }

  return node;
}
