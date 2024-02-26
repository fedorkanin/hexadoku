#include "MergeSort.h"

// get middle of linked list
Node* getMiddle(Node* head) {
    if (!head) return head;
    Node* slow = head;
    Node* fast = head->right;
    while (fast) {
        fast = fast->right;
        if (fast) {
            slow = slow->right;
            fast = fast->right;
        }
    }
    return slow;
}

Node* merge(Node* first, Node* second, bool (*comparator)(Node*, Node*)) {
    if (!first) return second;
    if (!second) return first;

    if (comparator(first, second)) {
        first->right       = merge(first->right, second, comparator);
        first->right->left = first;
        first->left        = NULL;
        return first;
    } else {
        second->right       = merge(first, second->right, comparator);
        second->right->left = second;
        second->left        = NULL;
        return second;
    }
}

Node* mergeSort(Node* head, bool (*comparator)(Node*, Node*)) {
    if (!head || !head->right) return head;

    // split list in two halves
    Node* middle  = getMiddle(head);
    Node* second  = middle->right;
    middle->right = NULL;

    // sort halves
    Node* left  = mergeSort(head, comparator);
    Node* right = mergeSort(second, comparator);

    // merge halves
    return merge(left, right, comparator);
}
