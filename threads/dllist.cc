
#include "dllist.h"
//#include <iostream>

DLLElement::DLLElement(void *itemPtr, int sortKey)//列表构造函数
{
	//currentThread->Yield();		//error
    item=itemPtr;//这是一个指针，指向的是一个线程
    //currentThread->Yield();		//error
    key=sortKey;
//    currentThread->Yield();		//5. error
    prev=NULL;
    //currentThread->Yield();		//error
    next=NULL;
    //currentThread->Yield();		//error
}

DLList::DLList()
{
    first=NULL;
    last=NULL;
}

DLList::~DLList()
{
    DLLElement *element=first;
    while(element!=NULL)
    {
        DLLElement *next=element->next;
        delete element;
        element=next;
    }
}

bool DLList::IsEmpty()
{
    if(first==NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DLList::Prepend(void *item)
{
    if(this->IsEmpty())
    {
        DLLElement *temp=new DLLElement(item,START_KEY);
        temp->prev=NULL;
        temp->next=NULL;
        first=temp;
        last=temp;
    }
    else
    {
        DLLElement *temp=new DLLElement(item,first->key-1);

        temp->prev=NULL;
        temp->next=first;
        first->prev=temp;
        first=temp;
    }
}

void DLList::Append(void *item)
{
    if(this->IsEmpty())
    {
        DLLElement *temp=new DLLElement(item,START_KEY);
        temp->prev=NULL;
        temp->next=NULL;
        first=temp;
        last=temp;
    }
    else
    {
        DLLElement *temp=new DLLElement(item,last->key+1);
        temp->prev=last;
        temp->next=NULL;
        last->next=temp;
        last=temp;
    }
}

void *DLList::Remove(int *keyPtr)//每次删除第一个节点
{
    void *RemovedItem;
    if(this->IsEmpty())
    {
    	//currentThread->Yield();
        keyPtr=NULL;
        //currentThread->Yield();
        return NULL;
    }
    else
    {
    	//currentThread->Yield();
        *keyPtr=first->key;
//        currentThread->Yield();	//4.error item key not match
        RemovedItem=first->item;   //10.删除过程被打断，导致存储信息错乱，打印信息为8号被删除两次
//        currentThread->Yield();	//error result error
        first=first->next;
        //currentThread->Yield();	//error result error
        if(first==NULL)
        {
//        	currentThread->Yield();  // 7. 没有插入完成就开始删除
            last = NULL;
            //currentThread->Yield();
        }
        else
        {
        	//currentThread->Yield();		//1.error destroy
            first->prev=NULL;
            //currentThread->Yield();		//error result error
        }
    }
    return RemovedItem;
}

void DLList::SortedInsert(void *item, int sortKey)//找到合适的位置插入节点
{
    // 5. 这里被执行了了两次，所以first被更新了两次
    if(this->IsEmpty())
    {
        DLLElement *temp=new DLLElement(item,sortKey);
        temp->prev=NULL;
        //currentThread->Yield();
        temp->next=NULL;
        //currentThread->Yield();
        first=temp;
        //currentThread->Yield();					//2.error destroy
        last=temp;
        //currentThread->Yield();
    }
    else
    {
//        currentThread->Yield();        //8. 两个线程最后插入的两个节点删除的时候打乱了从小到大的顺序
        DLLElement *temp=new DLLElement(item,sortKey);
        //currentThread->Yield();
        DLLElement *find=first;
        //currentThread->Yield();					//error destroy
        while (find!=NULL&&sortKey>=find->key)
        {
//            currentThread->Yield();				//9.第二个线程插入过程被打断，返回时候列表已经被销毁
            find=find->next;
            //currentThread->Yield();				//error destroy
        }
        if(find==NULL)		//在表尾插入
        {
        	//currentThread->Yield();
        	temp->prev=last;
        	//currentThread->Yield();			//error destroy
        	temp->next=NULL;
        	//currentThread->Yield();			//error destroy
            last->next=temp;//2.
        	//currentThread->Yield();      		//error destroy
        	last=temp;
        }
        else if(find==first)	//在表头插入
        {
        	//currentThread->Yield();
        	temp->prev=NULL;
        	//currentThread->Yield();
        	temp->next=first;
//        	currentThread->Yield();      		//3.error destroy
        	first->prev=temp;
        	//currentThread->Yield();      		//error destroy
        	first=temp;
        	//currentThread->Yield();
        }
        else
        {

//        	currentThread->Yield();				//6. error destroy
            temp->prev=find->prev;
            //currentThread->Yield();				//error destroy
            find->prev->next=temp;
            //currentThread->Yield();				//error destroy
            find->prev=temp;
            //currentThread->Yield();				//error destroy
            temp->next=find;
            //currentThread->Yield();
        }
    }
//    printf("last:%d\n",last->key);
}

void *DLList::SortedRemove(int sortKey)
{
    void *ReturnItem;
    if(this->IsEmpty())
    {
        return NULL;
    }
    else
    {
        DLLElement *find=first;
        while (find!=NULL&&sortKey!=find->key)
        {
            find=find->next;
        }
        if(find==NULL)
        {
            return NULL;
        }
        else
        {
            ReturnItem=find->item;
            if(find==first)
            {
                int *key;
                this->Remove(key);
            }
            else if(find==last)
            {
                last=last->prev;
                last->next=NULL;
                if(last==NULL)
                {
                    first = NULL;
                }
            }
            else
            {
                find->prev->next=find->next;
                find->next->prev=find->prev;
            }
            return ReturnItem;
        }
    }

}
