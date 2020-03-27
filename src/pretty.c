#include <stdio.h>
#include "pretty.h"

int tab = 0; // global

void pretty_program(DECLS *ds);
void pretty_type_spec(TYPE_SPEC ts);
void pretty_func_decl(FUNC_DECL *fd);
void traverse_vars_spec(VAR_SPEC vs);
void traverse_vars(PARAMS * v);
void pretty_type(TYPE * t);
void pretty_slice(TYPE_SLICE tsl);
void pretty_array(TYPE_ARRAY ta);
void pretty_struct(TYPE_STRUCT tstr);
void traverse_exps(EXPS *es);
void traverse_stmts(STMTS *ss);
void pretty_exp(EXP *e);
void pretty_unaryexp(UNARYEXP *e);
void pretty_binaryexp(BINARYEXP *e);
void pretty_assignop(ASSIGNOP a);
void traverse_idents(IDENTS ids);
void pretty_shortdecl(SHORTDECL sd);
void pretty_var_spec(VAR_SPEC vs);
void traverse_names(VAR_SPEC vs);
void traverse_vals(VAR_SPEC vs);
void pretty_if(IF_STMT is);
void pretty_switch(SWITCH_STMT ss);
void pretty_cases(CASES cs);
void pretty_for(FOR_STMT fs);
void pretty_stmt(STMT *s);
void pretty_block(STMTS *b);
void pretty_assign(ASSIGN a);
void indentation();

void pretty_program(DECLS *ds) {
    if(ds == NULL) return;

    // Go through declarations
    switch(ds->kind) {
        case tree_decls_kind_package:
            printf("package %s", ds->package);
            break;
        case tree_decls_kind_var_decl:
            if(ds->var_spec != NULL) pretty_var_spec(*ds->var_spec);
            break;
        case tree_decls_kind_type_spec:
            pretty_type_spec(ds->type_spec);
            break;
        case tree_decls_kind_func_decl:
            pretty_func_decl(&ds->func_decl);
            break;
    }
    printf(";\n");
    pretty_program(ds->next);
}

void pretty_type_spec(TYPE_SPEC ts) {
    printf("type %s ", ts.ident->name);
    pretty_type(ts.type);
}

void pretty_func_decl(FUNC_DECL *fd) {
    printf("func %s(", fd->ident->name);
    if (fd->params != NULL) traverse_vars(fd->params);
    printf(") ");
    if(fd->type != NULL) {
        pretty_type(fd->type);
        printf(" ");
    }
    printf("{\n");
    tab++;
    traverse_stmts(fd->body);
    tab--;
    printf("}");
}

void traverse_vars_spec(VAR_SPEC vs) {
    printf("%s ", vs.ident->name);
    if(vs.type != NULL) pretty_type(vs.type);
    if(vs.next != NULL) {
        printf(", ");
        if(vs.next != NULL) traverse_vars_spec(*vs.next);
    }
}

void traverse_fields(VARS * f) {
    indentation();
    if(f == NULL) return;
    printf("%s ", f->name);
    pretty_type(f->type);
    if(f->next != NULL) {
        printf("\n");
        traverse_fields(f->next);
    }
}

void traverse_vars(PARAMS * v) {
    printf("%s ", v->ident->name);
    pretty_type(v->type);
    if(v->next != NULL) {
        printf(", ");
        traverse_vars(v->next);
    }
}

void traverse_fields(FIELDS * v) {
    printf("%s ", v->ident->name);
    pretty_type(v->type);
    printf(";");
    if(v->next != NULL) {
        printf("\n");
        indentation();
        traverse_fields(v->next);
    }
}

void pretty_type(TYPE * t) {
    if(t == NULL) return;
    switch(t->kind) {
        case tree_type_kind_name:
            printf("%s", t->ident->name);
            break;
        case tree_type_kind_array:
            pretty_array(t->array);
            break;
        case tree_type_kind_slice:
            pretty_slice(t->slice);
            break;
        case tree_type_kind_struct:
            printf("struct {\n");
            tab++;
            pretty_struct(t->structtype);
            tab--;
            indentation(); printf("}");
            break;
        case tree_type_kind_base:
            switch(t->base)
            {
                case tree_base_type_int:
                    printf("int");
                    break;
                case tree_base_type_float64:
                    printf("float");
                    break;
                case tree_base_type_rune:
                    printf("rune");
                    break;
                case tree_base_type_bool:
                    printf("bool");
                    break;
                case tree_base_type_str:
                    printf("string");
                    break;
            }
    }
}

void pretty_slice(TYPE_SLICE tsl) {
    printf("[]");
    pretty_type(tsl.type);
}

void pretty_array(TYPE_ARRAY ta) {
    printf("[%d]", ta.len);
    pretty_type(ta.type);
}

void pretty_struct(TYPE_STRUCT tstr) {
    indentation();
    traverse_fields(tstr.fields);
    printf("\n");
}

void traverse_exps(EXPS *es) {
    if(es == NULL) return;

    // Go through an expression list
    pretty_exp(es->exp);

    if(es->next != NULL) {
        printf(", ");
        traverse_exps(es->next);
    }
}

void traverse_stmts(STMTS *ss) {
    if(ss == NULL) return;
    // Go through an stmts
    indentation();
    pretty_stmt(&ss->stmt);
    printf(";\n");

    if(ss->next != NULL) {
        traverse_stmts(ss->next);
    }
}

void pretty_exp(EXP *e) {
    if(e == NULL) return;
    switch(e->kind) {
        case tree_exp_kind_ident:
            printf("%s", e->ident->name);
            break;

        case tree_exp_kind_int:
            printf("%d", e->intval);
            break;

        case tree_exp_kind_float:
            printf("%lf", e->floatval);
            break;

        case tree_exp_kind_rune:
            printf("%c", e->runeval);
            break;

        case tree_exp_kind_str:
            printf("\"");
            for(char * p = e->strval; *p != 0; p++)
                switch(*p){
                    case '\a':
                        printf("\\a");
                        break;
                    case '\b':
                        printf("\\b");
                        break;
                    case '\f':
                        printf("\\f");
                        break;
                    case '\n':
                        printf("\\n");
                        break;
                    case '\r':
                        printf("\\r");
                        break;
                    case '\t':
                        printf("\\t");
                        break;
                    case '\v':
                        printf("\\v");
                        break;
                    case '\\':
                        printf("\\\\");
                        break;
                    case '"':
                        printf("\"");
                        break;
                    default:
                        printf("%c", *p);
                        break;
                }
            printf("\"");
            break;

        case tree_exp_kind_unary:
            pretty_unaryexp(&e->unary);
            break;

        case tree_exp_kind_binary:
            pretty_binaryexp(&e->binary);
            break;

        case tree_exp_kind_call:
            pretty_exp(e->call.func);
            printf("(");
            traverse_exps(e->call.exps);
            printf(")");
            break;

        case tree_exp_kind_index:
            pretty_exp(e->index.arr);
            printf("[");
            pretty_exp(e->index.index);
            printf("]");
            break;

        case tree_exp_kind_field:
            pretty_exp(e->field.instance);
            printf(".");
            printf("%s", e->field.field);
            break;

        case tree_exp_kind_append:
            printf("append(");
            pretty_exp(e->append.exp1);
            printf(", ");
            pretty_exp(e->append.exp2);
            printf(")");
            break;

        case tree_exp_kind_len:
            printf("len(");
            pretty_exp(e->exp);
            printf(")");
            break;

        case tree_exp_kind_cap:
            printf("cap(");
            pretty_exp(e->exp);
            printf(")");
            break;
    }
}

void pretty_unaryexp(UNARYEXP *e) {
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

void pretty_binaryexp(BINARYEXP *e) {
    if(e == NULL) return;
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
            printf(" %% ");
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

void pretty_assignop(ASSIGNOP a) {
    pretty_exp(a.left);
    printf(" ");
    switch(a.kind) {
        case tree_assignop_kind_plus:
            printf("+");
            break;
        case tree_assignop_kind_minus:
            printf("-");
            break;
        case tree_assignop_kind_or:
            printf("|");
            break;
        case tree_assignop_kind_xor:
            printf("^");
            break;
        case tree_assignop_kind_times:
            printf("*");
            break;
        case tree_assignop_kind_div:
            printf("/");
            break;
        case tree_assignop_kind_rem:
            printf("%%");
            break;
        case tree_assignop_kind_lshift:
            printf("<<");
            break;
        case tree_assignop_kind_rshift:
            printf(">>");
            break;
        case tree_assignop_kind_and:
            printf("&");
            break;
        case tree_assignop_kind_andnot:
            printf("&^");
            break;
    }
    printf("= ");
    pretty_exp(a.right);
}

void pretty_assign(ASSIGN a) {
    traverse_exps(a.left);
    printf(" = ");
    traverse_exps(a.right);
}

void traverse_idents(IDENTS ids) {
    // Go through an idents list
    printf("%s", ids.ident->name);

    if(ids.next != NULL) {
        printf(", ");
        if(ids.next != NULL) traverse_idents(*ids.next);
    }
}

void pretty_shortdecl(SHORTDECL sd) {
    if(sd.idents != NULL) traverse_idents(*sd.idents);
    printf(" := ");
    traverse_exps(sd.exps);
}

void pretty_var_spec(VAR_SPEC vs) {
    printf("var ");
    traverse_names(vs);


    if(vs.type != NULL) {
        printf(" ");
        pretty_type(vs.type);
    }

    if(vs.val != NULL) {
        printf(" = ");
        traverse_vals(vs);
    }

}

void traverse_names(VAR_SPEC vs) {
    printf("%s", vs.ident->name);
    if(vs.next != NULL) {
        printf(", ");
        if(vs.next != NULL) traverse_names(*vs.next);
    }
}

void traverse_vals(VAR_SPEC vs) {
    pretty_exp(vs.val);
    if(vs.next != NULL) {
        printf(", ");
        if(vs.next != NULL) traverse_vals(*vs.next);
    }
}

void pretty_if(IF_STMT is) {
    printf("if ");

    // initialization
    if(is.init != NULL) {
        pretty_stmt(is.init);
        printf("; ");
    }

    // condition
    pretty_exp(is.condition);

    // body
    printf(" {\n");
    tab++;
    traverse_stmts(is.body);
    tab--;
    indentation(); printf("}");

    // optional else, else if
    if(is.elsebody != NULL) {
        printf(" else {\n");
        tab++;
        traverse_stmts(is.elsebody);
        tab--;
        indentation(); printf("}");
    }
}

void pretty_switch(SWITCH_STMT ss) {
    printf("switch ");

    // initialization
    if(ss.init != NULL) {
        pretty_stmt(ss.init);
        printf("; ");
    }

    // expression
    if(ss.exp != NULL) pretty_exp(ss.exp);

    // cases
    printf(" {\n");
    tab++;
    if(ss.cases != NULL) pretty_cases(*ss.cases);
    tab--;
    indentation(); printf("}");

}

void pretty_cases(CASES cs) {
    if(cs.val != NULL) {
        indentation();
        printf("case ");
        traverse_exps(cs.val);
    }
    else {
        printf("default");
    }

    printf(":\n");
    tab++;
    traverse_stmts(cs.body);
    tab--;

    if(cs.next != NULL) {
        if(cs.next != NULL) pretty_cases(*cs.next);
    }
}

void pretty_for(FOR_STMT fs) {
    printf("for ");

    // initialization
    if(fs.init != NULL) {
        pretty_stmt(fs.init);
    }
    printf("; ");

    // condition
    if(fs.condition != NULL) {
        pretty_exp(fs.condition);
    }
    printf("; ");

    // iter statement
    if(fs.iter != NULL) {
        pretty_stmt(fs.iter);
        printf(" ");
    }

    // body
    printf("{\n");
    tab++;
    traverse_stmts(fs.body);
    tab--;
    indentation(); printf("}");
}

void pretty_block(STMTS *b) {
    if(b == NULL) return;
    indentation(); printf("{\n");
    tab++;
    traverse_stmts(b);
    tab--;
    indentation(); printf("}");
}

void pretty_stmt(STMT *s) {
    if(s == NULL) return;
    switch(s->kind) {

        case tree_stmt_kind_exp:
            pretty_exp(&s->expstmt);
            break;

        case tree_stmt_kind_assign:
            pretty_assign(s->assign);
            break;

        case tree_stmt_kind_assignop:
            pretty_assignop(s->assignop);
            break;

        case tree_stmt_kind_block:
            pretty_block(s->block);
            break;

        case tree_stmt_kind_shortdecl:
            pretty_shortdecl(s->shortdecl);
            break;

        case tree_stmt_kind_inc:
            pretty_exp(s->exp);
            printf("++");
            break;

        case tree_stmt_kind_dec:
            pretty_exp(s->exp);
            printf("--");
            break;

        case tree_stmt_kind_var_decl:
            if(s->var_spec != NULL) pretty_var_spec(*s->var_spec);
            break;

        case tree_stmt_kind_type_spec:
            pretty_type_spec(s->type_spec);
            break;

        case tree_stmt_kind_print:
            printf("print(");
            if(s->exps != NULL) traverse_exps(s->exps);
            printf(")");
            break;

        case tree_stmt_kind_println:
            printf("println(");
            if(s->exps != NULL) traverse_exps(s->exps);
            printf(")");
            break;

        case tree_stmt_kind_return:
            printf("return ");
            pretty_exp(s->exp);
            break;

        case tree_stmt_kind_if:
            pretty_if(s->ifstmt);
            break;

        case tree_stmt_kind_switch:
            pretty_switch(s->switchstmt);
            break;

        case tree_stmt_kind_for:
            pretty_for(s->forstmt);
            break;

        case tree_stmt_kind_break:
            printf("break");
            break;

        case tree_stmt_kind_continue:
            printf("continue");
            break;

        case tree_stmt_kind_fallthrough:
            printf("fallthrough");
            break;
    }
}

void indentation() {
    // Indentation of statements and nested counterparts
    int nspace = 4;
    for(int i = 0; i < tab; i++) {
        for(int s = 0; s < nspace; s++) printf(" ");
    }
}
