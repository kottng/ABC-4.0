# ABC 4.0
# Работа Коткова Дмитрия Павловича БПИ213 вариант номер 1 
 - ОТЧЕТ на оценку 8: 
  - Условие: \\
Задача о парикмахере. В тихом городке есть парикмахерская. Салон парикмахерской мал, работать в нем нем может только один парикмахер, обслуживающий одного посетителя. Есть несколько кресел для ожидания в очереди. Парикмахер всю жизнь обслуживает посетителей. Когда в салоне никого нет, он спит в кресле. Когда посетитель приходит и видит спящего парикмахера, он будет его, садится в кресло, «засыпая» на тот момент, пока парикмахер обслуживает его. Если посетитель приходит, а парикмахер занят, то он встает в очередь и «засыпает». После стрижки парикмахер сам провожает посетителя. Если есть ожидающие посетители, то парикмахер будит одного из них и ждет пока тот сядет в кресло парикмахера и начинает стрижку. Если никого нет, он снова садится в свое кресло и засыпает до прихода посетителя. Создать многопоточное приложение, моделирующее рабочий день парикмахерской.
# В Си-файле реализовано:


 * Ввод и вывод в программе осуществляется с помощью файлов, командной строки, или рандомной генерации.
 * Реализовано несколько функций, в том числе функция подсчета степенного ряда, генерации рандомизированных входных данных.
 * использованы только локальные переменные.
 * реализована генерация файла рандомным способом.
 * реализовано два потока данных, которые соединены между собой благодаря двум мьютексам(двоичным семафорам)
 # Тестировка программы:
