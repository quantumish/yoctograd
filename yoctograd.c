#include <stdlib.h>
#include "yoctograd.h"

Var* v_new(float value, Var* child_a, Var* child_b, Op op) {
	Var* out = malloc(sizeof(Var));
	out->value = value;
	out->parents[0] = child_a;
	out->parents[1] = child_b;
	out->op = op;
	return out;
}

void v_back(Var* v) {
	if (v->op == ADD) {	
		v->parents[0]->grad += v->grad;
		v->parents[1]->grad += v->grad;
	} else if (v->op == MUL) {
		v->parents[0]->grad += v->parents[1]->value * v->grad;
		v->parents[1]->grad += v->parents[0]->value * v->grad;
	} else if (v->op == RLU) {
		v->parents[0]->grad += (v->value > 0) * v->grad;
	}
	if (v->parents[0] != NULL) v_back(v->parents[0]);
	if (v->parents[1] != NULL) v_back(v->parents[1]);
}
