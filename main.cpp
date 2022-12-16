#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <algorithm>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
FILE *file_out;
std::queue<int> in_line; //очередь, в которой числа - это имена посетителей
int count_of_toserveCustomers = 10; //это количество посетителей, которые придут сегодня
int count_of_servedCustomers = 0; // количество посетителей, которых уже обслужили
bool isHairdresserSleeps; // состояние рабочего спит / не спит


void print(); // объялвение метода

void* customerCame(void* args) {
    //метод, описывающий, что происходит, когда приходит новый посетитель
    for (int i = 0 ; i < count_of_toserveCustomers; ++i) {
        int timetoWait = 1;
        srand(time(nullptr));
        timetoWait *= rand() % 5 + 1; // время через которое подходит следующий посетитель
        sleep(timetoWait);
        pthread_mutex_lock(&mutex);
        std::cout << "CUSTOMER CAME\n";
        std::cout << "QUEUE BEFORE NEW CUSTOMER:\n";
        fprintf(file_out, "CUSTOMER CAME\n");
        fprintf(file_out, "QUEUE BEFORE NEW CUSTOMER:\n");
        print();
        srand(time(NULL));
        int numberNameCustomer = rand() % 100;
        in_line.push(numberNameCustomer);
        std::cout << "QUEUE AFTER NEW CUSTOMER:\n";
        fprintf(file_out, "QUEUE AFTER NEW CUSTOMER:\n");
        print();
        pthread_mutex_unlock(&mutex);
    }
}

void* WorkerWork(void* args) {
    //метод, описывающий, что происходит с рабочим и очередью
    while(true){
        sleep(2);
        if (count_of_servedCustomers == count_of_toserveCustomers) { // если количетсов посетителей на сегодня кончилось
            std::cout << "WORK IS DONE, I GO TO SLEEP\n";
            fprintf(file_out, "WORK IS DONE, I GO TO SLEEP\n");
            break;
        } else {
            if (in_line.empty()) { // если в очереди никого нет
                std::cout << "I AM SLEEPING AND THERE ARE NO CLIENTS\n";
                fprintf(file_out, "I AM SLEEPING AND THERE ARE NO CLIENTS\n");
                isHairdresserSleeps = true;
                continue;
            } else { //если в очереди есть люди
                if (isHairdresserSleeps) {
                    std::cout << "HELLO, DEAR CUSTOMER " << in_line.front() << ", I AM WAKING UP AND GOING TO CUT YOUR HAIRCUT\n";
                    fprintf(file_out, "HELLO, DEAR CUSTOMER %d , I AM WAKING UP AND GOING TO CUT YOUR HAIRCUT\n", in_line.front());
                } else {
                    std::cout << "HELLO, DEAR CUSTOMER " << in_line.front() << "! I AM GOING TO CUT YOUR HAIRCUT\n";
                    fprintf(file_out, "HELLO, DEAR CUSTOMER %d ! I AM GOING TO CUT YOUR HAIRCUT\n", in_line.front());

                }
                pthread_mutex_lock(&mutex);
                isHairdresserSleeps = false;
                std::cout << "QUEUE BEFORE HAIRDRESSER:\n";
                fprintf(file_out, "QUEUE BEFORE HAIRDRESSER:\n");
                int time_to_serve_this_client = 1;
                srand(time(nullptr));
                time_to_serve_this_client *= rand() % 6;//сколько времени потребуется на то, чтобы подстричь одного клиента
                print();
                sleep(time_to_serve_this_client);
                in_line.pop();
                if (in_line.empty()) {
                    isHairdresserSleeps = true;
                }
                std::cout << "QUEUE AFTER HAIRDRESSER:\n";
                fprintf(file_out, "QUEUE AFTER HAIRDRESSER:\n");
                print();
                pthread_mutex_unlock(&mutex);
                ++count_of_servedCustomers;
                continue;
            }
        }
    }
}

int random_data() {
    // метод генерации рандомного числа посетителей
    srand(time(nullptr));
    int test = rand() % 15;
    return test;
}

void print() {
    //метода вывода очереди на экран
    std::queue<int> arr;
    arr = in_line;
    int customer = 0;
    if (!arr.empty()) {
        while (!arr.empty()) {
            customer = arr.front();
            std::cout << customer << "\t";
            fprintf(file_out, "%d\t", customer);
            arr.pop();
        }
        std::cout << "\n";
        fprintf(file_out, "\n");
    } else {
        std::cout << "QUEUE IS EMPTY!\n";
        fprintf(file_out, "QUEUE IS EMPTY!\n");
    }
}

int main(int argc, char* argv[]) {
    FILE *file_in = fopen("input.txt", "r");
    file_out = fopen("output.txt", "w");
    if (argc != 1) {
        if (!std::strcmp(argv[1], "-f")) {
            fscanf(file_in, "%d", &count_of_toserveCustomers);
        } else if (!std::strcmp(argv[1], "-r")) {
            FILE *file_rand = fopen("input.txt", "w");
            count_of_toserveCustomers = random_data();
            fprintf(file_rand, "%d", count_of_toserveCustomers);
            fclose(file_rand);
        } else if (!std::strcmp(argv[1], "-c")) {
            goto label;
        }
    } else {
        label:
        if (scanf("%d", &count_of_toserveCustomers) == 0){
            printf("ERROR, wrong input\n");
        }
    }
    if (count_of_toserveCustomers < 0) {
        throw std::runtime_error("Wrong input!");
    }
    pthread_mutex_init(&mutex, NULL);

    pthread_t th1;// объявляем потоки
    pthread_t th2;

    pthread_create(&th1, NULL, WorkerWork, NULL); //  создаем потоки
    pthread_create(&th2, NULL, customerCame, NULL);
    pthread_join(th1, NULL);  //  создаем последовательность потоков
    pthread_join(th2, NULL);
    return 0;
}
