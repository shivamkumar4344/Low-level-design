#include<bits/stdc++.h>
using namespace std;


// We have to design a splitwise with basis details . 

class User{
    public:
        int id;
        string name;
    
    User(int id,string name){
        this->id = id;
        this->name = name;
    }
};

class Expense{
    public:
        double amount;
        string description;
        vector<User*> users;
        User* paidBy;

    Expense(double amount, string description, vector<User*> users,User* paidBy){
        this->amount = amount;
        this->description = description;
        this->users = users;
        this->paidBy = paidBy;
    }
};


class Group{
    public:
        string name;
        vector<Expense> expenses;
        vector<User*> users;
        map<int,map<int,int>> balance;

    Group(string name){
            this->name = name;
        }

    void addMembers(User* user){
        users.push_back(user);
    }

    void addExpense(double amount,string description , vector<User*> users,User* paidBy){

        // Expense expense(amount,description, users,paidBy);

        // expenses.push_back(expense);

        double share = amount / users.size();

        for(User* p: users){
            if(p->id == paidBy->id){
                continue; // those who have put the request of split does not have to pay
            }
            balance[p->id][paidBy->id] += share; 
        }
        
    }

    void settleExpense(User* from , User* to , double amount){
        balance[from->id][to->id] -= amount;
    }

    void showBalances() {
        cout<<"Balances:- "<<endl;
        bool anyDue = false;

        for (User* a : users) {
            for (User* b : users) {
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

int main(){

    User user1(101,"Shivam");
    User user2(102,"Samir");
    User user3(103,"Raman");
    User user4(104,"Yash");

    Group grp1("Goa trip");
    grp1.addMembers(&user1);
    grp1.addMembers(&user2);
    grp1.addMembers(&user3);
    grp1.addMembers(&user4);

    grp1.addExpense(1000,"Hotel booking",{&user1,&user2,&user3,&user4},&user1);
    grp1.addExpense(200,"Breakfast",{&user1,&user2,&user3,&user4},&user3);
    grp1.addExpense(500,"Travel",{&user1,&user2,&user3,&user4},&user2);
    grp1.addExpense(100,"Snacks",{&user2,&user4},&user2);

    // grp1.showBalances();

    grp1.settleExpense(&user4,&user2,150);

    grp1.showBalances();

    return 0;
}