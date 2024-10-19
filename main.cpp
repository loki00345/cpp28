#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <typeinfo>
#include <algorithm>

using namespace std;


class Resident {
private:
    string name;
    int age;
    string gender;

public:
    Resident(const string& name, int age, const string& gender)
        : name(name), age(age), gender(gender) {}


    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    int getAge() const { return age; }
    void setAge(int age) { this->age = age; }

    string getGender() const { return gender; }
    void setGender(const string& gender) { this->gender = gender; }


    friend ostream& operator<<(ostream& os, const Resident& resident) {
        os << "Resident: " << resident.name << ", Age: " << resident.age << ", Gender: " << resident.gender;
        return os;
    }
};


class Premises {
protected:
    int rooms;
    double area;
    string address;
    vector<Resident> residents;

public:
    Premises(int rooms, double area, const string& address)
        : rooms(rooms), area(area), address(address) {}

    virtual ~Premises() {}


    int getRooms() const { return rooms; }
    void setRooms(int rooms) { this->rooms = rooms; }

    double getArea() const { return area; }
    void setArea(double area) { this->area = area; }

    string getAddress() const { return address; }
    void setAddress(const string& address) { this->address = address; }


    void addResident(const Resident& resident) {
        residents.push_back(resident);
    }

    void removeResident(const string& name) {
        residents.erase(
            remove_if(residents.begin(), residents.end(),
                [&name](const Resident& r) { return r.getName() == name; }),
            residents.end());
    }


    void printResidents() const {
        cout << "Residents of premises at " << address << ":\n";
        for (const auto& resident : residents) {
            cout << resident << endl;
        }
    }


    virtual void printInfo() const = 0;


    friend ostream& operator<<(ostream& os, const Premises& premises) {
        os << "Address: " << premises.address << ", Rooms: " << premises.rooms << ", Area: " << premises.area;
        return os;
    }
};


class Apartment : public Premises {
private:
    int entranceNumber;
    int floor;
    int apartmentNumber;

public:
    Apartment(int rooms, double area, const string& address, int entranceNumber, int floor, int apartmentNumber)
        : Premises(rooms, area, address), entranceNumber(entranceNumber), floor(floor), apartmentNumber(apartmentNumber) {}


    int getEntranceNumber() const { return entranceNumber; }
    void setEntranceNumber(int entranceNumber) { this->entranceNumber = entranceNumber; }

    int getFloor() const { return floor; }
    void setFloor(int floor) { this->floor = floor; }

    int getApartmentNumber() const { return apartmentNumber; }
    void setApartmentNumber(int apartmentNumber) { this->apartmentNumber = apartmentNumber; }


    void printInfo() const override {
        cout << *this << ", Entrance: " << entranceNumber << ", Floor: " << floor << ", Apartment: " << apartmentNumber << endl;
        printResidents();
    }
};


class PrivateHouse : public Premises {
private:
    double yardArea;

public:
    PrivateHouse(int rooms, double area, const string& address, double yardArea)
        : Premises(rooms, area, address), yardArea(yardArea) {}


    double getYardArea() const { return yardArea; }
    void setYardArea(double yardArea) { this->yardArea = yardArea; }


    void printInfo() const override {
        cout << *this << ", Yard Area: " << yardArea << " sq.m" << endl;
        printResidents();
    }
};


class Cottage : public Premises {
private:
    int bathrooms;
    int sleepingPlaces;

public:
    Cottage(int rooms, double area, const string& address, int bathrooms, int sleepingPlaces)
        : Premises(rooms, area, address), bathrooms(bathrooms), sleepingPlaces(sleepingPlaces) {}


    int getBathrooms() const { return bathrooms; }
    void setBathrooms(int bathrooms) { this->bathrooms = bathrooms; }

    int getSleepingPlaces() const { return sleepingPlaces; }
    void setSleepingPlaces(int sleepingPlaces) { this->sleepingPlaces = sleepingPlaces; }


    void printInfo() const override {
        cout << *this << ", Bathrooms: " << bathrooms << ", Sleeping Places: " << sleepingPlaces << endl;
        printResidents();
    }
};


class Hotel : public Premises {
private:
    int roomsNumber;
    double maxPrice;
    double minPrice;

public:
    Hotel(int rooms, double area, const string& address, int roomsNumber, double maxPrice, double minPrice)
        : Premises(rooms, area, address), roomsNumber(roomsNumber), maxPrice(maxPrice), minPrice(minPrice) {}


    int getRoomsNumber() const { return roomsNumber; }
    void setRoomsNumber(int roomsNumber) { this->roomsNumber = roomsNumber; }

    double getMaxPrice() const { return maxPrice; }
    void setMaxPrice(double maxPrice) { this->maxPrice = maxPrice; }

    double getMinPrice() const { return minPrice; }
    void setMinPrice(double minPrice) { this->minPrice = minPrice; }


    void printInfo() const override {
        cout << *this << ", Rooms: " << roomsNumber << ", Min. Price: " << minPrice << ", Max. Price: " << maxPrice << endl;
        printResidents();
    }
};


class HousingManager {
private:
    vector<shared_ptr<Premises>> premisesList;

public:

    void addPremises(const shared_ptr<Premises>& premises) {
        premisesList.push_back(premises);
    }


    void removePremises(const string& address) {
        premisesList.erase(
            remove_if(premisesList.begin(), premisesList.end(),
                [&address](const shared_ptr<Premises>& p) { return p->getAddress() == address; }),
            premisesList.end());
    }


    void printAllPremises() const {
        for (const auto& premises : premisesList) {
            premises->printInfo();
        }
    }


    template <typename T>
    void printPremisesByType() const {
        for (const auto& premises : premisesList) {
            if (dynamic_cast<T*>(premises.get())) {
                premises->printInfo();
            }
        }
    }


    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {

            for (const auto& premises : premisesList) {
                file << typeid(*premises).name() << " " << premises->getAddress() << endl;
            }
            file.close();
        }
    }


    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string type, address;
            while (file >> type >> address) {
                
                cout << "Loaded premises type: " << type << " with address: " << address << endl;
            }
            file.close();
        }
    }
};


int main() {
    HousingManager manager;

    shared_ptr<Apartment> apt = make_shared<Apartment>(3, 75.5, "10 Hrushevskogo St", 2, 5, 23);
    apt->addResident(Resident("Ivan Ivanenko", 30, "male"));
    apt->addResident(Resident("Maria Ivanenko", 28, "female"));

    shared_ptr<PrivateHouse> house = make_shared<PrivateHouse>(5, 150, "15 Lesya Ukrainka St", 300);
    house->addResident(Resident("Petro Petrenko", 45, "male"));

    manager.addPremises(apt);
    manager.addPremises(house);


    manager.printAllPremises();


    manager.saveToFile("premises.txt");

    return 0;
}
