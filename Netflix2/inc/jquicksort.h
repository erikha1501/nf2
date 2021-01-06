#pragma once
#include "jval.h"

typedef int (*Jval_Comparer)(Jval*, Jval*);
extern void qsort_jval(Jval arr[], int low, int high, Jval_Comparer comparer);