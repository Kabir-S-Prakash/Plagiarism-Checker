// User function template for C++

/*

struct Node
{
    int data;
    struct Node* next;

    Node(int x){
        data = x;
        next = NULL;
    }
};

*/

class Solution {
  public:
    Node* addOne(Node* head) {
        
        Node* dummy=new Node(0);
        Node* temp=dummy;
        
        Node* cur=head;
        Node* prev=nullptr;
        while(cur!=nullptr){
           Node* next = cur->next;
            cur->next=prev;
            prev=cur;
            cur=next;
        }
        int carry=1;
        Node* tail=prev;
        while(tail!=nullptr){
            int val=tail->data+carry;
             carry=val/10;
            temp->next=new Node(val%10);
            temp=temp->next;
            tail=tail->next;
            
        }
         if (carry > 0) {
            temp->next = new Node(carry);
        }
        
         Node* cur1=dummy->next;
        Node* prev1=nullptr;
        while(cur1!=nullptr){
            Node* next = cur1->next;
            cur1->next=prev1;
            prev1=cur1;
            cur1=next;
        }
        
        
        return prev1;
        
    }
};