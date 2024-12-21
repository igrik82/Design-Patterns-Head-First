// Паттерн наблюдатель
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
    }

    void notify() { std::cout << "Hello! I'm a " + _name << std::endl; }
};

class Subject {
private:
    std::forward_list<std::unique_ptr<Observer>> _observers;

public:
    void addObserver(std::unique_ptr<Observer> observer)
    {
        _observers.push_front(std::move(observer));
    }

    void removeObserver(std::unique_ptr<Observer> observer)
    {
        _observers.remove(observer);
        std::cout << "Observer removed" << std::endl;
    }

    void notify()
    {
        for (auto& o : _observers) {
            o->notify();
        }
    }
};

int main()
{

    Observer observer1("Observer1");
    Observer observer2("Observer2");
    Observer observer3("Observer3");
    Observer observer4("Observer4");

    auto ptr = std::make_unique<Observer>(observer2);

    Subject subject;
    subject.addObserver(std::make_unique<Observer>(observer1));
    subject.addObserver(std::move(ptr));
    subject.addObserver(std::make_unique<Observer>(observer3));
    subject.addObserver(std::make_unique<Observer>(observer4));

    subject.notify();

    std::cout << "\nRemove observer" << std::endl;
    subject.removeObserver(std::move(ptr));
    subject.notify();

    return 0;
}
