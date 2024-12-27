/*
 * Пример структуры, которая представляет "map" в виде красно-черного дерева
 * и односвязным списком forward_list.
 * Ключом является событие представленное "int", значение forward_list.
 *
 *                           +-------+------------+
 *                          /|Event 2|Forward_list|\
 *                         / +-------+------------+ \
 *                        /                |         \
 *                       v                 v          v
 *   +------+------------+             +---------+     +-------+------------+
 *   |Event1|Forward_list|             |Observer1|     |Event 1|Forward_list|
 *   +------+-----|------+             +---------+     +-------+------------+
 *                |                                                   |
 *                v                                                   v
 *           +---------+                                         +---------+
 *           |Observer2|                                         |Observer3|
 *           +----|----+                                         +----|----+
 *                |                                                   |
 *                v                                                   |
 *           +---------+                                         +----v----+
 *           |Observer1|                                         |Observer1|
 *           +---------+                                         +---------+
 *
 *
 */
#include <forward_list>
#include <iostream>
#include <map>
#include <string>

// Объявляем структуру в которой будут
// ключами <int>
// значениями <string>
// Для удобста объявим тип
typedef std::map<int, std::forward_list<std::string>> MapLists;
MapLists mapLists;

// Объявляем ключи
enum Topics { LOG,
    DATA,
    MQTT };

// Функция вывода значения, которое представляет forward_list в каждом ключе
void printListObservers(MapLists& mapLists, int event)
{
    switch (event) {
    case LOG:
        std::cout << "\nValue in LOG key:\n";
        break;
    case DATA:
        std::cout << "\nValue in DATA key:\n";
        break;
    case MQTT:
        std::cout << "\nValue in MQTT key:\n";
        break;
    }
    for (const auto& value : mapLists[event]) {
        std::cout << value << "\n";
    }
}
// Ищем событие в структуре и выводим список наблюдателей
void searchOservers(MapLists& mapLists, int event)
{
    switch (event) {
    case LOG:
        std::cout << "\nSearch rezult for LOG observers:\n";
        break;
    case DATA:
        std::cout << "\nSearch rezult for DATA observers:\n";
        break;
    case MQTT:
        std::cout << "\nSearch rezult for MQTT observers:\n";
        break;
    }
    auto it = mapLists.find(event);
    if (it == mapLists.end()) {
        std::cout << "Not found\n";
    } else {
        for (const auto& node : mapLists[event]) {
            std::cout << node << "\n";
        }
    }
}

void addObservers(MapLists& mapLists, int event, std::string observer)
{
    // Добавляем строки в forward_list
    mapLists[event].push_front(observer);
}

int main()
{
    addObservers(mapLists, LOG, "observer1");
    addObservers(mapLists, LOG, "observer2");
    addObservers(mapLists, DATA, "observer4");
    addObservers(mapLists, DATA, "observer5");
    addObservers(mapLists, MQTT, "observer6");
    addObservers(mapLists, MQTT, "observer7");
    addObservers(mapLists, MQTT, "observer8");

    // Выводим ключи
    std::cout << "There is " << mapLists.size() << " keys in map list:\n";
    for (const auto& pair : mapLists) {
        std::cout << pair.first << "\n";
    }

    // Выводим значение, которое представляет forward_list в каждом ключе
    printListObservers(mapLists, LOG);
    printListObservers(mapLists, DATA);
    printListObservers(mapLists, MQTT);

    // Ищем событие в структуре и выводим список наблюдателей
    searchOservers(mapLists, LOG);
    searchOservers(mapLists, DATA);
    searchOservers(mapLists, MQTT);

    // Ищем Observer5 в событии MQTT и удаляем
    std::cout << "\nSearch and remove Observer 5 from MQTT event.\n";
    auto itMQ = mapLists.find(MQTT);
    if (itMQ == mapLists.end()) {
        std::cout << "Not found\n";
    } else {
        // Удалить все вхождения Observer5 (проходит по всему списку)
        // mapLists[MQTT].remove(observer5);

        // Удалить вхождениt Observer5 (проходит до первого вхождения)
        auto it = mapLists[MQTT].before_begin();
        for (auto current = mapLists[MQTT].begin(); current != mapLists[MQTT].end();
            current++) {
            if ("observer6" == *current) {
                mapLists[MQTT].erase_after(it);
                break;
            }
            it = current;
        }
    }

    printListObservers(mapLists, MQTT);

    return 0;
}
