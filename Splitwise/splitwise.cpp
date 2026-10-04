// Simple Splitwise (SDE I level)
// Compile: g++ -std=c++17 splitwise_simple.cpp -o splitwise_simple

#include<bits/stdc++.h>
using namespace std;

// ---------- User ----------
class User {
public:
    int id;
    string name;

    User(int id, string name) {
        this->id = id;
        this->name = name;
    }
};

// ---------- Expense ----------
class Expense {
public:
    string description;
    User* paidBy;
    double amount;
    vector<User*> participants;   // people who share this expense

    Expense(string description, User* paidBy, double amount, vector<User*> participants) {
        this->description = description;
        this->paidBy = paidBy;
        this->amount = amount;
        this->participants = participants;
    }
};

// ---------- Group ----------
class Group {
public:
    string name;
    vector<User*> members;
    vector<Expense> expenses;

    // balance[a][b] = how much a owes b
    map<int, map<int, double>> balance;

    //constructor
    Group(string name) {
        this->name = name;
    }

    void addMember(User* user) {
        members.push_back(user);
    }

    // Split equally among participants
    void addExpense(string description, User* paidBy, double amount, vector<User*> participants) {
        Expense expense(description, paidBy, amount, participants);
        expenses.push_back(expense);

        double share = amount / participants.size();

        for (User* p : participants) {
            if (p->id == paidBy->id) continue;   // payer doesn't owe himself
            balance[p->id][paidBy->id] += share;
        }
    }

    // 'from' pays 'to' some money
    void settle(User* from, User* to, double amount) {
        balance[from->id][to->id] -= amount;
    }

    void showBalances() {
        cout << "--- Balances in " << name << " ---\n";
        bool anyDue = false;

        for (User* a : members) {
            for (User* b : members) {
                if (a->id == b->id) continue;

                // net amount a owes b (after cancelling what b owes a)
                double net = balance[a->id][b->id] - balance[b->id][a->id];

                if (net > 0.01) {
                    cout << a->name << " owes " << b->name << ": " << net << "\n";
                    anyDue = true;
                }
            }
        }
        if (!anyDue) cout << "All settled up!\n";
    }
};

// ---------- Main ----------
int main() {
    User alice(1, "Alice");
    User bob(2, "Bob");
    User charlie(3, "Charlie");

    Group trip("Goa Trip");
    trip.addMember(&alice);
    trip.addMember(&bob);
    trip.addMember(&charlie);

    // Alice paid 3000 for everyone
    trip.addExpense("Hotel", &alice, 3000, {&alice, &bob, &charlie});

    // Bob paid 600, only for himself and Charlie
    trip.addExpense("Dinner", &bob, 600, {&bob, &charlie});

    trip.showBalances();

    cout << "\nCharlie pays Alice 1000\n";
    trip.settle(&charlie, &alice, 1000);

    trip.showBalances();
    return 0;
}