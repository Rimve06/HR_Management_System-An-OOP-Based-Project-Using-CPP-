#include "hr_system.h"
#include <iostream>
#include <limits>
using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMainMenu() {
    cout << "\n========================================\n";
    cout << "   HR MANAGEMENT SYSTEM - MAIN MENU    \n";
    cout << "========================================\n";
    cout << "1. HR Manager Login\n";
    cout << "2. Employee Login\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void displayHRMenu() {
    cout << "\n========================================\n";
    cout << "        HR MANAGER DASHBOARD           \n";
    cout << "========================================\n";
    cout << "1.  Register New Employee\n";
    cout << "2.  View All Employees\n";
    cout << "3.  Assign Project to Employee\n";
    cout << "4.  View All Projects\n";
    cout << "5.  Review Submitted Projects\n";
    cout << "6.  Mark Employee Attendance\n";
    cout << "7.  View Performance Scoreboard\n";
    cout << "8.  Year-End Evaluation\n";
    cout << "9.  Post Notice\n";
    cout << "10. View All Notices\n";
    cout << "11. Recruitment Management\n";
    cout << "12. Leave Request Management\n";
    cout << "13. Email Project Details\n";
    cout << "14. Logout\n";
    cout << "Enter your choice: ";
}

void displayEmployeeMenu() {
    cout << "\n========================================\n";
    cout << "         EMPLOYEE DASHBOARD            \n";
    cout << "========================================\n";
    cout << "1. View My Profile\n";
    cout << "2. View My Projects\n";
    cout << "3. Submit Project\n";
    cout << "4. View My Performance\n";
    cout << "5. View Notices\n";
    cout << "6. Submit Leave Request\n";
    cout << "7. Logout\n";
    cout << "Enter your choice: ";
}

void hrManagerSession(HRSystem& system) {
    int choice;
    do {
        clearScreen();
        displayHRMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: { // Register Employee
                string name, email, phone, dept, position, password;
                double salary;

                cout << "\n=== REGISTER NEW EMPLOYEE ===\n";
                cout << "Name: "; getline(cin, name);
                cout << "Email: "; getline(cin, email);
                cout << "Phone: "; getline(cin, phone);
                cout << "Department: "; getline(cin, dept);
                cout << "Position: "; getline(cin, position);
                cout << "Salary: $"; cin >> salary;
                cin.ignore();
                cout << "Password: "; getline(cin, password);

                system.registerEmployee(name, email, phone, dept, position, salary, password);
                pauseScreen();
                break;
            }

            case 2: { // View All Employees
                system.displayAllEmployees();
                pauseScreen();
                break;
            }

            case 3: { // Assign Project
                int empId;
                string title, description, deadline, projectType;

                cout << "\n=== ASSIGN PROJECT ===\n";
                cout << "Employee ID: "; cin >> empId;
                cin.ignore();
                cout << "Project Title: "; getline(cin, title);
                cout << "Description: "; getline(cin, description);
                cout << "Deadline (YYYY-MM-DD): "; getline(cin, deadline);
                cout << "Project Type (Minor/Major/Urgent/Long-term): "; getline(cin, projectType);

                system.assignProject(empId, title, description, deadline, projectType);
                pauseScreen();
                break;
            }

            case 4: { // View All Projects
                system.displayAllProjects();
                pauseScreen();
                break;
            }

            case 5: { // Review Submitted Projects
                vector<Project> submitted = system.getSubmittedProjects();
                if (submitted.empty()) {
                    cout << "\nNo submitted projects to review.\n";
                } else {
                    for (const auto& proj : submitted) {
                        proj.display();
                        cout << "\nActions:\n";
                        cout << "1. Provide Feedback\n";
                        cout << "2. Accept Project\n";
                        cout << "3. Skip\n";
                        cout << "Choice: ";
                        int action;
                        cin >> action;
                        cin.ignore();

                        if (action == 1) {
                            string feedback;
                            cout << "Enter feedback: ";
                            getline(cin, feedback);
                            system.provideFeedback(proj.getProjectId(), feedback);
                        } else if (action == 2) {
                            system.acceptProject(proj.getProjectId());
                        }
                    }
                }
                pauseScreen();
                break;
            }

            case 6: { // Mark Attendance
                int empId, overtime;
                char present;

                cout << "\n=== MARK ATTENDANCE ===\n";
                cout << "Employee ID: "; cin >> empId;
                cout << "Present? (y/n): "; cin >> present;
                cout << "Overtime Hours: "; cin >> overtime;

                system.markAttendance(empId, (present == 'y' || present == 'Y'), overtime);
                pauseScreen();
                break;
            }

            case 7: { // Performance Scoreboard
                system.displayPerformanceScoreboard();
                pauseScreen();
                break;
            }

            case 8: { // Year-End Evaluation
                cout << "\nWARNING: This will update salaries based on performance!\n";
                cout << "Continue? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    system.yearEndEvaluation();
                }
                pauseScreen();
                break;
            }

            case 9: { // Post Notice
                string title, content, type;

                cout << "\n=== POST NOTICE ===\n";
                cout << "Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Content: "; getline(cin, content);
                cout << "Type (General/Recruitment/Urgent): "; getline(cin, type);

                system.postNotice(title, content, type);
                pauseScreen();
                break;
            }

            case 10: { // View Notices
                system.displayAllNotices();
                pauseScreen();
                break;
            }

            case 11: { // Recruitment Management
                cout << "\n=== RECRUITMENT MANAGEMENT ===\n";
                cout << "1. Add Candidate\n";
                cout << "2. View All Candidates\n";
                cout << "3. Update Candidate Status\n";
                cout << "Choice: ";
                int subChoice;
                cin >> subChoice;
                cin.ignore();

                if (subChoice == 1) {
                    string name, email, phone, position;
                    cout << "Candidate Name: "; getline(cin, name);
                    cout << "Email: "; getline(cin, email);
                    cout << "Phone: "; getline(cin, phone);
                    cout << "Position: "; getline(cin, position);
                    system.addCandidate(name, email, phone, position);
                } else if (subChoice == 2) {
                    system.displayAllCandidates();
                } else if (subChoice == 3) {
                    int candId;
                    string status, interviewDate;
                    cout << "Candidate ID: "; cin >> candId;
                    cin.ignore();
                    cout << "New Status (Applied/Interview Scheduled/Selected/Rejected): ";
                    getline(cin, status);
                    if (status == "Interview Scheduled") {
                        cout << "Interview Date (YYYY-MM-DD): ";
                        getline(cin, interviewDate);
                    }
                    system.updateCandidateStatus(candId, status, interviewDate);
                }
                pauseScreen();
                break;
            }

            case 12: { // Leave Management
                vector<LeaveRequest> pending = system.getPendingLeaveRequests();
                if (pending.empty()) {
                    cout << "\nNo pending leave requests.\n";
                } else {
                    for (const auto& leave : pending) {
                        leave.display();
                        cout << "\nApprove or Reject? (a/r/s=skip): ";
                        char decision;
                        cin >> decision;
                        if (decision == 'a' || decision == 'A') {
                            system.processLeaveRequest(leave.getRequestId(), "Approved");
                        } else if (decision == 'r' || decision == 'R') {
                            system.processLeaveRequest(leave.getRequestId(), "Rejected");
                        }
                    }
                }
                pauseScreen();
                break;
            }

            case 13: { // Email Project
                int projectId;
                string email;

                cout << "\n=== EMAIL PROJECT DETAILS ===\n";
                cout << "Project ID: "; cin >> projectId;
                cin.ignore();
                cout << "Recipient Email: "; getline(cin, email);

                system.emailProject(projectId, email);
                pauseScreen();
                break;
            }

            case 14: { // Logout
                cout << "\nLogging out...\n";
                break;
            }

            default:
                cout << "\nInvalid choice!\n";
                pauseScreen();
        }
    } while (choice != 14);
}

void employeeSession(HRSystem& system, Employee* employee) {
    int choice;
    do {
        clearScreen();
        cout << "\nWelcome, " << employee->getName() << "!\n";
        displayEmployeeMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: { // View Profile
                employee->display();
                pauseScreen();
                break;
            }

            case 2: { // View Projects
                vector<Project> myProjects = system.getEmployeeProjects(employee->getId());
                if (myProjects.empty()) {
                    cout << "\nNo projects assigned.\n";
                } else {
                    for (const auto& proj : myProjects) {
                        proj.display();
                    }
                }
                pauseScreen();
                break;
            }

            case 3: { // Submit Project
                int projectId;
                string content;

                cout << "\n=== SUBMIT PROJECT ===\n";
                cout << "Project ID: "; cin >> projectId;
                cin.ignore();
                cout << "Submission Content/Details: ";
                getline(cin, content);

                system.submitProject(projectId, content);
                pauseScreen();
                break;
            }

            case 4: { // View Performance
                system.calculateAndUpdatePerformance(employee->getId());
                Performance* perf = system.getEmployeePerformance(employee->getId());
                if (perf) {
                    perf->display();
                } else {
                    cout << "\nNo performance data available yet.\n";
                }
                pauseScreen();
                break;
            }

            case 5: { // View Notices
                system.displayAllNotices();
                pauseScreen();
                break;
            }

            case 6: { // Submit Leave Request
                string startDate, endDate, reason;

                cout << "\n=== SUBMIT LEAVE REQUEST ===\n";
                cout << "Start Date (YYYY-MM-DD): "; getline(cin, startDate);
                cout << "End Date (YYYY-MM-DD): "; getline(cin, endDate);
                cout << "Reason: "; getline(cin, reason);

                system.submitLeaveRequest(employee->getId(), startDate, endDate, reason);
                pauseScreen();
                break;
            }

            case 7: { // Logout
                cout << "\nLogging out...\n";
                break;
            }

            default:
                cout << "\nInvalid choice!\n";
                pauseScreen();
        }
    } while (choice != 7);
}

int main() {
    HRSystem system;
    int mainChoice;

    cout << "\n========================================\n";
    cout << "  WELCOME TO HR MANAGEMENT SYSTEM      \n";
    cout << "========================================\n";

    do {
        clearScreen();
        displayMainMenu();
        cin >> mainChoice;
        cin.ignore();

        switch(mainChoice) {
            case 1: { // HR Login
                if (!system.isHRPasswordSet()) {
                    cout << "\nHR password not set! Please set it first (Option 3).\n";
                    pauseScreen();
                    break;
                }

                string password;
                cout << "\nEnter HR Password: ";
                getline(cin, password);

                if (system.authenticateHR(password)) {
                    cout << "\nLogin successful!\n";
                    pauseScreen();
                    hrManagerSession(system);
                } else {
                    cout << "\nInvalid password!\n";
                    pauseScreen();
                }
                break;
            }

            case 2: { // Employee Login
                int empId;
                string password;

                cout << "\nEmployee ID: "; cin >> empId;
                cin.ignore();
                cout << "Password: "; getline(cin, password);

                Employee* emp = system.authenticateEmployee(empId, password);
                if (emp) {
                    cout << "\nLogin successful!\n";
                    pauseScreen();
                    employeeSession(system, emp);
                } else {
                    cout << "\nInvalid credentials!\n";
                    pauseScreen();
                }
                break;
            }

            case 3: { // Set HR Password
                if (system.isHRPasswordSet()) {
                    cout << "\nHR password already set!\n";
                    pauseScreen();
                    break;
                }

                string password, confirm;
                cout << "\nSet HR Password: ";
                getline(cin, password);
                cout << "Confirm Password: ";
                getline(cin, confirm);

                if (password == confirm && password.length() >= 6) {
                    system.setHRPassword(password);
                    pauseScreen();
                } else {
                    cout << "\nPasswords don't match or too short (min 6 characters)!\n";
                    pauseScreen();
                }
                break;
            }

            case 4: { // Exit
                cout << "\n========================================\n";
                cout << "  Thank you for using HR System!       \n";
                cout << "========================================\n";
                break;
            }

            default:
                cout << "\nInvalid choice!\n";
                pauseScreen();
        }
    } while (mainChoice != 4);

    return 0;
}
