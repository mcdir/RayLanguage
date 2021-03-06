/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation.h>
#include "Tests.h"

void printfInt(pointer ptr) {
    printf("%qu\n", (unsigned long long int) ptr);
}

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    RCTSingleton;
    ComplexTest();
    // place your code here

    RList *list = constructorOfRList(nil);
    list->printerDelegate = printfInt;

    forAll(iterator, 10) {
        $(list, m(addHead, RList)), (pointer) iterator);
        $(list, m(addTail, RList)), (pointer) iterator);
    }
    $(list, p(RList)));

    $(list, m(deleteObjects, RList)), makeRRange(5, 10));

    $(list, p(RList)));
    deleter(list, RList);



    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}