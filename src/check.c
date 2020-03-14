
#include <stdio.h>
#include <stdbool.h>

// TODO: decide if we should make symbol table a global variable?

// Some helper functions

void addsymbol(symboltable * st, symbol * s)
{
    addsymbol(st->head->table, s); // Does this even make sense?
}

void scope(symboltable * st)
{
    symboltablenode n = malloc(sizeof(symboltable));
    // Push onto head of stack
    n->next = st->head;
    st->head = n;
}

void unscope(symboltable st)
{
    // Pop
    st->head = st->head->next;
}

TYPE widest(TYPE s, TYPE t)
{
    if(s == tree_exp_kind_float || t == tree_exp_kind_float)
    {
        return tree_exp_kind_float;
    }
    else if(s == tree_exp_kind_int || t == tree_exp_kind_int)
    {
        return tree_exp_kind_int;
    }
    else if(s == tree_exp_kind_rune || t == tree_exp_kind_rune)
    {
        return tree_exp_kind_rune;
    }
    else
    {
        fprintf(stderr, "Internal Error: Reached part of control that \
                                should not be reached");
        exit(1);
    }
}

// void for now, returns the resolved type
TYPE t resolve(TYPE t)
{
    // go up the type inheritance tree until we hit a base type
    while
    (
        t != tree_exp_kind_int      ||
        t != tree_exp_kind_rune     ||
        t != tree_exp_kind_float    ||
        t != tree_exp_kind_str      ||
        t != tree_exp_kind_bool
    )
    {
        // get the ancestral type that it inherited from

        // scan symbol table for where it was defined

        // t = t.ancestral_type
    }
}

bool commonresolve(TYPE t1, TYPE t2)
{
    // Here we do not know where they land in inheritance tree

    // TODO
}

bool isnumeric(TYPE t)
{
    if(isinteger(t) || t == tree_exp_kind_float) return true;

    return false;
}

bool isinteger(TYPE t)
{
    if(t == tree_exp_kind_int || t == tree_exp_kind_rune) return true;

    return false;
}

symbol getsymbol(symboltable * st, expr * e)
{

    // base case
    if(st == NULL)
    {
        fprintf(stderr, "Error: (line %d).\n", lineno);
        exit(1);
    }
    // e is going to be an identifier, maybe perform a check?

    // traverse symbol table going up in scope if nothing is found

    // traverse current scope that we're in

    while(st->head != NULL)
    {
        if(st->head.ident == e) return st->head.type;

        // if not found, go to next
        st->head = st->head->next;
    }

    // if we're here then we couldn't find it in this scope, moving up
    getsymbol(unscope(st));
}

/* BEGINNING OF TYPE CHECKING FUNCTIONS */

/*
- [ ] Declarations
    - [ ] Variable decl
    - [ ] Type decl
    - [ ] Function decl
    - [ ] Special functions
*/

void checkdecl_var(symboltable st, ident id, TYPE t)
{
    /*
        Adds the mapping x:T to the symbol table
        Has the following forms:
            - var x T
            - var x T = expr
            - var x = expr
    */

    symbol * s = malloc(sizeof(symbol));
    s->id = id;
    s->type = t;

    // Check if it already exists in the current scope
    for(symbol cs = st->head->table; cs; cs = cs->next)
    {
        if(cs.id == id && cs.type == t)
        {
            // TODO: ID needs to be a string not some ident whatever
            fprintf(stderr, "Error: (line %d) variable (%s) \
                previously declared in current scope.\n", lineno, id);
            exit(1);
        }
    }

    // If we're here then we haven't exited
    // So we add to the symbol table

    addsymbol(st, s);
}
void checkdecl_type()
{
    /*
        `type T1 T2`
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
- [~] Statements
    - [~] Empty
    - [~] break + continue (+ fallthrough?)
    - [~] Expression statement
    - [~] return
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

void checkstmt(STMT * s)
{
    switch(s->kind)
    {
        case tree_stmt_kind_exp:
            checkstmt_exp(s->expstmt);
            break;
        case tree_stmt_kind_block:
            checkstmt_block(s->block);
            break;
        case tree_stmt_kind_assign:
            checkstmt_assign(s->assign);
            break;
        case tree_stmt_kind_assignop:
            checkstmt_assign(s->assignop);
            break;
        case tree_stmt_kind_shortdecl:
            checkstmt_shortdecl(s->shortdecl);
            break;
        case tree_stmt_kind_inc:
            checkstmt_incdec(s->expstmt);
            break;
        case tree_stmt_kind_dec:
            checkstmt_incdec(s->expstmt);
            break;
        case tree_stmt_kind_var_decl:
            checkstmt_incdec(s->expstmt);
            break;
        case tree_stmt_kind_type_spec:
            checkstmt_typespec(s->type_spec);
            break;
        case tree_stmt_kind_print:
            checkstmt_print(s->exps);
            break;
        case tree_stmt_kind_println:
            checkstmt_print(s->exps);
            break;
        case tree_stmt_kind_return:
            checkstmt_return(s->expstmt);
            break;
        case tree_stmt_kind_if:
            checkstmt_if(s->ifstmt);
            break;
        case tree_stmt_kind_switch:
            checkstmt_switch(s->switchstmt);
            break;
        case tree_stmt_kind_for:
            checkstmt_for(s->forstmt);
            break;
        case tree_stmt_kind_break:
            checkstmt_breakcontinue(s->expstmt);
            break;
        case tree_stmt_kind_continue:
            checkstmt_breakcontinue(s->expstmt);
            break;
        case tree_stmt_kind_fallthrough:
            // GoLite supports fallthrough?
            checkstmt_breakcontinue(s->expstmt);
            break;
    }
}

void checkstmt_empty(EXP * e)
{
    /*
        Trivially well typed, e.g.
        ;
    */

    if(e->kind != empty_stmt)
    {
        fprintf(stderr, "Error: (line %d) bad empty statement.\n", e->lineno);
        exit(1);
    }

}

// TODO: Needs work...not sure this is good.
void checkstmt_breakcontinue(EXP * e)
{
    /*
        See document for type rule, but it's pretty trivial.
    */
    if(e->kind != tree_stmt_kind_break && e->kind != tree_stmt_kind_continue)
    {
        fprintf(stderr, "Error: (line %d) expected statement \
                    `break` or `continue`.\n", e->lineno);
        exit(1);
    }

}
void checkstmt_expstmt()
{
    /*
        An expression is well typed if its expression child is well typed,
        see document for rule.
    */
}
void checkstmt_return()
{
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
void checkstmt_block(symboltable st, STMTS * block)
{
    // Typechecks if the statements typechecks, a block opens a NEW scope

    // create a new scope
    st = scope(st);

    // iterate over all the statements
    while(block != NULL)
    {
        checkstmt(block->stmt);
        block = block->next;
    }
}
void checkstmt_print_ln(EXPS * es)
{
    // Typechecks if all expressions are well typed and resolve to a base type
    while(es != NULL)
    {
        checkexp(es->exp);
        TYPE t = resolve(es->exp);
    }
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
    - [~] Literals
    - [~] Identifiers
    - [~] Unary exp
    - [ ] Binary exp
    - [~] Function call
    - [~] Indexing
    - [~] Field selection
    - [~] Builtins
        - [~] Append
        - [~] Capacity
        - [~] Length
    - [~] Type casting

*/

TYPE checkexp_literal(EXP * e)
{
    /*
        Check if int, float, rune or string literal by checking struct
    */

    if(e->kind != some_kind_of_literal)
    {
        //error
        exit(1);
    }

    switch(e->kind)
    {
        case tree_exp_kind_int:
            t = tree_exp_kind_int;
            break;
        case tree_exp_kind_str:
            t = tree_exp_kind_str;
            break;
        case rune:
            t = rune;
            break;
        case float:
            t = float;
            break;
    }

    return t;
}
bool checkexp_ident(EXP * e)
{
    /*
        Query symbol table to check if it is indeed an identifier
    */

    // TODO: blank identifier support

    symbol s = getsymbol(e); // triggers error if not found
    if(typeof(s) != ident) return false;

    return true;

}
void checkexp_unary(unaryop uop, EXP * e)
{
    /*
        Type of the child (`expr`) indicates type of `op expr`
        MRTE : must resolve to either
        `op expr`
        `+expr` MRTE: int, float64, rune
        `-expr` MRTE: int, float64, rune
        `!expr` MRTE: bool
        `^expr` MRTE: int, rune
    */
    // Check if expr is well typed
    checkexp(e);

    // resolve if we passed
    TYPE t = resolve(e);

    if((uop == tUNARY || uop == tNEG) && isnumeric(t))
    {
        // logical negation
    }
    else if(uop == tLNEG && t == bool)
    {
         // logical negation
    }
    else if(uop == tBNEG && (isinteger(t)))
    {
        // bitwise negation
    }
    else
    {
        fprintf(stderr, "Error: (line %d).\n", lineno);
        exit(1);
    }

    return t;

}
TYPE checkexp_binary(binaryop bop, EXP * e1, EXP * e2)
{
    /*
        Many different things going on here, refer to spec for this.
    */

    // type check both expressions
    checkexp(e1);
    checkexp(e2);

    // resolve both
    TYPE t1 = resolve(e1);
    TYPE t2 = resolve(e2);

    switch(bop)
    {
        // bool
        case TOK_OR:
            if(t1 == bool && t2 == bool) return bool;
            break;

        case TOK_AND:
            if(t1 == bool && t2 == bool) return bool;
            break;

        // comparable
        case TOK_EQ:
            if(t1 == t2) return bool;
            break;

        case TOK_NEQ:
            if(t1 == t2) return bool;
            break;

        // ordered
        case '<':
            if(t1 == t2) return bool;
            break;

        case TOK_LEQ:
            if(t1 == t2) return bool;
            break;

        case '>':
            if(t1 == t2) return bool;
            break;

        case TOK_LEQ:
            if(t1 == t2) return bool;
            break;

        // numeric or string
        case '+':

            if(isnumeric(t1) && isnumeric(t2))
                // helper function to decide "widest" type
                // rune < int < float <--- widest
                return widest(t1, t2);
                // although the result should be the "widest" type

            else if(t1 == tree_exp_kind_str && t2 == tree_exp_kind_str)
                return tree_exp_kind_str;

            break;

        // numeric
        case '-':
            if(isnumeric(t1) && isnumeric(t2))
                return widest(t1, t2);
            break;

        case '*':
            if(isnumeric(t1) && isnumeric(t2))
                return widest(t1, t2);
            break;

        case '/':
            if(isnumeric(t1) && isnumeric(t2))
                return widest(t1, t2);
            break;

        // integer
        case '%':
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case '|':
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case '&':
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case TOK_LSHIFT:
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case TOK_RSHIFT:
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case TOK_ANDNOT:
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
        case '^':
            if(isinteger(t1) && isinteger(t2))
                return widest(t1, t2);
            break;
    }

    fprintf(stderr, "Error: (line %d) invalid binary operation.\n", lineno);
    exit(1);

}
void checkexp_funccall(EXP * e, EXPS * args)
{
    /*
        expr(arg1, ... , argk) is well typed if argi are all well typed
        and have types Ti for every i, then expr is well typed with type Tr

        NOTE: init cannot be called, this should be weeded, or checked here.
    */

    // find function definition and get its prototype

    typelist protolist = prototype(e);

    TYPE a; // arg type
    TYPE p; // prototype type

    while(args != NULL && protolist != NULL){
        a = checkexp(args);
        p = checkexp(protolist);

        if(a != p)
        {
            fprintf(stderr, "Error: (line %d).\n", lineno);
            exit(1);
        }
    }

    // Check if there's a mismatch of number of args vs required args
    if(!(args == NULL && protolist == NULL))
    {
        fprintf(stderr, "Error: (line %d).\n", lineno);
        exit(1);
    }

    return rtype(e);

}
TYPE checkexp_index(EXP * e, TYPE index)
{
    /*
        expr[index] is well typed if `expr` resolves to []T or [N]T, and
        index is well typed and resolves to an int

        We can check if the index makes sense at runtime
    */

    TYPE t = resolve(e);
    TYPE i = resolve(index);

    if((t != tree_type_kind_slice && t != tree_type_kind_array) && i != tree_exp_kind_int)
    {
        fprintf(stderr, "Error: (line %d).\n", lineno);
        exit(1);
    }

    return t;
}
TYPE checkexp_field(EXP * e, field id)
{
    /*
        `expr` is well typed and has type S
        S resolves to a struct type that has a field named id

        the type of the field is the type who has the id in the struct def
    */

    TYPE s = checkexp(e);

    // find the struct that s resolves to and find the

    TYPE f = resolve(id); // ?
}
TYPE checkexp_builtin_append(EXP * e1, EXP * e2)
{
    /*
        `append(e1, e2)` is well typed if e1 is well typed and has type S and
        S resolves to []T, e2 is well typed and has type T.

        The type of append is S.
    */


    // TODO: helper function to determine if s can resolve to type t
    TYPE s = checkexp(e1);
    TYPE t = checkexp(e2);

    if(!commonresolve(s, t))
    {
        fprintf(stderr, "Error: (line %d) expression is \
            not of type slice or array.\n", lineno);
        exit(1);
    }

    return s;

}
TYPE checkexp_builtin_cap(EXP * e)
{
    /*
        `cap(expr)` is well type if `expr` is well typed and has type S and S
        resolves to []T or [N]T. The result has type int
    */

    TYPE s = checkexp(e);

    if(s != tree_type_kind_slice && s != tree_type_kind_array))
    {
        fprintf(stderr, "Error: (line %d) expression is not of \
            type slice or array.\n", lineno);
        exit(1);
    }

    return tree_exp_kind_str;

}
TYPE checkexp_builtin_len(EXP * e)
{
    /*
        `len(expr)` is well typed if its argument is well typed, has type S, and
        S resolves to a string, []T or [N]T. The result is of type int
    */
    // determines what kind of expression it is and feeds
    // into appropriate function
    TYPE s = checkexp(e); // Will trigger error if something is wrong inside

    if
    (
        s != tree_exp_kind_str      &&
        s != tree_type_kind_slice   &&
        s != tree_type_kind_array
    )
    {
        fprintf(stderr, "Error: (line %d) expression is not of \
            type string, slice or array.\n", lineno);
        exit(1);
    }

    return tree_exp_kind_str;

}
TYPE checkexp_typecast(TYPE cast, expr e)
{
    /*
        `type(expr)` is well typed if type resolves to one of the base types
        `expr` is well typed and if of a type that can be casted to type `type`
        `type` and `expr` have a common ancestral type or are numeric or
        `type` resolves to a string and `expr` resolves to an integer type
        (int or rune).
    */

    TYPE tc = resolve(cast);
    TYPE te = resolve(e);

    if(tc == te)
    {
        // resolve to same type
    }
    else if(isnumeric(tc) && isnumeric(te))
    {
        // both resolve to numeric types
    }
    else if(tc == tree_exp_kind_str && isinteger(te))
    {
        // cast is string and expr is int or rune
    }
    else
    {
        // not well typed
    }
}
