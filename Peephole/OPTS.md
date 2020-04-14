# PeepHole Optimization
## Some insights

A lot of the code that I've come up with are quite simple,
so I'm unsure there are more complicated ones than these. Also,
many of the optimizations involve operations that are commutative, such as: `x * 0 === 0 === 0 * x`. I included this one because it was included by Professor Hendren in the original `patterns.h` file.

Nevertheless, here are a bunch of (potential) optimizations. I've organized them by what they involve. I use `===` to indicate that they are equivalent (unless I've made a mistake).

### load / store

Any kind of load and immediate store (where the local variable or a value in an array is used) is useless as there was nothing done to the variable. An example of this is:

        iload_1
        istore_1
        --------
        (empty)

This can be done for all types.

### redundant type casting

Can be done for any pair of distinct types `t, t_`. Assuming `x` is of type `t_`:

* `(t)2(t_) (t_)2(t) x === x`

This can be done somewhat recursively (I think?)

### mathematical operations

Consider any operations that are commutative. Many of the patterns can be used for `floats`, `doubles`, etc.

#### arithmetic

##### subtraction

* `x - 0 === x`
* `0 - x === -x`
* `x - 1 === iinc x -1`

##### addition (commutative)

* `x + 0 === x` and `0 + x === x`
* `x + k === iinc x k` and `k + x === iinc x k` where `k = 1, 2, 3, 4, 5`

##### multiplication (commutative)

* `x * 1 === x` and `1 * x === x`
* `x * 0 === 0` and `0 * x === 0`

I wanted to include multiplying by a power of 2 as a bit shift, but something we can discuss (similar message below).

##### division

Not sure if we have to consider division by zero and throw the error ourselves.

* `x / 1 === x`

I wanted to include something about division by a power of 2 being a bit shift for integers, but we can discuss that.

#### bitwise

##### OR `|` (commutative)

* `x | 0 === 0` and `0 | x === 0`
* `x | 1 === 1` and `1 | x === 1`
* `x | x === x`

##### AND `&` (commutative)

* `x & 0 === 0` and `0 & x === 0`
* `x & 1 === x` and `1 & x === x`
* `x & x === x`

##### XOR `^|` (commutative)

* `x ^| 0 === x` and `0 ^| x === x`
* `x ^| 1 === ~x` and `1 ^| x === ~x`
* `x ^| x === 0`

#### bit shifting

**Note**: Shifting by negative numbers

An example of this is:
`x << -k === x >> k` and `x >> -k === x << k`. This could be
done for `k = 1, 2, 3, 4, 5` since they are constants.

##### left shift `<<`

* `x << 0 = x`

##### right shift `>>`

* `x >> 0 = x`

### redundant `nop`s (not sure)

    nop
    nop
    nop
    ...
    nop
    ---
    nop    

### redundant compares

Something that I'll discuss, but not sure if we can do it as it involves a big(ger) peephole to see if something like that is there.

Not sure where I was going with this...I will think about it.

### redundant clauses in conditions
`(expr1 || expr1 bop expr2) === (expr1 bop expr2)` and similarly for `&&`. This must be translated into the appropriate bytecode instructions, which I can definitely do to help.

### empty `goto`s

        iflt ... label_t label_f
        label_t:
            (empty)
        label_f:
            ...
