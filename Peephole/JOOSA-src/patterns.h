/*
 * JOOS is Copyright (C) 1997 Laurie Hendren & Michael I. Schwartzbach
 *
 * Reproduction of all or part of this software is permitted for
 * educational or research use on condition that this copyright notice is
 * included in any copy. This software comes with no warranty of any
 * kind. In no event will the authors be liable for any damages resulting from
 * use of this software.
 *
 * email: hendren@cs.mcgill.ca, mis@brics.dk
 */

/* iload x        iload x        iload x
 * ldc 0          ldc 1          ldc 2
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */

int simplify_multiplication_right(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_imul(next(next(*c))))
  {
    if (k == 0)
      return replace(c, 3, makeCODEldc_int(0, NULL));
    else if (k == 1)
      return replace(c, 3, makeCODEiload(x, NULL));
    else if (k == 2)
      return replace(c, 3, makeCODEiload(x, makeCODEdup(makeCODEiadd(NULL))));
    return 0;
  }
  return 0;
}

/* dup
 * astore x
 * pop
 * -------->
 * astore x
 */
int simplify_astore(CODE **c)
{
  int x;
  if (is_dup(*c) &&
      is_astore(next(*c), &x) &&
      is_pop(next(next(*c))))
  {
    return replace(c, 3, makeCODEastore(x, NULL));
  }
  return 0;
}

/* iload x
 * ldc k   (0<=k<=127)
 * iadd
 * istore x
 * --------->
 * iinc x k
 */
int positive_increment(CODE **c)
{
  int x, y, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_iadd(next(next(*c))) &&
      is_istore(next(next(next(*c))), &y) &&
      x == y && 0 <= k && k <= 127)
  {
    return replace(c, 4, makeCODEiinc(x, k, NULL));
  }
  return 0;
}

/* goto L1
 * ...
 * L1:
 * goto L2
 * ...
 * L2:
 * --------->
 * goto L2
 * ...
 * L1:    (reference count reduced by 1)
 * goto L2
 * ...
 * L2:    (reference count increased by 1)  
 */
int simplify_goto_goto(CODE **c)
{
  int l1, l2;
  if (is_goto(*c, &l1) && is_goto(next(destination(l1)), &l2) && l1 > l2)
  {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEgoto(l2, NULL));
  }
  return 0;
}

/* ========================================================= */
/* newly added patterns */

/* dup
 * istore x
 * pop
 * -------->
 * istore x
 */
int simplify_istore(CODE **c)
{
  int x;
  if (is_dup(*c) &&
      is_istore(next(*c), &x) &&
      is_pop(next(next(*c))))
  {
    return replace(c, 3, makeCODEistore(x, NULL));
  }
  return 0;
}

/* iload 0        iload 1        iload 2
 * ldc k          ldc k          ldc k
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload k        iload k
 *                               dup
 *                               iadd
 */

int simplify_multiplication_left(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_imul(next(next(*c))))
  {
    if (x == 0)
      return replace(c, 3, makeCODEldc_int(0, NULL));
    else if (x == 1)
      return replace(c, 3, makeCODEiload(k, NULL));
    else if (x == 2)
      return replace(c, 3, makeCODEiload(k, makeCODEdup(makeCODEiadd(NULL))));
    return 0;
  }
  return 0;
}

/* iload 0        iload x   
 * ldc k          iload k   
 * idiv           imul       
 * ------>        ------>   
 * ldc 0          if x == k  
 *                 ldc 1     
 */

int simplify_division_left(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x == 0)
      return replace(c, 3, makeCODEldc_int(0, NULL));
    else if (x == k)
      return replace(c, 3, makeCODEldc_int(1, NULL));
    return 0;
  }
  return 0;
}

/* iload x        iload x
 * ldc 1          iload k
 * idiv           idiv
 * ------>        ------>
 * iload x        if x == k
 *                 ldc 1
 */

int simplify_division_right(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (k == 1)
      return replace(c, 3, makeCODEiload(x, NULL));
    else if (x == k)
      return replace(c, 3, makeCODEldc_int(1, NULL));
    return 0;
  }
  return 0;
}

/* iload x                iload x
 * ldc k   (0<=k<=127)    ldc k
 * isub                   isub
 * istore x               istore x
 * --------->             --------->
 * if x == 0              if k == 0
 *  iload k                 iload x
 *  ineg 
 */
int simplify_subtraction(CODE **c)
{
  int x, y, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_isub(next(next(*c))) &&
      is_istore(next(next(next(*c))), &y) &&
      x == y && 0 <= k && k <= 127)
  {
    if (x == 0)
      return replace(c, 4, makeCODEiload(k, makeCODEineg(NULL)));
    else if (k == 0)
      return replace(c, 4, makeCODEiload(x, NULL));
    return 0;
  }
  return 0;
}


void init_patterns(void)
{
  ADD_PATTERN(simplify_multiplication_right);
  ADD_PATTERN(simplify_astore);
  ADD_PATTERN(positive_increment);
  ADD_PATTERN(simplify_goto_goto);

  /* ========================================================= */
  /* add new patterns */
  ADD_PATTERN(simplify_istore);
  ADD_PATTERN(simplify_multiplication_left);
  ADD_PATTERN(simplify_division_left);
  ADD_PATTERN(simplify_division_right);
  ADD_PATTERN(simplify_subtraction);
}
