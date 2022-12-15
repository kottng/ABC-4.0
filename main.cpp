#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <queue>
#include <algorithm>
#include <mutex>
#include <cstring>

FILE *file_out = fopen("output.txt", "w");
std::queue<int> in_line; //очередь, в которой числа - это имена посетителей
int count_of_toserveCustomers = 10; //это количество посетителей, которые придут сегодня
int count_of_servedCustomers = 0; // количество посетителей, которых уже обслужили
std::mutex mutex; // мьютекс (двоичный семафор)
bool isHairdresserSleeps; // состояние рабочего спит / не спит


void print(); // объялвение метода

void customerCame() {
    //метод, описывающий, что происходит, когда приходит новый посетитель
    for (int i = 0 ; i < count_of_toserveCustomers; ++i) {
        int timetoWait = 500;
        srand(time(nullptr));
        timetoWait *= rand() % 5 + 1; // время через которое подходит следующий посетитель
        std::this_thread::sleep_for(std::chrono::milliseconds(timetoWait));
        mutex.lock();
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
        mutex.unlock();
    }
}

void WorkerWork() {
    //метод, описывающий, что происходит с рабочим и очередью
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
                mutex.lock();
                isHairdresserSleeps = false;
                std::cout << "QUEUE BEFORE HAIRDRESSER:\n";
                fprintf(file_out, "QUEUE BEFORE HAIRDRESSER:\n");
                int time_to_serve_this_client = 1000;
                srand(time(nullptr));
                time_to_serve_this_client *= rand() % 6 + 1;//сколько времени потребуется на то, чтобы подстричь одного клиента
                print();
                std::this_thread::sleep_for(std::chrono::milliseconds(time_to_serve_this_client));
                in_line.pop();
                std::cout << "QUEUE AFTER HAIRDRESSER:\n";
                fprintf(file_out, "QUEUE AFTER HAIRDRESSER:\n");
                print();
                mutex.unlock();

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
    std::thread tWorker(WorkerWork); // создание потоков
    std::thread tCustomer(customerCame);
    tWorker.join();
    tCustomer.join();
    return 0;
}
