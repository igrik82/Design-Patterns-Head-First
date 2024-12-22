/*
 * Паттерн наблюдатель в упрощенном виде.
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

class Observer {
private:
    std::string _name;

public:
    Observer(std::string name)
        : _name(name)
    {
        std::cout << "Constructor for " + _name << std::endl;
    }

    // Метод который будет вызываться субъектом класса
    void notify() { std::cout << "Hello! I'm a " + _name << std::endl; }

    ~Observer() { std::cout << "Destructor for " + _name << std::endl; }

    // Объявляем статический метод для возврата умного указателя на объект класса
    static std::shared_ptr<Observer> make(std::string name)
    {
        return std::make_shared<Observer>(name);
    }
};

class Subject {
private:
    // Объявляем простой односвязный список для регистрации
    // объектов класса наблюдатель
    std::forward_list<std::shared_ptr<Observer>> _observers;

public:
    // Добавляем экземпляр наблюдателя в список
    void addObserver(std::shared_ptr<Observer> observer)
    {
        _observers.push_front(observer);
        std::cout << "Observer added to subscription." << std::endl;
    }

    // Удаляем экземпляр наблюдателя из списка
    void removeObserver(std::shared_ptr<Observer>& observer)
    {
        _observers.remove(observer);
        // Обнуляем счетчик ссылок наблюдателя для удаления его из памяти
        observer.reset();
        std::cout << "Observer removed from subscription." << std::endl;
    }

    // В цикле перебираем список наблюдателей и вызываем у них метод notify
    void notify()
    {
        for (auto& observer : _observers) {
            observer->notify();
        }
    }
};

int main()
{
    // Создаем 4 экземпляра наблюдателя
    auto observer1 = Observer::make("Observer1");
    auto observer2 = Observer::make("Observer2");
    auto observer3 = Observer::make("Observer3");
    auto observer4 = Observer::make("Observer4");

    // Создаем экземпляр субъекта
    Subject subject;
    // Добавляем наблюдателей в список
    subject.addObserver(observer1);
    subject.addObserver(observer2);
    subject.addObserver(observer3);
    subject.addObserver(observer4);

    // Вызываем метод notify у экземпляров наблюдателей
    subject.notify();

    // Удаляем наблюдателя из списка
    subject.removeObserver(observer2);
    subject.removeObserver(observer3);

    // Вызываем метод notify у оставшихся в списке
    // экземпляров наблюдателей
    subject.notify();

    return 0;
}
