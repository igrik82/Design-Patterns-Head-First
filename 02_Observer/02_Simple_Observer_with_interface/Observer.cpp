/*
 * https://youtu.be/BKmH7m_OrPI?si=FhZVkoj1ks1urlAA
 *
 * Паттерн наблюдатель в упрощенном виде c интерфейсом.
 * Основная идея это создать экземпляр субъекта со списком наблюдателей,
 * который вызывает метод, какого либо оповещения у зарегистрированного
 * списка наблюдателей.
 * По этому нужно в классе субъекта реализовать какой либо контейнерный
 * список (forward_list, list, vector) для хранения наблюдателей.
 */
#include <forward_list>
#include <iostream>
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
private:
    // Объявляем простой односвязный список для регистрации
    // объектов класса наблюдатель
    std::forward_list<std::shared_ptr<Observer>> _observers;

public:
    // Обязательно при объявлении виртуальной функции
    // создаем виртуальный деструктор
    virtual ~BaseSubject() { }

    // Добавляем экземпляр наблюдателя в список
    virtual void addObserver(std::shared_ptr<Observer> observer)
    {
        _observers.push_front(observer);
        std::cout << observer.get()->getName() << " added to subscription."
                  << std::endl;
    }

    // Удаляем экземпляр наблюдателя из списка
    virtual void removeObserver(std::shared_ptr<Observer>& observer)
    {
        _observers.remove(observer);
        // Обнуляем счетчик ссылок наблюдателя для удаления его из памяти
        std::cout << observer.get()->getName() << " removed from subscription."
                  << std::endl;
        observer.reset();
    }

    // В цикле перебираем список наблюдателей и вызываем у них метод notify
    void notify()
    {
        for (auto& observer : _observers) {
            observer->notify();
        }
    }
};

class Subject : public BaseSubject { };

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
    subject.addObserver(observer1);
    subject.addObserver(observer2);
    subject.addObserver(observer3);
    std::cout << std::endl;

    // Вызываем метод notify у экземпляров наблюдателей
    subject.notify();
    std::cout << std::endl;

    // Удаляем наблюдателя из списка
    subject.removeObserver(observer2);
    std::cout << std::endl;

    // Вызываем метод notify у оставшихся в списке
    // экземпляров наблюдателей
    subject.notify();

    return 0;
}
