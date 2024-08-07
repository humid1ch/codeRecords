#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
using std::cout;
using std::endl;

__thread int global_value = 100;

void* startRoutine(void* args) {
    const char* name = static_cast<const char*>(args);

    while (true) {
        printf("%s: %lu global_value: %d &global_value: %p Inc: %d lwp: %ld\n", 
                name, pthread_self(), global_value, &global_value, --global_value, ::syscall(SYS_gettid));

        sleep(1);
    }

    return nullptr;
}

int main() {
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6;

    pthread_create(&tid1, nullptr, startRoutine, (void*)"thread1");
    pthread_create(&tid2, nullptr, startRoutine, (void*)"thread2");
    pthread_create(&tid3, nullptr, startRoutine, (void*)"thread3");
    pthread_create(&tid4, nullptr, startRoutine, (void*)"thread4");
    pthread_create(&tid5, nullptr, startRoutine, (void*)"thread5");
    pthread_create(&tid6, nullptr, startRoutine, (void*)"thread6");

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    pthread_join(tid3, nullptr);
    pthread_join(tid4, nullptr);
    pthread_join(tid5, nullptr);
    pthread_join(tid6, nullptr);

    return 0;
}