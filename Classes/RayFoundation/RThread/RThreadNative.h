/**
 * RThreadNative.h
 * Simple compile-based wrapper posix and winapi thread dependency.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/16/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_THREAD_NATIVE__
#define __R_THREAD_NATIVE__

#ifndef __WIN32
    #include <pthread.h>
    typedef pthread_t                        RThreadDescriptor;
    typedef pthread_attr_t                   RThreadAttributes;
    typedef pthread_mutex_t                  RMutexDescriptor;
    typedef pthread_mutexattr_t              RMutexAttributes;

    #define RMutexInit                       pthread_mutex_init
    #define RMutexLock                       pthread_mutex_lock
    #define RMutexUnlock                     pthread_mutex_unlock
    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype

    #define RMutexRecursive                  PTHREAD_MUTEX_RECURSIVE
    #define RMutexNormal                     PTHREAD_MUTEX_NORMAL
    #define RMutexErrorCheck                 PTHREAD_MUTEX_ERRORCHECK
#else
    #include <windows.h>
    typedef HANDLE RThreadDescriptor;
    // fixme windows
    typedef LPVOID  RThreadAttributes;
    typedef HANDLE RThreadMutex;
//    #define exitThread FIXME
#endif

#endif /*__R_THREAD_NATIVE__*/