

//
// animal shelter:
//  has cats and dogs
//  animals can only go out in the order they came in
//  users can either ask for an animal, a cat, or a dog
//  if they ask for animal they get the oldest from the cat or dog queue
//  if they ask a cat or dog they get the oldest of the asked type
//
// Restriction: only linkedList can be used

#include <cassert>
#include <iostream>
#include <memory>
#include <list>
#include <utility>
#include <vector>

using namespace std;

class Animal {
public:
    Animal(int id) : m_id(id) {};
    virtual ~Animal() {};
    int id() const { return m_id; }
private:
    int m_id;
};

class Dog : public Animal {
public:
    Dog(int id) : Animal(id) {}
    virtual ~Dog() {};
};

class Cat : public Animal {
public: 
    Cat(int id) : Animal(id) {}
    virtual ~Cat() {};
};

using pAnimal_t     = shared_ptr<Animal>;
using pDog_t        = shared_ptr<Dog>;
using pCat_t        = shared_ptr<Cat>;
using pair_animal_t = pair<int, pAnimal_t>;

ostream &operator<<(ostream &os, const Animal &a) {
    if (typeid(a) == typeid(Dog)) {
        os << "dog";
    } else {
        os << "cat";
    }
    os << "(" << a.id() << ")"; 
    return os;
}

class AnimalShelter {
    static int arrival_counter;
public:
    void enqueue(pAnimal_t animal);
    pAnimal_t dequeueAny();
    pDog_t    dequeueDog();
    pCat_t    dequeueCat();

private:
    list<pair_animal_t> m_dogs;
    list<pair_animal_t> m_cats;
};

// static
int AnimalShelter::arrival_counter = 0;

void AnimalShelter::enqueue(pAnimal_t animal) {
    // annoying clang compiler creates a warning without the extra step
    Animal &a = *animal;
    if (typeid(a) == typeid(Dog)) {
        m_dogs.push_back({arrival_counter, animal});
    } else {
        m_cats.push_back( {arrival_counter, animal});
    }
    ++arrival_counter;
}

pDog_t AnimalShelter::dequeueDog() {
    pAnimal_t a = m_dogs.front().second;
    pDog_t rslt  = dynamic_pointer_cast<Dog>(a);
    m_dogs.pop_front();
    return rslt;
}

pCat_t AnimalShelter::dequeueCat() {
    pAnimal_t a = m_cats.front().second;
    pCat_t rslt  = dynamic_pointer_cast<Cat>(a);
    m_cats.pop_front();
    return rslt;
}

pAnimal_t AnimalShelter::dequeueAny() {
    pair_animal_t oldestDog = m_dogs.front();
    pair_animal_t oldestCat = m_cats.front();
    if (oldestDog.first < oldestCat.first) {
        m_dogs.pop_front();
        return oldestDog.second;
    } else {
        m_cats.pop_front();
        return oldestCat.second;
    }
}

int main(int argc, char **argv) {
    //                        10   11   12   13   14   15   16   17   18
    vector<char> init_vec = { 'c', 'd', 'd', 'c', 'd', 'c', 'c', 'd', 'd' };
    AnimalShelter as;
    int id_ctr = 10;
    for (char c : init_vec) {
        pAnimal_t a;
        if (c == 'c') {
            a = make_shared<Cat>(id_ctr);
        } else {
            a = make_shared<Dog>(id_ctr);
        }
        ++id_ctr;
        cout << *a << " ";
        as.enqueue(a);
    }
    cout << endl;

    pCat_t a_cat = dynamic_pointer_cast<Cat>( as.dequeueAny() ); // should be a cat
    pDog_t a_dog = dynamic_pointer_cast<Dog>( as.dequeueAny() ); // should be a dog
    cout << *a_cat << endl;
    cout << *a_dog << endl;

    assert(a_cat->id() == 10);
    assert(a_dog->id() == 11);
    a_cat = as.dequeueCat(); cout << *a_cat << endl;
    assert(a_cat->id() == 13);
    a_dog = as.dequeueDog(); cout << *a_dog << endl;
    assert(a_dog->id() == 12);
    a_dog = dynamic_pointer_cast<Dog>( as.dequeueAny() ); 
    assert(a_dog->id() == 14);
    a_dog = as.dequeueDog(); cout << *a_dog << endl;
    assert(a_dog->id() == 17);
}