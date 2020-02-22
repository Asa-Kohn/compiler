#include <stdio.h>
#include "pretty.h"

void pretty_program(PROGRAM *p) {
    // Simple statements

    // Variable declaration

    // Type declaration

    // print

    // println

    // return statement

    // if statement

    // break statement

    // for loop (various)

    // switch

    // block (of statements)

    // fallthrough?

    // continue

}

void traverse_exps(EXPS *es) {
    // Go through an expression list
    pretty_exp(es->exp);

    if(es->next != NULL) {
        traverse_exps(es->next)
    }
}

void pretty_exp(EXP *e) {
    switch(e->kind) {
        case tree_exp_kind_ident:
            printf("%s", e->ident);

        case tree_exp_kind_int:
            printf("%d", e->intval);

        case tree_exp_kind_float:
            printf("%lf", e->floatval);

        case tree_exp_kind_rune:
            printf("%c", e->runeval);

        case tree_exp_kind_str:
            printf("%s", e->strval);

        case tree_exp_kind_unary:
            pretty_unaryexp(e->unary);

        case tree_exp_kind_binary:
            pretty_binaryexp(e->binary);

        case tree_exp_kind_call:
            // assuming this will be a string, but not sure
            pretty_exp(e->call->func);
            printf("(");
            traverse_exps(e->call->exps);
            printf(")");

        case tree_exp_kind_index:
            pretty_exp(e->index->arr);
            printf("[");
            pretty_exp(e->call->index);
            printf("]");

        case tree_exp_kind_field:
            pretty_exp(e->field->instance);
            printf(".");
            printf("%s", e->field->field);

        case tree_exp_kind_append:
            printf("append(");
            pretty_exp(e->field->instance);
            printf(", ");
            pretty_exp(e->field->instance);
            printf(")");

        case tree_exp_kind_len:
            printf("len(");
            pretty_exp(e->exp);
            printf(")");

        case tree_exp_kind_cap:
            printf("cap(");
            pretty_exp(e->exp);
            printf(")");
    }
}

void pretty_unaryexp(EXP *e) {
    switch(e->kind) {
        case tree_unaryexp_kind_plus:
            printf("+(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_minus:
            printf("-(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_not:
            printf("!(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_comp:
            printf("^(");
            pretty_exp(e->right);
            printf(")");
            break;
    }

}

void pretty_binaryexp(EXP *e) {
    switch(e->kind) {
        case tree_binaryexp_kind_or:
            printf("(");
            pretty_exp(e->left);
            printf(" || ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_and:
            printf("(");
            pretty_exp(e->left);
            printf(" && ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_eq:
            printf("(");
            pretty_exp(e->left);
            printf(" == ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_neq:
            printf("(");
            pretty_exp(e->left);
            printf(" != ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_lt:
            printf("(");
            pretty_exp(e->left);
            printf(" < ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_leq:
            printf("(");
            pretty_exp(e->left);
            printf(" <= ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_gt:
            printf("(");
            pretty_exp(e->left);
            printf(" > ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_geq:
            printf("(");
            pretty_exp(e->left);
            printf(" >= ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_plus:
            printf("(");
            pretty_exp(e->left);
            printf(" + ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_minus:
            printf("(");
            pretty_exp(e->left);
            printf(" - ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_bitor:
            printf("(");
            pretty_exp(e->left);
            printf(" | ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_xor:
            printf("(");
            pretty_exp(e->left);
            printf(" ^ ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_times:
            printf("(");
            pretty_exp(e->left);
            printf(" * ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_div:
            printf("(");
            pretty_exp(e->left);
            printf(" / ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_rem:
            printf("(");
            pretty_exp(e->left);
            printf(" \% ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_lshift:
            printf("(");
            pretty_exp(e->left);
            printf(" << ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_rshift:
            printf("(");
            pretty_exp(e->left);
            printf(" >> ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_bitand:
            printf("(");
            pretty_exp(e->left);
            printf(" & ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_andnot:
            printf("(");
            pretty_exp(e->left);
            printf(" &^ ");
            pretty_exp(e->right);
            printf(")");
            break;
    }

}

void pretty_stmt(STMTS *s) {

}

void indentation(int len) {
    // Indentation of statements and nested counterparts
    byte nspace = 4;
    for(int i = 0; i < len; i++) {
        for(byte s = 0; s < nspace; s++) printf(" ");
    }
}
