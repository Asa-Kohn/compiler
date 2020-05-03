#include "c.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "symbol.h"
#include "tree.h"
#include "type.h"

#include "base_c.h"

static void print_indent(int n)
{
    for(int i = 0; i < n; i++)
        printf("    ");
}

static void c_copy(const char *var1, const char *var2,
                   struct tree_type *type, int indent)
{
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_base)
    {
        if(r->base == tree_base_type_str)
        {
            print_indent(indent);
            printf("%s.str = estrdup(%s.str);\n", var1, var2);
        }
        else if(r->base == tree_base_type_float64)
        {
            print_indent(indent);
            printf("%s.f = %s.f;\n", var1, var2);
        }
        else
        {
            print_indent(indent);
            printf("%s.i = %s.i;\n", var1, var2);
        }
    }
    else if(r->kind == tree_type_kind_array)
    {
        print_indent(indent);
        printf("%s.a.data = emalloc(sizeof(union obj) * %d);\n",
               var1, type->array.len);
        print_indent(indent);
        printf("%s.a.len = %d;\n", var1, type->array.len);
        char *index1 = emalloc(snprintf(NULL, 0, "%s.a.data[%d]",
                                        var1, type->array.len - 1) + 1);
        char *index2 = emalloc(snprintf(NULL, 0, "%s.a.data[%d]",
                                        var2, type->array.len - 1) + 1);
        for(int i = 0; i < type->array.len; i++)
        {
            sprintf(index1, "%s.a.data[%d]", var1, i);
            sprintf(index2, "%s.a.data[%d]", var2, i);
            c_copy(index1, index2, type->array.type, indent);
        }
        free(index1);
        free(index2);
    }
    else if(r->kind == tree_type_kind_slice)
    {
        print_indent(indent);
        printf("%s.s = %s.s;\n", var1, var2);
    }
    else if(r->kind == tree_type_kind_struct)
    {
        int len = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next)
            len++;
        print_indent(indent);
        printf("%s.st = emalloc(sizeof(union obj) * %d);\n", var1, len);
        char *fieldstr1 = emalloc(snprintf(NULL, 0, "%s.st[%d]",
                                           var1, len - 1) + 1);
        char *fieldstr2 = emalloc(snprintf(NULL, 0, "%s.st[%d]",
                                           var2, len - 1) + 1);
        int i = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next, i++)
        {
            sprintf(fieldstr1, "%s.st[%d]", var1, i);
            sprintf(fieldstr2, "%s.st[%d]", var2, i);
            c_copy(fieldstr1, fieldstr2, field->type, indent);
        }
        free(fieldstr1);
        free(fieldstr2);
    }
}

static void c_eq(const char *var1, const char *var2, struct tree_type *type,
                 int invert)
{
    char c = invert ? '!' : '=';
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_base)
    {
        if(r->base == tree_base_type_str)
            printf("strcmp(%s.str, %s.str) %c= 0", var1, var2, c);
        else if(r->base == tree_base_type_float64)
            printf("%s.f %c= %s.f", var1, c, var2);
        else
            printf("%s.i %c= %s.i", var1, c, var2);
    }
    else if(r->kind == tree_type_kind_array)
    {
        if(r->array.len)
        {
            int len1 = snprintf(NULL, 0, "%s.a.data[%d]", var1, r->array.len - 1);
            int len2 = snprintf(NULL, 0, "%s.a.data[%d]", var2, r->array.len - 1);
            char *index1 = emalloc(len1 + 1);
            char *index2 = emalloc(len2 + 1);
            sprintf(index1, "%s.a.data[0]", var1);
            sprintf(index2, "%s.a.data[0]", var2);
            c_eq(index1, index2, r->array.type, invert);
            for(int i = 1; i < r->array.len; i++)
            {
                sprintf(index1, "%s.a.data[%d]", var1, i);
                sprintf(index2, "%s.a.data[%d]", var2, i);
                if(invert)
                    printf(" || ");
                else
                    printf(" && ");
                c_eq(index1, index2, r->array.type, invert);
            }
            free(index1);
            free(index2);
        }
        else
            printf("%d", !invert);
    }
    else if(r->kind == tree_type_kind_struct)
    {
        int len = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next)
            len++;
        if(len)
        {
            int len1 = snprintf(NULL, 0, "%s.st[%d]", var1, len - 1);
            int len2 = snprintf(NULL, 0, "%s.st[%d]", var2, len - 1);
            char *fieldstr1 = emalloc(len1 + 1);
            char *fieldstr2 = emalloc(len2 + 1);
            sprintf(fieldstr1, "%s.st[0]", var1);
            sprintf(fieldstr2, "%s.st[0]", var2);
            struct tree_fields *field = r->structtype.fields;
            c_eq(fieldstr1, fieldstr2, field->type, invert);
            field = field->next;
            for(int i = 1; i < len; i++, field = field->next)
            {
                sprintf(fieldstr1, "%s.st[%d]", var1, i);
                sprintf(fieldstr2, "%s.st[%d]", var2, i);
                if(invert)
                    printf(" || ");
                else
                    printf(" && ");
                c_eq(fieldstr1, fieldstr2, field->type, invert);
            }
            free(fieldstr1);
            free(fieldstr2);
        }
        else
            printf("%d", !invert);
    }
}

static void c_free_temp(int offset, struct tree_type *type, int indent);

static void c_exp(struct tree_exp *exp, int offset, int indent)
{
    if(!exp)
    {
        print_indent(indent);
        printf("stack[%d].i = 1;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_ident)
    {
        print_indent(indent);
        printf("temp = _%zd;\n", exp->ident->symbol->num);
        c_copy("temp2", "temp", exp->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp2;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_int)
    {
        print_indent(indent);
        printf("stack[%d].i = %d;\n", offset, exp->intval);
    }
    else if(exp->kind == tree_exp_kind_float)
    {
        print_indent(indent);
        printf("stack[%d].f = %f;\n", offset, exp->floatval);
    }
    else if(exp->kind == tree_exp_kind_rune)
    {
        print_indent(indent);
        printf("stack[%d].i = %d;\n", offset, exp->runeval);
    }
    else if(exp->kind == tree_exp_kind_str)
    {
        print_indent(indent);
        printf("stack[%d].str = estrdup(\"", offset);
        for(char *c =  exp->strval; *c; c++)
            printf("\\x%02x", *c);
        printf("\");\n");
    }
    else if(exp->kind == tree_exp_kind_unary)
    {
        c_exp(exp->unary.right, offset, indent);
        if(exp->unary.kind == tree_unaryexp_kind_minus)
        {
            print_indent(indent);
            if(rt(exp->type)->base == tree_base_type_float64)
                printf("stack[%d].f = -stack[%d].f;\n", offset, offset);
            else
                printf("stack[%d].i = -stack[%d].i;\n", offset, offset);
        }
        else if(exp->unary.kind == tree_unaryexp_kind_not)
        {
            print_indent(indent);
            printf("stack[%d].i = !stack[%d].i;\n", offset, offset);
        }
        else if(exp->unary.kind == tree_unaryexp_kind_comp)
        {
            print_indent(indent);
            printf("stack[%d].i = ~stack[%d].i;\n", offset, offset);
        }
    }
    else if(exp->kind == tree_exp_kind_binary)
    {
        c_exp(exp->binary.left, offset, indent);
        if(exp->binary.kind == tree_binaryexp_kind_or)
        {
            print_indent(indent);
            printf("if(!stack[%d].i)\n", offset);
            print_indent(indent);
            printf("{\n");
            c_exp(exp->binary.right, offset, indent + 1);
            print_indent(indent);
            printf("}\n");
        }
        else if(exp->binary.kind == tree_binaryexp_kind_and)
        {
            print_indent(indent);
            printf("if(stack[%d].i)\n", offset);
            print_indent(indent);
            printf("{\n");
            c_exp(exp->binary.right, offset, indent + 1);
            print_indent(indent);
            printf("}\n");
        }
        else
            c_exp(exp->binary.right, offset + 1, indent);
        struct tree_type *r = rt(exp->binary.left->type);
        if(r->kind != tree_type_kind_base)
        {
            char *var1 = emalloc(snprintf(NULL, 0, "stack[%d]", offset));
            char *var2 = emalloc(snprintf(NULL, 0, "stack[%d]", offset + 1));
            sprintf(var1, "stack[%d]", offset);
            sprintf(var2, "stack[%d]", offset + 1);
            print_indent(indent);
            printf("temp.i = ");
            c_eq(var1, var2, exp->binary.left->type,
                 exp->binary.kind == tree_binaryexp_kind_neq);
            printf(";\n");
            c_free_temp(offset + 1, r, indent);
            c_free_temp(offset, r, indent);
            print_indent(indent);
            printf("stack[%d].i = temp.i;\n", offset);
            free(var1);
            free(var2);
        }
        else if(r->base == tree_base_type_str)
        {
            if(exp->binary.kind == tree_binaryexp_kind_plus)
            {
                print_indent(indent);
                printf("temp.str = "
                       "emalloc(strlen(stack[%d].str) +"
                       " strlen(stack[%d].str) + 1);\n",
                       offset, offset + 1);
                print_indent(indent);
                printf("strcpy(temp.str, stack[%d].str);\n", offset);
                print_indent(indent);
                printf("strcat(temp.str, stack[%d].str);\n", offset + 1);
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset);
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset + 1);
                print_indent(indent);
                printf("stack[%d] = temp;\n", offset);
            }
            else
            {
                print_indent(indent);
                printf("temp.i = strcmp(stack[%d].str, stack[%d].str) ",
                       offset, offset + 1);
                if(exp->binary.kind == tree_binaryexp_kind_eq)
                    printf("==");
                else if(exp->binary.kind == tree_binaryexp_kind_neq)
                    printf("!=");
                else if(exp->binary.kind == tree_binaryexp_kind_lt)
                    printf("<");
                else if(exp->binary.kind == tree_binaryexp_kind_leq)
                    printf("<=");
                else if(exp->binary.kind == tree_binaryexp_kind_gt)
                    printf(">");
                else if(exp->binary.kind == tree_binaryexp_kind_geq)
                    printf(">=");
                printf(" 0;\n");
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset);
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset + 1);
                print_indent(indent);
                printf("stack[%d] = temp;\n", offset);
            }
        }
        else if(exp->binary.kind != tree_binaryexp_kind_or &&
                exp->binary.kind != tree_binaryexp_kind_and)
        {
            print_indent(indent);
            if(r->base == tree_base_type_float64)
                printf("stack[%d].f = stack[%d].f ", offset, offset);
            else
                printf("stack[%d].i = stack[%d].i ", offset, offset);
            if(exp->binary.kind == tree_binaryexp_kind_eq)
                printf("==");
            else if(exp->binary.kind == tree_binaryexp_kind_neq)
                printf("!=");
            else if(exp->binary.kind == tree_binaryexp_kind_lt)
                printf("<");
            else if(exp->binary.kind == tree_binaryexp_kind_leq)
                printf("<=");
            else if(exp->binary.kind == tree_binaryexp_kind_gt)
                printf(">");
            else if(exp->binary.kind == tree_binaryexp_kind_geq)
                printf(">=");
            else if(exp->binary.kind == tree_binaryexp_kind_plus)
                printf("+");
            else if(exp->binary.kind == tree_binaryexp_kind_minus)
                printf("-");
            else if(exp->binary.kind == tree_binaryexp_kind_bitor)
                printf("|");
            else if(exp->binary.kind == tree_binaryexp_kind_xor)
                printf("^");
            else if(exp->binary.kind == tree_binaryexp_kind_times)
                printf("*");
            else if(exp->binary.kind == tree_binaryexp_kind_div)
                printf("/");
            else if(exp->binary.kind == tree_binaryexp_kind_rem)
                printf("%%");
            else if(exp->binary.kind == tree_binaryexp_kind_lshift)
                printf("<<");
            else if(exp->binary.kind == tree_binaryexp_kind_rshift)
                printf(">>");
            else if(exp->binary.kind == tree_binaryexp_kind_bitand)
                printf("&");
            else if(exp->binary.kind == tree_binaryexp_kind_andnot)
                printf("&~");
            if(r->base == tree_base_type_float64)
                printf("stack[%d].f;\n", offset + 1);
            else
                printf("stack[%d].i;\n", offset + 1);
        }
    }
    else if(exp->kind == tree_exp_kind_call)
    {
        if(exp->call.func->ident->symbol->kind == symbol_kind_func)
        {
            int i = offset;
            for(struct tree_exps *exps = exp->call.exps; exps;
                exps = exps->next, i++)
                c_exp(exps->exp, i, indent);
            print_indent(indent);
            printf("temp = _%zd(", exp->call.func->ident->symbol->num);
            i = offset;
            for(struct tree_exps *exps = exp->call.exps; exps;
                exps = exps->next, i++)
            {
                printf("stack[%d]", i);
                if(exps->next)
                    printf(", ");
            }
            printf(");\n");
            i = offset;
            for(struct tree_exps *exps = exp->call.exps; exps;
                exps = exps->next, i++)
                c_free_temp(i, exp->type, indent);
            c_copy("temp2", "temp", exp->type, indent);
            print_indent(indent);
            printf("stack[%d] = temp2;\n", offset);
        }
        else
        {
            c_exp(exp->call.exps->exp, offset, indent);
            struct tree_type *r = rt(exp->call.func->ident->symbol->type);
            if(r->kind == tree_type_kind_base)
            {
                if(r->base == tree_base_type_float64)
                {
                    print_indent(indent);
                    printf("stack[%d].f = (float) stack[%d].i;\n", offset, offset);
                }
                else if(r->base == tree_base_type_str &&
                        rt(exp->call.exps->exp->type)->base !=
                        tree_base_type_str)
                {
                    print_indent(indent);
                    printf("temp = stack[%d];\n", offset);
                    print_indent(indent);
                    printf("stack[%d].str = emalloc(2);\n", offset);
                    print_indent(indent);
                    printf("stack[%d].str[0] = temp.i;\n", offset);
                    print_indent(indent);
                    printf("stack[%d].str[1] = '\\0';\n", offset);
                }
            }
        }
    }
    else if(exp->kind == tree_exp_kind_index)
    {
        c_exp(exp->index.arr, offset, indent);
        c_exp(exp->index.index, offset + 1, indent);
        print_indent(indent);
        if(exp->index.arr->type->kind == tree_type_kind_array)
            printf("if(stack[%d].i < 0 || stack[%d].i >= %d)\n",
                   offset + 1, offset + 1, exp->index.arr->type->array.len);
        else
            printf("if(stack[%d].i < 0 || stack[%d].i >= stack[%d].s.len)\n",
                   offset + 1, offset + 1, offset);
        print_indent(indent);
        printf("{\n");
        print_indent(indent + 1);
        printf("fprintf(stderr, \"Error: index out of bounds\\n\");\n");
        print_indent(indent + 1);
        printf("exit(1);\n");
        print_indent(indent);
        printf("}\n");
        print_indent(indent);
        if(exp->index.arr->type->kind == tree_type_kind_array)
            printf("temp = stack[%d].a.data[stack[%d].i];\n",
                   offset, offset + 1);
        else
            printf("temp = stack[%d].s.data->data[stack[%d].i];\n",
                   offset, offset + 1);
        c_copy("temp2", "temp", exp->type, indent);
        c_free_temp(offset, exp->index.arr->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp2;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_field)
    {
        c_exp(exp->field.instance, offset, indent);
        print_indent(indent);
        int i = 0;
        for(struct tree_fields *field =
                rt(exp->field.instance->type)->structtype.fields;
            strcmp(exp->field.field, field->name) != 0;
            field = field->next)
            i++;
        print_indent(indent);
        printf("temp = stack[%d].st[%d];\n", offset, i);
        c_copy("temp2", "temp", exp->type, indent);
        c_free_temp(offset, exp->field.instance->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp2;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_append)
    {
        c_exp(exp->append.exp1, offset, indent);
        c_exp(exp->append.exp2, offset + 1, indent);
        print_indent(indent);
        printf("temp.s = stack[%d].s;\n", offset);
        print_indent(indent);
        printf("temp.s.len++;\n");
        print_indent(indent);
        printf("if(stack[%d].s.len == stack[%d].s.cap)\n", offset, offset);
        print_indent(indent);
        printf("{\n");
        print_indent(indent + 1);
        printf("if(stack[%d].s.cap == 0)\n", offset);
        print_indent(indent + 2);
        printf("temp.s.cap = 2;\n");
        print_indent(indent + 1);
        printf("else\n");
        print_indent(indent + 2);
        printf("temp.s.cap = stack[%d].s.cap * 2;\n", offset);
        print_indent(indent + 1);
        printf("temp.s.data = emalloc(sizeof(struct array));\n");
        print_indent(indent + 1);
        printf("temp.s.data->len = temp.s.cap;\n");
        print_indent(indent + 1);
        printf("temp.s.data->refcount = 0;\n");
        print_indent(indent + 1);
        printf("temp.s.data->data = "
               "emalloc(sizeof(union obj) * temp.s.cap);\n");
        print_indent(indent + 1);
        printf("for(size_t i = 0; i < stack[%d].s.data->len; i++)\n", offset);
        print_indent(indent + 1);
        printf("{\n");
        char *var2 = emalloc(snprintf(NULL, 0, "stack[%d].s.data->data[i]",
                                      offset) + 1);
        sprintf(var2, "stack[%d].s.data->data[i]", offset);
        c_copy("temp.s.data->data[i]", var2,
               exp->append.exp1->type->slice.type, indent + 2);
        print_indent(indent + 1);
        printf("}\n");
        print_indent(indent);
        printf("}\n");
        print_indent(indent);
        printf("temp.s.data->data[temp.s.len - 1] = stack[%d];\n", offset + 1);
        c_free_temp(offset, exp->append.exp1->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_len)
    {
        c_exp(exp->exp, offset, indent);
        print_indent(indent);
        struct tree_type *r = rt(exp->exp->type);
        if(r->kind == tree_type_kind_array)
            printf("temp.i = stack[%d].a.len;\n", offset);
        else if(r->kind == tree_type_kind_slice)
            printf("temp.i = stack[%d].s.len;\n", offset);
        else
            printf("temp.i = strlen(stack[%d].str);\n", offset);
        c_free_temp(offset, exp->exp->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp;\n", offset);
    }
    else if(exp->kind == tree_exp_kind_cap)
    {
        c_exp(exp->exp, offset, indent);
        print_indent(indent);
        if(rt(exp->exp->type)->kind == tree_type_kind_array)
            printf("temp.i = stack[%d].a.len;\n", offset);
        else
            printf("temp.i = stack[%d].s.cap;\n", offset);
        c_free_temp(offset, exp->exp->type, indent);
        print_indent(indent);
        printf("stack[%d] = temp;\n", offset);
    }
}

static const char *c_get_suffix(struct tree_type *type)
{
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_base)
    {
        if(r->base == tree_base_type_float64)
            return "f";
        else if(r->base == tree_base_type_str)
            return "str";
        else
            return "i";
    }
    else if(r->kind == tree_type_kind_array)
        return "a";
    else if(r->kind == tree_type_kind_slice)
        return "s";
    else if(r->kind == tree_type_kind_struct)
        return "st";
    return "";
}

static void c_lvalue_push(struct tree_exp *exp, int offset, int indent)
{
    if(exp->kind == tree_exp_kind_index)
    {
        c_exp(exp->index.index, offset, indent);
        c_lvalue_push(exp->index.arr, offset + 1, indent);
    }
    else if(exp->kind == tree_exp_kind_field)
        c_lvalue_push(exp->field.instance, offset, indent);
}

static char *c_lvalue_build(struct tree_exp *exp, int offset, int buf_extra,
                            int indent)
{
    if(exp->kind == tree_exp_kind_ident)
    {
        char *s = emalloc(snprintf(NULL, 0, "_%zd.%s",
                                   exp->ident->symbol->num,
                                   c_get_suffix(exp->type)) + buf_extra + 1);
        sprintf(s, "_%zd.%s", exp->ident->symbol->num, c_get_suffix(exp->type));
        return s;
    }
    else if(exp->kind == tree_exp_kind_index)
    {
        char *s = c_lvalue_build(exp->index.arr, offset + 1,
                                 snprintf(NULL, 0,
                                          ".data->data[stack[%d].i].---",
                                          offset) + buf_extra, indent);
        print_indent(indent);
        if(exp->index.arr->type->kind == tree_type_kind_array)
            printf("if(stack[%d].i < 0 || stack[%d].i >= %d)\n",
                   offset, offset, exp->index.arr->type->array.len);
        else
            printf("if(stack[%d].i < 0 || stack[%d].i >= %s.len)\n",
                   offset, offset, s);
        print_indent(indent);
        printf("{\n");
        print_indent(indent + 1);
        printf("fprintf(stderr, \"Error: index out of bounds\\n\");\n");
        print_indent(indent + 1);
        printf("exit(1);\n");
        print_indent(indent);
        printf("}\n");
        struct tree_type *r = rt(exp->index.arr->type);
        if(r->kind == tree_type_kind_array)
            sprintf(s + strlen(s), ".data[stack[%d].i].%s",
                    offset, c_get_suffix(r->array.type));
        else
            sprintf(s + strlen(s), ".data->data[stack[%d].i].%s",
                    offset, c_get_suffix(r->slice.type));
        return s;
    }
    else if(exp->kind == tree_exp_kind_field)
    {
        int i = 0;
        struct tree_fields *field =
            rt(exp->field.instance->type)->structtype.fields;
        for(; strcmp(exp->field.field, field->name) != 0;
            field = field->next)
            i++;
        char *s = c_lvalue_build(exp->field.instance, offset,
                                 snprintf(NULL, 0, "[%d].%s",
                                          i, c_get_suffix(field->type)) +
                                 buf_extra, indent);
        sprintf(s + strlen(s), "[%d].%s", i, c_get_suffix(field->type));
        return s;
    }
    return NULL;
}

static void c_free(const char *var, struct tree_type *type, int indent,
                   int nest, int ref);
static void c_lvalue(struct tree_exp *exp, int offset, int indent)
{
    c_lvalue_push(exp, offset, indent);
    char *s = c_lvalue_build(exp, offset, 0, indent);
    c_free(s, exp->type, indent, 0, 1);
    print_indent(indent);
    printf("%s", s);
    free(s);
}

static void c_free(const char *var, struct tree_type *type, int indent,
                   int nest, int ref)
{
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_base)
    {
        if(r->base == tree_base_type_str)
        {
            print_indent(indent);
            printf("free(%s);\n", var);
        }
    }
    else if(r->kind == tree_type_kind_array)
    {
        char *index = emalloc(snprintf(NULL, 0, "%s.data[%d].%s",
                                       var, r->array.len - 1,
                                       c_get_suffix(r->array.type)) + 1);
        for(int i = 0; i < r->array.len; i++)
        {
            sprintf(index, "%s.data[%d].%s",
                    var, i, c_get_suffix(r->array.type));
            c_free(index, r->array.type, indent, nest, ref);
        }
        print_indent(indent);
        printf("free(%s.data);\n", var);
        free(index);
    }
    else if(r->kind == tree_type_kind_slice)
    {
        print_indent(indent);
        printf("if(");
        if(ref)
            printf("--");
        printf("%s.data->refcount == 0)\n", var);
        print_indent(indent);
        printf("{\n");
        print_indent(indent + 1);
        printf("for(int _free%d = 0; _free%d < %s.data->len; _free%d++)\n",
               nest, nest, var, nest);
        print_indent(indent + 1);
        printf("{\n");
        char *index = emalloc(snprintf(NULL, 0, "%s.data->data[_free%d].%s",
                                       var, nest, c_get_suffix(r->slice.type)) +
                              1);
        sprintf(index, "%s.data->data[_free%d].%s",
                var, nest, c_get_suffix(r->slice.type));
        c_free(index, r->slice.type, indent + 2, nest + 1, 0);
        free(index);
        print_indent(indent + 1);
        printf("}\n");
        print_indent(indent + 1);
        printf("free(%s.data->data);\n", var);
        print_indent(indent + 1);
        printf("free(%s.data);\n", var);
        print_indent(indent);
        printf("}\n");
    }
    else if(r->kind == tree_type_kind_struct)
    {
        int len = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next)
            len++;
        int i = 0;
        char *fieldstr = emalloc(snprintf(NULL, 0, "%s[%d].---", var,
                                          len - 1) + 1);
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next, i++)
        {
            sprintf(fieldstr, "%s[%d].%s", var, i, c_get_suffix(field->type));
            c_free(fieldstr, field->type, indent, nest, ref);
        }
        free(fieldstr);
        print_indent(indent);
        printf("free(%s);\n", var);
    }
}

static void c_free_temp(int offset, struct tree_type *type, int indent)
{
    if(offset == -1)
        c_free("temp", type, indent, 0, 0);
    else
    {
        char *var = emalloc(snprintf(NULL, 0, "stack[%d].%s", offset,
                                     c_get_suffix(type)) + 1);
        sprintf(var, "stack[%d].%s", offset, c_get_suffix(type));
        c_free(var, type, indent, 0, 0);
        free(var);
    }
}

static void c_increfcount(char *var, struct tree_type *type, int indent)
{
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_array)
    {
        char *index = emalloc(snprintf(NULL, 0, "%s.a.data[%d]", var,
                                       r->array.len - 1) + 1);
        for(int i = 0; i < r->array.len; i++)
        {
            sprintf(index, "%s.a.data[%d]", var, i);
            c_increfcount(index, r->array.type, indent);
        }
        free(index);
    }
    else if(r->kind == tree_type_kind_slice)
    {
        print_indent(indent);
        printf("%s.s.data->refcount++;\n", var);
    }
    else if(r->kind == tree_type_kind_struct)
    {
        int len = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next)
            len++;
        char *fieldstr = emalloc(snprintf(NULL, 0, "%s.st[%d]", var,
                                       len - 1) + 1);
        int i = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next, i++)
        {
            sprintf(fieldstr, "%s.st[%d]", var, i);
            c_increfcount(fieldstr, field->type, indent);
        }
        free(fieldstr);
    }
}

static void c_stack_increfcount(int offset, struct tree_type *type, int indent)
{
    char *var = emalloc(snprintf(NULL, 0, "stack[%d]", offset) + 1);
    sprintf(var, "stack[%d]", offset);
    c_increfcount(var, type, indent);
    free(var);
}

struct iterstate
{
    struct tree_stmt *iter;
    struct tree_exp *condition;
    int offset;
};

static void c_stmts(struct tree_stmts *stmts, int offset, int indent,
                    struct iterstate state, struct symbol_rec *symbols,
                    struct tree_func_decl *scope);
static void c_varspec(struct tree_var_spec *node, int offset, int indent);

static void c_stmt(struct tree_stmt *stmt, int offset, int indent,
                   struct iterstate state, struct symbol_rec *symbols,
                   struct tree_func_decl *scope)
{
    if(!stmt)
        return;

    if(stmt->kind == tree_stmt_kind_exp)
    {
        int i = offset;
        for(struct tree_exps *exps = stmt->expstmt.call.exps; exps;
            exps = exps->next, i++)
            c_exp(exps->exp, i, indent);
        print_indent(indent);
        printf("_%zd(", stmt->expstmt.call.func->ident->symbol->num);
        i = offset;
        for(struct tree_exps *exps = stmt->expstmt.call.exps; exps;
            exps = exps->next, i++)
        {
            printf("stack[%d]", i);
            if(exps->next)
                printf(", ");
        }
        printf(");\n");
        if(stmt->expstmt.call.func->ident->symbol->type)
            c_free_temp(-1, stmt->expstmt.call.func->ident->symbol->type,
                        indent);
    }
    else if(stmt->kind == tree_stmt_kind_block)
        c_stmts(stmt->block, offset, indent, state, symbols, scope);
    else if(stmt->kind == tree_stmt_kind_assign)
    {
        struct tree_exps *left, *right;
        int assign_offset = offset;
        for(left = stmt->assign.left, right = stmt->assign.right; left;
            left = left->next, right = right->next)
        {
            c_exp(right->exp, assign_offset, indent);
            if(left->exp->kind == tree_exp_kind_ident &&
               !left->exp->ident->symbol)
                c_free_temp(assign_offset, right->exp->type, indent);
            else
                assign_offset++;
        }
        int lvalue_offset = assign_offset;
        assign_offset = offset;
        for(left = stmt->assign.left; left; left = left->next)
            if(!(left->exp->kind == tree_exp_kind_ident &&
                 !left->exp->ident->symbol))
            {
                c_lvalue_push(left->exp, lvalue_offset, indent);
                c_stack_increfcount(assign_offset, left->exp->type, indent);
                char *s = c_lvalue_build(left->exp, lvalue_offset, 0, indent);
                c_free(s, left->exp->type, indent, 0, 1);
                print_indent(indent);
                printf("%s = stack[%d].%s;\n",
                       s, assign_offset++, c_get_suffix(left->exp->type));
                free(s);
            }
    }
    else if(stmt->kind == tree_stmt_kind_assignop)
    {
        struct tree_type *r = rt(stmt->assignop.left->type);
        if(r->base == tree_base_type_str)
        {
            c_exp(stmt->assignop.right, offset, indent);
            char *s = c_lvalue_build(stmt->assignop.left, offset + 1, 0,
                                     indent);
            print_indent(indent);
            printf("temp.str = emalloc(strlen(%s) + strlen(stack[%d]) + 1);\n",
                   s, offset);
            print_indent(indent);
            printf("strcpy(temp.str, %s);\n", s);
            print_indent(indent);
            printf("strcat(temp.str, stack[%d].str);\n", offset);
            print_indent(indent);
            printf("free(%s);\n", s);
            print_indent(indent);
            printf("free(stack[%d].str);\n", offset);
            print_indent(indent);
            printf("stack[%d] = temp;\n", offset);
        }
        else
        {
            c_exp(stmt->assignop.right, offset, indent);
            print_indent(indent);
            c_lvalue(stmt->assignop.left, offset + 1, indent);
            printf(" ");
            if(stmt->assignop.kind == tree_assignop_kind_plus)
                printf("+");
            else if(stmt->assignop.kind == tree_assignop_kind_minus)
                printf("-");
            else if(stmt->assignop.kind == tree_assignop_kind_or)
                printf("|");
            else if(stmt->assignop.kind == tree_assignop_kind_xor)
                printf("^");
            else if(stmt->assignop.kind == tree_assignop_kind_times)
                printf("*");
            else if(stmt->assignop.kind == tree_assignop_kind_div)
                printf("/");
            else if(stmt->assignop.kind == tree_assignop_kind_rem)
                printf("%%");
            else if(stmt->assignop.kind == tree_assignop_kind_lshift)
                printf("<<");
            else if(stmt->assignop.kind == tree_assignop_kind_rshift)
                printf(">>");
            else if(stmt->assignop.kind == tree_assignop_kind_rshift)
                printf("&");
            else if(stmt->assignop.kind == tree_assignop_kind_rshift)
                printf("&");
            printf("= ");
            if(stmt->assignop.kind == tree_assignop_kind_andnot)
                printf("~");
            printf("stack[%d].%c;\n",
                   offset, r->base == tree_base_type_float64 ? 'f' : 'i');
        }
    }
    else if(stmt->kind == tree_stmt_kind_shortdecl)
    {
        int sd_offset = offset;
        struct tree_idents *left;
        struct tree_exps *right;
        for(left = stmt->shortdecl.idents; left; left = left->next)
            if(left->ident->symbol && left->ident->declared)
            {
//                print_indent(indent);
//                printf("union obj _%zd;\n", left->ident->symbol->num);
            }
        for(left = stmt->shortdecl.idents, right = stmt->shortdecl.exps; left;
            left = left->next, right = right->next)
        {
            c_exp(right->exp, sd_offset, indent);
            if(left->ident->symbol)
                sd_offset++;
            else
                c_free_temp(sd_offset, right->exp->type, indent);
        }
        for(left = stmt->shortdecl.idents; left; left = left->next)
            if(left->ident->symbol)
            {
                char *var = emalloc(snprintf(NULL, 0, "_%zd",
                                             left->ident->symbol->num) + 1);
                c_free(var, left->ident->symbol->type, indent, 0, 1);
                free(var);
            }
        sd_offset = offset;
        for(left = stmt->shortdecl.idents, right = stmt->shortdecl.exps; left;
            left = left->next, right = right->next)
            if(left->ident->symbol)
            {
                c_stack_increfcount(sd_offset,
                                    left->ident->symbol->type, indent);
                print_indent(indent);
                printf("_%zd = stack[%d];\n",
                       left->ident->symbol->num, sd_offset++);
            }
    }
    else if(stmt->kind == tree_stmt_kind_inc)
    {
        c_lvalue(stmt->exp, offset, indent);
        printf("++;\n");
    }
    else if(stmt->kind == tree_stmt_kind_dec)
    {
        c_lvalue(stmt->exp, offset, indent);
        printf("--;\n");
    }
    else if(stmt->kind == tree_stmt_kind_var_decl)
        c_varspec(stmt->var_spec, offset, indent);
    else if(stmt->kind == tree_stmt_kind_print)
    {
        for(struct tree_exps *exps = stmt->exps; exps; exps = exps->next)
        {
            c_exp(exps->exp, offset, indent);
            print_indent(indent);
            struct tree_type *r = rt(exps->exp->type);
            if(r->base == tree_base_type_str)
            {
                printf("printf(\"%%s\", stack[%d].str);\n", offset);
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset);
            }
            else if(r->base == tree_base_type_float64)
                printf("printf(\"%%g\", stack[%d].f);\n", offset);
            else if(r->base == tree_base_type_bool)
                printf("printf(stack[%d].i ? \"true\" : \"false\");\n", offset);
            else
                printf("printf(\"%%d\", stack[%d].i);\n", offset);
        }
    }
    else if(stmt->kind == tree_stmt_kind_println)
    {
        for(struct tree_exps *exps = stmt->exps; exps; exps = exps->next)
        {
            c_exp(exps->exp, offset, indent);
            print_indent(indent);
            struct tree_type *r = rt(exps->exp->type);
            if(r->base == tree_base_type_str)
            {
                printf("printf(\"%%s\", stack[%d].str);\n", offset);
                print_indent(indent);
                printf("free(stack[%d].str);\n", offset);
            }
            else if(r->base == tree_base_type_float64)
                printf("printf(\"%%+e\", stack[%d].f);\n", offset);
            else if(r->base == tree_base_type_bool)
                printf("printf(stack[%d].i ? \"true\" : \"false\");\n", offset);
            else
                printf("printf(\"%%d\", stack[%d].i);\n", offset);
            if(exps->next)
            {
                print_indent(indent);
                printf("printf(\" \");\n");
            }
        }
        print_indent(indent);
        printf("printf(\"\\n\");\n");
    }
    else if(stmt->kind == tree_stmt_kind_return)
    {
        for(size_t i = 0; symbols[i].name; i++)
            if(symbols[i].scope == scope)
            {
                char *var = emalloc(snprintf(NULL, 0, "_%zd.%s",
                                             symbols[i].num,
                                             c_get_suffix(symbols[i].type)));
                sprintf(var, "_%zd.%s",
                        symbols[i].num, c_get_suffix(symbols[i].type));
                c_free(var, symbols[i].type, indent + 1, 0, 1);
                free(var);
            }
        if(stmt->exp)
        {
            c_exp(stmt->exp, offset, indent);
            print_indent(indent);
            printf("return stack[%d];\n", offset);
        }
        else
        {
            print_indent(indent);
            printf("return;\n");
        }
    }
    else if(stmt->kind == tree_stmt_kind_if)
    {
        c_stmt(stmt->ifstmt.init, offset, indent, state, symbols, scope);
        c_exp(stmt->ifstmt.condition, offset, indent);
        print_indent(indent);
        printf("if(stack[%d].i)\n", offset);
        print_indent(indent);
        printf("{\n");
        c_free_temp(offset, stmt->ifstmt.condition->type, indent + 1);
        c_stmts(stmt->ifstmt.body, offset + 1, indent + 1, state, symbols,
                scope);
        print_indent(indent);
        printf("}\n");
        print_indent(indent);
        printf("else\n");
        print_indent(indent);
        printf("{\n");
        c_free_temp(offset, stmt->ifstmt.condition->type, indent + 1);
        c_stmts(stmt->ifstmt.elsebody, offset + 1, indent + 1, state, symbols,
                scope);
        print_indent(indent);
        printf("}\n");
    }
    else if(stmt->kind == tree_stmt_kind_switch)
    {
        c_stmt(stmt->switchstmt.init, offset, indent, state, symbols, scope);
        c_exp(stmt->switchstmt.exp, offset, indent);
        int end_offset = offset + 1;
        for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
            cases = cases->next)
            for(struct tree_exps *exps = cases->val; exps; exps = exps->next)
                c_exp(exps->exp, end_offset++, indent);
        print_indent(indent);
        printf("while(1)\n");
        print_indent(indent);
        printf("{\n");
        struct tree_cases *cases = stmt->switchstmt.cases;
        if(cases)
        {
            struct tree_stmts *defaultbody = NULL;
            if(!cases->val)
            {
                defaultbody = cases->body;
                cases = cases->next;
            }
            if(cases)
            {
                print_indent(indent + 1);
                printf("if(");
                int val_offset = offset + 1;
                char *valstr = emalloc(snprintf(NULL, 0, "stack[%d]",
                                                offset) + 1);
                sprintf(valstr, "stack[%d]", offset);
                char *cmpstr = emalloc(snprintf(NULL, 0, "stack[%d]",
                                                end_offset - 1) + 1);
                sprintf(cmpstr, "stack[%d]", val_offset++);
                if(stmt->switchstmt.exp)
                    c_eq(valstr, cmpstr, stmt->switchstmt.exp->type, 0);
                else
                    printf("%s.i", cmpstr);
                for(struct tree_exps *exp = cases->val->next; exp;
                    exp = exp->next)
                {
                    printf(" || ");
                    sprintf(cmpstr, "stack[%d]", val_offset++);
                    if(stmt->switchstmt.exp)
                        c_eq(valstr, cmpstr, stmt->switchstmt.exp->type, 0);
                    else
                        printf("%s.i", cmpstr);
                }
                printf(")\n");
                print_indent(indent + 1);
                printf("{\n");
                c_stmts(cases->body, end_offset, indent + 2, state, symbols,
                        scope);
                print_indent(indent + 1);
                printf("}\n");
                for(cases = cases->next; cases; cases = cases->next)
                {
                    if(cases->val)
                    {
                        print_indent(indent + 1);
                        printf("else if(");
                        sprintf(cmpstr, "stack[%d]", val_offset++);
                        if(stmt->switchstmt.exp)
                            c_eq(valstr, cmpstr, stmt->switchstmt.exp->type, 0);
                        else
                            printf("%s.i", cmpstr);
                        for(struct tree_exps *exp = cases->val->next; exp;
                            exp = exp->next)
                        {
                            printf(" || ");
                            sprintf(cmpstr, "stack[%d]", val_offset++);
                            if(stmt->switchstmt.exp)
                                c_eq(valstr, cmpstr, stmt->switchstmt.exp->type, 0);
                            else
                                printf("%s.i", cmpstr);
                        }
                        printf(")\n");
                        print_indent(indent + 1);
                        printf("{\n");
                        c_stmts(cases->body, end_offset, indent + 2, state,
                                symbols, scope);
                        print_indent(indent + 1);
                        printf("}\n");
                    }
                    else
                        defaultbody = cases->body;
                }
                free(valstr);
                free(cmpstr);
                print_indent(indent + 1);
                printf("else\n");
                print_indent(indent + 1);
                printf("{\n");
                c_stmts(defaultbody, end_offset, indent + 2, state, symbols,
                        scope);
                print_indent(indent + 1);
                printf("}\n");
            }
            else
                c_stmts(defaultbody, end_offset, indent + 1, state, symbols,
                        scope);
        }
        int free_offset = offset;
        if(stmt->switchstmt.exp)
            c_free_temp(free_offset++, stmt->switchstmt.exp->type, indent);
        for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
            cases = cases->next)
            for(struct tree_exps *exps = cases->val; exps; exps = exps->next)
                c_free_temp(free_offset++, exps->exp->type, indent);
        print_indent(indent + 1);
        printf("break;\n");
        print_indent(indent);
        printf("}\n");
    }
    else if(stmt->kind == tree_stmt_kind_for)
    {
        c_stmt(stmt->forstmt.init, offset, indent, state, symbols, scope);
        c_exp(stmt->forstmt.condition, offset, indent);
        print_indent(indent);
        printf("while(stack[%d].i)\n", offset);
        print_indent(indent);
        printf("{\n");
        struct iterstate innerstate = {
            .iter = stmt->forstmt.iter,
            .condition = stmt->forstmt.condition,
            .offset = offset
        };
        c_stmts(stmt->forstmt.body, offset + 1, indent + 1, innerstate, symbols,
                scope);
        c_stmt(stmt->forstmt.iter, offset + 1, indent + 1,
               (struct iterstate) {0}, symbols, scope);
        c_exp(stmt->forstmt.condition, offset, indent + 1);
        print_indent(indent);
        printf("}\n");
    }
    else if(stmt->kind == tree_stmt_kind_break)
    {
        print_indent(indent);
        printf("break;\n");
    }
    else if(stmt->kind == tree_stmt_kind_continue)
    {
        if(state.iter)
            c_stmt(state.iter, offset, indent, (struct iterstate) {0}, symbols,
                   scope);
        if(state.condition)
            c_exp(state.condition, state.offset, indent);
        print_indent(indent);
        printf("continue;\n");
    }
}

static void c_stmts(struct tree_stmts *stmts, int offset, int indent,
                    struct iterstate state, struct symbol_rec *symbols,
                    struct tree_func_decl *scope)
{
    if(!stmts)
        return;

    for(struct tree_stmts *stmt = stmts; stmt; stmt = stmt->next)
        c_stmt(&stmt->stmt, offset, indent, state, symbols, scope);
}

static void c_varsinit(struct tree_var_spec *spec, int offset, int indent);

static void c_varspec(struct tree_var_spec *node, int offset, int indent)
{
    if(!node)
        return;

    for(struct tree_var_spec *spec = node; spec; spec = spec->next)
        if(spec->ident->symbol)
        {
//            print_indent(indent);
//            printf("union obj _%zd;\n", spec->ident->symbol->num);
        }
    c_varsinit(node, offset, indent);
}

static void c_zeroval(char *lval, struct tree_type *type, int indent)
{
    struct tree_type *r = rt(type);
    if(r->kind == tree_type_kind_base)
    {
        print_indent(indent);
        if(r->base == tree_base_type_str)
            printf("%s.str = zerostr();\n", lval);
        else if(r->base == tree_base_type_float64)
            printf("%s.f = 0.0;\n", lval);
        else
            printf("%s.i = 0;\n", lval);
    }
    else if(r->kind == tree_type_kind_array)
    {
        print_indent(indent);
        printf("%s.a = zeroarray(%d);\n", lval, r->array.len);
        char *index = emalloc(snprintf(NULL, 0, "%s.a.data[%d]",
                                       lval, r->array.len - 1) + 1);
        for(int i = 0; i < r->array.len; i++)
        {
            sprintf(index, "%s.a.data[%d]", lval, i);
            c_zeroval(index, r->array.type, indent);
        }
        free(index);
    }
    else if(r->kind == tree_type_kind_slice)
    {
        print_indent(indent);
        printf("%s.s = zeroslice();\n", lval);
    }
    else if(r->kind == tree_type_kind_struct)
    {
        int len = 0;
        for(struct tree_fields *field = r->structtype.fields; field;
            field = field->next)
            len++;
        print_indent(indent);
        printf("%s.st = emalloc(sizeof(union obj) * %d);\n", lval, len);
        char *fieldstr = emalloc(snprintf(NULL, 0, "%s.st[%d]", lval, len - 1) + 1);
        struct tree_fields *field = r->structtype.fields;
        for(int i = 0; i < len; i++, field = field->next)
        {
            sprintf(fieldstr, "%s.st[%d]", lval, i);
            c_zeroval(fieldstr, field->type, indent);
        }
        free(fieldstr);
    }
}

static void c_varsinit(struct tree_var_spec *spec, int offset, int indent)
{
    int vi_offset = offset;
    for(struct tree_var_spec *c = spec; c; c = c->next)
        if(c->val)
        {
            c_exp(c->val, vi_offset, indent);
            if(c->ident->symbol)
                vi_offset++;
            else
                c_free_temp(vi_offset, c->val->type, indent);
        }
    vi_offset = offset;
    for(struct tree_var_spec *c = spec; c; c = c->next)
        if(c->ident->symbol)
        {
            if(c->val)
            {
                c_stack_increfcount(vi_offset,
                                    c->ident->symbol->type, indent);
                print_indent(indent);
                printf("_%zd = stack[%d];\n", c->ident->symbol->num,
                       vi_offset++);
            }
            else
            {
                char *var = emalloc(snprintf(NULL, 0, "_%zd",
                                             c->ident->symbol->num) + 1);
                sprintf(var, "_%zd", c->ident->symbol->num);
                c_zeroval(var, c->type, indent);
                c_increfcount(var, c->ident->symbol->type, indent);
                free(var);
            }
        }
}

static int max_depth_exp(struct tree_exp *exp)
{
    if(!exp)
        return 1;

    if(exp->kind == tree_exp_kind_ident ||
       exp->kind == tree_exp_kind_int ||
       exp->kind == tree_exp_kind_float ||
       exp->kind == tree_exp_kind_rune ||
       exp->kind == tree_exp_kind_str)
        return 1;
    else if(exp->kind == tree_exp_kind_unary)
        return max_depth_exp(exp->unary.right);
    else if(exp->kind == tree_exp_kind_binary)
    {
        int x = max_depth_exp(exp->binary.left);
        int y = max_depth_exp(exp->binary.right) + 1;
        return x > y ? x : y;
    }
    else if(exp->kind == tree_exp_kind_call)
    {
        if(exp->call.func->ident->symbol->kind == symbol_kind_func)
        {
            int x = 0;
            int offset = 0;
            for(struct tree_exps *exps = exp->call.exps; exps; exps = exps->next)
            {
                int temp = max_depth_exp(exps->exp) + (offset++);
                x = temp > x ? temp : x;
            }
            return x;
        }
        else
            return max_depth_exp(exp->call.exps->exp) + 1;
    }
    else if(exp->kind == tree_exp_kind_index)
    {
        int x = max_depth_exp(exp->index.arr);
        int y = max_depth_exp(exp->index.index) + 1;
        return x > y ? x : y;
    }
    else if(exp->kind == tree_exp_kind_field)
        return max_depth_exp(exp->field.instance);
    else if(exp->kind == tree_exp_kind_append)
    {
        int x = max_depth_exp(exp->append.exp1);
        int y = max_depth_exp(exp->append.exp2) + 1;
        return x > y ? x : y;
    }
    else if(exp->kind == tree_exp_kind_len || exp->kind == tree_exp_kind_cap)
        return max_depth_exp(exp->exp);
    return 0;
}

static int max_depth_lvalue(struct tree_exp *exp)
{
    if(exp->kind == tree_exp_kind_index)
    {
        int x = max_depth_lvalue(exp->index.arr);
        int y = max_depth_exp(exp->index.index) + 1;
        return x > y ? x : y;
    }
    else if(exp->kind == tree_exp_kind_field)
        return max_depth_lvalue(exp->field.instance);
    return 0;
}

static int max_depth_stmts(struct tree_stmts *stmts);

static int max_depth_stmt(struct tree_stmt *stmt)
{
    if(!stmt)
        return 0;

    int x = 0;
    if(stmt->kind == tree_stmt_kind_exp)
        x = max_depth_exp(&stmt->expstmt);
    else if(stmt->kind == tree_stmt_kind_block)
        x = max_depth_stmts(stmt->block);
    else if(stmt->kind == tree_stmt_kind_assign)
    {
        int offset = 0;
        for(struct tree_exps *exps = stmt->assign.left; exps; exps = exps->next)
            offset += max_depth_lvalue(exps->exp);
        for(struct tree_exps *exps = stmt->assign.right; exps; exps = exps->next)
        {
            int temp = max_depth_exp(exps->exp) + (offset++);
            x = temp > x ? temp : x;
        }
    }
    else if(stmt->kind == tree_stmt_kind_assignop)
    {
        x = max_depth_lvalue(stmt->assignop.left);
        int temp = max_depth_exp(stmt->assignop.right) + 1;
        x = temp > x ? temp : x;
    }
    else if(stmt->kind == tree_stmt_kind_shortdecl)
    {
        int offset = 0;
        for(struct tree_exps *exps = stmt->shortdecl.exps; exps;
            exps = exps->next)
        {
            int temp = max_depth_exp(exps->exp) + (offset++);
            x = temp > x ? temp : x;
        }
    }
    else if(stmt->kind == tree_stmt_kind_inc ||
            stmt->kind == tree_stmt_kind_dec)
        x = max_depth_lvalue(stmt->exp);
    else if(stmt->kind == tree_stmt_kind_var_decl)
    {
        int offset = 0;
        for(struct tree_var_spec *spec = stmt->var_spec; spec;
            spec = spec->next)
            if(spec->val)
            {
                int temp = max_depth_exp(spec->val) + (offset++);
                x = temp > x ? temp : x;
            }
    }
    else if(stmt->kind == tree_stmt_kind_print ||
            stmt->kind == tree_stmt_kind_println)
        for(struct tree_exps *exps = stmt->exps; exps; exps = exps->next)
        {
            int temp = max_depth_exp(exps->exp);
            x = temp > x ? temp : x;
        }
    else if(stmt->kind == tree_stmt_kind_return)
    {
        if(stmt->exp)
            x = max_depth_exp(stmt->exp);
    }
    else if(stmt->kind == tree_stmt_kind_if)
    {
        x = max_depth_stmt(stmt->ifstmt.init);
        int temp = max_depth_exp(stmt->ifstmt.condition);
        x = temp > x ? temp : x;
        temp = max_depth_stmts(stmt->ifstmt.body) + 1;
        x = temp > x ? temp : x;
        temp = max_depth_stmts(stmt->ifstmt.elsebody) + 1;
        x = temp > x ? temp : x;
    }
    else if(stmt->kind == tree_stmt_kind_switch)
    {
        x = max_depth_stmt(stmt->switchstmt.init);
        int temp = max_depth_exp(stmt->switchstmt.exp);
        x = temp > x ? temp : x;
        int offset = 1;
        for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
            cases = cases->next)
        {
            for(struct tree_exps *exps = cases->val; exps; exps = exps->next)
            {
                temp = max_depth_exp(exps->exp) + (offset++);
                x = temp > x ? temp : x;
            }
            temp = max_depth_stmts(cases->body) + offset;
            x = temp > x ? temp : x;
        }
    }
    else if(stmt->kind == tree_stmt_kind_for)
    {
        x = max_depth_stmt(stmt->forstmt.init);
        int temp = max_depth_exp(stmt->forstmt.condition) + 1;
        x = temp > x ? temp : x;
        temp = max_depth_stmts(stmt->forstmt.body) + 2;
        x = temp > x ? temp : x;
        temp = max_depth_stmt(stmt->forstmt.iter) + 2;
        x = temp > x ? temp : x;
    }
    return x;
}

static int max_depth_stmts(struct tree_stmts *stmts)
{
    if(!stmts)
        return 0;

    int x = max_depth_stmt(&stmts->stmt);
    int y = max_depth_stmts(stmts->next);
    return x > y ? x : y;
}

static void c_funcdecl(struct tree_func_decl *node, int indent,
                       struct symbol_rec *symbols)
{
    print_indent(indent);
    if(node->type)
        printf("union obj");
    else
        printf("void");
    printf(" _%zd(", node->ident->symbol->num);
    size_t blanknum = 0;
    size_t last_param = 0;
    for(struct tree_params *param = node->params; param; param = param->next)
    {
        if(param->ident->symbol)
            printf("union obj _%zd", param->ident->symbol->num);
        else
            printf("union obj _blankparam%zd", blanknum++);
        if(param->next)
            printf(", ");
        last_param = param->ident->symbol->num;
    }
    printf(")\n");
    print_indent(indent);
    printf("{\n");
    print_indent(indent + 1);
    printf("union obj temp, temp2;\n");
    print_indent(indent + 1);
    printf("union obj stack[%d];\n", max_depth_stmts(node->body));
    for(size_t i = last_param + 1; symbols[i].name; i++)
        if(symbols[i].scope == node)
        {
            print_indent(indent + 1);
            printf("union obj _%zd = {0};\n", i);
        }
    c_stmts(node->body, 0, indent + 1, (struct iterstate) {0}, symbols, node);
    for(size_t i = 0; symbols[i].name; i++)
        if(symbols[i].scope == node)
        {
            char *var = emalloc(snprintf(NULL, 0, "_%zd.%s",
                                         symbols[i].num,
                                         c_get_suffix(symbols[i].type)));
            sprintf(var, "_%zd.%s",
                    symbols[i].num, c_get_suffix(symbols[i].type));
            c_free(var, symbols[i].type, indent + 1, 0, 1);
            free(var);
        }
    print_indent(indent);
    printf("}\n");
}

static int max_depth_decls(struct tree_decls *decls)
{
    if(!decls)
        return 0;

    int max = 0;
    if(decls->kind == tree_decls_kind_var_decl)
    {
        int offset = 0;
        for(struct tree_var_spec *spec = decls->var_spec; spec;
            spec = spec->next)
            if(spec->val)
            {
                int temp = max_depth_exp(spec->val) + (offset++);
                max = temp > max ? temp : max;
            }
    }
    int temp = max_depth_decls(decls->next);
    return temp > max ? temp : max;
}

void c_program(struct tree_decls *root, struct symbol_rec *symbols)
{
    printf("%.*s", base_c_len, base_c);

    for(size_t i = 0; symbols[i].name; i++)
        if(symbols[i].kind == symbol_kind_const)
            printf("union obj _%zd = (union obj) %d;\n",
                   symbols[i].num, symbols[i].constrec.constval);

    size_t mainfunc = 0;
    for(struct tree_decls *c = root; c; c = c->next)
        if(c->kind == tree_decls_kind_var_decl)
            for(struct tree_var_spec *spec = c->var_spec; spec;
                spec = spec->next)
                if(spec->ident->symbol)
                    printf("union obj _%zd;\n", spec->ident->symbol->num);
    printf("\n");
    for(struct tree_decls *c = root; c; c = c->next)
        if(c->kind == tree_decls_kind_func_decl && c->func_decl.ident->symbol)
        {
            printf("\n");
            c_funcdecl(&c->func_decl, 0, symbols);
            if(strcmp(c->func_decl.ident->name, "main") == 0)
                mainfunc = c->func_decl.ident->symbol->num;
        }

    printf("\nint main()\n");
    printf("{\n");
    printf("    union obj temp, temp2;\n");
    printf("    union obj stack[%d];\n", max_depth_decls(root));
    for(struct tree_decls *c = root; c; c = c->next)
        if(c->kind == tree_decls_kind_var_decl)
            c_varsinit(c->var_spec, 0, 1);
    for(size_t i = 0; symbols[i].name; i++)
        if(strcmp(symbols[i].name, "init") == 0)
            printf("    _%zd();\n", symbols[i].num);
    if(mainfunc)
        printf("    _%zd();\n", mainfunc);
    printf("}\n");
}
