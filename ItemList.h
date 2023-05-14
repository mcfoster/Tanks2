#ifndef SPRITELIST_H
#define SPRITELIST_H


class ItemList
{
    public:

        struct TItemRec {
            int x, y, scrn;
            int directionIdx, color;
            int dist;
        };

        ItemList();
        ~ItemList();
        int Count() {return length;}
        int addItem(TItemRec *data);
        void deleteList();
        void deleteItem(int idx);
        TItemRec *getItem(int idx);

    protected:
        struct Node{
            TItemRec *data;
            Node *next;
        };
        int length;

    private:
        Node *head;
};

#endif // SPRITELIST_H
