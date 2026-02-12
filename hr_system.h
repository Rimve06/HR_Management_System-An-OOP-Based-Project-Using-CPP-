#ifndef HR_SYSTEM_H
#define HR_SYSTEM_H

#include "employee.h"
#include "hrmanager.h"
#include <map>
#include <ctime>

class HRSystem {
private:
    vector<Employee> employees;
    string hrPassword; // Single fixed HR password
    vector<Project> projects;
    vector<Attendance> attendanceRecords;
    vector<Notice> notices;
    vector<Candidate> candidates;
    vector<LeaveRequest> leaveRequests;
    map<int, Performance> performanceMap;

    int nextEmployeeId;
    int nextProjectId;
    int nextNoticeId;
    int nextCandidateId;
    int nextLeaveRequestId;

    const string EMPLOYEE_FILE = "employees.dat";
    const string HR_FILE = "hr_password.dat";
    const string PROJECT_FILE = "projects.dat";
    const string ATTENDANCE_FILE = "attendance.dat";
    const string NOTICE_FILE = "notices.dat";
    const string CANDIDATE_FILE = "candidates.dat";
    const string LEAVE_FILE = "leave_requests.dat";
    const string PERFORMANCE_FILE = "performance.dat";

public:
    HRSystem() : hrPassword(""), nextEmployeeId(1001), nextProjectId(2001),
                 nextNoticeId(3001), nextCandidateId(4001), nextLeaveRequestId(5001) {
        loadAllData();
    }

    ~HRSystem() {
        saveAllData();
    }

    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d", ltm);
        return string(buffer);
    }

    string getCurrentDateTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", ltm);
        return string(buffer);
    }

    bool isValidEmail(const string& email) {
        regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        return regex_match(email, pattern);
    }

    bool isValidPhone(const string& phone) {
        return phone.length() >= 10 && regex_match(phone, regex("^[0-9+\\-() ]+$"));
    }

    bool isValidName(const string& name) {
        return name.length() >= 2 && regex_match(name, regex("^[A-Za-z ]+$"));
    }

    // HR Authentication - Single password only
    bool authenticateHR(string password) {
        if (hrPassword.empty()) {
            return false;
        }
        return hrPassword == password;
    }

    bool isHRPasswordSet() {
        return !hrPassword.empty();
    }

    void setHRPassword(string password) {
        hrPassword = password;
        saveHRPassword();
        cout << "\nHR Password set successfully!\n";
    }

    // Employee Management
    Employee* authenticateEmployee(int id, string password) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i].getId() == id && employees[i].getPassword() == password) {
                return &employees[i];
            }
        }
        return nullptr;
    }

    void registerEmployee(string name, string email, string phone, string dept,
                         string position, double salary, string password) {
        if (!isValidName(name)) {
            cout << "Error: Invalid name format!\n";
            return;
        }
        if (!isValidEmail(email)) {
            cout << "Error: Invalid email format!\n";
            return;
        }
        if (!isValidPhone(phone)) {
            cout << "Error: Invalid phone format!\n";
            return;
        }

        Employee emp(name, email, phone, nextEmployeeId++, dept, position,
                     salary, getCurrentDate(), password);
        employees.push_back(emp);
        saveEmployees();
        cout << "\nEmployee registered successfully! ID: " << emp.getId() << endl;
    }

    Employee* findEmployeeById(int id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i].getId() == id) {
                return &employees[i];
            }
        }
        return nullptr;
    }

    void displayAllEmployees() {
        if (employees.empty()) {
            cout << "\nNo employees found.\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "          ALL EMPLOYEES LIST\n";
        cout << "========================================\n";
        cout << left << setw(8) << "ID" << setw(20) << "Name"
             << setw(15) << "Department" << setw(15) << "Position"
             << setw(12) << "Salary" << endl;
        cout << "----------------------------------------\n";

        for (const auto& emp : employees) {
            cout << left << setw(8) << emp.getId()
                 << setw(20) << emp.getName()
                 << setw(15) << emp.getDepartment()
                 << setw(15) << emp.getPosition()
                 << "$" << setw(11) << fixed << setprecision(2) << emp.getSalary() << endl;
        }
        cout << "========================================\n";
    }

    // Project Management
    void assignProject(int empId, string title, string description,
                      string deadline, string projectType) {
        Employee* emp = findEmployeeById(empId);
        if (!emp) {
            cout << "Employee not found!\n";
            return;
        }

        Project proj(nextProjectId, title, description, emp->getName(),
                    empId, deadline, projectType);
        projects.push_back(proj);
        emp->addProjectId(nextProjectId);

        saveProjects();
        saveEmployees();

        cout << "\nProject assigned successfully! Project ID: " << nextProjectId << endl;
        nextProjectId++;
    }

    Project* findProjectById(int pid) {
        for (size_t i = 0; i < projects.size(); i++) {
            if (projects[i].getProjectId() == pid) {
                return &projects[i];
            }
        }
        return nullptr;
    }

    vector<Project> getEmployeeProjects(int empId) {
        vector<Project> empProjects;
        for (const auto& proj : projects) {
            if (proj.getEmployeeId() == empId) {
                empProjects.push_back(proj);
            }
        }
        return empProjects;
    }

    vector<Project> getSubmittedProjects() {
        vector<Project> submitted;
        for (const auto& proj : projects) {
            if (proj.getStatus() == "Submitted") {
                submitted.push_back(proj);
            }
        }
        return submitted;
    }

    vector<Project> getCompletedProjects() {
        vector<Project> completed;
        for (const auto& proj : projects) {
            if (proj.getStatus() == "Completed") {
                completed.push_back(proj);
            }
        }
        return completed;
    }

    void submitProject(int projectId, string content) {
        Project* proj = findProjectById(projectId);
        if (!proj) {
            cout << "Project not found!\n";
            return;
        }

        proj->setSubmissionContent(content);
        proj->setSubmissionDate(getCurrentDateTime());
        proj->setStatus("Submitted");
        saveProjects();
        cout << "\nProject submitted successfully!\n";
    }

    void provideFeedback(int projectId, string feedback) {
        Project* proj = findProjectById(projectId);
        if (!proj) {
            cout << "Project not found!\n";
            return;
        }

        proj->setFeedback(feedback);
        proj->setStatus("Feedback");
        saveProjects();
        cout << "\nFeedback sent to employee!\n";
    }

    void acceptProject(int projectId) {
        Project* proj = findProjectById(projectId);
        if (!proj) {
            cout << "Project not found!\n";
            return;
        }

        proj->setStatus("Completed");
        saveProjects();
        cout << "\nProject accepted and marked as completed!\n";
    }

    void displayAllProjects() {
        if (projects.empty()) {
            cout << "\nNo projects found.\n";
            return;
        }

        cout << "\n================================================\n";
        cout << "              ALL PROJECTS RECORDS\n";
        cout << "================================================\n";
        cout << left << setw(8) << "Proj ID" << setw(25) << "Title"
             << setw(20) << "Assigned To" << setw(12) << "Deadline"
             << setw(12) << "Status" << endl;
        cout << "------------------------------------------------\n";

        for (const auto& proj : projects) {
            cout << left << setw(8) << proj.getProjectId()
                 << setw(25) << proj.getTitle()
                 << setw(20) << proj.getAssignedTo()
                 << setw(12) << proj.getDeadline()
                 << setw(12) << proj.getStatus() << endl;
        }
        cout << "================================================\n";
    }

    void emailProject(int projectId, string recipientEmail) {
        Project* proj = findProjectById(projectId);
        if (!proj) {
            cout << "Project not found!\n";
            return;
        }

        if (!isValidEmail(recipientEmail)) {
            cout << "Invalid email format!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "     EMAIL SENT SUCCESSFULLY!\n";
        cout << "========================================\n";
        cout << "To: " << recipientEmail << endl;
        cout << "Subject: Project Submission - " << proj->getTitle() << endl;
        cout << "Project ID: " << proj->getProjectId() << endl;
        cout << "Status: " << proj->getStatus() << endl;
        cout << "Submitted by: " << proj->getAssignedTo() << endl;
        cout << "========================================\n";
    }

    // Attendance Management
    void markAttendance(int empId, bool present, int overtimeHours) {
        Attendance att(empId, getCurrentDate(), present, overtimeHours);
        attendanceRecords.push_back(att);
        saveAttendance();
        cout << "\nAttendance marked successfully!\n";
    }

    double calculateAttendancePercentage(int empId) {
        int totalDays = 0;
        int presentDays = 0;

        for (const auto& att : attendanceRecords) {
            if (att.getEmployeeId() == empId) {
                totalDays++;
                if (att.isPresent()) presentDays++;
            }
        }

        if (totalDays == 0) return 0.0;
        return (double)presentDays / totalDays * 100.0;
    }

    int getTotalOvertimeHours(int empId) {
        int total = 0;
        for (const auto& att : attendanceRecords) {
            if (att.getEmployeeId() == empId) {
                total += att.getOvertimeHours();
            }
        }
        return total;
    }

    // Performance Management
    void calculateAndUpdatePerformance(int empId) {
        double attPercentage = calculateAttendancePercentage(empId);
        int overtimeHours = getTotalOvertimeHours(empId);

        int completedProjects = 0;
        int onTimeSubmissions = 0;

        for (const auto& proj : projects) {
            if (proj.getEmployeeId() == empId && proj.getStatus() == "Completed") {
                completedProjects++;
                if (proj.getSubmissionDate() <= proj.getDeadline()) {
                    onTimeSubmissions++;
                }
            }
        }

        Performance perf(empId, attPercentage, overtimeHours,
                        completedProjects, onTimeSubmissions);
        performanceMap[empId] = perf;
        savePerformance();
    }

    Performance* getEmployeePerformance(int empId) {
        if (performanceMap.find(empId) != performanceMap.end()) {
            return &performanceMap[empId];
        }
        return nullptr;
    }

    void displayPerformanceScoreboard() {
        cout << "\n==============================================\n";
        cout << "         PERFORMANCE SCOREBOARD\n";
        cout << "==============================================\n";

        for (auto& emp : employees) {
            calculateAndUpdatePerformance(emp.getId());
        }

        vector<pair<int, Performance>> perfList;
        for (const auto& entry : performanceMap) {
            perfList.push_back(entry);
        }

        sort(perfList.begin(), perfList.end(),
             [](const pair<int, Performance>& a, const pair<int, Performance>& b) {
                 return a.second.getPerformanceScore() > b.second.getPerformanceScore();
             });

        cout << left << setw(6) << "Rank" << setw(8) << "Emp ID"
             << setw(20) << "Name" << setw(10) << "Score"
             << setw(8) << "Grade" << endl;
        cout << "----------------------------------------------\n";

        int rank = 1;
        for (const auto& entry : perfList) {
            Employee* emp = findEmployeeById(entry.first);
            if (emp) {
                cout << left << setw(6) << rank++
                     << setw(8) << emp->getId()
                     << setw(20) << emp->getName()
                     << setw(10) << fixed << setprecision(2)
                     << entry.second.getPerformanceScore()
                     << setw(8) << entry.second.getPerformanceGrade() << endl;
            }
        }
        cout << "==============================================\n";
    }

    void yearEndEvaluation() {
        cout << "\n========================================\n";
        cout << "     YEAR-END EVALUATION REPORT\n";
        cout << "========================================\n";

        for (auto& emp : employees) {
            calculateAndUpdatePerformance(emp.getId());
            Performance* perf = getEmployeePerformance(emp.getId());

            if (!perf) continue;

            cout << "\n--- Employee: " << emp.getName() << " (ID: " << emp.getId() << ") ---\n";
            cout << "Current Position: " << emp.getPosition() << endl;
            cout << "Current Salary: $" << fixed << setprecision(2) << emp.getSalary() << endl;
            cout << "Performance Score: " << perf->getPerformanceScore() << " (Grade: "
                 << perf->getPerformanceGrade() << ")\n";

            double salaryIncrease = 0;
            string action = "";

            if (perf->getPerformanceScore() >= 90) {
                salaryIncrease = emp.getSalary() * 0.20;
                action = "PROMOTION + 20% Salary Increase";
            } else if (perf->getPerformanceScore() >= 85) {
                salaryIncrease = emp.getSalary() * 0.15;
                action = "15% Salary Increase";
            } else if (perf->getPerformanceScore() >= 75) {
                salaryIncrease = emp.getSalary() * 0.10;
                action = "10% Salary Increase";
            } else if (perf->getPerformanceScore() >= 65) {
                salaryIncrease = emp.getSalary() * 0.05;
                action = "5% Salary Increase";
            } else if (perf->getPerformanceScore() >= 50) {
                action = "No Salary Change";
            } else {
                salaryIncrease = -(emp.getSalary() * 0.10);
                action = "DEMOTION - 10% Salary Decrease";
            }

            emp += salaryIncrease;

            cout << "Action: " << action << endl;
            cout << "New Salary: $" << fixed << setprecision(2) << emp.getSalary() << endl;
            cout << "----------------------------------------\n";
        }

        saveEmployees();
        cout << "\nYear-end evaluation completed and saved!\n";
    }

    // Notice Management
    void postNotice(string title, string content, string type) {
        Notice notice(nextNoticeId++, title, content, getCurrentDate(), type);
        notices.push_back(notice);
        saveNotices();
        cout << "\nNotice posted successfully!\n";
    }

    void displayAllNotices() {
        if (notices.empty()) {
            cout << "\nNo notices available.\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "           NOTICE BOARD\n";
        cout << "========================================\n";
        for (const auto& notice : notices) {
            notice.display();
        }
    }

    vector<Notice> getNoticesByType(string type) {
        vector<Notice> filtered;
        for (const auto& notice : notices) {
            if (notice.getType() == type) {
                filtered.push_back(notice);
            }
        }
        return filtered;
    }

    // Recruitment Management
    void addCandidate(string name, string email, string phone, string position) {
        if (!isValidName(name) || !isValidEmail(email) || !isValidPhone(phone)) {
            cout << "Invalid candidate details!\n";
            return;
        }

        Candidate candidate(nextCandidateId++, name, email, phone,
                          position, getCurrentDate());
        candidates.push_back(candidate);
        saveCandidates();
        cout << "\nCandidate added successfully! ID: " << candidate.getCandidateId() << endl;
    }

    Candidate* findCandidateById(int cid) {
        for (size_t i = 0; i < candidates.size(); i++) {
            if (candidates[i].getCandidateId() == cid) {
                return &candidates[i];
            }
        }
        return nullptr;
    }

    void updateCandidateStatus(int candidateId, string newStatus, string interviewDate = "") {
        Candidate* candidate = findCandidateById(candidateId);
        if (!candidate) {
            cout << "Candidate not found!\n";
            return;
        }

        candidate->setStatus(newStatus);
        if (interviewDate != "") {
            candidate->setInterviewDate(interviewDate);
        }
        saveCandidates();
        cout << "\nCandidate status updated!\n";
    }

    void displayAllCandidates() {
        if (candidates.empty()) {
            cout << "\nNo candidates found.\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "          CANDIDATE LIST\n";
        cout << "========================================\n";
        for (const auto& candidate : candidates) {
            candidate.display();
        }
    }

    vector<Candidate> getCandidatesByStatus(string status) {
        vector<Candidate> filtered;
        for (const auto& candidate : candidates) {
            if (candidate.getStatus() == status) {
                filtered.push_back(candidate);
            }
        }
        return filtered;
    }

    // Leave Management
    void submitLeaveRequest(int empId, string startDate, string endDate, string reason) {
        Employee* emp = findEmployeeById(empId);
        if (!emp) {
            cout << "Employee not found!\n";
            return;
        }

        LeaveRequest leave(nextLeaveRequestId++, empId, emp->getName(),
                          startDate, endDate, reason, getCurrentDate());
        leaveRequests.push_back(leave);
        saveLeaveRequests();
        cout << "\nLeave request submitted successfully!\n";
    }

    LeaveRequest* findLeaveRequestById(int rid) {
        for (size_t i = 0; i < leaveRequests.size(); i++) {
            if (leaveRequests[i].getRequestId() == rid) {
                return &leaveRequests[i];
            }
        }
        return nullptr;
    }

    void processLeaveRequest(int requestId, string decision) {
        LeaveRequest* leave = findLeaveRequestById(requestId);
        if (!leave) {
            cout << "Leave request not found!\n";
            return;
        }

        leave->setStatus(decision);
        saveLeaveRequests();
        cout << "\nLeave request " << decision << "!\n";
    }

    vector<LeaveRequest> getPendingLeaveRequests() {
        vector<LeaveRequest> pending;
        for (const auto& leave : leaveRequests) {
            if (leave.getStatus() == "Pending") {
                pending.push_back(leave);
            }
        }
        return pending;
    }

    // File Handling Functions
    void saveAllData() {
        saveEmployees();
        saveHRPassword();
        saveProjects();
        saveAttendance();
        saveNotices();
        saveCandidates();
        saveLeaveRequests();
        savePerformance();
    }

    void loadAllData() {
        loadHRPassword();
        loadEmployees();
        loadProjects();
        loadAttendance();
        loadNotices();
        loadCandidates();
        loadLeaveRequests();
        loadPerformance();
    }

    void saveHRPassword() {
        ofstream out(HR_FILE);
        if (out.is_open()) {
            out << hrPassword << "\n";
            out.close();
        }
    }

    void loadHRPassword() {
        ifstream in(HR_FILE);
        if (in.is_open()) {
            getline(in, hrPassword);
            in.close();
        }
    }

    void saveEmployees() {
        ofstream out(EMPLOYEE_FILE);
        if (out.is_open()) {
            for (const auto& emp : employees) {
                emp.saveToFile(out);
            }
            out.close();
        }
    }

    void loadEmployees() {
        ifstream in(EMPLOYEE_FILE);
        if (in.is_open()) {
            employees.clear();
            Employee emp;
            while (emp.loadFromFile(in)) {
                employees.push_back(emp);
                if (emp.getId() >= nextEmployeeId) {
                    nextEmployeeId = emp.getId() + 1;
                }
            }
            in.close();
        }
    }

    void saveProjects() {
        ofstream out(PROJECT_FILE);
        if (out.is_open()) {
            for (const auto& proj : projects) {
                proj.saveToFile(out);
            }
            out.close();
        }
    }

    void loadProjects() {
        ifstream in(PROJECT_FILE);
        if (in.is_open()) {
            projects.clear();
            Project proj;
            while (proj.loadFromFile(in)) {
                projects.push_back(proj);
                if (proj.getProjectId() >= nextProjectId) {
                    nextProjectId = proj.getProjectId() + 1;
                }
            }
            in.close();
        }
    }

    void saveAttendance() {
        ofstream out(ATTENDANCE_FILE);
        if (out.is_open()) {
            for (const auto& att : attendanceRecords) {
                att.saveToFile(out);
            }
            out.close();
        }
    }

    void loadAttendance() {
        ifstream in(ATTENDANCE_FILE);
        if (in.is_open()) {
            attendanceRecords.clear();
            Attendance att;
            while (att.loadFromFile(in)) {
                attendanceRecords.push_back(att);
            }
            in.close();
        }
    }

    void saveNotices() {
        ofstream out(NOTICE_FILE);
        if (out.is_open()) {
            for (const auto& notice : notices) {
                notice.saveToFile(out);
            }
            out.close();
        }
    }

    void loadNotices() {
        ifstream in(NOTICE_FILE);
        if (in.is_open()) {
            notices.clear();
            Notice notice;
            while (notice.loadFromFile(in)) {
                notices.push_back(notice);
                if (notice.getNoticeId() >= nextNoticeId) {
                    nextNoticeId = notice.getNoticeId() + 1;
                }
            }
            in.close();
        }
    }

    void saveCandidates() {
        ofstream out(CANDIDATE_FILE);
        if (out.is_open()) {
            for (const auto& candidate : candidates) {
                candidate.saveToFile(out);
            }
            out.close();
        }
    }

    void loadCandidates() {
        ifstream in(CANDIDATE_FILE);
        if (in.is_open()) {
            candidates.clear();
            Candidate candidate;
            while (candidate.loadFromFile(in)) {
                candidates.push_back(candidate);
                if (candidate.getCandidateId() >= nextCandidateId) {
                    nextCandidateId = candidate.getCandidateId() + 1;
                }
            }
            in.close();
        }
    }

    void saveLeaveRequests() {
        ofstream out(LEAVE_FILE);
        if (out.is_open()) {
            for (const auto& leave : leaveRequests) {
                leave.saveToFile(out);
            }
            out.close();
        }
    }

    void loadLeaveRequests() {
        ifstream in(LEAVE_FILE);
        if (in.is_open()) {
            leaveRequests.clear();
            LeaveRequest leave;
            while (leave.loadFromFile(in)) {
                leaveRequests.push_back(leave);
                if (leave.getRequestId() >= nextLeaveRequestId) {
                    nextLeaveRequestId = leave.getRequestId() + 1;
                }
            }
            in.close();
        }
    }

    void savePerformance() {
        ofstream out(PERFORMANCE_FILE);
        if (out.is_open()) {
            for (const auto& entry : performanceMap) {
                entry.second.saveToFile(out);
            }
            out.close();
        }
    }

    void loadPerformance() {
        ifstream in(PERFORMANCE_FILE);
        if (in.is_open()) {
            performanceMap.clear();
            Performance perf;
            while (perf.loadFromFile(in)) {
                performanceMap[perf.getEmployeeId()] = perf;
            }
            in.close();
        }
    }
};

#endif
