#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>
#include <limits>

using namespace std;
const string RED = "\033[1;31m";

// ------------------- Safe Input Function -------------------
template <typename T>
T safeInput(const string& prompt = "") {
    T value;
    while (true) {
        if (!prompt.empty()) cout << prompt;
        cin >> value;
        if (!cin.fail()) break;
        cout << "Invalid input. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

// ------------------- Donor Class -------------------
class Donor {
private:
    int donorID;
    string name, contact, donorType;

public:
    Donor() {}
    Donor(int id, string n, string c, string t) : donorID(id), name(n), contact(c), donorType(t) {}

    void input() {
        donorID = safeInput<int>("\nEnter Donor ID: ");
        cout << "Enter Donor Name: "; getline(cin, name);
        cout << "Enter Contact: "; getline(cin, contact);
        cout << "Enter Donor Type (Individual/Organization): "; getline(cin, donorType);
    }

    void display() const {
        const string CYAN = "\033[1;32m";
        const string RESET = "\033[0m";
        cout << "------------------------------\n";
        cout << "ID     : " << donorID << '\n';
        cout << "Name   : " << CYAN << name << RESET << '\n';
        cout << "Contact: " << contact << '\n';
        cout << "Type   : " << donorType << '\n';
        cout << "------------------------------\n";
    }

    int getID() const { return donorID; }
    string getName() const { return name; }

    void saveToFile(ofstream& fout) const {
        fout << donorID << ',' << name << ',' << contact << ',' << donorType << '\n';
    }

    void loadFromLine(const string& line) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        donorID = stoi(line.substr(0, pos1));
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        contact = line.substr(pos2 + 1, pos3 - pos2 - 1);
        donorType = line.substr(pos3 + 1);
    }
};

// ------------------- Donation Base Class -------------------
class Donation {
protected:
    int donationID, donorID;
    string date, cause;

public:
    virtual void input() = 0;
    virtual void display() const = 0;
    virtual float getValue() const = 0;
    virtual string getType() const = 0;
    virtual void saveToFile(ofstream& fout) const = 0;

    int getDonorID() const { return donorID; }
    string getCause() const { return cause; }
    string getDate() const { return date; }
    int getID() const { return donationID; }

    virtual ~Donation() {}
};

// ------------------- Money Donation -------------------
class MoneyDonation : public Donation {
private:
    float amount;

public:
    MoneyDonation() {}

    MoneyDonation(int id, int donorId, const string& date, const string& cause, float amount) {
        donationID = id;
        donorID = donorId;
        this->date = date;
        this->cause = cause;
        this->amount = amount;
    }

    void input() override {
        cout << "\n--- Money Donation ---";
        donationID = safeInput<int>("\nEnter Donation ID: ");
        donorID = safeInput<int>("Enter Donor ID: ");
        cout << "Enter Date (DD-MM-YYYY): "; getline(cin, date);
        cout << "Enter Cause: "; getline(cin, cause);
        amount = safeInput<float>("Enter Amount: ");
    }

    void display() const override {
        cout << "\n[Money Donation] ID: " << donationID << ", DonorID: " << donorID
            << ", Amount: $" << amount << ", Cause: " << cause << ", Date: " << date << endl;
    }

    float getValue() const override { return amount; }
    string getType() const override { return "Money"; }

    void saveToFile(ofstream& fout) const override {
        fout << "Money," << donationID << ',' << donorID << ',' << date << ',' << cause << ',' << amount << '\n';
    }
};

// ------------------- Goods Donation -------------------
class GoodsDonation : public Donation {
private:
    string description;
    int quantity;
    float estimatedValue;

public:
    GoodsDonation() {}

    GoodsDonation(int id, int donorId, const string& date, const string& cause,
        const string& desc, int qty, float val) {
        donationID = id;
        donorID = donorId;
        this->date = date;
        this->cause = cause;
        description = desc;
        quantity = qty;
        estimatedValue = val;
    }

    void input() override {
        cout << "\n--- Goods Donation ---";
        donationID = safeInput<int>("\nEnter Donation ID: ");
        donorID = safeInput<int>("Enter Donor ID: ");
        cout << "Enter Date (DD-MM-YYYY): "; getline(cin, date);
        cout << "Enter Cause: "; getline(cin, cause);
        cout << "Enter Description: "; getline(cin, description);
        quantity = safeInput<int>("Enter Quantity: ");
        estimatedValue = safeInput<float>("Enter Estimated Value: ");
    }

    void display() const override {
        cout << "\n[Goods Donation] ID: " << donationID << ", DonorID: " << donorID
            << ", Description: " << description << ", Qty: " << quantity
            << ", Value: $" << estimatedValue << ", Cause: " << cause << ", Date: " << date << endl;
    }

    float getValue() const override { return estimatedValue; }
    string getType() const override { return "Goods"; }

    void saveToFile(ofstream& fout) const override {
        fout << "Goods," << donationID << ',' << donorID << ',' << date << ',' << cause << ',' << description << ',' << quantity << ',' << estimatedValue << '\n';
    }
};

// ------------------- Service Donation -------------------
class ServiceDonation : public Donation {
private:
    string serviceType;
    int hours;
    float hourlyRate;

public:
    ServiceDonation() {}

    ServiceDonation(int id, int donorId, const string& date, const string& cause,
        const string& serviceType, int hours, float rate) {
        donationID = id;
        donorID = donorId;
        this->date = date;
        this->cause = cause;
        this->serviceType = serviceType;
        this->hours = hours;
        this->hourlyRate = rate;
    }

    void input() override {
        cout << "\n--- Service Donation ---";
        donationID = safeInput<int>("\nEnter Donation ID: ");
        donorID = safeInput<int>("Enter Donor ID: ");
        cout << "Enter Date (DD-MM-YYYY): "; getline(cin, date);
        cout << "Enter Cause: "; getline(cin, cause);
        cout << "Enter Service Type: "; getline(cin, serviceType);
        hours = safeInput<int>("Enter Hours Donated: ");
        hourlyRate = safeInput<float>("Enter Estimated Hourly Rate: ");
    }

    void display() const override {
        cout << "\n[Service Donation] ID: " << donationID << ", DonorID: " << donorID
            << ", Service: " << serviceType << ", Hours: " << hours
            << ", Value: $" << (hours * hourlyRate) << ", Cause: " << cause << ", Date: " << date << endl;
    }

    float getValue() const override { return hours * hourlyRate; }
    string getType() const override { return "Service"; }

    void saveToFile(ofstream& fout) const override {
        fout << "Service," << donationID << ',' << donorID << ',' << date << ',' << cause << ',' << serviceType << ',' << hours << ',' << hourlyRate << '\n';
    }
};
// ------------------- Cause Class -------------------
class Cause {
private:
    string name;
    float totalDonations = 0;

public:
    Cause() {}
    Cause(string n) : name(n) {}

    void input() {
        cout << "\nEnter Cause Name: ";
        getline(cin, name);
    }

    void addDonation(float value) {
        totalDonations += value;
    }

    void display() const {
        cout << "\nCause: " << name << " | Total Donations: $" << fixed << setprecision(2) << totalDonations << endl;
    }

    string getName() const { return name; }

    void saveToFile(ofstream& fout) const {
        fout << name << ',' << totalDonations << '\n';
    }

    void loadFromLine(const string& line) {
        size_t pos = line.find(',');
        name = line.substr(0, pos);
        totalDonations = stof(line.substr(pos + 1));
    }

    float getTotalDonations() const {
        return totalDonations;
    }
};

// ------------------- Donation Manager -------------------
class DonationManager {
private:
    vector<Donor> donors;
    vector<Donation*> donations;
    vector<Cause> causes;

public:
    ~DonationManager() {
        for (auto d : donations)
            delete d;
    }

    void loadDonors() {
        ifstream fin("donors.txt");
        string line;
        while (getline(fin, line)) {
            Donor d;
            d.loadFromLine(line);
            donors.push_back(d);
        }
        fin.close();
    }

    void loadCauses() {
        ifstream fin("causes.txt");
        string line;
        while (getline(fin, line)) {
            Cause c;
            c.loadFromLine(line);
            causes.push_back(c);
        }
        fin.close();
    }

    void loadDonations() {
        ifstream fin("donations.txt");
        string line;
        while (getline(fin, line)) {
            istringstream sin(line);
            string type;
            getline(sin, type, ',');

            Donation* d = nullptr;

            if (type == "Money") {
                int id, donorId;
                string date, cause;
                float amount;
                string token;

                getline(sin, token, ','); id = stoi(token);
                getline(sin, token, ','); donorId = stoi(token);
                getline(sin, date, ',');
                getline(sin, cause, ',');
                getline(sin, token); amount = stof(token);

                d = new MoneyDonation(id, donorId, date, cause, amount);
            }
            else if (type == "Goods") {
                int id, donorId, qty;
                string date, cause, desc;
                float val;
                string token;

                getline(sin, token, ','); id = stoi(token);
                getline(sin, token, ','); donorId = stoi(token);
                getline(sin, date, ',');
                getline(sin, cause, ',');
                getline(sin, desc, ',');
                getline(sin, token, ','); qty = stoi(token);
                getline(sin, token); val = stof(token);

                d = new GoodsDonation(id, donorId, date, cause, desc, qty, val);
            }
            else if (type == "Service") {
                int id, donorId, hours;
                string date, cause, service;
                float rate;
                string token;

                getline(sin, token, ','); id = stoi(token);
                getline(sin, token, ','); donorId = stoi(token);
                getline(sin, date, ',');
                getline(sin, cause, ',');
                getline(sin, service, ',');
                getline(sin, token, ','); hours = stoi(token);
                getline(sin, token); rate = stof(token);

                d = new ServiceDonation(id, donorId, date, cause, service, hours, rate);
            }

            if (d != nullptr) donations.push_back(d);
        }
        fin.close();
    }

    void saveAll() {
        ofstream foutDonors("donors.txt");
        for (const auto& d : donors)
            d.saveToFile(foutDonors);
        foutDonors.close();

        ofstream foutCauses("causes.txt");
        for (const auto& c : causes)
            c.saveToFile(foutCauses);
        foutCauses.close();

        ofstream foutDonations("donations.txt");
        for (const auto& d : donations)
            d->saveToFile(foutDonations);
        foutDonations.close();
    }

    void addDonor() {
        Donor d;
        d.input();
        donors.push_back(d);
    }

    void addCause() {
        Cause c;
        c.input();
        causes.push_back(c);
    }

    void addDonation() {
        int choice = safeInput<int>("\n1. Money Donation\n2. Goods Donation\n3. Service Donation\nEnter type: ");
        Donation* d = nullptr;

        if (choice == 1)
            d = new MoneyDonation();
        else if (choice == 2)
            d = new GoodsDonation();
        else if (choice == 3)
            d = new ServiceDonation();
        else {
            cout << "Invalid choice!\n";
            return;
        }

        d->input();
        donations.push_back(d);

        // Update cause total
        for (auto& c : causes) {
            if (c.getName() == d->getCause()) {
                c.addDonation(d->getValue());
                break;
            }
        }

        // Find donor name for thank-you message
        string donorName = "Donor";
        for (const auto& donor : donors) {
            if (donor.getID() == d->getDonorID()) {
                donorName = donor.getName();
                break;
            }
        }

        cout << "\n Thank you, " << donorName << ", for your generous "
            << d->getType() << " donation towards \"" << d->getCause() << "\"!\n";
    }


    void viewDonationsByDonor() {
        int id = safeInput<int>("\nEnter Donor ID: ");
        for (auto d : donations) {
            if (d->getDonorID() == id)
                d->display();
        }
    }

    void viewCauseSummary() {
        cout << "\n========== Cause Donation Summary ==========\n\n";

        // Table header
        cout << left << setw(30) << "Cause Name"
            << setw(20) << "Total Donations (Rs)" << '\n';
        cout << string(50, '-') << '\n';

        // Display each cause with total donation
        for (const auto& c : causes) {
            cout << left
                << setw(30) << c.getName()
                << "Rs. " << fixed << setprecision(0) << setw(16) << c.getTotalDonations()
                << '\n';
        }

        cout << "\n============================================\n";
    }

    void rankTopDonors() {
        map<int, float> donorTotals;
        for (auto d : donations)
            donorTotals[d->getDonorID()] += d->getValue();

        vector<pair<int, float>> ranking(donorTotals.begin(), donorTotals.end());
        sort(ranking.begin(), ranking.end(), [](auto& a, auto& b) {
            return a.second > b.second;
            });

        cout << "\n--- Top Donors ---\n";
        cout << left << setw(6) << "Rank"
            << setw(10) << "DonorID"
            << setw(30) << "Name"
            << setw(15) << "Total Donated" << '\n';
        cout << string(61, '-') << '\n';

        int rank = 1;
        for (const auto& p : ranking) {
            auto it = find_if(donors.begin(), donors.end(),
                [&](const Donor& d) { return d.getID() == p.first; });
            if (it != donors.end()) {
                cout << left << setw(6) << rank++
                    << setw(10) << it->getID()
                    << setw(30) << it->getName()
                    << "$" << fixed << setprecision(2) << setw(14) << p.second << '\n';
            }
        }
    }

    void viewDonors() {
        cout << "\n--- Donor List ---";
        for (const auto& d : donors)
            d.display();
    }

    void viewAllDonations() {
        cout << "\n--- All Donations ---\n\n";

        cout << left
            << setw(8) << "ID"
            << setw(10) << "Type"
            << setw(10) << "DonorID"
            << setw(14) << "Date"
            << setw(25) << "Cause"
            << setw(25) << "Details"
            << setw(12) << "Amount (Rs)"
            << '\n';

        cout << string(104, '-') << '\n';

        for (const auto& d : donations) {
            stringstream details;
            string type = d->getType();

            if (type == "Money") {
                details << "Cash";
            }
            else if (type == "Goods") {
                auto* g = dynamic_cast<GoodsDonation*>(d);
                details << g->getType();
            }
            else if (type == "Service") {
                auto* s = dynamic_cast<ServiceDonation*>(d);
                details << s->getType();
            }

            cout << left
                << setw(8) << d->getID()
                << setw(10) << d->getType()
                << setw(10) << d->getDonorID()
                << setw(14) << d->getDate()
                << setw(25) << d->getCause()
                << setw(25) << details.str()
                << "Rs. " << fixed << setprecision(0) << d->getValue()
                << '\n';
        }
    }

    void generateReceipts() {
        ofstream fout("receipts.txt");
        cout << "\n--- Donation Receipts ---";
        for (const auto& d : donations) {
            stringstream ss;
            ss << "Receipt\nDonation ID: " << d->getID()
                << "\nType: " << d->getType()
                << "\nDonor ID: " << d->getDonorID()
                << "\nAmount/Value: $" << fixed << setprecision(2) << d->getValue()
                << "\nCause: " << d->getCause()
                << "\nDate: " << d->getDate()
                << "\n-------------------------\n";
            cout << ss.str();
            fout << ss.str();
        }
        fout.close();
        cout << "\nReceipts saved to receipts.txt\n";
    }

    void searchDonorByName() {
        string keyword;

        cout << "\nEnter donor name to search: ";
        // REMOVE this line: cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, keyword);

        // Convert keyword to lowercase
        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

        bool found = false;
        cout << "\n--- Search Results ---\n";

        for (const auto& d : donors) {
            string name = d.getName();
            string lowerName = name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

            if (lowerName.find(keyword) != string::npos) {
                d.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No donors found with the name containing \"" << keyword << "\".\n";
        }
    }

    const string GREEN = "\033[1;33m";
    const string YELLOW = "\033[1;33m";
    const string RESET = "\033[0m";

    void menu() {
        int choice;
        do {
            cout << "\n" << GREEN << "===== Aid & Donation Tracking Menu =====" << RESET << "\n";
            cout << YELLOW << " 1. " << RESET << "Add Donor\n";
            cout << YELLOW << " 2. " << RESET << "View Donors\n";
            cout << YELLOW << " 3. " << RESET << "Add Donation\n";
            cout << YELLOW << " 4. " << RESET << "View Donations\n";
            cout << YELLOW << " 5. " << RESET << "View Causes\n";
            cout << YELLOW << " 6. " << RESET << "Rank Top Donors\n";
            cout << YELLOW << " 7. " << RESET << "Generate Receipts\n";
            cout << YELLOW << " 8. " << RESET << "Search Donor by Name\n";
            cout << YELLOW << " 9. " << RESET << "Exit\n";

            cout << GREEN << "\nEnter your choice: " << RESET;
            choice = safeInput<int>("");

            switch (choice) {
            case 1: addDonor(); break;
            case 2: viewDonors(); break;
            case 3: addDonation(); break;
            case 4: viewAllDonations(); break;
            case 5: viewCauseSummary(); break;
            case 6: rankTopDonors(); break;
            case 7: generateReceipts(); break;
            case 8: searchDonorByName(); break;
            case 9: saveAll(); cout << "\nExiting and saving data...\n"; break;
            default:
                cout << RED << "Invalid choice! Please enter a number between 1 and 9." << RESET << "\n";
            }
        } while (choice != 9);
    }

};

// ------------------- Main Function -------------------
int main() {
    DonationManager dm;
    dm.loadDonors();
    dm.loadCauses();
    dm.loadDonations();
    dm.menu();
    return 0;
}
