/*
 * https://youtu.be/BKmH7m_OrPI?si=FhZVkoj1ks1urlAA
 *
 * Паттерн наблюдатель в упрощенном виде c интерфейсом и возможностью
 * уведомления разных наблюдателей в связи с наступлением разных событий.
 * Для этого необходимо реализовать разветвляющуюся структуру.
 * Для простого понимания возьмем простой forward_list и к определенной ячейке
 * события, сделаем дополнительный forward_list в котором в свою очередь,
 * находятся разные наблюдатели.
 *
 * +---------+   +----------+   +---------+
 * | Event 3 |-->| Event 2  |-->| Event 1 |
 * +---------+   +----------+   +---------+
 *      |             |              |
 *      v             v              v
 * +---------+   +----------+   +---------+
 * |Observer2|   |Observer1 |   |Observer3|
 * +---------+   +----------+   +---------+
 *      |                            |
 *      v                            v
 * +---------+                  +---------+
 * |Observer1|                  |Observer1|
 * +---------+                  +---------+
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
    virtual void notify() = 0;
};

class Subject : public BaseSubject {
public:
    // Распределяем наблюдателей по типу сообщений которые они
    // будут получать, в зависимости от подписки на выбранное событие
    enum MessageTypes { DATA,
        MQTT,
        LOG };

    // Добавляем экземпляр наблюдателя в список
    void addObserver(int messageTypes, std::shared_ptr<Observer> observer)
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
    void removeObserver(int messageTypes, std::shared_ptr<Observer>& observer)
    {
        auto it = _observers.find(messageTypes);
        if (it == _observers.end()) {
            return;
        } else {
            ObserversList& list = _observers[messageTypes];
            for (ObserversList::iterator it = list.begin(); it != list.end();) {
                if (*it == observer) {
                    list.remove(observer);
                    // Обнуляем счетчик ссылок наблюдателя для удаления его из памяти
                    std::cout << observer.get()->getName()
                              << " removed from subscription #" << messageTypes
                              << std::endl;
                    observer.reset();
                } else {
                    ++it;
                }
            }
        }
    }

    // В цикле перебираем список наблюдателей и вызываем у них метод notify
    void notify()
    {
        // for (auto& observer : _observers) {
        // observer->notify();
        // }
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
    std::cout << std::endl;

    // Создаем экземпляр субъекта
    Subject subject;
    // Добавляем наблюдателей в список
    subject.addObserver(Subject::LOG, observer1);
    subject.addObserver(Subject::DATA, observer2);
    subject.addObserver(Subject::MQTT, observer3);
    std::cout << std::endl;

    // Вызываем метод notify у экземпляров наблюдателей
    subject.notify();
    std::cout << std::endl;

    // Удаляем наблюдателя из списка
    // subject.removeObserver(Subject::DATA, observer2);
    subject.removeObserver(Subject::DATA, observer3);
    std::cout << std::endl;

    // Вызываем метод notify у оставшихся в списке
    // экземпляров наблюдателей
    subject.notify();

    return 0;
}
