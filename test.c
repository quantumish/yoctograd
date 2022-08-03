#include <stdio.h>
#include "yoctograd.h"

int main() {
    Var* a = v_const(-4);
    Var* b = v_const(2);
    Var* c = v_mul(a, b);
    Var* d = v_add(v_mul(c, a), v_const(2));
    d->grad = 1;
    v_back(d);
    printf("Var sizeof: %zu\n", sizeof(Var));
    printf("a grad: %f\n", a->grad);
    printf("b grad: %f\n", b->grad);
    printf("c grad: %f\n", c->grad);
    printf("d grad: %f\n", d->grad);    
}
