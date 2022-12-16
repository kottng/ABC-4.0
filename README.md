# ABC 4.0
# Работа Коткова Дмитрия Павловича БПИ213 вариант номер 1 
ОТЧЕТ на оценку 8:  
Условие:  
Задача о парикмахере. В тихом городке есть парикмахерская. Салон парикмахерской мал, работать в нем нем может только один парикмахер, обслуживающий одного посетителя. Есть несколько кресел для ожидания в очереди. Парикмахер всю жизнь обслуживает посетителей. Когда в салоне никого нет, он спит в кресле. Когда посетитель приходит и видит спящего парикмахера, он будет его, садится в кресло, «засыпая» на тот момент, пока парикмахер обслуживает его. Если посетитель приходит, а парикмахер занят, то он встает в очередь и «засыпает». После стрижки парикмахер сам провожает посетителя. Если есть ожидающие посетители, то парикмахер будит одного из них и ждет пока тот сядет в кресло парикмахера и начинает стрижку. Если никого нет, он снова садится в свое кресло и засыпает до прихода посетителя. Создать многопоточное приложение, моделирующее рабочий день парикмахерской.
# Алгоритм программы  
https://pro-prof.com/forums/topic/parallel-programming-paradigms?ysclid=lbqwdy8ze5623650220  
основан на парадигме производители и потребители.
Производители и потребители — это взаимодействующие процессы. Они часто организуются в конвейер, через который проходит информация.
Прежде чем переходить к организации конвейеров, определим, что процесс-производитель генерирует в некотором буфере информацию, которая используется процессом-потребителем.
Здесь возникают проблемы переполнения и исчерпания буфера. Тогда при переполнении буфера производитель должен будет ждать, пока в буфере не освободится хотя бы один элемент, а при исчерпании буфера должен будет ждать потребитель, пока хотя бы один новый элемент не появится в буфере [7].
Как было сказано ранее, часто производители и потребители объединены в конвейер – последовательность процессов, в которой каждый потребляет данные предшественника и поставляет данные для последующего процесса.
Классическим примером являются конвейеры в ОС Unix. Рассмотрим взаимодействие команд:
$ps –Af | grep mc. 
Вертикальная черта «|» между командами обозначает конвейер т.е. выход одной команды переназначается на вход другой команды, таким образом, в приведенном выше примере результат выполнения ps –Af будет передан команде grep, которая произведет трансформацию входного потока в соответствии с маской (в данном случае на выходе будут все строки, содержащие подстроку «mc») [8, c. 22].
Потоки могут быть связаны с файлами особого типа – каналами. Канал – это буфер (очередь типа FIFO) между процессом-производителем ипроцессом-потребителем. Процесс-производитель записывает данные в конец очереди, а процесс-потребитель читает данные из ее начала, при этом символы удаляются. В общем случае канал – это тот самый ограниченный буфер, поэтому производитель при необходимости ожидает, пока в буфере появится свободное место для очередной порции данных, а потребитель при необходимости ждет появления в буфере данных [8, c. 23].
Недостаток данной модели взаимодействия процессов: ограниченность буфера, необходимо всегда проверять размер буфера, иначе возникает опасность  переполнения или истощения буфера.    
  
В нашем случае мы имеем 1 поток, ответственный за генерацию новых клиентов, и 2 поток, ответственный за обработку всех клиентов, то есть соответсвенно производитель и потребитель. Программа базируется на канале - очереди(in_line). Количество посетителей варьируется от нуля и задается при помощи одного из форматов ввода: через консоль, файл или при помощи сгенерированного случайно числа. Программа завершается, когда кончатся посетители.
# В Си-файле реализовано:
 * Ввод и вывод в программе осуществляется с помощью файлов, командной строки, или рандомной генерации.(все возможные вводимые значения должно быть неотрицательными)
 * Реализовано несколько функций, в том числе функция создания событий "пришел посетитель", "парикмахер начал стричь посетителя", а также функции рандомной
 генерации чисел, вывода на экран результатов работы функций создания событий.
 * реализовано два потока данных, которые соединены между собой благодаря двум мьютексам(двоичным семафорам), реализована парадигма неравноправных потоков: "Производители и потребители". Один поток производит, второй потребляет.
 * модель параллельных вычислений: Потоки связываем между друг другом с помощью очереди типа FIFO(в программе in_line). Создаем мьютексы(двоичные семафоры), через которые разграничиваем вывод программы таким образом, чтобы сразу два потока не имели доступа к выводу(результат без введения мбютексов показан в тестировании программы).
 # Тестировка программы:
Программа работает исправно: весь вывод функции четкий и понятный, никаких ошибок не возникает, вот результаты:

![Снимок экрана 2022-12-16 в 20 58 20](https://user-images.githubusercontent.com/75154790/208159902-79f31797-7ae1-481e-a7e9-e120fcf8536c.png)

Проведем эксперемент, что произойдет, если у программы убрать разделение потоков при помощи мьютекосв, вот результаты:  
![Снимок экрана 2022-12-16 в 21 43 52](https://user-images.githubusercontent.com/75154790/208167307-a566ddeb-3948-4a9a-8cc8-70da74189178.png)  
Легко заметить, что программа начинает выдавать некорректный вывод на 22 строчке


Таким образом, мьютексы помогают не допустить вывода информации результатов обеих функций событий одновременно, то есть при помощи этой технологии программа работает корректно: своевременно выводить все корректные результаты функции на экран.  
  
Проверим исправность работы программы с помощью ввода через файл    
![Снимок экрана 2022-12-16 в 22 45 15](https://user-images.githubusercontent.com/75154790/208177055-df3df3db-5e25-4ead-b075-44ccb9c39741.png)  
или случайно сгенерированного числа соответственно:  
![Снимок экрана 2022-12-16 в 22 52 17](https://user-images.githubusercontent.com/75154790/208178229-69484bd3-fd1d-4483-82d6-398e615a7f8a.png)

