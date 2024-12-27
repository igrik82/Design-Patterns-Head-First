/*
 * https://youtu.be/BKmH7m_OrPI?si=FhZVkoj1ks1urlAA
 *
 * Паттерн наблюдатель в упрощенном виде c интерфейсом и возможностью
 * уведомления разных наблюдателей в связи с наступлением разных событий.
 * Для этого необходимо реализовать разветвляющуюся структуру.
 * Для простого понимания возьмем "map" в котором ключ это событие, а значение
 * forward_list в котором находятся разные наблюдатели.
 *
 * Пример структуры, которая представляет "map" в виде красно-черного дерева
 * и односвязным списком forward_list с наблюдателями.
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
 */
#include <forward_list>
#include <iostream>
#include <map>
#include <memory>
#include <string>

// Объявляем базовый класс который будет выступать в роли интерфейса
class BaseObserver {
public:
    // Обязательно при объявлении виртуальной функции
    // создаем виртуальный деструктор
    virtual ~BaseObserver() { }

    virtual void notify() = 0;
};

class Observer : public BaseObserver {
private:
    std::string _name;

public:
    Observer(const std::string& name)
        : _name(name)
    {
        std::cout << "Constructor for " + getName() << std::endl;
    }

    // Метод который будет вызываться субъектом класса.
    // При перегрузке метода не забываем объявить намеренную перегрузку override
    void notify() override
    {
        std::cout << "Hello! I'm a " + getName() << std::endl;
    }

    std::string getName() { return _name; }

    ~Observer() { std::cout << "Destructor for " + getName() << std::endl; }

    // Объявляем статический метод для возврата умного указателя на объект класса
    static std::shared_ptr<Observer> make(std::string name)
    {
        return std::make_shared<Observer>(name);
    }
};

// Объявляем базовый класс субъекта который будет выступать в роли интерфейса
class BaseSubject {
protected:
    // Объявляем простой односвязный список для регистрации
    // наблюдателей. Для удобства объявим алиас.
    typedef std::forward_list<std::shared_ptr<Observer>> ObserversList;
    // Применим функцию map для хранения "int" как ключ.
    // Т.е. пара ключ - событие. Также объявим алиас.
    typedef std::map<int, ObserversList> ObserversMap;

    // Ключ-значение
    // Ключ "int", значение std::forward_list
    ObserversMap _observers;

public:
    // Обязательно при объявлении виртуальной функции
    // создаем виртуальный деструктор
    virtual ~BaseSubject() { }

    // Добавляем экземпляр наблюдателя в список
    virtual void addObserver(int messageTypes,
        std::shared_ptr<Observer> observer)
        = 0;
    // Удаляем экземпляр наблюдателя из списка
    virtual void removeObserver(int messageTypes,
        std::shared_ptr<Observer>& observer)
        = 0;

    // В цикле перебираем список наблюдателей и вызываем у них метод notify
    virtual void notify(int event) = 0;
};

class Subject : public BaseSubject {
public:
    // Распределяем наблюдателей по типу сообщений которые они
    // будут получать, в зависимости от подписки на выбранное событие
    enum MessageTypes { DATA,
        MQTT,
        LOG,
        ALL };

    // Добавляем экземпляр наблюдателя в список
    void addObserver(int messageTypes,
        std::shared_ptr<Observer> observer) override
    {
        // Ищем тип сообщения в списке, т.е. его номер ENUM
        auto it = _observers.find(messageTypes);

        // Если дошли до конца и не находим, добавляем его в forward_list
        if (it == _observers.end()) {
            _observers[messageTypes] = ObserversList();
        }

        // Добавляем экземпляр наблюдателя в forward_list, согласно
        // типу сообщения
        _observers[messageTypes].push_front(observer);
        std::cout << observer.get()->getName()
                  << " added to subscription on event #" << messageTypes
                  << std::endl;
    }

    // Удаляем экземпляр наблюдателя из списка
    void removeObserver(int messageTypes,
        std::shared_ptr<Observer>& observer) override
    {
        // Ищем топик по ключу map
        auto it = _observers.find(messageTypes);
        if (it == _observers.end()) {
            std::cout << "Topic not found\n";
        } else {
            // Удалить все вхождения
            // Не очень эффективно, т.к. проходится по всему списку. Но просто.
            // _observers[messageTypes].remove(observer);
            // observer.reset();

            bool found = false;
            // Ставим указатель на самое начало
            auto it = _observers[messageTypes].before_begin();
            // В цикле итерируем
            for (auto current = _observers[messageTypes].begin();
                current != _observers[messageTypes].end(); current++) {
                if (*current == observer) {
                    // Сообщаем об удалении если нашли
                    found = true;
                    std::cout << observer->getName() << " removed\n";

                    // Удаляем элемент после указателя
                    _observers[messageTypes].erase_after(it);
                    // Сбрасываем счетчик ссылок для уничтожения объекта
                    // умного указателя
                    observer.reset();

                    break;
                }
                // Продвигаем указатель на следующий элемент
                it = current;
            }
            // Сообщаем если не найдено
            if (!found) {
                std::cout << observer->getName() << " not found in event #"
                          << messageTypes << "\n";
            }
        }
    }

    // В цикле перебираем список наблюдателей и вызываем у них метод notify
    void notify(int event) override
    {
        for (auto& mObserver : _observers) {
            // Если сообщения направлены всем или определенным наблюдателям
            if (event == ALL || event == mObserver.first) {
                // Перебираем forward_list
                for (auto fObserver : mObserver.second) {
                    fObserver->notify();
                }
            }
        }
    }
};

int main()
{
    // Создаем 4 экземпляра наблюдателя
    auto observer1 = Observer::make("Observer1");
    auto observer2 = Observer::make("Observer2");
    auto observer3 = Observer::make("Observer3");
    {
        // Тест автоматического удаления наблюдателя
        // при выходе из области видимости.
        // Без использования умных указателей необходимо
        // в деструкторе произвести удаление объекта.
        auto observer4 = Observer::make("Observer4");
    };
    auto observer5 = Observer::make("Observer5");
    auto observer6 = Observer::make("Observer6");
    auto observer7 = Observer::make("Observer7");
    auto observer8 = Observer::make("Observer8");
    std::cout << std::endl;

    // Создаем экземпляр субъекта
    Subject subject;
    // Добавляем наблюдателей в список
    subject.addObserver(Subject::LOG, observer1);
    subject.addObserver(Subject::DATA, observer2);
    subject.addObserver(Subject::MQTT, observer3);
    subject.addObserver(Subject::LOG, observer5);
    subject.addObserver(Subject::LOG, observer6);
    subject.addObserver(Subject::DATA, observer7);
    subject.addObserver(Subject::DATA, observer8);
    std::cout << std::endl;

    // Вызываем метод notify у всех экземпляров наблюдателей
    subject.notify(Subject::ALL);
    std::cout << std::endl;

    // Удаляем наблюдателя из списка
    subject.removeObserver(Subject::MQTT, observer2);
    subject.removeObserver(Subject::DATA, observer2);
    std::cout << std::endl;

    // Вызываем метод notify у наблюдателей MQTT
    subject.notify(Subject::MQTT);
    std::cout << std::endl;

    return 0;
}
