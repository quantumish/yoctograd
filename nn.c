#include "yoctograd.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct __attribute__((__packed__)) Neuron {
    int16_t n_in;
    Var* b;
    Var** w;
} Neuron;

Neuron* neuron_new(uint16_t n_in, bool nonlin) {
    Neuron* out = malloc(sizeof(Neuron));
    out->w = malloc(n_in * sizeof(Var*));
    for (uint16_t i = 0; i < n_in; i++) out->w[i] = v_const(.1*rand()/(float)RAND_MAX);
    out->b = v_const(0);
    out->n_in = n_in;
    if (!nonlin) out->n_in = -out->n_in;
    return out;
}

Var* neuron_forward(Neuron* n, Var** xs) {
    Var* out = v_const(0);
    for (uint16_t i = 0; i < n->n_in; i++) {
        out = v_add(out, v_mul(n->w[i], xs[i]));
    }
    out = v_add(out, n->b);
    if (n->n_in > 0) out = v_relu(out);
    return out;
}

typedef struct Layer {
    uint16_t sz;
    Neuron** neurons;
} Layer;

Layer* layer_new(uint16_t n_in, uint16_t n_out) {
    Layer* layer = malloc(sizeof(Layer));
    layer->neurons = malloc(n_out * sizeof(Neuron));
    for (uint16_t i = 0; i < n_out; i++) {
        layer->neurons[i] = neuron_new(n_in, true);
    }
    layer->sz = n_out;
    return layer;
}

Var** layer_forward(Layer* l, Var** xs) {
    Var** out = malloc(l->sz * sizeof(Var*));
    for (uint16_t i = 0; i < l->sz; i++) {
        out[i] = neuron_forward(l->neurons[i], xs);
    }
    return out;
}

typedef struct Net {
    uint16_t sz;
    float lr;
    Layer** layers;
} Net;

Net* net_new(uint16_t n_layers, float lr) {
    Net* out = malloc(sizeof(Net));
    out->layers = malloc(n_layers * sizeof(Layer*));
    out->sz = n_layers;
    out->lr = lr;
    return out;
}

void net_update(Net* n) {
    for (int i = 0; i < n->sz; i++) {
        Layer* layer = n->layers[i];
        for (int j = 0; j < layer->sz; j++) {
            Neuron* neuron = n->layers[i]->neurons[j];
            for (int k = 0; k < neuron->n_in; k++) {
                #ifdef DEBUG
                printf(
                    "Layer %d, neuron %d, weight %d has value %f and grad %f\n",
                    i, j, k,
                    neuron->w[k]->value,
                    neuron->w[k]->grad
                );
                #endif

                neuron->w[k]->value -= neuron->w[k]->grad * n->lr;
            }
            #ifdef DEBUG
            printf(
                "Layer %d, neuron %d, bias has value %f and grad %f\n",
                i, j,
                neuron->b->value,
                neuron->b->grad
            );
            #endif
            neuron->b->value -= neuron->b->grad * n->lr;
        }
    }
}

void v_zero(Var* v) {
    v->grad = 0;
    if (v->parents[0] != NULL) v_zero(v->parents[0]);
    if (v->parents[1] != NULL) v_zero(v->parents[1]);
}

Var** net_forward(Net* n, Var** inputs) {
    Var** prev = inputs;
    for (int i = 0; i < n->sz; i++) {
        prev = layer_forward(n->layers[i], prev);
    }
    return prev;
}

float read_data_line(char* line, Var** inputs) {
    int counter = 0;
    float label;
    char* pch = strtok(line, ",");
    while (pch != NULL) {
        if (counter == 4) {
            label = strtof(pch, NULL);
        } else {
            inputs[counter]->value = strtof(pch, NULL);
        }
        pch = strtok(NULL, ",");
        counter += 1;
    }
    return label;
}

int main() {
    srand(time(NULL));
    size_t EPOCHS = 100;
    float LR = 0.0001;

    Net* net = net_new(3, LR);
    net->layers[0] = layer_new(4, 8);
    net->layers[1] = layer_new(8, 8);
    net->layers[2] = layer_new(8, 1);
    
    Var** inputs = malloc(4 * sizeof(Var*));
    for (int i = 0; i < 4; i++) inputs[i] = v_const(0.0);

    size_t MAX_LINE = 64;
    FILE* train = fopen("./train.txt", "r");
    FILE* test = fopen("./test.txt", "r");
    char* line = malloc(MAX_LINE * sizeof(char));    
    for (int i = 0; i < EPOCHS; i++) {
        float total_loss = 0;
        float total_val_loss = 0;
        int train_lines = 0;
        int test_lines = 0;
        
        while (getline(&line, &MAX_LINE, train) != -1) {            
            float label = read_data_line(line, inputs);            
            Var* out = net_forward(net, inputs)[0];
            Var* err = v_add(out, v_mul(v_const(-1), v_const(label)));
            Var* loss = v_mul(err, err);
            #ifdef DEBUG
            printf("  batch | out: %f label: %f loss: %f\n", out->value, label, loss->value);
            #endif
            total_loss += loss->value;
            loss->grad = 1;
            v_back(loss);
            net_update(net);
            v_zero(loss);
            train_lines += 1;
        }
        
        while (getline(&line, &MAX_LINE, test) != -1) {            
            float label = read_data_line(line, inputs);            
            Var* out = net_forward(net, inputs)[0];
            Var* err = v_add(out, v_mul(v_const(-1), v_const(label)));
            Var* loss = v_mul(err, err);
            total_val_loss += loss->value;
            loss->grad = 1;            
            test_lines += 1;
        }

        fseek(train, 0, SEEK_SET);
        fseek(test, 0, SEEK_SET);
        printf("epoch %d/%zu: avg train loss of %f, avg val loss of %f\n", i+1, EPOCHS, total_loss/train_lines, total_val_loss/test_lines);
    }
}
