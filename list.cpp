#include <iostream>
#include <cassert>

using namespace std;

template <class T> class Link;
template <class T> class Iterator;

// ======================================================================
//                    Class Template List
// ======================================================================
template <class T>
class List {
protected:
    Link <T> *first;

public:
    List() : first(nullptr) {}
    List(const List &source);
    virtual ~List() {};

    virtual void add(T value);
    virtual void deleteAll();
    T firstElement() const;
    virtual bool includes(T value) const;
    bool isEmpty() const;
    virtual void removeFirst();

    friend class Iterator <T>;
};
// ======================================================================
//                    Class Template Link
// ======================================================================
template <class T>
class Link {
private:
    T value;
    Link <T> *next;

    Link(T val, Link *ptr) : value(val), next(ptr) { }
public:
    Link <T> *insert(T value);

    friend class List <T>;
    friend class Iterator <T>;
};


// ======================================================================

//                    Class Template Iterator
// ======================================================================
template <class T> class Iterator {
public:
    Iterator(List <T> &aList);
    virtual bool init();
    virtual T operator()();
    virtual bool operator !();
    virtual bool operator++(); // for prefix, for postfix add dummy int
    virtual bool operator++(int);
    virtual void operator=(T value);
    void removeCurrent();
    void addBefore(T new_value);
    void addAfter(T new_value);
 protected:
    List <T> &myList;   // data fields referencja na obiekt klasy list
    Link <T> *previous;//pokazuje na poprzednik
    Link <T> *current;
 };

// ======================================================================
//                    Class Template Link - attributes
// ======================================================================
template <class T> Link <T> * Link <T> :: insert(T value) {
    next = new Link <T>(value, next);
    return next;
}

// ======================================================================
//                    Class Template List - attributes
// ======================================================================
template <class T> void List <T> :: add(T value) {
    first = new Link <T>(value, first);
}

template <class T> T List <T> :: firstElement() const {
    assert(first != nullptr);
    return first->value;
}


template <class T> bool List <T> :: isEmpty() const {
    return first == nullptr;
}


template <class T> bool List <T> :: includes(T value) const {
    for (Link <T> *p = first; p; p = p->next)
        if (value == p->value)return true;
    return false;
}

template <class T> void List <T> :: removeFirst() {
    assert(first != nullptr);
    Link <T> *ptr = first;
    first = ptr->next;
    delete ptr;
}

template <class T> void List <T> :: deleteAll() {
    Link <T> *next;
    for (Link <T> *p = first; p; p = next) {
        next = p->next;
        delete p;
    }
    first = nullptr;
}

// ======================================================================
//                    Class Template Iterator - attributes
// ======================================================================
template <class T> Iterator <T> ::
    Iterator(List <T> &aList) : myList(aList) {
    init();//inicjuje iterator
    //konstruktor domyslny
}
template <class T> bool Iterator <T> :: init() {
    previous = nullptr;
    current = myList.first;//my list nie jest wskaŸnikiem tylko referencj¹!
    return current != nullptr;//ustawi³ sie na pierwszej liœcie po to jakby by³a tablica pusta
}
template <class T> T Iterator <T> :: operator()() { //nawiasy okr¹g³e operator ktory odczytywany jest przez current tam gdszie jest ierator
    assert(current != nullptr);
    return current->value;// zwraca wartoœæ
}
template <class T> void Iterator <T> :: operator=(T val) {
    assert(current != nullptr);
    current->value = val;
}

template <class T> bool Iterator <T> :: operator++() {
    if (current == nullptr){    // move current pointer
        if (previous == nullptr) // to next element
            current = myList.first;
        else
            current = previous->next;
    }
    else {
        previous = current;
        current = current->next;
    }
    return current != nullptr;
} // valid for prefix operator only

template <class T> bool Iterator <T> :: operator++(int){
    return operator++();
}

template <class T> bool Iterator <T> :: operator !() {
    if (current == nullptr and previous != nullptr)
        current = previous->next;
    return current != nullptr;
}

template <class T> void Iterator <T> :: removeCurrent() {
    assert(current != nullptr);
    if (previous == nullptr)
        myList.first = current->next;
    else
        previous->next = current->next;
    delete current;
    current = nullptr;
}

template <class T> void Iterator <T> :: addBefore(T val) {
    if (previous) previous = previous->insert(val);
    else {
        myList.List<T>::add(val);     // to avoid subclass
        previous = myList.first;
        current = previous->next;      // if current is NULL
    }
}

template <class T> void Iterator <T> :: addAfter(T val) {
    if (current){current->insert(val); return;}  // not shown
    if (previous)current = previous->insert(val);
    else myList.List<T>::add(val);
}

// ======================================================================
//                            Point
// ======================================================================

class Point {
    private:
        string name_;
        double x_,y_;
    public:
        Point(string,double,double);
        ~Point(){};
        double sum();
        friend ostream& operator<<(ostream&, Point&);
        //string name=name_;
        //double x=x_,y=y_;
         bool operator==(Point toCompare) {
            if(name_ == toCompare.name_ && x_ == toCompare.x_&& y_ == toCompare.y_)
                return true;
            return false;
        }


};
Point::Point(string a,double b,double c) : name_(a), x_(b), y_(c)
{

}
double Point::sum()
{
    return x_+y_;
}
ostream& operator<<(ostream& out, Point& var)
{
    out<<var.name_<<" "<<var.x_<<" "<<var.y_<<" ";

    return out;
}
// ======================================================================
//                            main
// ======================================================================

int main() {

    string name; double X,Y;
    //Point myPoint;
    List<Point> myList;
    int n;
    cin>>n;
    for(int i = 0; i < n; i++){
        cin >> name >> X>>Y;
        Point myPoint(name,X,Y);
        myList.add(myPoint);
    }
    //3. stwórz iterator dla listy z punktu pierwszego,
    Iterator<Point> it(myList);
    auto currentPoint = it();
    //4. wyœwietl element, na który ustawiony jest iterator,
    //std::cout << currentApple.type_ << " " << currentApple.weight_ << " ";
    //5. u¿ywaj¹c iteratora wyœwietl listê od jej pocz¹tku do koñca,
    for(it.init(); !it; ++it){
        auto currentPoint = it();
        cout<<currentPoint;
    }
    for(it.init(); !it; ++it){
        auto currentPoint = it();
        cout<<currentPoint;
    }

    return 0;
}
