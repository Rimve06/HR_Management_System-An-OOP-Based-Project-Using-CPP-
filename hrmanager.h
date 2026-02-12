#ifndef HRMANAGER_H
#define HRMANAGER_H

#include "employee.h"
#include <algorithm>
#include <sstream>

// HR Manager class (inherits from Person)
class HRManager : public Person {
private:
    string password;
    string designation;

public:
    HRManager() : Person(), password(""), designation("HR Manager") {}

    HRManager(string n, string e, string p, int i, string pass, string desig)
        : Person(n, e, p, i), password(pass), designation(desig) {}

    ~HRManager() {
        cout << "HRManager destructor called for: " << name << endl;
    }
    HRManager(const HRManager& hr) : Person(hr) {
        password = hr.password;
        designation = hr.designation;
    }

    // Assignment operator
    HRManager& operator=(const HRManager& hr) {
        if (this != &hr) {
            Person::operator=(hr);
            password = hr.password;
            designation = hr.designation;
        }
        return *this;
    }

    void display() const override {
        cout << "\n========== HR MANAGER DETAILS ==========\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Designation: " << designation << endl;
        cout << "========================================\n";
    }

    string getPassword() const { return password; }
    string getDesignation() const { return designation; }

    void setPassword(string p) { password = p; }

    void saveToFile(ofstream& out) const {
        out << id << "|" << name << "|" << email << "|" << phone << "|"
            << password << "|" << designation << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string line;
        if (getline(in, line)) {
            size_t pos = 0;
            string delimiter = "|";
            vector<string> tokens;
            while ((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            if (tokens.size() >= 6) {
                id = stoi(tokens[0]);
                name = tokens[1];
                email = tokens[2];
                phone = tokens[3];
                password = tokens[4];
                designation = tokens[5];
                return true;
            }
        }
        return false;
    }
};

// Notice class
class Notice {
private:
    int noticeId;
    string title;
    string content;
    string date;
    string type; // "General", "Recruitment", "Urgent"

public:
    Notice() : noticeId(0), title(""), content(""), date(""), type("General") {}
    Notice(int nid, string t, string c, string d, string ty)
        : noticeId(nid), title(t), content(c), date(d), type(ty) {}

    ~Notice() {}

    int getNoticeId() const { return noticeId; }
    string getTitle() const { return title; }
    string getContent() const { return content; }
    string getDate() const { return date; }
    string getType() const { return type; }

    void display() const {
        cout << "\n========== NOTICE ==========\n";
        cout << "ID: " << noticeId << " [" << type << "]\n";
        cout << "Date: " << date << endl;
        cout << "Title: " << title << endl;
        cout << "Content: " << content << endl;
        cout << "============================\n";
    }

    void saveToFile(ofstream& out) const {
        out << noticeId << "|" << title << "|" << content << "|"
            << date << "|" << type << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string line;
        if (getline(in, line)) {
            size_t pos = 0;
            string delimiter = "|";
            vector<string> tokens;
            while ((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            if (tokens.size() >= 5) {
                noticeId = stoi(tokens[0]);
                title = tokens[1];
                content = tokens[2];
                date = tokens[3];
                type = tokens[4];
                return true;
            }
        }
        return false;
    }
};

// Candidate class for recruitment
class Candidate {
private:
    int candidateId;
    string name;
    string email;
    string phone;
    string position;
    string status; // "Applied", "Interview Scheduled", "Selected", "Rejected"
    string appliedDate;
    string interviewDate;

public:
    Candidate() : candidateId(0), status("Applied") {}
    Candidate(int cid, string n, string e, string p, string pos, string ad)
        : candidateId(cid), name(n), email(e), phone(p), position(pos),
          status("Applied"), appliedDate(ad), interviewDate("") {}

    ~Candidate() {}

    // Copy constructor
    Candidate(const Candidate& c) {
        candidateId = c.candidateId;
        name = c.name;
        email = c.email;
        phone = c.phone;
        position = c.position;
        status = c.status;
        appliedDate = c.appliedDate;
        interviewDate = c.interviewDate;
    }
// Assignment operator
    Candidate& operator=(const Candidate& c) {
        if (this != &c) {
            candidateId = c.candidateId;
            name = c.name;
            email = c.email;
            phone = c.phone;
            position = c.position;
            status = c.status;
            appliedDate = c.appliedDate;
            interviewDate = c.interviewDate;
        }
        return *this;
    }
    int getCandidateId() const { return candidateId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getPosition() const { return position; }
    string getStatus() const { return status; }
    string getAppliedDate() const { return appliedDate; }
    string getInterviewDate() const { return interviewDate; }

    void setStatus(string s) { status = s; }
    void setInterviewDate(string d) { interviewDate = d; }

    void display() const {
        cout << "\n========== CANDIDATE DETAILS ==========\n";
        cout << "ID: " << candidateId << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Position Applied: " << position << endl;
        cout << "Status: " << status << endl;
        cout << "Applied Date: " << appliedDate << endl;
        if (interviewDate != "") {
            cout << "Interview Date: " << interviewDate << endl;
        }
        cout << "=======================================\n";
    }

    void saveToFile(ofstream& out) const {
        out << candidateId << "|" << name << "|" << email << "|" << phone << "|"
            << position << "|" << status << "|" << appliedDate << "|"
            << interviewDate << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string line;
        if (getline(in, line)) {
            size_t pos = 0;
            string delimiter = "|";
            vector<string> tokens;
            while ((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            if (tokens.size() >= 8) {
                candidateId = stoi(tokens[0]);
                name = tokens[1];
                email = tokens[2];
                phone = tokens[3];
                position = tokens[4];
                status = tokens[5];
                appliedDate = tokens[6];
                interviewDate = tokens[7];
                return true;
            }
        }
        return false;
    }
};

// Leave Request class
class LeaveRequest {
private:
    int requestId;
    int employeeId;
    string employeeName;
    string startDate;
    string endDate;
    string reason;
    string status; // "Pending", "Approved", "Rejected"
    string requestDate;

public:
    LeaveRequest() : requestId(0), employeeId(0), status("Pending") {}
    LeaveRequest(int rid, int eid, string ename, string sd, string ed, string r, string rd)
        : requestId(rid), employeeId(eid), employeeName(ename), startDate(sd),
          endDate(ed), reason(r), status("Pending"), requestDate(rd) {}

    ~LeaveRequest() {}

    int getRequestId() const { return requestId; }
    int getEmployeeId() const { return employeeId; }
    string getEmployeeName() const { return employeeName; }
    string getStartDate() const { return startDate; }
    string getEndDate() const { return endDate; }
    string getReason() const { return reason; }
    string getStatus() const { return status; }
    string getRequestDate() const { return requestDate; }

    void setStatus(string s) { status = s; }

    void display() const {
        cout << "\n========== LEAVE REQUEST ==========\n";
        cout << "Request ID: " << requestId << endl;
        cout << "Employee: " << employeeName << " (ID: " << employeeId << ")" << endl;
        cout << "From: " << startDate << " To: " << endDate << endl;
        cout << "Reason: " << reason << endl;
        cout << "Status: " << status << endl;
        cout << "Request Date: " << requestDate << endl;
        cout << "===================================\n";
    }

    void saveToFile(ofstream& out) const {
        out << requestId << "|" << employeeId << "|" << employeeName << "|"
            << startDate << "|" << endDate << "|" << reason << "|"
            << status << "|" << requestDate << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string line;
        if (getline(in, line)) {
            size_t pos = 0;
            string delimiter = "|";
            vector<string> tokens;
            while ((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            if (tokens.size() >= 8) {
                requestId = stoi(tokens[0]);
                employeeId = stoi(tokens[1]);
                employeeName = tokens[2];
                startDate = tokens[3];
                endDate = tokens[4];
                reason = tokens[5];
                status = tokens[6];
                requestDate = tokens[7];
                return true;
            }
        }
        return false;
    }
};

// Template class for generic data storage
template <typename T>
class DataStorage {
private:
    vector<T> items;

public:
    DataStorage() {}
    ~DataStorage() {
        items.clear();
    }

    void addItem(const T& item) {
        items.push_back(item);
    }

    T* findById(int id) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].getId() == id) {
                return &items[i];
            }
        }
        return nullptr;
    }

    vector<T>& getAllItems() {
        return items;
    }

    bool removeById(int id) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].getId() == id) {
                items.erase(items.begin() + i);
                return true;
            }
        }
        return false;
    }

    int getCount() const {
        return items.size();
    }

    void clearAll() {
        items.clear();
    }
};

#endif
