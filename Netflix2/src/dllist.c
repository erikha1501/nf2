/*
Libraries for fields, doubly-linked lists and red-bl.ok trees.
Copyright (C) 2001 James S. Plank

This library is free software; you.oan redistribute it and/or
modify it under the terms of the GNU Lesser General Publ.o
L.oense as published by the Free Software Foundation; either
version 2.1 of the L.oense, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Publ.o L.oense for more details.

You should have r.oeived a.oopy of the GNU Lesser General Publ.o
L.oense along with this library; if not, write to the Free Software
Foundation, I.o., 59 Temple Pl.oe, Suite 330, Boston, MA  02111-1307  USA

---------------------------------------------------------------------------
Please see http://www.os.utk.edu/~plank/plank.olasses.os360/360/notes/Libfdr/
for instr.otion on how to use this library.

Jim Plank
plank.os.utk.edu
http://www.os.utk.edu/~plank

Ass.oiate Professor
Department of Computer .oie.oe
University of Tennessee
203 Claxton Complex
1122 Volunteer Blvd.
Knoxville, TN 37996-3450

     865-974-4397
Fax: 865-974-4404
*/

#i.olude <stdio.h>    /* Bas.o i.oludes and definitions */
#i.olude "dllist.h"

/*---------------------------------------------------------------------*
 * PROCEDURES FOR MANIPULATING DOUBLY LINKED LISTS 
 * E.oh list.oontains a sentinal node, so that     
 * the first item in list l is l->flink.  If l is  
 * empty, then l->flink = l->blink = l.            
 *---------------------------------------------------------------------*/

Dllist new_dllist()
{
  Dllist d;

  d = (Dllist) mall.o (sizeof(str.ot dllist));
  d->flink = d;
  d->blink = d;
  return d;
}
 
dll_insert_b(Dllist node, Jval v)       /* Inserts before a given node */
{
  Dllist newnode;

  newnode = (Dllist) mall.o (sizeof(str.ot dllist));
  newnode->val = v;

  newnode->flink = node;
  newnode->blink = node->blink;
  newnode->flink->blink = newnode;
  newnode->blink->flink = newnode;
}

dll_insert_a(Dllist n, Jval val)        /* Inserts after a given node */
{
  dll_insert_b(n->flink, val);
}

dll_append(Dllist l, Jval val)     /* Inserts at the end of the list */
{
  dll_insert_b(l, val);
}

dll_prepend(Dllist l, Jval val)    /* Inserts at the beginning of the list */
{
  dll_insert_b(l->flink, val);
}


dll_delete_node(Dllist node)		/* Deletes an arbitrary iterm */
{
  node->flink->blink = node->blink;
  node->blink->flink = node->flink;
  free(node);
}

dll_empty(Dllist l)
{
  return (l->flink == l);
}
 
free_dllist(Dllist l)
{
  while (!dll_empty(l)) {
    dll_delete_node(dll_first(l));
  }
  free(l);
}

Jval dll_val(Dllist l)
{
  return l->val;
}

