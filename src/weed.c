#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

#include "weed.h"

//iterate through cases
static void weed_c_stmts(STMTS *stmts)
{
    if(stmts == NULL)
        return;

    if(stmts->stmt.kind == tree_stmt_kind_continue)
    {
        fprintf(stderr,
                "Error: invalid continue statement not allowed(line %d)",
                stmts->stmt.lineno);
        exit(1);
    }

    weed_c_stmts(stmts->next);
}

static void weed_bc_cases(CASES *cases)
{
    if(cases == NULL)
        return;
    weed_c_stmts(cases->body);
    weed_bc_cases(cases->next);
}

//for weeding invalid break & continue statement
static void weed_bc_stmts(STMTS *stmts)
{
    if(stmts == NULL)
        return;

    if(stmts->stmt.kind == tree_stmt_kind_break)
    {
        fprintf(stderr,
                "Error: invalid break statement not allowed (line %d)",
                stmts->stmt.lineno);
        exit(1);
    }
    else if(stmts->stmt.kind == tree_stmt_kind_continue)
    {
        fprintf(stderr,
                "Error: invalid continue statement not allowed(line %d)",
                stmts->stmt.lineno);
        exit(1);
    }
    else if(stmts->stmt.kind == tree_stmt_kind_if)
    {
        weed_bc_stmts(stmts->stmt.ifstmt.body);
        weed_bc_stmts(stmts->stmt.ifstmt.elsebody);
    }
    else if(stmts->stmt.kind == tree_stmt_kind_switch)
        weed_bc_cases(stmts->stmt.switchstmt.cases);
    else if(stmts->stmt.kind == tree_stmt_kind_block)
        weed_bc_stmts(stmts->stmt.block);

    weed_bc_stmts(stmts->next);
}

static void weed_stmts(STMTS *stmts)
{
    if(stmts == NULL)
        return;

    if(stmts->stmt.kind == tree_stmt_kind_assign)
    {
        struct tree_exps *i = stmts->stmt.assign.left,
            *j = stmts->stmt.assign.right;
        while(i && j)
        {
            i = i->next;
            j = j->next;
        }
        if(i || j)
        {
            fprintf(stderr, "Error: assignment number mismatch on line %d\n",
                    stmts->stmt.lineno);
            exit(1);
        }
    }
    else if(stmts->stmt.kind == tree_stmt_kind_shortdecl)
    {
        struct tree_idents *i = stmts->stmt.shortdecl.idents;
        struct tree_exps *j = stmts->stmt.shortdecl.exps;
        while(i && j)
        {
            i = i->next;
            j = j->next;
        }
        if(i || j)
        {
            fprintf(stderr,
                    "Error: short declaration number mismatch on line %d\n",
                    stmts->stmt.lineno);
            exit(1);
        }
    }
    else if(stmts->stmt.kind == tree_stmt_kind_switch)
    {
        int hasdefault = 0;
        for(struct tree_cases *i = stmts->stmt.switchstmt.cases; i; i = i->next)
        {
            if(!i->val)
            {
                if(hasdefault)
                {
                    fprintf(stderr, "Error: multiple default cases in switch at\
 line %d\n", stmts->stmt.lineno);
                    exit(1);
                }
                else
                    hasdefault = 1;
            }
            weed_stmts(i->body);
        }
    }
    else if(stmts->stmt.kind == tree_stmt_kind_for)
    {
        if(stmts->stmt.forstmt.iter &&
           stmts->stmt.forstmt.iter->kind == tree_stmt_kind_shortdecl)
        {
            fprintf(stderr,
                    "Error: post statement in for loop at line %d is short decl\
aration\n", stmts->stmt.lineno);
            exit(1);
        }
        weed_stmts(stmts->stmt.forstmt.body);
    }
    else if(stmts->stmt.kind == tree_stmt_kind_block)
        weed_stmts(stmts->stmt.block);
    else if(stmts->stmt.kind == tree_stmt_kind_if)
    {
        weed_stmts(stmts->stmt.ifstmt.body);
        weed_stmts(stmts->stmt.ifstmt.elsebody);
    }

    weed_stmts(stmts->next);
}

static int hasbreak(struct tree_stmts *stmts)
{
    if(stmts->stmt.kind == tree_stmt_kind_break)
        return 1;
    if(stmts->stmt.kind == tree_stmt_kind_if)
        return hasbreak(stmts->stmt.ifstmt.body) ||
            hasbreak(stmts->stmt.ifstmt.elsebody);
    if(stmts->stmt.kind == tree_stmt_kind_block)
        return hasbreak(stmts->stmt.block);
    return 0;
}

static int isterminated(struct tree_stmts *stmts)
{
    if(!stmts)
        return 0;
    if(stmts->next)
        return isterminated(stmts->next);

    if(stmts->stmt.kind == tree_stmt_kind_return)
        return 1;
    if(stmts->stmt.kind == tree_stmt_kind_block)
        return isterminated(stmts->stmt.block);
    if(stmts->stmt.kind == tree_stmt_kind_if)
        return isterminated(stmts->stmt.ifstmt.body) &&
            isterminated(stmts->stmt.ifstmt.elsebody);
    if(stmts->stmt.kind == tree_stmt_kind_for)
        return !stmts->stmt.forstmt.condition &&
            !hasbreak(stmts->stmt.forstmt.body);
    if(stmts->stmt.kind == tree_stmt_kind_switch)
    {
        int hasdefault = 0;
        for(struct tree_cases *cases = stmts->stmt.switchstmt.cases; cases;
            cases = cases->next)
        {
            if(!isterminated(cases->body) || hasbreak(cases->body))
                return 0;
            if(!cases->val)
                hasdefault = 1;
        }
        return hasdefault;
    }
    return 0;
}

void weed(DECLS *decls)
{
    if(decls == NULL)
        return;

    if(decls->kind == tree_decls_kind_func_decl)
    {
        weed_bc_stmts(decls->func_decl.body);
        weed_stmts(decls->func_decl.body);
        if(decls->func_decl.type && !isterminated(decls->func_decl.body))
        {
            fprintf(stderr, "Error: function on line %d does not end in "
                    "terminating statement\n", decls->lineno);
            exit(1);
        }
    }

    weed(decls->next);
}
