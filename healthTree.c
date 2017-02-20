#include "healthTree.h"


healthTree *
insertTree (healthTree * tree, int id, double health)
{

  //If tree == NULL, a new node is created.
  if (tree == NULL)
    {
      //Mallocs for the size of the node.
      tree = (healthTree *) malloc (sizeof (*tree));
      if (tree == NULL)
	{
	  printf ("Memory Error");
	  return NULL;
	}
      else
	{
	  //callocs for the size of the string. The string is then strncpy'd
	  tree->id = id;
	  tree->health = health;
	  tree->left = tree->right = NULL;
	}
    }

  else
    {

      if (id < tree->id)
	{
	  tree->left = insertTree (tree->left, id, health);
	}
      else if (id > tree->id)
	{
	  tree->right = insertTree (tree->right, id, health);
	}
      else
	{
	  return tree;
	}

    }
  return tree;
}

void
destroy_healthTree (healthTree * tree)
{
  if (tree == NULL)
    {
      return;
    }

  destroy_healthTree (tree->left);
  destroy_healthTree (tree->right);
  free (tree);
}

void
print_tree (healthTree * tree, int lowHealth)
{
  //The function will move to the bottom left. 
  if (tree->left)
    {
      print_tree (tree->left, lowHealth);
    }

  if (tree->health <= lowHealth)
    {
      printf ("Zerg #%d\n", tree->id);
    }

  if (tree->right)
    {
      print_tree (tree->right, lowHealth);
    }

}
