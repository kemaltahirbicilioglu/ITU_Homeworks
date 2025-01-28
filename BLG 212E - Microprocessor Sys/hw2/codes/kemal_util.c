#include "kemal_util.h"

int check_if_linked_list_sorted(t_list *lst){
    t_list* prev_node;
    if(lst){    // if head is not null, initialize the prev_node 
        prev_node = lst;
    }else{
        return 1; // if head is null, linked list is empty, return true
    }

    int is_sorted = 1;
    t_list* current_node = prev_node->next;
    int prev_val, current_val;
    while(current_node && is_sorted){
        prev_val = prev_node->num;
        current_val = current_node->num;
        if(current_val < prev_val){ // it means linked list is not sorted
            is_sorted = 0;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    return is_sorted;
}
