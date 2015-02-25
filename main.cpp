#include <iostream>
#include <cstdlib>
//#include "UnorderedLinkedList.h"
#include "OrderedLinkedList.h"
#include <memory>
//#include "unistd.h"
#include "boost/thread.hpp"
using namespace std;

 orderedLinkedList<int> list1;
// nodeType<int> *fst=list1.first;


//���� ��� ������ ������ �� �������
 void print_list()

     {


         list1.print();


     }

//������� ��������
  void insert_node_test(int i)

    {


        std::cout << "Worker: running insert task" << std::endl;
        list1.insert(i);


        std::cout << "Worker: finished insert task" << std::endl;



    }
//������� �������� � ��������� ������� �������� �������
  void step_write_out_insert_test(int i)

    {


         std::cout << "Worker: running insert node" << std::endl;
         list1.insert(i);
         std::cout << "Worker: finished insert node" << std::endl;
         list1.print();


    }

 //�������� prev ����
 void delete_node_test(int i)

       {
          std::cout << "Worker: running delete node task" << std::endl;

          list1.deleteNode(i);
          std::cout << "Worker: finished delete node task" << std::endl;

       }

 int main()

    {
         //��������� �������� ������� (���� ������� �� ����������� ������ ��� ���� � ������, �� �� ��� �� ���������)
         std::cout << "main:Testing List startup" << std::endl;



 for (int i=7000;i>6000;i--)

               {

                   boost::thread workerThread(insert_node_test,i);
                   std::cout << "main: waiting for thread" <<workerThread.get_id()<< std::endl;
                   workerThread.join();
                   std::cout << "\n main: thread done "<< std::endl;


               }

          for (int i=1;i<1000;i++)

              {

                  boost::thread workerThread(insert_node_test,i);

                 std::cout << "main: waiting for thread" <<workerThread.get_id()<< std::endl;
                  workerThread.join();

                  std::cout << "\n main: thread done  " <<std::endl;


              }

           for (int i=5000;i<6000;i++)

               {

                   boost::thread workerThread(insert_node_test,i);
                   std::cout << "main: waiting for thread" <<workerThread.get_id()<< std::endl;
                   workerThread.join();
                   std::cout << "\n main: thread done "<< std::endl;


               }
                 //��������� �������� �������� �����
                 for (int i=5000;i<5200;i++)

               {

                   boost::thread workerThread(delete_node_test,i);
                   std::cout << "main: waiting for thread" <<workerThread.get_id()<< std::endl;
                   workerThread.join();
                   std::cout << "\n main: thread done "<< std::endl;


               }
                 for (int i=1;i<1000;i++)

               {

                   boost::thread workerThread(delete_node_test,i);
                   std::cout << "main: waiting for thread" <<workerThread.get_id()<< std::endl;
                   workerThread.join();
                   std::cout << "\n main: thread done "<< std::endl;


               }

         boost::thread workerThread_sec(print_list);
         workerThread_sec.join();
         return 0;

    }
