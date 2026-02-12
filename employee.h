#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <regex>
using namespace std;

// Forward declarations
class Project;
class Attendance;
class Performance;

// Base Person class
class Person {
protected:
    string name;
    string email;
    string phone;
    int id;
public:
    Person() : name(""), email(""), phone(""), id(0) {}
    Person(string n, string e, string p, int i) : name(n), email(e), phone(p), id(i) {}
    virtual ~Person() { cout << "Person destructor called for: " << name << endl; }

    virtual void display() const = 0; // Pure virtual
    Person(const Person& p) : name(p.name), email(p.email),
                              phone(p.phone), id(p.id) {}

    // Assignment operator
    Person& operator=(const Person& p) {
        if (this != &p) {
            name = p.name;
            email = p.email;
            phone = p.phone;
            id = p.id;
        }
        return *this;
    }
    // Getters
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    int getId() const { return id; }

    // Setters with validation
    bool setName(string n) {
        if (n.length() >= 2 && regex_match(n, regex("^[A-Za-z ]+$"))) {
            name = n;
            return true;
        }
        return false;
    }

    bool setEmail(string e) {
        regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (regex_match(e, pattern)) {
            email = e;
            return true;
        }
        return false;
    }

    bool setPhone(string p) {
        if (p.length() >= 10 && regex_match(p, regex("^[0-9+\\-() ]+$"))) {
            phone = p;
            return true;
        }
        return false;
    }
};

// Project class
class Project {
private:
    int projectId;
    string title;
    string description;
    string assignedTo;
    int employeeId;
    string deadline;
    string status; // "Assigned", "Submitted", "Feedback", "Completed"
    string feedback;
    string submissionContent;
    string submissionDate;
    string projectType; // "Minor", "Major", "Urgent", "Long-term"

public:
    Project() : projectId(0), employeeId(0), status("Assigned") {}
    Project(int pid, string t, string desc, string emp, int eid, string dl, string type)
        : projectId(pid), title(t), description(desc), assignedTo(emp),
          employeeId(eid), deadline(dl), status("Assigned"), projectType(type) {}

    ~Project() { /* cout << "Project destructor: " << title << endl; */ }

    // Copy constructor
    Project(const Project& p) {
        projectId = p.projectId;
        title = p.title;
        description = p.description;
        assignedTo = p.assignedTo;
        employeeId = p.employeeId;
        deadline = p.deadline;
        status = p.status;
        feedback = p.feedback;
        submissionContent = p.submissionContent;
        submissionDate = p.submissionDate;
        projectType = p.projectType;
    }

    // Assignment operator overloading
    Project& operator=(const Project& p) {
        if (this != &p) {
            projectId = p.projectId;
            title = p.title;
            description = p.description;
            assignedTo = p.assignedTo;
            employeeId = p.employeeId;
            deadline = p.deadline;
            status = p.status;
            feedback = p.feedback;
            submissionContent = p.submissionContent;
            submissionDate = p.submissionDate;
            projectType = p.projectType;
        }
        return *this;
    }

    // Getters
    int getProjectId() const { return projectId; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getAssignedTo() const { return assignedTo; }
    int getEmployeeId() const { return employeeId; }
    string getDeadline() const { return deadline; }
    string getStatus() const { return status; }
    string getFeedback() const { return feedback; }
    string getSubmissionContent() const { return submissionContent; }
    string getSubmissionDate() const { return submissionDate; }
    string getProjectType() const { return projectType; }

    // Setters
    void setStatus(string s) { status = s; }
    void setFeedback(string f) { feedback = f; }
    void setSubmissionContent(string s) { submissionContent = s; }
    void setSubmissionDate(string d) { submissionDate = d; }

    void display() const {
        cout << "\n========== PROJECT DETAILS ==========\n";
        cout << "Project ID: " << projectId << endl;
        cout << "Title: " << title << endl;
        cout << "Type: " << projectType << endl;
        cout << "Description: " << description << endl;
        cout << "Assigned To: " << assignedTo << " (ID: " << employeeId << ")" << endl;
        cout << "Deadline: " << deadline << endl;
        cout << "Status: " << status << endl;
        if (status == "Submitted" || status == "Feedback" || status == "Completed") {
            cout << "Submission Date: " << submissionDate << endl;
        }
        if (feedback != "") {
            cout << "Feedback: " << feedback << endl;
        }
        cout << "=====================================\n";
    }

    void saveToFile(ofstream& out) const {
        out << projectId << "|" << title << "|" << description << "|"
            << assignedTo << "|" << employeeId << "|" << deadline << "|"
            << status << "|" << feedback << "|" << submissionContent << "|"
            << submissionDate << "|" << projectType << "\n";
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

            if (tokens.size() >= 11) {
                projectId = stoi(tokens[0]);
                title = tokens[1];
                description = tokens[2];
                assignedTo = tokens[3];
                employeeId = stoi(tokens[4]);
                deadline = tokens[5];
                status = tokens[6];
                feedback = tokens[7];
                submissionContent = tokens[8];
                submissionDate = tokens[9];
                projectType = tokens[10];
                return true;
            }
        }
        return false;
    }
};

// Attendance class
class Attendance {
private:
    int employeeId;
    string date;
    bool present;
    int overtimeHours;

public:
    Attendance() : employeeId(0), present(false), overtimeHours(0) {}
    Attendance(int eid, string d, bool p, int ot)
        : employeeId(eid), date(d), present(p), overtimeHours(ot) {}

    ~Attendance() {}

    int getEmployeeId() const { return employeeId; }
    string getDate() const { return date; }
    bool isPresent() const { return present; }
    int getOvertimeHours() const { return overtimeHours; }

    void saveToFile(ofstream& out) const {
        out << employeeId << "|" << date << "|" << present << "|" << overtimeHours << "\n";
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

            if (tokens.size() >= 4) {
                employeeId = stoi(tokens[0]);
                date = tokens[1];
                present = (tokens[2] == "1");
                overtimeHours = stoi(tokens[3]);
                return true;
            }
        }
        return false;
    }
};

// Performance class
class Performance {
private:
    int employeeId;
    double attendancePercentage;
    int totalOvertimeHours;
    int projectsCompleted;
    int onTimeSubmissions;
    double performanceScore;
    string performanceGrade; // A+, A, B+, B, C, D

public:
    Performance() : employeeId(0), attendancePercentage(0), totalOvertimeHours(0),
                    projectsCompleted(0), onTimeSubmissions(0), performanceScore(0),
                    performanceGrade("N/A") {}

    Performance(int eid, double att, int ot, int pc, int ontime)
        : employeeId(eid), attendancePercentage(att), totalOvertimeHours(ot),
          projectsCompleted(pc), onTimeSubmissions(ontime) {
        calculatePerformance();
    }

    ~Performance() {}

    void calculatePerformance() {
        // Weight: Attendance(40%) + Projects(30%) + OnTime(20%) + Overtime(10%)
        double attScore = (attendancePercentage / 100.0) * 40;
        double projScore = (projectsCompleted * 5 > 30) ? 30 : (projectsCompleted * 5);
        double onTimeScore = (onTimeSubmissions * 4 > 20) ? 20 : (onTimeSubmissions * 4);
        double otScore = (totalOvertimeHours * 0.5 > 10) ? 10 : (totalOvertimeHours * 0.5);

        performanceScore = attScore + projScore + onTimeScore + otScore;

        if (performanceScore >= 90) performanceGrade = "A+";
        else if (performanceScore >= 85) performanceGrade = "A";
        else if (performanceScore >= 80) performanceGrade = "B+";
        else if (performanceScore >= 70) performanceGrade = "B";
        else if (performanceScore >= 60) performanceGrade = "C";
        else performanceGrade = "D";
    }

    // Getters
    double getPerformanceScore() const { return performanceScore; }
    string getPerformanceGrade() const { return performanceGrade; }
    double getAttendancePercentage() const { return attendancePercentage; }
    int getTotalOvertimeHours() const { return totalOvertimeHours; }
    int getProjectsCompleted() const { return projectsCompleted; }
    int getOnTimeSubmissions() const { return onTimeSubmissions; }
    int getEmployeeId() const { return employeeId; }
    void display() const {
        cout << "\n===== PERFORMANCE REPORT =====\n";
        cout << "Attendance: " << fixed << setprecision(2) << attendancePercentage << "%\n";
        cout << "Total Overtime: " << totalOvertimeHours << " hours\n";
        cout << "Projects Completed: " << projectsCompleted << "\n";
        cout << "On-Time Submissions: " << onTimeSubmissions << "\n";
        cout << "Performance Score: " << performanceScore << "/100\n";
        cout << "Grade: " << performanceGrade << "\n";
        cout << "==============================\n";
    }

    void saveToFile(ofstream& out) const {
        out << employeeId << "|" << attendancePercentage << "|" << totalOvertimeHours << "|"
            << projectsCompleted << "|" << onTimeSubmissions << "|" << performanceScore << "|"
            << performanceGrade << "\n";
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

            if (tokens.size() >= 7) {
                employeeId = stoi(tokens[0]);
                attendancePercentage = stod(tokens[1]);
                totalOvertimeHours = stoi(tokens[2]);
                projectsCompleted = stoi(tokens[3]);
                onTimeSubmissions = stoi(tokens[4]);
                performanceScore = stod(tokens[5]);
                performanceGrade = tokens[6];
                return true;
            }
        }
        return false;
    }
};

// Employee class (inherits from Person)
class Employee : public Person {
private:
    string department;
    string position;
    double salary;
    string joiningDate;
    string password;
    vector<int> assignedProjectIds;

public:
    Employee() : Person(), department(""), position(""), salary(0), joiningDate("") {}

    Employee(string n, string e, string p, int i, string dept, string pos, double sal, string jd, string pass)
        : Person(n, e, p, i), department(dept), position(pos), salary(sal), joiningDate(jd), password(pass) {}

    ~Employee() {
        cout << "Employee destructor called for: " << name << endl;
    }

    // Copy constructor
    Employee(const Employee& emp) : Person(emp) {
        department = emp.department;
        position = emp.position;
        salary = emp.salary;
        joiningDate = emp.joiningDate;
        password = emp.password;
        assignedProjectIds = emp.assignedProjectIds;
    }

    // Assignment operator
    Employee& operator=(const Employee& emp) {
        if (this != &emp) {
            Person::operator=(emp);
            department = emp.department;
            position = emp.position;
            salary = emp.salary;
            joiningDate = emp.joiningDate;
            password = emp.password;
            assignedProjectIds = emp.assignedProjectIds;
        }
        return *this;
    }

    void display() const override {
        cout << "\n========== EMPLOYEE DETAILS ==========\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Department: " << department << endl;
        cout << "Position: " << position << endl;
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
        cout << "Joining Date: " << joiningDate << endl;
        cout << "======================================\n";
    }

    // Getters
    string getDepartment() const { return department; }
    string getPosition() const { return position; }
    double getSalary() const { return salary; }
    string getJoiningDate() const { return joiningDate; }
    string getPassword() const { return password; }
    vector<int> getAssignedProjectIds() const { return assignedProjectIds; }

    // Setters
    void setDepartment(string d) { department = d; }
    void setPosition(string p) { position = p; }
    void setSalary(double s) { salary = s; }
    void setPassword(string p) { password = p; }
    void addProjectId(int pid) { assignedProjectIds.push_back(pid); }

    // Operator overloading for salary increment
    Employee& operator+=(double increment) {
        salary += increment;
        return *this;
    }

    // Type conversion: Employee to double (returns salary)
    operator double() const {
        return salary;
    }

    void saveToFile(ofstream& out) const {
        out << id << "|" << name << "|" << email << "|" << phone << "|"
            << department << "|" << position << "|" << salary << "|"
            << joiningDate << "|" << password << "|";
        for (size_t i = 0; i < assignedProjectIds.size(); i++) {
            out << assignedProjectIds[i];
            if (i < assignedProjectIds.size() - 1) out << ",";
        }
        out << "\n";
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

            if (tokens.size() >= 10) {
                id = stoi(tokens[0]);
                name = tokens[1];
                email = tokens[2];
                phone = tokens[3];
                department = tokens[4];
                position = tokens[5];
                salary = stod(tokens[6]);
                joiningDate = tokens[7];
                password = tokens[8];

                if (tokens[9] != "") {
                    string projIds = tokens[9];
                    size_t p = 0;
                    while ((p = projIds.find(",")) != string::npos) {
                        assignedProjectIds.push_back(stoi(projIds.substr(0, p)));
                        projIds.erase(0, p + 1);
                    }
                    if (projIds != "") assignedProjectIds.push_back(stoi(projIds));
                }
                return true;
            }
        }
        return false;
    }
};

#endif
