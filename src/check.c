
/*
- [ ] Declarations
    - [ ] Variable decl
    - [ ] Type decl
    - [ ] Function decl
    - [ ] Special functions
*/

void checkdecl_var()
{
    /*
        Adds the mapping x:T to the symbol table
        Has the following forms:
            - var x T
            - var x T = expr
            - var x = expr
    */
}
void checkdecl_type()
{
    /*
        Adds the mapping T1 -> def(T2) to the symbol table. Error is raised
        if one already exists in the current scope, otherwise if it is outside
        of the scope but exists, the previous one gets _shadowed_
    */


}
void checkdecl_func()
{
    /*
        func f(p1 T1, ..., pn Tn) Tr {...}

        f:(T1 * T2 * ... * Tn -> Tr) gets added to the symbol table. Error is
        raised if previously declared in current (global) scope. For each
        parameter, pi:Ti is added to the symbol table. Error is raised if
        two parameters have the same name. Parameters can have the same name
        as the function.

        A function type checks if its body (statements) type check

        Functions that return a value need to have a terminating statement
        (weeding pass)
    */
}
void checkdecl_specialfunc()
{
    /*
        Two special functions: init and main. They follow the previous rules
        above and have no parameters or return type.

        No binding for init (can be declared many times)

        These two functions can only be declared at the top level scope
    */
}

/*
- [ ] Statements
    - [ ] Empty
    - [ ] break + continue
    - [ ] Expression statement
    - [ ] return
    - [ ] Short decl
    - [ ] Decl
    - [ ] Assignments
    - [ ] OP-assignment
    - [ ] Block
    - [ ] Print / Println
    - [ ] For loop
    - [ ] If
    - [ ] Switch statement
    - [ ] Increment / Decrement
*/

void checkstmt_empty()
{
    /*
        Trivially well typed, e.g.

        ;

    */

}
void checkstmt_breakcontinue()
{
    /*
        See document for type rule.
    */

}
void checkstmt_expstmt() {
    /*
        An expression is well typed if its expression child is well typed,
        see document for rule.
    */
}
void checkstmt_return() {
    // `return` is well typed if the enclosing function has no return type

    /*
        `return expr` is well typed if `expr` is well typed and the enclosing
        function has the same return type
    */
}
void checkstmt_shortdecl()
{
    /*
        x1, x2, ..., xk := e1, e2, ... , ek

        - All the expressions on the right hand side are well typed
        - At least one variable on the left hand side is not declared in the
          current scope
        - The variable is already declared in the current scope but it and
          the assigned expression have the same type

        If these conditions are met then xi -> typeof(ei) is added to the
        symbol table
    */
}
void checkstmt_decl()
{
    // Described above
}
void checkstmt_assign()
{
    /*
        Assignment typechecks if LHS is well typed, RHS is well typed
        and every pair (vi, ei) has the same type
    */
}
void checkstmt_opassign()
{
    /*
        Op-assignment typechecks if LHS is well typed, RHS is well typed
        and the operator `op` accepts both types t(v) and t(expr) and returns
        t(v) as a result type
    */
}
void checkstmt_block()
{
    // Typechecks if the statements typechecks, a block opens a NEW scope
}
void checkstmt_print_ln()
{
    // Typechecks if all expressions are well typed and resolve to a base type
}

void checkstmt_for()
{
    // This is a bit complicated, come back in a bit
}
void checkstmt_if()
{
    // Kinda more work as well, come back later
}
void checkstmt_switch()
{
    // Same as other two above
}
void checkstmt_incdec()
{
    /*  Typechecks if `expr` is well typed and resolves to base type in
        int, float64 or rune
    */
}

/*

- [ ] Expressions
    - [ ] Literals
    - [ ] Identifiers
    - [ ] Unary exp
    - [ ] Binary exp
    - [ ] Function call
    - [ ] Indexing
    - [ ] Field selection
    - [ ] Builtins
        - [ ] Append
        - [ ] Capacity
        - [ ] Length
    - [ ] Type casting

*/

void checkexp_literal ()
{
    /*
        something to fill in here...
    */
}
void checkexp_ident ()
{
    /*
        something to fill in here...
    */
}
void checkexp_unary ()
{
    /*
        something to fill in here...
    */
}
void checkexp_binary ()
{
    /*
        something to fill in here...
    */
}
void checkexp_funccall ()
{
    /*
        something to fill in here...
    */
}
void checkexp_index ()
{
    /*
        something to fill in here...
    */
}
void checkexp_field ()
{
    /*
        something to fill in here...
    */
}
void checkexp_builtin_append ()
{
    /*
        something to fill in here...
    */
}
void checkexp_builtin_cap ()
{
    /*
        something to fill in here...
    */
}
void checkexp_builtin_len ()
{
    /*
        something to fill in here...
    */
}
void checkexp_typecast ()
{
    /*
        something to fill in here...
    */
}
