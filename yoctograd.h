typedef enum Op {
	NIL, ADD, MUL, RLU,
} Op;

typedef struct __attribute__((__packed__)) Var {
	float value;
	float grad;
    Op op;
	struct Var* parents[2];
} Var;

Var* v_new(float value, Var* child_a, Var* child_b, Op op);
#define v_add(a,b) v_new(a->value + b->value, a, b, ADD)
#define v_mul(a,b) v_new(a->value * b->value, a, b, MUL)
#define v_const(v) v_new(v, NULL, NULL, NIL)
#define v_relu(v) v_new((v->value > 0) ? v->value : 0, v, NULL, RLU)
void v_back(Var* v);
