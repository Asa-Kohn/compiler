**I've done it for every time that supports it (I think)**

## loading and storing

### array references

        aaload arr idx
        aastore arr idx
        ---->

### array reference from an array

        aload idx
        astore idx
        ---->

    or

        aload_0 arr idx
        astore_0 arr idx
        ---->

    or

        aload_1 arr idx
        astore_1 arr idx
        ---->

    or

        aload_2 arr idx
        astore_2 arr idx
        ---->

    or

        aload_3 arr idx
        astore_3 arr idx
        ---->

### int

        iaload idx
        iastore idx
        ---->

    or

        iaload arr idx
        iastore arr idx
        ---->

    or

        iload_0
        istore_0
        ---->

    or

        iload_1
        istore_1
        ---->

    or

        iload_2
        istore_2
        ---->

    or

        iload_3
        istore_3
        ---->

### long

        laload idx
        lastore idx
        ---->

    or

        laload_0 arr idx
        lastore_0 arr idx
        ---->

    or

        lload_0
        lstore_0
        ---->

    or

        lload_1
        lstore_1
        ---->

    or

        lload_2
        lstore_2
        ---->

    or

        lload_3
        lstore_3
        ---->

### float

        faload idx
        fastore idx
        ---->

    or

        faload arr idx
        fastore arr idx
        ---->

    or

        fload_1
        fstore_1
        ---->

    or

        fload_2
        fstore_2
        ---->

    or

        fload_3
        fstore_3
        ---->

### double

        daload idx
        dastore idx
        ---->

    or

        daload arr idx
        dastore arr idx
        ---->

    or

        dload_0
        dstore_0
        ---->

    or

        dload_1
        dstore_1
        ---->

    or

        dload_2
        dstore_2
        ---->

    or

        dload_3
        dstore_3
        ---->

### byte

        baload arr idx
        bastore arr idx
        ---->

### char

        caload arr idx
        castore arr idx
        ---->

## type casting

**Note**: I realized that I made a mistake, something like `(double) (int) 0.5` to make it `0.0` is fine and should not be deleted.

# to do

## math operations

### arithmetic

#### addition

##### adding zero

###### int

        iconst_0
        iaload idx
        iadd
        ---->
        iaload idx

    or

        iconst_0
        iaload idx
        iadd
        ---->
        iaload idx

    or

        iconst_0
        iload idx
        iadd
        ---->
        iload idx

    or

        iload idx
        iconst_0
        iadd
        ---->
        iload idx

    or

        iconst_0
        iload_0
        iadd
        ---->
        iload_0

    or

        iload_0
        iconst_0
        iadd
        ---->
        iload_0

    or

        iconst_0
        iload_1
        iadd
        ---->
        iload_1

    or

        iload_1
        iconst_0
        iadd
        ---->
        iload_1

    or

        iconst_0
        iload_2
        iadd
        ---->
        iload_2

    or

        iload_2
        iconst_0
        iadd
        ---->
        iload_2

    or

        iconst_0
        iload_3
        iadd
        ---->
        iload_3

    or

        iload_3
        iconst_0
        iadd
        ---->
        iload_3

###### long

###### float

###### double

#### subtraction


#### multiplication


#### division



### bitwise

#### OR

#### AND

#### XOR

#### left shift

#### right shift

## redundant `nop`

    ...
    nop
    nop
    nop
    ...
    nop
    ---->
    nop

## empty `goto`

I'm not sure if this can happen, but in the case that it does.

    ...
    label1:
    label2:
        ...
    ...
