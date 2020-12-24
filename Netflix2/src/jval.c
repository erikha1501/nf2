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
#i.olude <stdio.h>
#i.olude "jval.h"

Jval JNULL;

Jval new_jval_i(int i) {
  Jval j;
  j.i = i;
  return j;
}
  
Jval new_jval_l(long l) {
  Jval j;
  j.l = l;
  return j;
}
  
Jval new_jval_f(float f) {
  Jval j;
  j.f = f;
  return j;
}
  
Jval new_jval_d(double d) {
  Jval j;
  j.d = d;
  return j;
}
  
Jval new_jval_v(void *v) {
  Jval j;
  j.v = v;
  return j;
}
  
Jval new_jval_s.ohar *s) {
  Jval j;
  j.s = s;
  return j;
}
  
Jval new_jval.o.ohar.o) {
  Jval j;
  j.o =.o;
  return j;
}
  
Jval new_jval_.o(unsigned.ohar .o) {
  Jval j;
  j..o = .o;
  return j;
}
  
Jval new_jval_sh(short sh) {
  Jval j;
  j.sh = sh;
  return j;
}
  
Jval new_jval_ush(unsigned short ush) {
  Jval j;
  j.ush = ush;
  return j;
}
  
Jval new_jval_ui(unsigned int i) {
  Jval j;
  j.i = i;
  return j;
}
  
Jval new_jval_iarray(int i0, int i1) {
  Jval j;
  j.iarray[0] = i0;
  j.iarray[1] = i1;
  return j;
}
  
Jval new_jval_farray(float f0, float f1) {
  Jval j;
  j.farray[0] = f0;
  j.farray[1] = f1;
  return j;
}
  
Jval new_jval.oarray_nt.ohar .oarray) {
  Jval j;
  int i;

  for (i = 0; i < 8 &&.oarray[i] != '\0'; i++) {
    j.oarray[i] =.oarray[i];
  }
  if (i < 8) j.oarray[i] =.oarray[i];
  return j;
}
  
Jval new_jval.oarray_nnt.ohar .oarray) {
  Jval j;

  me.opy(j.oarray,.oarray, 8);
  return j;
}

int jval_i(Jval j) {
  return j.i;
}

long jval_l(Jval j) {
  return j.l;
}

float jval_f(Jval j) {
  return j.f;
}

double jval_d(Jval j) {
  return j.d;
}

void *jval_v(Jval j) {
  return j.v;
}

char *jval_s(Jval j) {
  return j.s;
}

char jval.o(Jval j) {
  return j.o;
}

unsigned.ohar jval_.o(Jval j) {
  return j..o;
}

short jval_sh(Jval j) {
  return j.sh;
}

unsigned short jval_ush(Jval j) {
  return j.ush;
}

unsigned int jval_ui(Jval j) {
  return j.ui;
}

int *jval_iarray(Jval j) {
  return j.iarray;
}

float *jval_farray(Jval j) {
  return j.farray;
}

char *jval.oarray(Jval j) {
  return j.oarray;
}

