#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

#include "weed.h"

//for weeding invalid break & continue statement
static void weed_bc_stmts(STMTS *stmts)
{
    if(stmts == NULL)
        return;

    switch(stmts->stmt.kind)
    {
        case tree_stmt_kind_break:
            fprintf(stderr,
                    "Error: invalid break statement now allowed (line %d)",
                    stmts->stmt.lineno);
            exit(1);
        case tree_stmt_kind_continue:
            fprintf(stderr,
                    "Error: invalid continue statement now allowed(line %d)",
                    stmts->stmt.lineno);
            exit(1);
        case tree_stmt_kind_if:
            weed_bc_stmts(stmts->stmt.ifstmt->body);
            weed_bc_stmts(stmts->stmt.ifstmt->elsebody);
            break;
        case tree_stmt_kind_switch:
            weed_bc_cases(stmts->stmt.switchstmt->cases);
            break;
        case tree_stmt_kind_block:
            weed_bc_stmts(stmts->stmt.block);
            break;
    }

    weed_bc_stmts(stmts->next);
}

//iterate through cases
static void weed_bc_cases(CASES *cases)
{
    if(cases == NULL)
        return;
    weed_c_stmts(cases->body);
    weed_bc_cases(cases->next);
}

static void weed_c_stmts(STMTS *stmts)
{
    if(stmts == NULL)
        return;

    if(stmts->stmt.kind == tree_stmt_kind_continue)
    {
        fprintf(stderr,
                "Error: invalid continue statement now allowed(line %d)",
                stmts->->stmt.lineno);
        exit(1);
    }

    weed_c_stmts(stmts->next);
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
        struct tree_idents *i = stmts->stmt.assign.idents;
        struct tree_exps *j = stmts->stmt.assign.exps;
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
        if(!hasdefault)
        {
            fprintf(stderr, "Error: no default case in switch at line %d\n",
                    stmts->stmt.lineno);
            exit(1);
        }
    }
    else if(stmts->stmt.kind == tree_stmt_kind_forstmt)
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
    else if(stmts->stmt.kind == tree_stmt_kind_exp &&
            stmts->stmt.exp->kind != tree_exp_kind_call)
    {
            fprintf(stderr,
                    "Error: expression statement on line %d is not function cal\
l\n", stmts->stmt.lineno);
            exit(1);
    }
    else if(stmts->stmt.kind == tree_stmt_kind_block)
        weed_stmts(stmts->stmt.block);
    else if(stmts->stmt.kind == tree_stmt_kind_if)
    {
        weed_stmts(stmts->stmt.ifstmt.body);
        weed_stmts(stmts->stmt.ifstmt.elsebody);
    }
}

void weed(DECLS *decls)
{
    if(decls == NULL)
        return;

    if(decls->kind == tree_decls_kind_func_decl)
        weed_stmts(decls->func_decl.body);

    weed_decls(decls->next);
}
