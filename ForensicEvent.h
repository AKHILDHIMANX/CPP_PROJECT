#ifndef FORENSIC_EVENT_H
#define FORENSIC_EVENT_H

#include <iostream>
#include <string>

using namespace std;

// ForensicEvent Class - Market-Ready Enterprise Evidence Data Model
class ForensicEvent {
private:
    int eventId;
    string date;        // Format: DD-MM-YYYY
    string time;        // Format: HH:MM
    string eventType;   // e.g., Login, File Access, USB Connection, File Deletion
    string severity;    // e.g., INFO, WARNING, CRITICAL
    string user;        // Username or system account
    string description; // Detailed description of activity

public:
    // Default Constructor
    ForensicEvent();

    // Parameterized Constructor
    ForensicEvent(int id, string d, string t, string type, string sev, string u, string desc);

    // Getter methods
    int getEventId() const;
    string getDate() const;
    string getTime() const;
    string getEventType() const;
    string getSeverity() const;
    string getUser() const;
    string getDescription() const;

    // Setter methods
    void setEventId(int id);
    void setDate(string d);
    void setTime(string t);
    void setEventType(string type);
    void setSeverity(string sev);
    void setUser(string u);
    void setDescription(string desc);

    // Display event in terminal box card format
    void displayEvent() const;

    // Serialization for Enterprise Formats
    string toCSV() const;
    string toJSON() const;

    // Evidence Hashing & Sorting
    string getHashSignature() const;
    string getSortKey() const;
};

#endif
