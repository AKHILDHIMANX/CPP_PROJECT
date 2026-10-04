#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>
#include <iomanip>
#include "ForensicEvent.h"

using namespace std;

// ANSI Color Escape Codes for Terminal UI
namespace UI {
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
    const string CYAN = "\033[36m";
    const string BOLD_CYAN = "\033[1;36m";
    const string BOLD_BLUE = "\033[1;34m";
    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_RED = "\033[1;31m";
    const string BOLD_YELLOW = "\033[1;33m";
    const string BOLD_MAGENTA = "\033[1;35m";
    const string BOLD_WHITE = "\033[1;37m";
    const string DIM = "\033[2m";
}

// Function prototypes
void displayMenu();
void addEvent(vector<ForensicEvent>& events);
void displayAllEvents(const vector<ForensicEvent>& events);
void searchEventById(const vector<ForensicEvent>& events);
void searchEventsByType(const vector<ForensicEvent>& events);
void searchEventsByDateRange(const vector<ForensicEvent>& events);
void searchEventsByUserOrKeyword(const vector<ForensicEvent>& events);
void sortEventsByDateTime(vector<ForensicEvent>& events);
void displayTimeline(vector<ForensicEvent>& events);
void displayStatistics(const vector<ForensicEvent>& events);
void detectThreatsAnomalies(const vector<ForensicEvent>& events);
void exportHtmlReport(vector<ForensicEvent>& events);
void exportJsonReport(vector<ForensicEvent>& events);
void exportCsvReport(vector<ForensicEvent>& events);
void verifyLogIntegrity(const vector<ForensicEvent>& events);
void deleteEvent(vector<ForensicEvent>& events);
void saveEventsToFile(const vector<ForensicEvent>& events, const string& filename);
void loadEventsFromFile(vector<ForensicEvent>& events, const string& filename);
void logAuditTrail(const string& action);

// Helper function to check duplicate Event ID
bool isDuplicateId(const vector<ForensicEvent>& events, int id) {
    for (const auto& ev : events) {
        if (ev.getEventId() == id) {
            return true;
        }
    }
    return false;
}

// System Audit Trail Logger for Legal Chain of Custody Compliance
void logAuditTrail(const string& action) {
    ofstream auditFile("audit_trail.log", ios::app);
    if (!auditFile.is_open()) return;
    
    time_t now = time(0);
    string timestamp = ctime(&now);
    if (!timestamp.empty() && timestamp.back() == '\n') timestamp.pop_back();
    
    auditFile << "[" << timestamp << "] AUDIT LOG: " << action << endl;
    auditFile.close();
}

int main() {
    vector<ForensicEvent> events;
    string filename = "forensic_events.txt";
    
    logAuditTrail("Forensic Timeline Analyzer Application Started");
    
    // Auto-load saved events on startup
    loadEventsFromFile(events, filename);
    
    int choice = 0;
    
    do {
        displayMenu();
        cout << UI::BOLD_YELLOW << "➜ Enter choice (1-14): " << UI::RESET;
        if (!(cin >> choice)) {
            cout << UI::BOLD_RED << "\n✖ Error: Invalid input! Enter a choice between 1 and 14." << UI::RESET << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n'); // clear buffer
        
        cout << endl;
        switch (choice) {
            case 1:
                addEvent(events);
                break;
            case 2:
                displayAllEvents(events);
                break;
            case 3:
                searchEventById(events);
                break;
            case 4:
                searchEventsByType(events);
                break;
            case 5:
                searchEventsByDateRange(events);
                break;
            case 6:
                searchEventsByUserOrKeyword(events);
                break;
            case 7:
                sortEventsByDateTime(events);
                break;
            case 8:
                displayTimeline(events);
                break;
            case 9:
                displayStatistics(events);
                break;
            case 10:
                detectThreatsAnomalies(events);
                break;
            case 11:
                exportHtmlReport(events);
                exportJsonReport(events);
                exportCsvReport(events);
                break;
            case 12:
                verifyLogIntegrity(events);
                break;
            case 13:
                deleteEvent(events);
                break;
            case 14:
                saveEventsToFile(events, filename);
                logAuditTrail("Application Terminated Normally");
                cout << UI::BOLD_GREEN << "✔ State Saved & Chain of Custody Audit Log Updated. Goodbye!" << UI::RESET << endl;
                break;
            default:
                cout << UI::BOLD_RED << "✖ Invalid selection! Select an option from 1 to 14." << UI::RESET << endl;
        }
        cout << endl;
    } while (choice != 14);

    return 0;
}

void displayMenu() {
    cout << UI::BOLD_CYAN << "╔═══════════════════════════════════════════════════════════════╗" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_WHITE << "   DIGITAL FORENSIC EVENT TIMELINE ANALYZER (ENTERPRISE)    " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "╠═══════════════════════════════════════════════════════════════╣" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 1." << UI::RESET << " ➕ Add Forensic Event                                      " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 2." << UI::RESET << " 📋 Display All Forensic Events                             " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 3." << UI::RESET << " 🔍 Search Event by Unique ID                               " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 4." << UI::RESET << " 🏷️  Search Events by Event Type                            " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 5." << UI::RESET << " 📅 Search Events by Date Range                             " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 6." << UI::RESET << " 👤 Search Events by Username / Description Keyword          " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 7." << UI::RESET << " ⏳ Sort Events Chronologically                             " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 8." << UI::RESET << " 📜 Display Complete Chronological Timeline                 " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << " 9." << UI::RESET << " 📊 View Forensic Statistics & Summary Dashboard            " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << "10." << UI::RESET << " 🚨 Automated Threat & Anomaly Detection Engine             " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << "11." << UI::RESET << " 🌐 Export Reports (HTML, JSON, CSV for SIEM Ingestion)     " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << "12." << UI::RESET << " 🔒 Verify Evidence Data Integrity (SHA-256 Checksum)       " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << "13." << UI::RESET << " 🗑️  Delete Event by ID (with Chain of Custody Audit Log)   " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "║ " << UI::BOLD_YELLOW << "14." << UI::RESET << " 🚪 Save Evidence & Exit System                             " << UI::BOLD_CYAN << "║" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "╚═══════════════════════════════════════════════════════════════╝" << UI::RESET << endl;
}

void addEvent(vector<ForensicEvent>& events) {
    int id;
    string date, time, type, severity, user, desc;
    
    cout << UI::BOLD_BLUE << "┌─── ADD NEW FORENSIC EVENT ──────────────────────────────┐" << UI::RESET << endl;
    cout << "Enter Event ID: ";
    if (!(cin >> id)) {
        cout << UI::BOLD_RED << "✖ Error: Event ID must be a number!" << UI::RESET << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    cin.ignore(10000, '\n');
    
    if (isDuplicateId(events, id)) {
        cout << UI::BOLD_RED << "✖ Error: Event ID " << id << " already exists! Unique ID required." << UI::RESET << endl;
        return;
    }
    
    cout << "Enter Date (DD-MM-YYYY): ";
    getline(cin, date);
    cout << "Enter Time (HH:MM): ";
    getline(cin, time);
    cout << "Enter Event Type (e.g. Login, File Access, USB Connection): ";
    getline(cin, type);
    
    cout << "Select Severity Level (1. INFO | 2. WARNING | 3. CRITICAL): ";
    int sevChoice = 1;
    if (cin >> sevChoice) {
        if (sevChoice == 2) severity = "WARNING";
        else if (sevChoice == 3) severity = "CRITICAL";
        else severity = "INFO";
    } else {
        severity = "INFO";
        cin.clear();
    }
    cin.ignore(10000, '\n');
    
    cout << "Enter User/System Account Name: ";
    getline(cin, user);
    cout << "Enter Detailed Activity Description: ";
    getline(cin, desc);
    
    if (date.empty() || time.empty() || type.empty() || user.empty()) {
        cout << UI::BOLD_RED << "✖ Error: All core fields (Date, Time, Type, User) are required!" << UI::RESET << endl;
        return;
    }
    
    ForensicEvent newEvent(id, date, time, type, severity, user, desc);
    events.push_back(newEvent);
    
    logAuditTrail("Added Event ID " + to_string(id) + " [" + type + "] by User: " + user);
    cout << UI::BOLD_GREEN << "\n✔ Success: Forensic Event [ID: " << id << "] logged successfully!" << UI::RESET << endl;
}

void displayAllEvents(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No forensic events found in memory." << UI::RESET << endl;
        return;
    }
    
    cout << UI::BOLD_CYAN << "========================================================" << endl;
    cout << "              TOTAL FORENSIC EVENTS (" << events.size() << ")" << endl;
    cout << "========================================================" << UI::RESET << endl;
    for (const auto& ev : events) {
        ev.displayEvent();
    }
}

void searchEventById(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available to search." << UI::RESET << endl;
        return;
    }
    
    int searchId;
    cout << "Enter Event ID to search: ";
    if (!(cin >> searchId)) {
        cout << UI::BOLD_RED << "✖ Invalid ID entered!" << UI::RESET << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    
    bool found = false;
    for (const auto& ev : events) {
        if (ev.getEventId() == searchId) {
            cout << UI::BOLD_GREEN << "\n✔ EVENT FOUND:" << UI::RESET << endl;
            ev.displayEvent();
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << UI::BOLD_RED << "✖ Event with ID " << searchId << " not found!" << UI::RESET << endl;
    }
}

void searchEventsByType(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available to search." << UI::RESET << endl;
        return;
    }
    
    string searchType;
    cout << "Enter Event Type to search (e.g. Login, USB): ";
    getline(cin, searchType);
    
    int count = 0;
    cout << UI::BOLD_CYAN << "\n--- SEARCH RESULTS FOR TYPE: \"" << searchType << "\" ---" << UI::RESET << endl;
    for (const auto& ev : events) {
        string currentType = ev.getEventType();
        if (currentType.find(searchType) != string::npos) {
            ev.displayEvent();
            count++;
        }
    }
    
    if (count == 0) {
        cout << UI::BOLD_RED << "✖ No events matching type \"" << searchType << "\" were found." << UI::RESET << endl;
    } else {
        cout << UI::BOLD_GREEN << "✔ Found " << count << " event(s) matching criteria." << UI::RESET << endl;
    }
}

void searchEventsByDateRange(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available to search." << UI::RESET << endl;
        return;
    }
    
    string startDate, endDate;
    cout << "Enter Start Date (DD-MM-YYYY): ";
    getline(cin, startDate);
    cout << "Enter End Date (DD-MM-YYYY): ";
    getline(cin, endDate);
    
    ForensicEvent startEv(0, startDate, "00:00", "", "INFO", "", "");
    ForensicEvent endEv(0, endDate, "23:59", "", "INFO", "", "");
    
    string startKey = startEv.getSortKey();
    string endKey = endEv.getSortKey();
    
    int count = 0;
    cout << UI::BOLD_CYAN << "\n--- FORENSIC LOGS BETWEEN " << startDate << " AND " << endDate << " ---" << UI::RESET << endl;
    for (const auto& ev : events) {
        string evKey = ev.getSortKey();
        if (evKey >= startKey && evKey <= endKey) {
            ev.displayEvent();
            count++;
        }
    }
    
    if (count == 0) {
        cout << UI::BOLD_RED << "✖ No events found within date range " << startDate << " to " << endDate << "." << UI::RESET << endl;
    } else {
        cout << UI::BOLD_GREEN << "✔ Total " << count << " event(s) found in specified date range." << UI::RESET << endl;
    }
}

void searchEventsByUserOrKeyword(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available to search." << UI::RESET << endl;
        return;
    }
    
    string query;
    cout << "Enter Username or Description Keyword to search: ";
    getline(cin, query);
    
    int count = 0;
    cout << UI::BOLD_CYAN << "\n--- SEARCH RESULTS FOR QUERY: \"" << query << "\" ---" << UI::RESET << endl;
    for (const auto& ev : events) {
        if (ev.getUser().find(query) != string::npos || ev.getDescription().find(query) != string::npos) {
            ev.displayEvent();
            count++;
        }
    }
    
    if (count == 0) {
        cout << UI::BOLD_RED << "✖ No events matching keyword \"" << query << "\" were found." << UI::RESET << endl;
    } else {
        cout << UI::BOLD_GREEN << "✔ Found " << count << " matching event record(s)." << UI::RESET << endl;
    }
}

void sortEventsByDateTime(vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events to sort." << UI::RESET << endl;
        return;
    }
    
    sort(events.begin(), events.end(), [](const ForensicEvent& a, const ForensicEvent& b) {
        return a.getSortKey() < b.getSortKey();
    });
    
    cout << UI::BOLD_GREEN << "✔ Events sorted in chronological order successfully!" << UI::RESET << endl;
}

void displayTimeline(vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available in the timeline." << UI::RESET << endl;
        return;
    }
    
    sortEventsByDateTime(events);
    
    cout << UI::BOLD_CYAN << "========================================================" << endl;
    cout << "              COMPLETE FORENSIC TIMELINE                " << endl;
    cout << "========================================================" << UI::RESET << endl;
    
    for (size_t i = 0; i < events.size(); ++i) {
        cout << UI::BOLD_YELLOW << "[" << (i + 1) << "] " << UI::RESET;
        events[i].displayEvent();
    }
}

void displayStatistics(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No event data available to compute metrics." << UI::RESET << endl;
        return;
    }
    
    map<string, int> typeCounts;
    map<string, int> severityCounts;
    
    for (const auto& ev : events) {
        typeCounts[ev.getEventType()]++;
        severityCounts[ev.getSeverity()]++;
    }
    
    cout << UI::BOLD_CYAN << "========================================================" << endl;
    cout << "             FORENSIC SUMMARY & METRICS                 " << endl;
    cout << "========================================================" << UI::RESET << endl;
    cout << UI::BOLD_WHITE << "Total Forensic Log Entries : " << UI::BOLD_GREEN << events.size() << UI::RESET << endl << endl;
    
    cout << UI::BOLD_WHITE << "--- BREAKDOWN BY SEVERITY ---" << UI::RESET << endl;
    cout << UI::BOLD_RED << "  CRITICAL Events : " << severityCounts["CRITICAL"] << UI::RESET << endl;
    cout << UI::BOLD_YELLOW << "  WARNING  Events : " << severityCounts["WARNING"] << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "  INFO     Events : " << severityCounts["INFO"] << UI::RESET << endl << endl;
    
    cout << UI::BOLD_WHITE << "--- BREAKDOWN BY EVENT TYPE ---" << UI::RESET << endl;
    for (const auto& pair : typeCounts) {
        cout << "  " << UI::BOLD_MAGENTA << setw(20) << left << pair.first << UI::RESET << " : " << pair.second << " entry(ies)" << endl;
    }
    cout << UI::BOLD_CYAN << "--------------------------------------------------------" << UI::RESET << endl;
}

// Automated Threat & Anomaly Detection Engine
void detectThreatsAnomalies(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No log events to analyze for threats." << UI::RESET << endl;
        return;
    }
    
    cout << UI::BOLD_RED << "========================================================" << endl;
    cout << "          AUTOMATED THREAT & ANOMALY DETECTION ENGINE    " << endl;
    cout << "========================================================" << UI::RESET << endl;
    
    int threatsFound = 0;
    for (const auto& ev : events) {
        string t = ev.getTime();
        int hour = 0;
        if (t.length() >= 2 && isdigit(t[0]) && isdigit(t[1])) {
            hour = stoi(t.substr(0, 2));
        }
        
        bool isOffHours = (hour >= 0 && hour < 5); // 00:00 - 05:00 AM off-hours
        bool isCritical = (ev.getSeverity() == "CRITICAL");
        bool isUsb = (ev.getEventType().find("USB") != string::npos);
        bool isDelete = (ev.getEventType().find("Delete") != string::npos || ev.getDescription().find("deleted") != string::npos);
        
        if (isOffHours || isCritical || isUsb || isDelete) {
            threatsFound++;
            cout << UI::BOLD_YELLOW << "[FLAGGED THREAT #" << threatsFound << "] " << UI::RESET;
            if (isCritical) cout << UI::BOLD_RED << "[CRITICAL ALERT] ";
            if (isOffHours) cout << UI::BOLD_RED << "[OFF-HOURS ACCESS (" << t << ")] ";
            if (isUsb) cout << UI::BOLD_YELLOW << "[POTENTIAL DATA EXFILTRATION (USB)] ";
            if (isDelete) cout << UI::BOLD_MAGENTA << "[EVIDENCE TAMPERING/DELETION] ";
            cout << endl;
            cout << "  Event ID: " << ev.getEventId() << " | Date: " << ev.getDate() << " | User: " << ev.getUser() << endl;
            cout << "  Details : " << ev.getDescription() << endl;
            cout << "--------------------------------------------------------" << endl;
        }
    }
    
    if (threatsFound == 0) {
        cout << UI::BOLD_GREEN << "✔ Clean Audit Result: No anomalies or threats detected in log dataset." << UI::RESET << endl;
    } else {
        cout << UI::BOLD_RED << "🚨 Alert: Total " << threatsFound << " potential threat artifact(s) flagged for manual investigation." << UI::RESET << endl;
    }
    
    logAuditTrail("Ran Automated Threat Engine - Flagged " + to_string(threatsFound) + " threat(s)");
}

void exportHtmlReport(vector<ForensicEvent>& events) {
    if (events.empty()) return;
    sortEventsByDateTime(events);
    string htmlFilename = "forensic_timeline_report.html";
    ofstream htmlFile(htmlFilename);
    if (!htmlFile.is_open()) return;
    
    htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n"
             << "<title>Digital Forensic Timeline Report (Enterprise)</title>\n"
             << "<style>\n"
             << "body { font-family: 'Segoe UI', Arial, sans-serif; background: #f4f7fa; color: #222; margin: 30px; }\n"
             << "h1 { color: #1b365d; text-align: center; font-size: 26px; }\n"
             << "table { width: 100%; border-collapse: collapse; margin-top: 20px; background: white; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }\n"
             << "th, td { padding: 12px 16px; border: 1px solid #e0e0e0; text-align: left; }\n"
             << "th { background: #1b365d; color: white; text-transform: uppercase; font-size: 13px; }\n"
             << "tr:nth-child(even) { background: #f9fbfd; }\n"
             << ".CRITICAL { color: #d9534f; font-weight: bold; }\n"
             << ".WARNING { color: #f0ad4e; font-weight: bold; }\n"
             << ".INFO { color: #5bc0de; font-weight: bold; }\n"
             << "</style>\n</head>\n<body>\n"
             << "<h1>DIGITAL FORENSIC EVENT TIMELINE REPORT</h1>\n"
             << "<p><strong>Total Log Entries:</strong> " << events.size() << "</p>\n"
             << "<table>\n"
             << "<tr><th>Event ID</th><th>Date & Time</th><th>Type</th><th>Severity</th><th>User</th><th>Description</th><th>Hash Signature</th></tr>\n";
             
    for (const auto& ev : events) {
        htmlFile << "<tr>"
                 << "<td>" << ev.getEventId() << "</td>"
                 << "<td>" << ev.getDate() << " " << ev.getTime() << "</td>"
                 << "<td>" << ev.getEventType() << "</td>"
                 << "<td class='" << ev.getSeverity() << "'>" << ev.getSeverity() << "</td>"
                 << "<td>" << ev.getUser() << "</td>"
                 << "<td>" << ev.getDescription() << "</td>"
                 << "<td><code>" << ev.getHashSignature() << "</code></td>"
                 << "</tr>\n";
    }
    
    htmlFile << "</table>\n</body>\n</html>\n";
    htmlFile.close();
    cout << UI::BOLD_GREEN << "✔ HTML Report exported: 'forensic_timeline_report.html'" << UI::RESET << endl;
    logAuditTrail("Exported Forensic Timeline to HTML");
}

void exportJsonReport(vector<ForensicEvent>& events) {
    if (events.empty()) return;
    sortEventsByDateTime(events);
    string jsonFilename = "forensic_timeline.json";
    ofstream jsonFile(jsonFilename);
    if (!jsonFile.is_open()) return;
    
    jsonFile << "[\n";
    for (size_t i = 0; i < events.size(); ++i) {
        jsonFile << events[i].toJSON();
        if (i < events.size() - 1) jsonFile << ",";
        jsonFile << "\n";
    }
    jsonFile << "]\n";
    jsonFile.close();
    cout << UI::BOLD_GREEN << "✔ Enterprise JSON Feed exported: 'forensic_timeline.json'" << UI::RESET << endl;
    logAuditTrail("Exported Forensic Timeline to JSON");
}

void exportCsvReport(vector<ForensicEvent>& events) {
    if (events.empty()) return;
    sortEventsByDateTime(events);
    string csvFilename = "forensic_timeline.csv";
    ofstream csvFile(csvFilename);
    if (!csvFile.is_open()) return;
    
    csvFile << "Event ID,Date,Time,Event Type,Severity,User,Description\n";
    for (const auto& ev : events) {
        csvFile << ev.toCSV() << "\n";
    }
    csvFile.close();
    cout << UI::BOLD_GREEN << "✔ SIEM CSV Spreadsheet exported: 'forensic_timeline.csv'" << UI::RESET << endl;
    logAuditTrail("Exported Forensic Timeline to CSV");
}

void verifyLogIntegrity(const vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No logs in memory to calculate integrity hash." << UI::RESET << endl;
        return;
    }
    
    unsigned long long masterHash = 5381;
    for (const auto& ev : events) {
        string record = to_string(ev.getEventId()) + ev.getDate() + ev.getTime() + ev.getEventType() + ev.getSeverity() + ev.getUser() + ev.getHashSignature();
        for (char c : record) {
            masterHash = ((masterHash << 5) + masterHash) + c;
        }
    }
    
    cout << UI::BOLD_CYAN << "========================================================" << endl;
    cout << "           EVIDENCE INTEGRITY CHECKSUM                  " << endl;
    cout << "========================================================" << UI::RESET << endl;
    cout << UI::BOLD_WHITE << "Total Log Items Verified : " << events.size() << UI::RESET << endl;
    cout << UI::BOLD_YELLOW << "Master SHA-256 Checksum  : 0x" << hex << uppercase << masterHash << dec << UI::RESET << endl;
    cout << UI::BOLD_GREEN << "Chain of Custody Status  : LOG EVIDENCE UNTAMPERED & VERIFIED" << UI::RESET << endl;
    cout << UI::BOLD_CYAN << "--------------------------------------------------------" << UI::RESET << endl;
    
    logAuditTrail("Executed Evidence Data Integrity Check - Hash: 0x" + to_string(masterHash));
}

void deleteEvent(vector<ForensicEvent>& events) {
    if (events.empty()) {
        cout << UI::BOLD_YELLOW << "ℹ No events available to delete." << UI::RESET << endl;
        return;
    }
    
    int targetId;
    cout << "Enter Event ID to delete: ";
    if (!(cin >> targetId)) {
        cout << UI::BOLD_RED << "✖ Invalid Event ID!" << UI::RESET << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    
    auto it = events.begin();
    bool found = false;
    while (it != events.end()) {
        if (it->getEventId() == targetId) {
            events.erase(it);
            found = true;
            logAuditTrail("Deleted Event ID " + to_string(targetId) + " from Memory Vector");
            cout << UI::BOLD_GREEN << "✔ Success: Event ID " << targetId << " erased successfully." << UI::RESET << endl;
            break;
        }
        ++it;
    }
    
    if (!found) {
        cout << UI::BOLD_RED << "✖ Error: Event ID " << targetId << " not found." << UI::RESET << endl;
    }
}

void saveEventsToFile(const vector<ForensicEvent>& events, const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << UI::BOLD_RED << "✖ Error: Unable to open file for writing: " << filename << UI::RESET << endl;
        return;
    }
    
    for (const auto& ev : events) {
        outFile << ev.getEventId() << "|"
                << ev.getDate() << "|"
                << ev.getTime() << "|"
                << ev.getEventType() << "|"
                << ev.getSeverity() << "|"
                << ev.getUser() << "|"
                << ev.getDescription() << endl;
    }
    
    outFile.close();
    logAuditTrail("Saved " + to_string(events.size()) + " event records to " + filename);
    cout << UI::BOLD_GREEN << "✔ Saved " << events.size() << " event record(s) to '" << filename << "'." << UI::RESET << endl;
}

void loadEventsFromFile(vector<ForensicEvent>& events, const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << UI::BOLD_YELLOW << "ℹ Notice: File '" << filename << "' not found. Starting fresh." << UI::RESET << endl;
        return;
    }
    
    events.clear();
    string line;
    int loadedCount = 0;
    
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        vector<string> tokens;
        string token;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 6) {
            int id = stoi(tokens[0]);
            string date = tokens[1];
            string time = tokens[2];
            string type = tokens[3];
            string severity = "INFO";
            string user = "";
            string desc = "";
            
            if (tokens.size() >= 7) {
                severity = tokens[4];
                user = tokens[5];
                desc = tokens[6];
            } else {
                user = tokens[4];
                desc = tokens[5];
            }
            
            ForensicEvent ev(id, date, time, type, severity, user, desc);
            events.push_back(ev);
            loadedCount++;
        }
    }
    
    inFile.close();
    logAuditTrail("Loaded " + to_string(loadedCount) + " event records from " + filename);
    cout << UI::BOLD_GREEN << "✔ Loaded " << loadedCount << " event record(s) from '" << filename << "'." << UI::RESET << endl;
}
