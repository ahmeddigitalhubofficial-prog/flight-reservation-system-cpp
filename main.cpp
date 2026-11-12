// ================= FINAL PRIVATE FLIGHT SYSTEM =================

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// ================= ENUMS =================
enum class Role { USER, ADMIN };
enum class SeatStatus { AVAILABLE, BOOKED };
enum class PaymentMethod { CASH, CARD };
enum class PaymentStatus { SUCCESS, FAILED };

// ================= INPUT =================
int getInt() {

    int val;

    while (!(cin >> val)) {

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid input, enter a number: ";
    }

    return val;
}

// ================= SEAT =================
class Seat {

protected:

    int number;
    SeatStatus status;
    double basePrice;

public:

    Seat(int n, double base)
        : number(n),
          status(SeatStatus::AVAILABLE),
          basePrice(base) {}

    virtual ~Seat() {}

    bool isAvailable() const {

        return status == SeatStatus::AVAILABLE;
    }

    void book() {

        status = SeatStatus::BOOKED;
    }

    void unbook() {

        status = SeatStatus::AVAILABLE;
    }

    int getNumber() const {

        return number;
    }

    virtual double getFee() const = 0;
    virtual string getClassName() const = 0;

    double getPrice() const {

        return basePrice + getFee();
    }
};

// ================= ECONOMY =================
class EconomySeat : public Seat {

public:

    EconomySeat(int n, double base)
        : Seat(n, base) {}

    double getFee() const override {

        return 0;
    }

    string getClassName() const override {

        return "Economy";
    }
};

// ================= ECONOMY PLUS =================
class EconomyPlusSeat : public Seat {

public:

    EconomyPlusSeat(int n, double base)
        : Seat(n, base) {}

    double getFee() const override {

        return 1500;
    }

    string getClassName() const override {

        return "Economy Plus";
    }
};

// ================= BUSINESS =================
class BusinessSeat : public Seat {

public:

    BusinessSeat(int n, double base)
        : Seat(n, base) {}

    double getFee() const override {

        return 3500;
    }

    string getClassName() const override {

        return "Business";
    }
};

// ================= FLIGHT =================
class Flight {

private:

    int id;
    string from;
    string to;
    string departureTime;
    double basePrice;

    vector<Seat*> seats;

public:

    Flight(int i,
           string f,
           string t,
           string time,
           int seatsPerClass,
           double price)

        : id(i),
          from(f),
          to(t),
          departureTime(time),
          basePrice(price) {

        int seatNumber = 1;

        for (int i = 0; i < seatsPerClass; i++)
            seats.push_back(new EconomySeat(seatNumber++, basePrice));

        for (int i = 0; i < seatsPerClass; i++)
            seats.push_back(new EconomyPlusSeat(seatNumber++, basePrice));

        for (int i = 0; i < seatsPerClass; i++)
            seats.push_back(new BusinessSeat(seatNumber++, basePrice));
    }

    ~Flight() {

        for (auto seat : seats)
            delete seat;
    }

    int getID() const {

        return id;
    }

    string getFrom() const {

        return from;
    }

    string getTo() const {

        return to;
    }

    string getTime() const {

        return departureTime;
    }

    void setFrom(string f) {

        from = f;
    }

    void setTo(string t) {

        to = t;
    }

    void setTime(string t) {

        departureTime = t;
    }

    void display() const {

        cout << "\n====================================\n";
        cout << "Flight " << id << endl;
        cout << from << " -> " << to << endl;
        cout << "Time : " << departureTime << endl;
        cout << "Base Price : $" << basePrice << endl;
        cout << "====================================\n";
    }

    void showSeats() const {

        cout << "\nSeats For Flight " << id << "\n\n";

        for (auto seat : seats) {

            cout << "Seat "
                 << seat->getNumber()
                 << " ["
                 << seat->getClassName()
                 << "] $"
                 << seat->getPrice()
                 << " - "
                 << (seat->isAvailable() ? "AVAILABLE" : "BOOKED")
                 << endl;
        }
    }

    Seat* getSeat(int num) {

        for (auto seat : seats) {

            if (seat->getNumber() == num)
                return seat;
        }

        return nullptr;
    }
};

// ================= PAYMENT =================
class Payment {

private:

    double amount;
    PaymentMethod method;
    PaymentStatus status;

public:

    Payment(double a)
        : amount(a),
          method(PaymentMethod::CASH),
          status(PaymentStatus::FAILED) {}

    void chooseMethod() {

        cout << "\n1. Cash\n";
        cout << "2. Card\n";
        cout << "Choice: ";

        int choice = getInt();

        if (choice == 2)
            method = PaymentMethod::CARD;
        else
            method = PaymentMethod::CASH;
    }

    void process() {

        cout << "\nProcessing payment of $" << amount << "...\n";

        cout << "Confirm Payment? (1 = Yes): ";

        int confirm = getInt();

        if (confirm == 1) {

            status = PaymentStatus::SUCCESS;
            cout << "Payment SUCCESS\n";
        }

        else {

            status = PaymentStatus::FAILED;
            cout << "Payment FAILED\n";
        }
    }

    bool isSuccess() const {

        return status == PaymentStatus::SUCCESS;
    }
};

// ================= USER =================
class User {

private:

    string name;

public:

    User(string n)
        : name(n) {}

    string getName() const {

        return name;
    }
};

// ================= TICKET =================
class Ticket {

private:

    string user;
    string from;
    string to;
    string departureTime;
    string seatClass;

    int seatNumber;
    int flightID;

    double price;

public:

    Ticket(string u, Flight* flight, Seat* seat)
        : user(u) {

        from = flight->getFrom();
        to = flight->getTo();
        departureTime = flight->getTime();

        seatNumber = seat->getNumber();
        seatClass = seat->getClassName();

        price = seat->getPrice();
        flightID = flight->getID();
    }

    string getUser() const {

        return user;
    }

    int getSeatNumber() const {

        return seatNumber;
    }

    int getFlightID() const {

        return flightID;
    }

    void print() const {

        cout << "\n====================================\n";
        cout << "              TICKET\n";
        cout << "====================================\n\n";

        cout << "Passenger : " << user << endl;
        cout << "Route     : " << from << " -> " << to << endl;
        cout << "Time      : " << departureTime << endl;
        cout << "Seat      : " << seatNumber
             << " (" << seatClass << ")" << endl;
        cout << "Price     : $" << price << endl;

        cout << "\n====================================\n";
    }
};

// ================= ACCOUNT =================
class Account {

private:

    string username;
    string password;
    Role role;

public:

    Account(string u, string p, Role r)
        : username(u),
          password(p),
          role(r) {}

    bool login(string u, string p) const {

        return username == u && password == p;
    }

    string getUsername() const {

        return username;
    }

    Role getRole() const {

        return role;
    }
};

// ================= REGISTER =================
void registerAccount(vector<Account>& accounts) {

    string username;
    string password;

    cout << "Enter Username: ";
    cin >> username;

    bool valid = true;

    for (char c : username) {

        if (!isalpha(c))
            valid = false;
    }

    while (!valid || username.empty()) {

        cout << "Invalid username (letters only): ";
        cin >> username;

        valid = true;

        for (char c : username) {

            if (!isalpha(c))
                valid = false;
        }
    }

    for (auto& account : accounts) {

        if (account.getUsername() == username) {

            cout << "Username already exists!\n";
            system("pause");
            return;
        }
    }

    cout << "Enter Password: ";
    cin >> password;

    while (password.length() < 3) {

        cout << "Password too short: ";
        cin >> password;
    }

    accounts.push_back(
        Account(username, password, Role::USER)
    );

    cout << "\nAccount created successfully!\n";

    system("pause");
}

// ================= LOGIN =================
Account* login(vector<Account>& accounts) {

    string username;
    string password;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    for (auto& account : accounts) {

        if (account.login(username, password))
            return &account;
    }

    cout << "Wrong username or password!\n";

    system("pause");

    return nullptr;
}

// ================= MAIN =================
int main() {

    vector<Account> accounts = {

        Account("admin", "admin", Role::ADMIN),
        Account("user", "user", Role::USER)
    };

    vector<Flight*> flights;

    flights.push_back(
        new Flight(1, "Egypt", "Dubai", "10:00AM", 2, 5000)
    );

    flights.push_back(
        new Flight(2, "Egypt", "France", "08:00AM", 3, 12000)
    );

    vector<Ticket> tickets;

    int choice;

    while (true) {

        Account* currentAccount = nullptr;

        do {

            system("cls");

            cout << "====================================\n";
            cout << "      PRIVATE FLIGHT SYSTEM\n";
            cout << "====================================\n\n";

            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Exit\n\n";

            cout << "Choice: ";

            choice = getInt();

            if (choice < 1 || choice > 3) {

                cout << "Invalid choice!\n";
                system("pause");
                continue;
            }

            if (choice == 1) {

                currentAccount = login(accounts);
            }

            else if (choice == 2) {

                registerAccount(accounts);
            }

        } while (!currentAccount && choice != 3);

        if (choice == 3)
            break;

        User user(currentAccount->getUsername());

        // ================= ADMIN =================
        if (currentAccount->getRole() == Role::ADMIN) {

            do {

                system("cls");

                cout << "=========== ADMIN MENU ===========\n\n";

                cout << "1. Add Flight\n";
                cout << "2. View Flights + Seats\n";
                cout << "3. Edit/Delete Flight\n";
                cout << "4. View Users\n";
                cout << "5. Cancel Ticket\n";
                cout << "6. Logout\n\n";

                cout << "Choice: ";

                choice = getInt();

                if (choice < 1 || choice > 6) {

                    cout << "Invalid choice!\n";
                    system("pause");
                    continue;
                }

                // ADD FLIGHT
                if (choice == 1) {

                    int id;
                    int seatsPerClass;

                    string from;
                    string to;
                    string departureTime;

                    double basePrice;

                    cout << "ID: ";
                    id = getInt();

                    bool idExists = false;

                    for (auto flight : flights) {

                        if (flight->getID() == id)
                            idExists = true;
                    }

                    if (idExists) {

                        cout << "Flight ID already exists!\n";
                        system("pause");
                        continue;
                    }

                    cout << "From: ";
                    cin >> from;

                    cout << "To: ";
                    cin >> to;

                    cout << "Time: ";
                    cin >> departureTime;

                    cout << "Seats Per Class: ";
                    seatsPerClass = getInt();

                    cout << "Base Price: ";
                    cin >> basePrice;

                    flights.push_back(
                        new Flight(
                            id,
                            from,
                            to,
                            departureTime,
                            seatsPerClass,
                            basePrice
                        )
                    );

                    cout << "\nFlight added successfully!\n";

                    system("pause");
                }

                // VIEW FLIGHTS
                else if (choice == 2) {

                    if (flights.empty()) {

                        cout << "No flights available.\n";
                    }

                    else {

                        for (auto flight : flights) {

                            flight->display();
                            flight->showSeats();
                        }
                    }

                    system("pause");
                }

                // EDIT / DELETE
                else if (choice == 3) {

                    int id;

                    cout << "Flight ID: ";
                    id = getInt();

                    bool found = false;

                    for (size_t i = 0; i < flights.size(); i++) {

                        if (flights[i]->getID() == id) {

                            found = true;

                            cout << "\n1. Edit\n";
                            cout << "2. Delete\n";
                            cout << "Choice: ";

                            int editChoice = getInt();

                            if (editChoice == 1) {

                                string value;

                                cout << "New From: ";
                                cin >> value;
                                flights[i]->setFrom(value);

                                cout << "New To: ";
                                cin >> value;
                                flights[i]->setTo(value);

                                cout << "New Time: ";
                                cin >> value;
                                flights[i]->setTime(value);

                                cout << "\nFlight updated successfully!\n";
                            }

                            else if (editChoice == 2) {

                                delete flights[i];

                                flights.erase(
                                    flights.begin() + i
                                );

                                cout << "\nFlight deleted successfully!\n";
                            }

                            else {

                                cout << "Invalid choice!\n";
                            }

                            break;
                        }
                    }

                    if (!found) {

                        cout << "Flight not found!\n";
                    }

                    system("pause");
                }

                // VIEW USERS
                else if (choice == 4) {

                    for (auto& account : accounts) {

                        cout << account.getUsername() << " | ";

                        if (account.getRole() == Role::ADMIN)
                            cout << "Admin\n";
                        else
                            cout << "User\n";
                    }

                    system("pause");
                }

                // CANCEL TICKET
                else if (choice == 5) {

                    if (tickets.empty()) {

                        cout << "No tickets available.\n";
                        system("pause");
                        continue;
                    }

                    for (size_t i = 0; i < tickets.size(); i++) {

                        cout << i << ". ";
                        tickets[i].print();
                    }

                    int cancelChoice;

                    cout << "Choose Ticket Number: ";
                    cancelChoice = getInt();

                    if (cancelChoice >= 0 &&
                        static_cast<size_t>(cancelChoice) < tickets.size()) {

                        int flightID =
                            tickets[cancelChoice].getFlightID();

                        int seatNumber =
                            tickets[cancelChoice].getSeatNumber();

                        for (auto flight : flights) {

                            if (flight->getID() == flightID) {

                                Seat* seat =
                                    flight->getSeat(seatNumber);

                                if (seat)
                                    seat->unbook();
                            }
                        }

                        tickets.erase(
                            tickets.begin() + cancelChoice
                        );

                        cout << "\nTicket cancelled successfully!\n";
                    }

                    else {

                        cout << "Invalid ticket number!\n";
                    }

                    system("pause");
                }

            } while (choice != 6);
        }

        // ================= USER =================
        else {

            do {

                system("cls");

                cout << "=========== USER MENU ===========\n\n";

                cout << "1. View Flights\n";
                cout << "2. Book Flight\n";
                cout << "3. My Tickets\n";
                cout << "4. Cancel Ticket\n";
                cout << "5. Logout\n\n";

                cout << "Choice: ";

                choice = getInt();

                if (choice < 1 || choice > 5) {

                    cout << "Invalid choice!\n";
                    system("pause");
                    continue;
                }

                // VIEW FLIGHTS
                if (choice == 1) {

                    if (flights.empty()) {

                        cout << "No flights available.\n";
                    }

                    else {

                        for (auto flight : flights)
                            flight->display();
                    }

                    system("pause");
                }

                // BOOK FLIGHT
                else if (choice == 2) {

                    if (flights.empty()) {

                        cout << "No flights available.\n";

                        system("pause");
                        continue;
                    }

                    cout << "\n=========== AVAILABLE FLIGHTS ===========\n";

                    for (auto flight : flights) {

                        flight->display();
                    }

                    int flightID;

                    cout << "Enter Flight ID: ";
                    flightID = getInt();

                    Flight* chosenFlight = nullptr;

                    for (auto flight : flights) {

                        if (flight->getID() == flightID)
                            chosenFlight = flight;
                    }

                    if (!chosenFlight) {

                        cout << "Flight not found!\n";

                        system("pause");
                        continue;
                    }

                    chosenFlight->showSeats();

                    int seatNumber;

                    cout << "Enter Seat Number: ";
                    seatNumber = getInt();

                    Seat* seat =
                        chosenFlight->getSeat(seatNumber);

                    if (!seat) {

                        cout << "Seat does not exist!\n";

                        system("pause");
                        continue;
                    }

                    if (!seat->isAvailable()) {

                        cout << "Seat already booked!\n";

                        system("pause");
                        continue;
                    }

                    Payment payment(
                        seat->getPrice()
                    );

                    payment.chooseMethod();
                    payment.process();

                    if (payment.isSuccess()) {

                        seat->book();

                        tickets.push_back(

                            Ticket(
                                user.getName(),
                                chosenFlight,
                                seat
                            )
                        );

                        cout << "\nReservation Confirmed!\n";

                        tickets.back().print();
                    }

                    else {

                        cout << "Payment failed. Booking cancelled!\n";
                    }

                    system("pause");
                }

                // MY TICKETS
                else if (choice == 3) {

                    bool found = false;

                    for (auto& ticket : tickets) {

                        if (ticket.getUser() == user.getName()) {

                            ticket.print();
                            found = true;
                        }
                    }

                    if (!found)
                        cout << "No tickets found.\n";

                    system("pause");
                }

                // CANCEL TICKET
                else if (choice == 4) {

                    vector<int> myTickets;

                    for (size_t i = 0; i < tickets.size(); i++) {

                        if (tickets[i].getUser() == user.getName()) {

                            cout << myTickets.size() << ". ";

                            tickets[i].print();

                            myTickets.push_back(i);
                        }
                    }

                    if (myTickets.empty()) {

                        cout << "No tickets found.\n";

                        system("pause");
                        continue;
                    }

                    int cancelChoice;

                    cout << "Choose Ticket Number: ";
                    cancelChoice = getInt();

                    if (cancelChoice >= 0 &&
                        static_cast<size_t>(cancelChoice) < myTickets.size()) {

                        int realIndex =
                            myTickets[cancelChoice];

                        int flightID =
                            tickets[realIndex].getFlightID();

                        int seatNumber =
                            tickets[realIndex].getSeatNumber();

                        for (auto flight : flights) {

                            if (flight->getID() == flightID) {

                                Seat* seat =
                                    flight->getSeat(seatNumber);

                                if (seat)
                                    seat->unbook();
                            }
                        }

                        tickets.erase(
                            tickets.begin() + realIndex
                        );

                        cout << "\nTicket cancelled successfully!\n";
                    }

                    else {

                        cout << "Invalid ticket number!\n";
                    }

                    system("pause");
                }

            } while (choice != 5);
        }
    }

    for (auto flight : flights)
        delete flight;

    cout << "\nThank you for using Private Flight System.\n";

    return 0;
}
