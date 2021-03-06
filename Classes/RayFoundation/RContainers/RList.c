/**
 * RList.h
 * Realization of C dynamic
 * double-linked list, in Ray additions.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 1/23/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RList.h>
#include <RClassTable.h>

#ifdef RAY_LIST_THREAD_SAFE
    #define listMutex &object->mutex
    #define RMutexLockList() RMutexLock(listMutex)
    #define RMutexUnlockList() RMutexUnlock(listMutex)
#else
    // sets empty
    #define listMutex
    #define RMutexLockList()
    #define RMutexUnlockList()
#endif

constructor(RList)) {
    object = allocator(RList);
    if(object != nil) {
        object->head = nil;
        object->tail = object->head;

        object->count = 0;
        object->destructorDelegate = nil;
        object->printerDelegate = nil;
        object->classId = registerClassOnce(toString(RList));
#ifdef RAY_LIST_THREAD_SAFE
        object->mutex = mutexWithType(RMutexNormal); // note: be aware with normal mute
#endif
    }
    return object;
}

destructor(RList) {
    RNode *iterator = object->tail;
    RMutexLockList();
    if(iterator != nil) {
        if (object->destructorDelegate != nil) {
            for (; iterator != nil; iterator = iterator->next) {
                object->destructorDelegate(iterator->data);
                deallocator(iterator);
            }
        } else {
            for (; iterator->next != nil; iterator = iterator->next) {
                deallocator(iterator);
            }
        }
    }
    RMutexUnlockList();
}

printer(RList) {
    RNode *iterator = object->tail;
    RMutexLockList();
    RPrintf("\n%s object %p: { \n", toString(RList), object);
    RPrintf(" Count : %lu \n", object->count);
    for(; iterator != nil; iterator = iterator->next) {
        RPrintf("\t");
        if(object->printerDelegate != nil) {
            object->printerDelegate(iterator->data);
        } else {
            RPrintf("%p \n", iterator->data);
        }
    }
    RPrintf("} end of %s object %p \n\n", toString(RList), object);
    RMutexUnlockList();
}

#pragma mark Add

method(void, addHead, RList), pointer src) {
    RNode *temp = allocator(RNode);
    RMutexLockList();
    if(temp != nil) {
        temp->data = src;
        temp->next = nil;
        if (object->count == 0) {
            object->head = temp;
            object->tail = temp;
            temp->previous = nil;
        } else {
            object->head->next = temp;
            temp->previous = object->head;
            object->head = temp;
        }
        ++object->count;
    }
    RMutexUnlockList();
}

method(void, addTail, RList), pointer src) {
    RNode *temp = allocator(RNode);
    RMutexLockList();
    if(temp != nil) {
        temp->data = src;
        temp->previous = nil;
        if (object->count == 0) {
            object->head = temp;
            object->tail = temp;
            temp->next = nil;
        } else {
            object->tail->previous = temp;
            temp->next = object->tail;
            object->tail = temp;
        }
        ++object->count;
    }
    RMutexUnlockList();
}

#pragma mark Private Node At Index

method(RNode *, nodeAtIndex, RList), size_t index) {
    size_t  delta   = object->count - 1 - index;
    RNode *iterator = nil;
    RMutexLockList();
    if(delta < object->count) {
        // go from head
        if(delta < object->count / 2) {
            iterator = object->head;
            for (; delta != 0; --delta) {
                iterator = iterator->previous;
            }

        // go from tail
        } else {
            iterator = object->tail;
            for (; index != 0; --index) {
                iterator = iterator->next;
            }
        }
    } else {
        RWarning("RList. Bad index for node.", object);
    }
    RMutexUnlockList();
    return iterator;
}

#pragma mark Get

method(pointer, objectAtIndex, RList), size_t index) {
    RNode *node = $(object, m(nodeAtIndex, RList)), index);
    if(node != nil) {
        return node->data;
    }
    return nil;
}

#pragma mark Delete

method(void, deleteObjects, RList), RRange range) {
    if(range.size + range.start <= object->count) {
        RNode *iterator = nil,
              *storedHead = nil,
              *storedTail = nil;

        rbool toLeftDirection = yes;

        if(range.start > object->count / 2) {
            iterator = $(object, m(nodeAtIndex, RList)), range.start);
        } else {
            iterator = $(object, m(nodeAtIndex, RList)), range.start + range.size);
            toLeftDirection = no;
        }
        RMutexLockList();
        object->count -= range.size;

        // if moves to head ---->
        if(toLeftDirection) {
            storedTail = iterator->previous;

            if(object->destructorDelegate != nil) {
                for(; range.size != 0; --range.size) {
                    object->destructorDelegate(iterator->data);
                    storedHead = iterator->next;
                    deallocator(iterator);
                    iterator = storedHead;
                }
            } else {
                for(; range.size != 0; --range.size) {
                    storedHead = iterator->next;
                    deallocator(iterator);
                    iterator = storedHead;
                }
            }


        // if moves to tail <----
        } else {
            storedHead = iterator->next;

            if(object->destructorDelegate != nil) {
                for(; range.size != 0; --range.size) {
                    object->destructorDelegate(iterator->data);
                    storedTail = iterator->previous;
                    deallocator(iterator);
                    iterator = storedTail;
                }
            } else {
                for(; range.size != 0; --range.size) {
                    storedTail = iterator->previous;
                    deallocator(iterator);
                    iterator = storedTail;
                }
            }
        }

        // finally connect
        if(storedTail != nil) {
            storedTail->next = storedHead;
        }
        if(storedHead != nil) {
            storedHead->previous = storedTail;
        }
        RMutexUnlockList();
    } else {
        RWarning("RList. Bad range to delete.", object);
    }
}

method(void, deleteObject,  RList), size_t index) {
    RNode *node = $(object, m(nodeAtIndex, RList)), index);
    if(node != nil) {
        RMutexLockList();
        if(object->destructorDelegate != nil) {
            object->destructorDelegate(node->data);
        }
        node->next->previous = node->previous;
        node->previous->next = node->next;
        deallocator(node);
        --object->count;
        RMutexUnlockList();
    }
}

