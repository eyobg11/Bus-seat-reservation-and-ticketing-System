#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

#define ADMIN_USER "admin1"
#define ADMIN_PASS "ad111"
#define ADMIN_SECRET "@dm!n#"

void clearScreen() { system("cls"); }
void setUserTheme() { system("color 8B"); }
void setAdminTheme() { system("color 70"); }

void typeText(const string& text, int speed = 30) {
    for (char c : text) {
        cout<<c;
        Sleep(speed);
    }
}
void waitForEnter() {
    cout<<"\n\nPress ENTER to continue...";
    cin.ignore();
    cin.get();
}

struct Bus {
    char busNumber[6];
    char source[20];
    char destination[20];
    char date[20];
    char startingTime[10];
    int fare;
    int totalSeats;
};

struct Ticket {
    int ticketID;
    char busNumber[6];
    char passengerName[30];
    int age;
    char gender[10];
    int seatNumber;
    int fare;
};

void introScreen();
void mainMenu();
void adminLogin();
void adminMenu();
void userMenu();

void createBus();
void viewBuses();
void modifyBus();
void deleteBus();

void viewAvailableBuses();
void searchByRoute();
void bookTicket();
void cancelTicket();
void viewTickets();
void viewAllTickets();

int getNextTicketID();
void displayAvailableSeats(Bus &b);

void introScreen() {
    setUserTheme();
    clearScreen();
    typeText("\t\tWELCOME to E-GO\n\n", 40);
    typeText("\tBus Reservation & Seat Ticketing System\n", 30);
    typeText("============================================\n\n", 5);
    typeText("Press ENTER to continue...", 25);
    string input;
    getline(cin, input);
    if (input == ADMIN_SECRET)
        adminLogin();
    else
        mainMenu();
}

void mainMenu() {
    setUserTheme();
    clearScreen();
    cout<<"\t\t\t MAIN MENU \n\n";
    cout<<"[1] User Menu \n";
    cout<<"[0] Exit\n\n";
    cout<<"Enter choice: ";
    string ch;
    getline(cin, ch);
    if (ch == "1") userMenu();
    else if (ch == "0") exit(0);
    else mainMenu();
}

void adminLogin() {
    setAdminTheme();
    clearScreen();
    string user, pass;
    cout<<"Username: ";
    getline(cin, user);
    cout<<"Password: ";
    getline(cin, pass);

    if (user == ADMIN_USER && pass == ADMIN_PASS)
        adminMenu();
    else
        introScreen();
}

void adminMenu() {
    setAdminTheme();
    clearScreen();
    cout<<"\t\t\t ADMIN MENU \n\n";
    cout<<"[1] Create Bus\n";
    cout<<"[2] View Buses\n";
    cout<<"[3] Modify Bus\n";
    cout<<"[4] Delete Bus\n";
    cout<<"[5] View All Booked Tickets\n";
    cout<<"[0] Logout\n\n";
    cout<<"Enter choice: ";

    string ch;
    getline(cin, ch);

    if (ch == "1") createBus();
    else if (ch == "2") viewBuses();
    else if (ch == "3") modifyBus();
    else if (ch == "4") deleteBus();
    else if (ch == "5") viewAllTickets();
    else introScreen();
}

void userMenu() {
    setUserTheme();
    clearScreen();
    cout<<"\t\t\t USER MENU\n\n";
    cout<<"[1] View Available Buses\n";
    cout<<"[2] Search by Route\n";
    cout<<"[3] Book Ticket\n";
    cout<<"[4] Cancel Ticket\n";
    cout<<"[5] View My Ticket\n";
    cout<<"[0] Back\n\n";
    cout<<"Enter choice: ";

    string ch;
    getline(cin, ch);

    if (ch == "1") viewAvailableBuses();
    else if (ch == "2") searchByRoute();
    else if (ch == "3") bookTicket();
    else if (ch == "4") cancelTicket();
    else if (ch == "5") viewTickets();
    else mainMenu();
}

void createBus() {
    clearScreen();
    Bus b;
    ofstream file("busRecord.dat", ios::binary | ios::app);

    cout<<"Bus Number  : ";
    cin>>b.busNumber;
    cout<<"Source  : ";
    cin>>b.source;
    cout<<"Destination  : ";
    cin>>b.destination;
    cout<<"Date (DD/MM/YYYY)  : ";
    cin>>b.date;
    cout<<"Start Time (HH:MM AM/PM) : ";
    cin.ignore();
    cin.getline(b.startingTime,10);
    cout<<"Fare (ETB)  : ";
    cin>>b.fare;
    cout<<"Total Seats  : ";
    cin>>b.totalSeats;

    file.write((char*)&b, sizeof(b));
    file.close();

    cout<<"\nBus successfully created!\n";
    waitForEnter();
    adminMenu();
}

void viewBuses() {
    clearScreen();
    Bus b;
    bool found = false;

    ifstream file("busRecord.dat", ios::binary);
    while (file.read((char*)&b, sizeof(b))) {
        found = true;
        cout<<b.busNumber << " | "<<b.source<<" -> "
             <<b.destination << " | Date: "<<b.date
             <<" | Time: " << b.startingTime
             <<" | Seats: " << b.totalSeats
             <<" | Fare: " << b.fare<<" (ETB)"<<endl;
    }
    file.close();

    if (!found)
        cout<<"No buses available!";
    waitForEnter();
    adminMenu();
}

void modifyBus() {
    clearScreen();
    Bus b;
    char no[6];
    bool found = false;

    cout<<"Enter Bus Number to modify: ";
    cin>>no;

    fstream file("busRecord.dat", ios::in | ios::out | ios::binary);
    while (file.read((char*)&b, sizeof(b))) {
        if (!strcmp(b.busNumber, no)) {
            found = true;
            cout<<"\tEnter new details for Bus " <<no<< ":\n";
            cout<<"Bus Number  : ";
            cin>>b.busNumber;
            cout<<"Source  : ";
            cin>>b.source;
            cout<<"Destination  : ";
            cin>>b.destination;
            cout<<"Date (DD/MM/YYYY)  : ";
            cin>>b.date;
            cout<<"Start Time (HH:MM AM/PM)  : ";
            cin.ignore();
            cin.getline(b.startingTime,10);
            cout<<"Fare (ETB)  : ";
            cin>>b.fare;
            cout<<"Total Seats  : ";
            cin>>b.totalSeats;

            file.seekp(-sizeof(b), ios::cur);
            file.write((char*)&b, sizeof(b));
            cout<<"\nBus successfully modified!\n";
            break;
        }
    }
    file.close();

    if (!found)
        cout<<"Invalid Bus Number!";
    waitForEnter();
    adminMenu();
}

void deleteBus() {
    clearScreen();
    Bus b;
    char no[6];
    bool found = false;

    cout<<"Enter Bus Number to delete: ";
    cin>>no;

    ifstream in("busRecord.dat", ios::binary);
    ofstream out("temp.dat", ios::binary);

    while (in.read((char*)&b, sizeof(b))) {
        if (!strcmp(b.busNumber, no))
            found = true;
        else
            out.write((char*)&b, sizeof(b));
    }

    in.close();
    out.close();
    remove("busRecord.dat");
    rename("temp.dat", "busRecord.dat");

    if (found) cout<<"\nBus successfully deleted!\n";
    else cout<<"Invalid Bus Number!\n";

    waitForEnter();
    adminMenu();
}

void viewAvailableBuses() {
    clearScreen();
    Bus b;
    bool found = false;

    ifstream file("busRecord.dat", ios::binary);
    while (file.read((char*)&b, sizeof(b))) {
        found = true;
        cout<<b.busNumber << " | " << b.source
             << " -> " << b.destination
             <<" | Date: " << b.date
             <<" | Time: " << b.startingTime
             <<" | Seats: " << b.totalSeats
             <<" | Fare: " << b.fare << " (ETB)" << endl;
    }
    file.close();

    if (!found) cout<<"No buses available!";
    waitForEnter();
    userMenu();
}

void searchByRoute() {
    clearScreen();
    Bus b;
    char src[20], dest[20];
    bool found = false;

    cout<<"Source      : ";
    cin>>src;
    cout<<"Destination : ";
    cin>>dest;

    ifstream file("busRecord.dat", ios::binary);
    while (file.read((char*)&b, sizeof(b))) {
        if (!strcmp(b.source, src) && !strcmp(b.destination, dest)) {
            found = true;
            cout << "Bus " << b.busNumber
                 <<" | Date: " << b.date
                 <<" | Time: " << b.startingTime
                 <<" | Fare: " << b.fare << " (ETB)" << endl;
        }
    }
    file.close();

    if (!found)
        cout<<"No matching buses found!";
    waitForEnter();
    userMenu();
}

int getNextTicketID() {
    int maxID = 1000;
    Ticket t;
    ifstream file("ticketRecord.dat", ios::binary);
    while (file.read((char*)&t, sizeof(t))) {
        if (t.ticketID > maxID)
            maxID = t.ticketID;
    }
    file.close();
    return maxID + 1;
}

void displayAvailableSeats(Bus &b) {
    bool seatsTaken[100] = {false};
    Ticket t;

    ifstream file("ticketRecord.dat", ios::binary);
    while (file.read((char*)&t, sizeof(t))) {
        if (!strcmp(t.busNumber, b.busNumber)) {
            seatsTaken[t.seatNumber - 1] = true;
        }
    }
    file.close();

    cout<<"\nSeats (booked seats = [X]):\n";
    for (int i = 0; i < b.totalSeats; i++) {
        if (seatsTaken[i])
            cout<<"[X]\t";
        else
            cout<<i + 1 << "\t";

        if ((i + 1) % 10 == 0)
            cout <<"\n";
    }
    cout<<"\n";
}

void bookTicket() {
    clearScreen();
    Bus b;
    Ticket t;

    cout<<"Enter Bus Number: ";
    cin>>t.busNumber;

    fstream busFile("busRecord.dat", ios::in | ios::out | ios::binary);
    bool found = false;

    while (busFile.read((char*)&b, sizeof(b))) {
        if (!strcmp(b.busNumber, t.busNumber)) {
            found = true;

            if (b.totalSeats <= 0) {
                cout<<"No seats available on this bus!\n";
                waitForEnter();
                userMenu();
                return;
            }

            displayAvailableSeats(b);

            bool seatTaken;
            int seatNum;
            do {
                seatTaken = false;
                cout<<"Enter Seat Number (1 - " << b.totalSeats << "): ";
                cin>>seatNum;

                if (seatNum < 1 || seatNum > b.totalSeats) {
                    cout<<"Invalid seat number! Try again.\n";
                    seatTaken = true;
                    continue;
                }

                ifstream ticketFileCheck("ticketRecord.dat", ios::binary);
                Ticket tempT;
                while (ticketFileCheck.read((char*)&tempT, sizeof(tempT))) {
                    if (!strcmp(tempT.busNumber, b.busNumber) && tempT.seatNumber == seatNum) {
                        seatTaken = true;
                        cout << "Seat " << seatNum << " is already taken! Choose another.\n";
                        break;
                    }
                }
                ticketFileCheck.close();
            } while (seatTaken);

            t.seatNumber = seatNum;

            cin.ignore();
            cout<<"Passenger Name: ";
            cin.getline(t.passengerName, 30);
            cout<<"Age: ";
            cin>>t.age;
            cout<<"Gender: ";
            cin>>t.gender;

            t.ticketID = getNextTicketID();
            t.fare = b.fare;

            busFile.seekp(-sizeof(b), ios::cur);
            b.totalSeats--;
            busFile.write((char*)&b, sizeof(b));

            ofstream ticketFile("ticketRecord.dat", ios::binary | ios::app);
            ticketFile.write((char*)&t, sizeof(t));
            ticketFile.close();

            cout<<"\nTicket successfully booked! Your Ticket ID: " << t.ticketID << endl;
            break;
        }
    }

    busFile.close();

    if (!found)
        cout<<"Invalid Bus Number!\n";
    waitForEnter();
    userMenu();
}

void cancelTicket() {
    clearScreen();
    Ticket t;
    Bus b;
    int id;
    bool found = false;

    cout<<"Enter Ticket ID to cancel: ";
    cin>>id;

    ifstream in("ticketRecord.dat", ios::binary);
    ofstream out("temp.dat", ios::binary);

    while (in.read((char*)&t, sizeof(t))) {
        if (t.ticketID == id) {
            found = true;

            fstream busFile("busRecord.dat", ios::in | ios::out | ios::binary);
            while (busFile.read((char*)&b, sizeof(b))) {
                if (!strcmp(b.busNumber, t.busNumber)) {
                    b.totalSeats++;
                    busFile.seekp(-sizeof(b), ios::cur);
                    busFile.write((char*)&b, sizeof(b));
                    break;
                }
            }
            busFile.close();

            cout<<"Ticket successfully canceled!\n";
        } else {
            out.write((char*)&t, sizeof(t));
        }
    }

    in.close();
    out.close();
    remove("ticketRecord.dat");
    rename("temp.dat", "ticketRecord.dat");

    if (!found)
        cout<<"Invalid Ticket ID!\n";
    waitForEnter();
    userMenu();
}

void viewTickets() {
    clearScreen();
    Ticket t;
    Bus b;
    bool found = false;
    int id;

    cout<<"Enter your Ticket ID to view your ticket: ";
    cin>>id;

    ifstream file("ticketRecord.dat", ios::binary);
    while (file.read((char*)&t, sizeof(t))) {
        if (t.ticketID == id) {
            found = true;

            ifstream busFile("busRecord.dat", ios::binary);
            while (busFile.read((char*)&b, sizeof(b))) {
                if (!strcmp(b.busNumber, t.busNumber)) break;
            }
            busFile.close();

            cout<<"\n\t\tTicket Details:\n";
            cout<<"Ticket ID : " << t.ticketID <<endl;
            cout<<"Passenger Name : " << t.passengerName <<endl;
            cout<<"Bus Number : " << t.busNumber <<endl;
            cout<<"Route : " << b.source << " -> " << b.destination <<endl;
            cout<<"Date : " << b.date <<endl;
            cout<<"Time : " << b.startingTime <<endl;
            cout<<"Seat Number : " << t.seatNumber <<endl;
            cout<<"Fare : " << t.fare << " (ETB)" <<endl;
            break;
        }
    }
    file.close();

    if (!found)
        cout<<"No ticket found with this Ticket ID!\n";
    waitForEnter();
    userMenu();
}

void viewAllTickets() {
    clearScreen();
    Ticket t;
    Bus b;
    bool found = false;

    ifstream file("ticketRecord.dat", ios::binary);
    cout << "\t\t ALL BOOKED TICKETS \n\n";

    while (file.read((char*)&t, sizeof(t))) {
        found = true;

        ifstream busFile("busRecord.dat", ios::binary);
        while (busFile.read((char*)&b, sizeof(b))) {
            if (!strcmp(b.busNumber, t.busNumber)) break;
        }
        busFile.close();

        cout<<"Ticket ID : " << t.ticketID
             << " | Passenger Name : " << t.passengerName
             <<" | Bus Number : " << t.busNumber
             <<" | Route : " << b.source << " -> " << b.destination
             <<" | Date : " << b.date
             <<" | Time : " << b.startingTime
             <<" | Seat Number : " << t.seatNumber
             <<" | Fare : " << t.fare << " (ETB)" << endl;
    }
    file.close();

    if (!found)
        cout<<"No tickets booked yet!\n";
    waitForEnter();
    adminMenu();
}

int main() {
    introScreen();
    return 0;
}
