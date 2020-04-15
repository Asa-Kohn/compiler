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

/* ldc 0          ldc 1          ldc 2
 * iload k        iload k        iload k
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload k        iload k
 *                               dup
 *                               iadd
 */

int simplify_multiplication_left(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_iload(next(*c), &k) &&
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

/* iload x
 * ldc 1
 * idiv
 * ------>
 * iload x
 */

int simplify_division_rightconst(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (k == 1)
      return replace(c, 3, makeCODEiload(x, NULL));
    return 0;
  }
  return 0;
}

/* iload x
 * iload k
 * idiv
 * ------>
 * if x == k
 *  ldc 1
 */

int simplify_division_rightk(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_iload(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x == k)
      return replace(c, 3, makeCODEldc_int(1, NULL));
    return 0;
  }
  return 0;
}

/* ldc 0
 * iload k
 * idiv
 * ------>
 * ldc 0
 */

int simplify_division_leftconst(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_iload(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x == 0)
      return replace(c, 3, makeCODEldc_int(0, NULL));
    return 0;
  }
  return 0;
}

/* iload x
 * iload k
 * idiv
 * ------>
 * if x == k
 *  ldc 1
 */

int simplify_division_leftk(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_iload(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x == k)
      return replace(c, 3, makeCODEldc_int(1, NULL));
    return 0;
  }
  return 0;
}

/* iload x                iload x           iload x
 * ldc k   (0<=k<=127)    ldc k             ldc k (0<=k<=127)
 * isub                   isub              isub
 * istore x               istore x          istore x
 * --------->             --------->        -------->
 * if x == 0              if k == 0         if k > 0
 *  iload k                 iload x             iinc x (-k)
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
    else if (k > 0)
      return replace(c, 4, makeCODEiinc(x, -k, NULL));
    return 0;
  }
  return 0;
}

/* ldc_int x
 * ldc_int k
 * imul
 * ------>
 * ldc x*k
 */

int simplify_constfold_mult(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_imul(next(next(*c))))
  {
    if (x * k >= 0 && x * k <= 127)
      return replace(c, 3, makeCODEldc_int(x * k, NULL));
    return 0;
  }
  return 0;
}

/* ldc_int x
 * ldc_int k
 * idiv
 * ------>
 * ldc x/k
 */

int simplify_constfold_div(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x / k >= 0 && x / k <= 127)
      return replace(c, 3, makeCODEldc_int(x / k, NULL));

    return 0;
  }
  return 0;
}

/* ldc_int x
 * ldc_int k 
 * irem   
 * ------> 
 * ldc x%k 
 */

int simplify_constfold_rem(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_idiv(next(next(*c))))
  {
    if (x * k >= 0 && x * k <= 127)
      return replace(c, 3, makeCODEldc_int(x / k, NULL));
    return 0;
  }
  return 0;
}

/* ldc_int x
 * ldc_int k
 * iadd
 * ------>
 * ldc x+k
 */

int simplify_constfold_add(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_iadd(next(next(*c))))
  {
    return replace(c, 3, makeCODEldc_int(x + k, NULL));
  }
  return 0;
}

/* ldc_int x
 * ldc_int k
 * isub
 * ------>
 * ldc x*k
 */

int simplify_constfold_sub(CODE **c)
{
  int x, k;
  if (is_ldc_int(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_isub(next(next(*c))))
  {
    return replace(c, 3, makeCODEldc_int(x - k, NULL));
  }
  return 0;
}

/* iload x      aload x
 * istore x     astore x
 * ------>      ------>
 * null         null
 */

int simplify_load_store(CODE **c)
{
    int x, y;
  if (is_iload(*c, &x) && is_istore(next(*c), &y) && x == y)
    return replace(c, 2, NULL);
  else if (is_aload(*c, &x) && is_astore(next(*c), &y) && x == y)
    return replace(c, 2, NULL);

  return 0;
}

/* dup
 * getfield x
 * putfield x
 * ------>
 * null
 */

int simplify_get_put(CODE **c)
{
    char *f1, *f2;
    if (is_dup(*c) && is_getfield(next(*c), &f1) &&
        is_putfield(next(next(*c)), &f2) && strcmp(f1, f2) == 0)
    return replace(c, 2, NULL);

  return 0;
}

/* iload x      iload x
 * iload 0      iload k (0<=k<=127)
 * iadd         iadd
 * ----->       ------>
 * iload x      if(k > 0)
 *                  iinc x k
 */

int simplify_addition_left(CODE **c)
{
    int x, y, k;
  if (
      is_iload(*c, &x) &&
      is_ldc_int(next(*c), &k) &&
      is_iadd(next(next(*c))) &&
      is_istore(next(next(next(*c))), &y) && x == y)
  {
    if (k == 0)
      return replace(c, 4, NULL);
    else if (0 < k && k <= 127)
      return replace(c, 4, makeCODEiinc(x, k, NULL));
  }

  return 0;
}

/* iload 0      iload k (0<=k<=127)
 * iload x      iload x
 * iadd         iadd
 * ----->       ------>
 * iload x      if(k > 0)
 *                  iinc x k
 */

int simplify_addition_right(CODE **c)
{
    int x, y, k;
  if (is_iload(*c, &k) &&
      is_ldc_int(next(*c), &x) &&
      is_iadd(next(next(*c))) &&
      is_istore(next(next(next(*c))), &y) && x == y)
  {
    if (k == 0)
      return replace(c, 4, NULL);
    else if (0 < k && k <= 127)
      return replace(c, 4, makeCODEiinc(x, k, NULL));
  }
  return 0;
}

/* swap
 * swap
 * ----->
 * null
 */

int simplify_swap(CODE **c)
{
  if (is_swap(*c) && is_swap(next(*c)))
    return replace(c, 2, NULL);

  return 0;
}

/* ineg
 * ineg
 * ---->
 * null
 */

int simplify_negation(CODE **c)
{
  if (is_ineg(*c) && is_ineg(next(*c)))
    return replace(c, 2, NULL);

  return 0;
}

/* iload x
 * iload k (0<=k<=127)
 * irem
 * ----->
 * if(k == 1)
 *  ldc 0
 */

int simplify_remainder(CODE **c)
{
  int x, k;
  if (is_iload(*c, &x) &&
      is_iload(*c, &k) &&
      is_irem(*c) &&
      k == 1)
  {
    return replace(c, 4, makeCODEldc_int(0, NULL));
  }
  return 0;
}

int delete_extra_noop(CODE **c)
{
    if(is_nop(next(*c)) && next(next(*c)))
        return replace(c, 1, NULL);
    return 0;
}

int simplify_if(CODE **c)
{
    int l1, l2, l3, l4, l5, c1, c2;
    if(is_ldc_int(next(*c), &c1) &&
       is_goto(next(next(*c)), &l2) &&
       is_label(next(next(next(*c))), &l3) &&
       is_ldc_int(next(next(next(next(*c)))), &c2) &&
       is_label(next(next(next(next(next(*c))))), &l4) &&
       is_ifeq(next(next(next(next(next(next(*c)))))), &l5) &&
       l2 == l4 && c1 == 0 && c2 == 1 && uniquelabel(l3) && uniquelabel(l2))
    {
        if(is_if_icmplt(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_icmpge(l5, NULL));
        else if(is_if_icmpgt(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_icmple(l5, NULL));
        else if(is_if_icmple(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_icmpgt(l5, NULL));
        else if(is_if_icmpge(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_icmplt(l5, NULL));
        else if(is_ifnull(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEifnonnull(l5, NULL));
        else if(is_ifnonnull(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEifnull(l5, NULL));
        else if(is_if_acmpeq(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_acmpne(l5, NULL));
        else if(is_if_acmpne(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEif_acmpeq(l5, NULL));
        else if(is_ifeq(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEifne(l5, NULL));
        else if(is_ifne(*c, &l1) && l1 == l3)
            return replace_modified(c, 7, makeCODEifeq(l5, NULL));
    }
    return 0;
}

int remove_unreachable(CODE **c)
{
    int l1, l2;
    if(is_return(*c) && next(*c) && !is_label(next(*c), &l1))
        return replace(c, 2, makeCODEreturn(NULL));
    if(is_areturn(*c) && next(*c) && !is_label(next(*c), &l1))
        return replace(c, 2, makeCODEareturn(NULL));
    if(is_ireturn(*c) && next(*c) && !is_label(next(*c), &l1))
        return replace(c, 2, makeCODEireturn(NULL));
    if(is_goto(*c, &l2) && next(*c) && !is_label(next(*c), &l1))
        return replace(c, 2, makeCODEgoto(l2, NULL));
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
  ADD_PATTERN(simplify_division_rightconst);
  ADD_PATTERN(simplify_division_leftconst);
  ADD_PATTERN(simplify_division_leftk);
  ADD_PATTERN(simplify_subtraction);
  ADD_PATTERN(simplify_constfold_mult);
  ADD_PATTERN(simplify_constfold_div);
  ADD_PATTERN(simplify_constfold_rem);
  ADD_PATTERN(simplify_constfold_add);
  ADD_PATTERN(simplify_constfold_sub);
  ADD_PATTERN(simplify_load_store);
  ADD_PATTERN(simplify_addition_right);
  ADD_PATTERN(simplify_addition_left);
  ADD_PATTERN(simplify_get_put);
  ADD_PATTERN(simplify_load_store);
  ADD_PATTERN(simplify_swap);
  ADD_PATTERN(simplify_negation);
  ADD_PATTERN(simplify_remainder);
  ADD_PATTERN(delete_extra_noop);
  ADD_PATTERN(simplify_if);
  ADD_PATTERN(remove_unreachable);
}
