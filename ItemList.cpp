
#include "ItemList.h"

ItemList::ItemList()
{
    head= nullptr;
}
ItemList::~ItemList()
{
    deleteList();
}

/****************************************************************************
* Add a new node to the end of the list.
****************************************************************************/
int ItemList::addItem(TItemRec *data)
{
    auto *newNode = new Node;
	newNode->data = data;
	newNode->next = nullptr;

    if(head !=nullptr)
    {
        Node *cur = head;
        length = 0;
        while(cur) {
            length++;
            if(cur->next == nullptr) {
                cur->next = newNode;
                length++;
                cur = nullptr;
            }
            else
                cur = cur->next;
        }
    }
    else
    {
        head = newNode;
        length = 1;
    }
    return length-1; // index of the new item
} // ItemList::addItem

/****************************************************************************
* Node at the index
****************************************************************************/
void ItemList::deleteItem(int idx)
{
    auto cur = head;

    if((idx == 0) && (head != nullptr)) {
            head = cur->next;
            delete cur->data;
            delete cur;
            if(length > 0)
        length--;
    }  // end if deleting head node
    else
    {
        int count = 1; // skip item 0
        Node *prev = head;
        cur=head->next;
        while(cur) {
            if(idx == count) {
                prev->next = cur->next;
                delete cur->data;
                delete cur;
                cur = nullptr;
                if(length > 0)
                    length--;
            }
            else{
                prev= cur;
                cur =cur->next;
                count++;
                if(!cur)
                    length = count;
            }
        } // while
    } // end else not deleting head node
} // ItemList::deleteItem

ItemList::TItemRec *ItemList::getItem(int idx)
{
    auto cur = head;
    int count = 0;
    TItemRec *rval = nullptr;

    while(cur) {
        if(idx == count) {
            rval = cur->data;
            cur = nullptr;
        }
        else{
            count++;
            cur =cur->next;
        }
    }
    return rval;
}// ItemList::getItem

void ItemList::deleteList()
{
	Node *tmpNode;
	while(head) {
		tmpNode = head;
		head = tmpNode->next;
		delete tmpNode->data;
		delete tmpNode;
	}
	length = 0;
} // ItemList::deleteList
