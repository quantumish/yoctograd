#include <stdio.h>
#include "yoctograd.h"

extern int test(void);

int main() {
    Var* a = v_new(1.5, NULL, NULL, ADD);
    printf("%p\n", a);
    printf("Var { val=%f, grad=%f, op=%d, parents = {%p, %p} }\n",
        a->value, a->grad, a->op, a->parents[0], a->parents[1]    
    );
    printf("Var { val=%f, grad=%f, op=%d, parents = {%p, %p} }\n",
        *(float*)(a), *(float*)(a+4), *(int*)(a+8), *(int**)(a+12), *(int**)(a+20)    
    );
    // Var* a = v_const(-4);
    // Var* b = v_const(2);
    // Var* c = v_mul(a, b);
    // Var* d = v_add(v_mul(c, a), v_const(2));
    // d->grad = 1;
    // v_back(d);
    // printf("Var sizeof: %zu\n", sizeof(Var));
    // printf("a grad: %f\n", a->grad);
    // printf("b grad: %f\n", b->grad);
    // printf("c grad: %f\n", c->grad);
    // printf("d grad: %f\n", d->grad);    
}
