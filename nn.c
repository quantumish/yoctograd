
#include "yoctograd.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct __attribute__((__packed__)) Neuron {
    int16_t n_in;
    Var* b;
    Var** w;    
} Neuron;

Neuron* neuron_new(int n_in, bool nonlin) {
    Neuron* out = malloc(sizeof(Neuron));
    out->w = malloc(n_in * sizeof(Var*));
    for (int i = 0; i < n_in; i++) out->w[i] = v_const(0);
    out->b = v_const(0);
    out->n_in = n_in;
    if (!nonlin) out->n_in = -out->n_in;
    return out;
}

Var* neuron_forward(Neuron* n, float* xs) {
    Var* out = v_const(0);
    for (int i = 0; i < n->n_in; i++) {
	out = v_add(out, v_mul(n->w[i], v_const(xs[i])));
    }
    out = v_add(out, n->b);
    // if (n->n_in > 0) out = v_relu(out);
    return out;
}

typedef struct Layer {
    uint16_t sz;
    Neuron** neurons;
} Layer;

Layer* layer_new(int n_in, int n_out) {
    Layer* layer = malloc(sizeof(Layer));
    layer->neurons = malloc(n_out * sizeof(Neuron));
    for (int i = 0; i < n_out; i++) {
	layer->neurons[i] = neuron_new(n_in, true);
    }
    layer->sz = n_out;
    return layer;
}

Var** layer_forward(Layer* l, float* xs) {
    Var** out = malloc(l->sz * sizeof(Var*));
    for (int i = 0; i < l->sz; i++) {
	out[i] = neuron_forward(l->neurons[i], xs);
    }
    return out;
}

int main() {
    Layer* l1 = layer_new(4, 8);
    // Layer* l2 = layer_new(8, 8);
    // Layer* l3 = layer_new(8, 1);
    float* inputs = malloc(4 * sizeof(float));
    inputs[0] = 1.0;
    inputs[1] = 2.0;
    inputs[2] = 3.0;
    inputs[3] = 4.0;
    Var* out = layer_forward(l1, inputs)[0];
    Var* loss = v_add(out, v_const(-1));
    loss->grad =1;
    v_back(loss);
    printf("%p %p\n", loss->parents[0], out);
    printf("%p %p\n", out->parents[0], out);    
    printf("%f\n", l1->neurons[0]->b->grad);
    printf("%f\n", out->parents[0]->parents[0]->grad);
}
