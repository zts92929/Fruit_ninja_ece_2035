//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

LLNode* create_llnode(void* data) {

    LLNode* newnode= (LLNode*)malloc(sizeof(LLNode));
    newnode->data= data;
    newnode->prev=NULL;
    newnode->next=NULL;

    return newnode;
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

int getSize(DLinkedList* dLinkedList){

  return dLinkedList->size;
}

LLNode* getHead(DLinkedList* dLinkedList){


  return dLinkedList->head;
}

LLNode* getTail(DLinkedList* dLinkedList){

  return dLinkedList->tail;
}

LLNode* getNext(LLNode* node){

  return node->next;
}

LLNode* getPrev(LLNode* node){

  return node->prev;
}

void* getData(LLNode* node){

  return node->data;
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
  if (prev_node == NULL) {
    printf("the given previous node cannot be NULL");
    return;
  }
  LLNode* temp= create_llnode(data);
  if (prev_node==dLinkedList->tail)
  {
    temp->next=NULL;
    temp->prev=dLinkedList->tail;
    (dLinkedList->tail)->next=temp;
    dLinkedList->tail=temp;
  }
  else
  {
    temp->prev=prev_node;
    temp->next=prev_node->next;
    prev_node->next=temp;
  }

dLinkedList->size++;
  return;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
  if (next_node == NULL) {
    printf("the given next node cannot be NULL");
    return;
  }
  LLNode* temp = create_llnode(data);
  if (next_node==dLinkedList->head)
  {
    temp->prev=NULL;
    temp->next=dLinkedList->head;
    (dLinkedList->head)->prev=temp;
    dLinkedList->head=temp;
  }
  else{

  temp->next=next_node;
  temp->prev=next_node->prev;
  next_node->prev=temp;

}
dLinkedList->size++;


  return;
}

void insertHead(DLinkedList* dLinkedList, void* data){
  if(dLinkedList->head == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++;
  }else{
    insertBefore(dLinkedList,dLinkedList->head, data);
  }

}

void insertTail(DLinkedList* dLinkedList, void* data){

  if(dLinkedList->tail == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++;
  }else{
    insertAfter(dLinkedList, dLinkedList->tail, data);
  }

}

void deleteNode(DLinkedList* dLinkedList, LLNode* Node){
  Node->data=NULL;
  if (dLinkedList->size==1)
  {
    dLinkedList->head=Node->next;
  }
  else if(Node->prev==NULL)
  {

    dLinkedList->head=Node->next;
  }
  else if(Node->next==NULL)
  {
    dLinkedList->tail=Node->prev;
    (dLinkedList->tail)->next=NULL;
  }
  else
  {
    (Node->prev)->next=Node->next;
    (Node->next)->prev=Node->prev;
  }
  dLinkedList->size-=1;
}

void destroyList(DLinkedList* dLinkedList){

  while(dLinkedList->size >0)
  {
    deleteNode(dLinkedList,dLinkedList->tail);
  }

}

void reverse(DLinkedList* dLinkedList)
{

  LLNode*previous=NULL;
  LLNode*current=dLinkedList->head;
  LLNode*following=dLinkedList->head;

  while(current != NULL)
  {
    following=following->next;
    current->next=previous;
    current->prev=following;
    previous=current;
    current=following;
  }

  dLinkedList->tail=dLinkedList->head;
  dLinkedList->head=previous;



}
