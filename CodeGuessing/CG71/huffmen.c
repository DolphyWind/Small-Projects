#include <stdio.h>
#include <string.h>

#include "vector.h"

typedef struct HuffmanNode {
  char c;
  int freq;
  struct HuffmanNode *left;
  struct HuffmanNode *right;
} HuffmanNode;

DEFINE_VEC(HuffmanNode);
IMPLEMENT_VEC(HuffmanNode);

void count_freqs(const char *str, size_t len, vector_HuffmanNode *out) {
  for (size_t i = 0; i < len; ++i) {
    char c = str[i];
    int found = 0;
    FORV(i, *out) {
      HuffmanNode *node = vector_HuffmanNode_atp(out, i);
      if (node->c == c) {
        ++node->freq;
        found = 1;
      }
    }

    if (!found) {
      vector_HuffmanNode_push(
          out, (HuffmanNode){.c = c, .freq = 1, .left = NULL, .right = NULL});
    }
  }
}

void free_tree(HuffmanNode *node) {
  if (node->c == '\0') {
    free_tree(node->left);
    free_tree(node->right);
  }
  free(node);
}

void swap_nodes(HuffmanNode *const a, HuffmanNode *const b) {
  HuffmanNode temp = *a;
  *a = *b;
  *b = temp;
}

void min_heapify(vector_HuffmanNode *const vec, size_t idx) {
  size_t left = 2 * idx + 1;
  size_t right = 2 * idx + 2;
  size_t smallest = idx;

  if (left < vec->len && vector_HuffmanNode_at(vec, left).freq <
                             vector_HuffmanNode_at(vec, smallest).freq)
    smallest = left;
  if (right < vec->len && vector_HuffmanNode_at(vec, right).freq <
                              vector_HuffmanNode_at(vec, smallest).freq)
    smallest = right;
  if (smallest != idx) {
    swap_nodes(vector_HuffmanNode_atp(vec, idx),
               vector_HuffmanNode_atp(vec, smallest));
    min_heapify(vec, smallest);
  }
}

void build_min_heap(vector_HuffmanNode *const vec) {
  for (int i = vec->len / 2 - 1; i >= 0; --i) {
    min_heapify(vec, i);
  }
}

void build_tree(vector_HuffmanNode *const vec, HuffmanNode *out) {
  build_min_heap(vec);
  while (vec->len > 1) {
    swap_nodes(vector_HuffmanNode_atp(vec, 0),
               vector_HuffmanNode_atp(vec, vec->len - 1));
    HuffmanNode right = vector_HuffmanNode_pop(vec);
    min_heapify(vec, 0);
    swap_nodes(vector_HuffmanNode_atp(vec, 0),
               vector_HuffmanNode_atp(vec, vec->len - 1));
    HuffmanNode left = vector_HuffmanNode_pop(vec);
    min_heapify(vec, 0);
    HuffmanNode *rightp = malloc(sizeof(HuffmanNode));
    HuffmanNode *leftp = malloc(sizeof(HuffmanNode));
    *rightp = right;
    *leftp = left;

    HuffmanNode new = (HuffmanNode){
        .c = '\0',
        .freq = left.freq + right.freq,
        .left = leftp,
        .right = rightp,
    };
    vector_HuffmanNode_push(vec, new);
    size_t i = vec->len - 1;
    while (i > 0) {
      size_t parent = (i - 1) / 2;
      if (vector_HuffmanNode_at(vec, i).freq >=
          vector_HuffmanNode_at(vec, parent).freq)
        break;
      swap_nodes(vector_HuffmanNode_atp(vec, i),
                 vector_HuffmanNode_atp(vec, parent));
      i = parent;
    }
    *out = new;
  }
}

void generate_table(HuffmanNode *const node, char **const table, char *text) {
  if (node->c != '\0') {
    size_t len = strlen(text);
    table[node->c] = malloc((len + 1) * sizeof(char));
    strncpy(table[node->c], text, len);
    table[node->c][len] = '\0';
    return;
  }

  size_t len = strlen(text);
  char *left_text = malloc((len + 2) * sizeof(char));
  char *right_text = malloc((len + 2) * sizeof(char));

  strncpy(left_text, text, len);
  strncpy(right_text, text, len);
  left_text[len] = '0';
  right_text[len] = '1';
  left_text[len + 1] = right_text[len + 1] = '\0';
  generate_table(node->left, table, left_text);
  generate_table(node->right, table, right_text);
  free(left_text);
  free(right_text);
}

void entry(char *const message, char **const out_encoded,
           char ***const out_table, size_t *out_len) {
  vector_HuffmanNode frequencies;
  vector_HuffmanNode_init(&frequencies);

  size_t message_len = strlen(message);
  count_freqs(message, message_len, &frequencies);

  HuffmanNode *tree = malloc(sizeof(HuffmanNode));
  build_tree(&frequencies, tree);

  char *text = malloc(sizeof(char));
  text = "\0";
  char *table[256] = {0};
  generate_table(tree, table, text);

  // encode
  char output[16384] = {0};
  size_t total_len = 0;
  for (size_t i = 0; i < message_len; ++i) {
    char c = message[i];
    size_t current_len = strlen(table[c]);
    strncpy(output + total_len, table[c], current_len);
    total_len += current_len;
  }
  output[total_len] = '\0';

  vector_HuffmanNode_free(&frequencies);
  *out_encoded = output;
  *out_table = table;
  *out_len = total_len;
  free_tree(tree);
}
