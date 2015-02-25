#ifndef ORDEREDLINKEDLIST_H_INCLUDED
#define ORDEREDLINKEDLIST_H_INCLUDED
#include "LinkedListType.h"
#include <iostream>
#include <cassert>
#include <type_traits>
#include <windows.h>
using namespace std;
using namespace boost;

 template <class Type>
 class orderedLinkedList:public LinkedListType<Type>

     {

        public:
        Type insert (const Type&);
        Type deleteNode(const Type&);
        nodeType<Type>*temp;
         private:
         int compare (int one, int two);
         bool try_insert(nodeType<Type>* prev, nodeType<Type>* next, nodeType<Type>* node);
         bool try_delete(nodeType<Type>* prev, nodeType<Type>* next,nodeType<Type>* node, const Type& item);

     };


 template <class Type>
   int orderedLinkedList<Type>::compare (int one, int two)

       {
            if (one>two) return +1;
            if (one==two) return 0;
            if (two>one) return -1;
             return 0;
       }


//help ����� ��� ������ insert
 template <class Type>
 bool orderedLinkedList<Type>::try_insert(nodeType<Type>* prev, nodeType<Type>* next, nodeType<Type>* node)

 {

 nodeType<Type>* tmp;

 for (;;)

     {

         if (node->info<first->info)

              {
                  tmp=first;
                  first=node;
                  first->link.store(next, memory_order_relaxed);

                   if (first->markedAtomic.compare_exchange_strong(first->marked, false, memory_order_release))

                     if (first->link.compare_exchange_strong(next, tmp, memory_order_release))// ���� cas �������, ������ ���������
                         {
                              count++;
 node=node++;
                                     cout<<"Current inserted node's value: "<<node->info<<endl;

                              return true;
                         }


              } else {
                        // �������� �������� ���� cas ��������
                        node->link.store(next, memory_order_relaxed);
                        //������� marked ����

                         if (node->markedAtomic.compare_exchange_strong(node->marked, false, memory_order_release))

                          if (prev->link.compare_exchange_strong(next, node, memory_order_release))// ���� cas �������, ������ ���������

                              {

                                     count++;
                                     cout<<"Current inserted node's value: "<<node->info<<endl;
                                     return true;


                              }

                                // � ������ ������, ��� - �� ����������,
                                // � �� ������ ����� ����� ����� ��� �������
                                // ��������� ���������� � ��������� ���������

                                for (;;)

                                   {
                                         // ���������� ��������
                                         int cmp = compare(node->info,next->info);

                                         if (cmp > 0)

                                             {

                                                   // ���� cas �������, ������ ���������

                                                   // new 'next' ���� ���� ������ ������������ �����
                                                   // ���������� ��� ���������� ���� � �������������� ��������
                                                    prev = next;
                                                    next = prev->link.load(memory_order_consume);

                                                   if (next == 0)
                                                      break;
                                                   continue;

                                             }

                                              else if (cmp == 0)

                                                 {
                                                      // ����� ���� ��� ���� � ������

                                                      return false;


                                                 }
                                                   else /* (cmp < 0) */

                                                      {
                          // new 'next' ���� ���� ������ ��� ���� ������������ ��������
                         // ������ cas ��� ���
                                                          break;

                                                      }


                                   }


                     }

     }

 if (node->info>last->info)

      {

         tmp=last;
         last=node;
         last->link.store(next, memory_order_relaxed);
         if (last->markedAtomic.compare_exchange_strong(last->marked, false, memory_order_release))
          if (last->link.compare_exchange_strong(next, tmp, memory_order_release))
            // ���� cas �������, ������ ���������
             {

                 //last++;
                 count++;
                 return true;

             }

       }

  }
//help ������� ��� delete ������
template <class Type>
bool orderedLinkedList<Type>::try_delete(nodeType<Type>* prev, nodeType<Type>* next,nodeType<Type>* node,const Type& item)

 {
     nodeType<Type> *current,*trailCurrent;
     bool found;
     for (;;)

        {
                   //�������� �� ������
            current=first;
            found=false;
            while(current !=NULL && !found)
                if (current->info>=item)
                found=true;
                else

                       {

                           trailCurrent = current;
                           current =  current->link.load(memory_order_consume);
                           current->markedAtomic.compare_exchange_strong(current->marked, true, memory_order_release);

                       }
                       //���� ������ ��� ���� ������ � �� ����� ������ ��� ,�� ������� ��������� � ��� ��� ���� � ����� ������ �� ������
                  if (current ==NULL)

                      {

                        cout<< "The item isn't in List."<<endl;
                        return false;
                        break;


                      }

                  //���� ��� ���� ����� ������� �� �����
                  else if (current->info==item)

                    {
                      //������� ������ ������� ������
                         if (first==current)

                             {

                              current->markedAtomic.compare_exchange_strong(current->marked, true, memory_order_release);


                             }
               //������� ������ ��������
             else

                  {

                       trailCurrent->link= current->link.load(memory_order_consume);
                       if (current==last)
                       last=trailCurrent;
                       current->markedAtomic.compare_exchange_strong(current->marked, true, memory_order_release);


                 }

                return true;
                break;

                    }



         }

 }





//lock free ������� ������ �������� � ������
 template <class Type>
Type orderedLinkedList<Type>::insert (const Type& item)

 {

     nodeType<Type>  *newNode;
     newNode=new nodeType<Type>;
     newNode->info=item;
     newNode->link =NULL;
     try_insert( first, last, newNode);
     return 1;

 }


//lock free ��������  �������� �� ������
 template <class Type>
 Type orderedLinkedList<Type>::deleteNode(const Type& item)

     {

         nodeType<Type>  *node;
         try_delete( first, last,node, item);

               if (node->marked==true)

                               {
                                     cout<<"Current deleted node's value: "<<node->info<<endl;
                                     first->link.compare_exchange_strong(node, nullptr, memory_order_release);
                                     count--;

                               }





     return 1;

     }

#endif // ORDEREDLINKEDLIST_H_INCLUDED
