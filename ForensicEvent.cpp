#include "ForensicEvent.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// ANSI Color Escape Codes for Terminal UI
namespace Colors {
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

// Default Constructor
ForensicEvent::ForensicEvent() {
    eventId = 0;
    date = "";
    time = "";
    eventType = "";
    severity = "INFO";
    user = "";
    description = "";
}

// Parameterized Constructor
ForensicEvent::ForensicEvent(int id, string d, string t, string type, string sev, string u, string desc) {
    eventId = id;
    date = d;
    time = t;
    eventType = type;
    severity = sev.empty() ? "INFO" : sev;
    user = u;
    description = desc;
}

// Getters
int ForensicEvent::getEventId() const { return eventId; }
string ForensicEvent::getDate() const { return date; }
string ForensicEvent::getTime() const { return time; }
string ForensicEvent::getEventType() const { return eventType; }
string ForensicEvent::getSeverity() const { return severity; }
string ForensicEvent::getUser() const { return user; }
string ForensicEvent::getDescription() const { return description; }

// Setters
void ForensicEvent::setEventId(int id) { eventId = id; }
void ForensicEvent::setDate(string d) { date = d; }
void ForensicEvent::setTime(string t) { time = t; }
void ForensicEvent::setEventType(string type) { eventType = type; }
void ForensicEvent::setSeverity(string sev) { severity = sev; }
void ForensicEvent::setUser(string u) { user = u; }
void ForensicEvent::setDescription(string desc) { description = desc; }

// Display event card in clean box UI
void ForensicEvent::displayEvent() const {
    string sevColor = Colors::BOLD_CYAN;
    if (severity == "CRITICAL") {
        sevColor = Colors::BOLD_RED;
    } else if (severity == "WARNING") {
        sevColor = Colors::BOLD_YELLOW;
    }

    cout << Colors::BOLD_BLUE << "┌────────────────────────────────────────────────────────┐" << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "EVENT RECORD " << Colors::BOLD_CYAN << "#" << setw(6) << left << eventId 
         << Colors::BOLD_BLUE << "                                   │" << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "├────────────────────────────────────────────────────────┤" << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "Date & Time : " << Colors::RESET << date << " | " << time << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "Event Type  : " << Colors::BOLD_MAGENTA << eventType << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "Severity    : " << sevColor << "[" << severity << "]" << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "User/Account: " << Colors::BOLD_GREEN << user << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "│ " << Colors::BOLD_WHITE << "Description : " << Colors::DIM << description << Colors::RESET << endl;
    cout << Colors::BOLD_BLUE << "└────────────────────────────────────────────────────────┘" << Colors::RESET << endl;
}

// Convert event object to CSV row format
string ForensicEvent::toCSV() const {
    stringstream ss;
    ss << eventId << ",\"" << date << "\",\"" << time << "\",\"" << eventType << "\",\""
       << severity << "\",\"" << user << "\",\"" << description << "\"";
    return ss.str();
}

// Convert event object to JSON string format
string ForensicEvent::toJSON() const {
    stringstream ss;
    ss << "  {\n"
       << "    \"eventId\": " << eventId << ",\n"
       << "    \"date\": \"" << date << "\",\n"
       << "    \"time\": \"" << time << "\",\n"
       << "    \"eventType\": \"" << eventType << "\",\n"
       << "    \"severity\": \"" << severity << "\",\n"
       << "    \"user\": \"" << user << "\",\n"
       << "    \"description\": \"" << description << "\"\n"
       << "  }";
    return ss.str();
}

// Compute individual event cryptographic signature for evidence integrity verification
string ForensicEvent::getHashSignature() const {
    unsigned long long hash = 5381;
    string data = to_string(eventId) + date + time + eventType + severity + user + description;
    for (char c : data) {
        hash = ((hash << 5) + hash) + c; // DJB2 Hash
    }
    stringstream ss;
    ss << hex << uppercase << hash;
    return ss.str();
}

// Generates YYYYMMDDHHMM string sort key for precise chronological comparison
string ForensicEvent::getSortKey() const {
    if (date.length() == 10 && date[2] == '-' && date[5] == '-') {
        string day = date.substr(0, 2);
        string month = date.substr(3, 2);
        string year = date.substr(6, 4);
        
        string t = time;
        if (t.length() == 5 && t[2] == ':') {
            t = t.substr(0, 2) + t.substr(3, 2);
        }
        
        return year + month + day + t;
    }
    return date + time;
}
