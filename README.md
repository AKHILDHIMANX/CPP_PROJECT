# Digital Forensic Event Timeline Analyzer (Enterprise Edition)

[![Language](https://img.shields.io/badge/Language-C%2B%2B14%20%2F%20Python3-blue.svg)](https://isocpp.org/)
[![UI](https://img.shields.io/badge/GUI-Tkinter%20Cyber%20Suite-00d2ff.svg)](https://docs.python.org/3/library/tkinter.html)
[![Compliance](https://img.shields.io/badge/Standards-Chain%20of%20Custody%20%2F%20SIEM-success.svg)](https://csrc.nist.gov/)
[![Institution](https://img.shields.io/badge/Chandigarh%20University-MCA%20Sem%201-red.svg)](https://www.cuchd.in/)

A high-performance incident response and reconstructive digital forensics application. Featuring a dual-engine architecture: a high-efficiency native **C++ CLI Engine** with ANSI card rendering alongside an enterprise **Python Tkinter Desktop GUI Console**.

---

## Student & Project Metadata

| Detail | Information |
| :--- | :--- |
| **Student Name** | **Akhil Dhiman** |
| **UID** | **26MCA20162** |
| **Program** | Master of Computer Applications (MCA) |
| **Class & Section** | MCA - 3 General (Group A), Semester 1 |
| **Course** | Computing Aptitude |
| **Institution** | Chandigarh University |
| **Version** | Enterprise Dual-Engine Edition v2.0 |

---

## 1. Project Overview & Architecture

During digital crime investigations, security analysts gather heterogeneous log streams across servers, workstations, and network perimeters. The **Digital Forensic Event Timeline Analyzer** reconstructs the timeline of an attack, flags Indicators of Compromise (IoCs), preserves judicial evidence integrity through append-only audit logging, and exports SIEM-compatible feeds for Splunk and ElasticSearch.

### Dual-Interface Architecture
1. **Python Tkinter Desktop GUI (`forensic_gui.py`)**:
   - Modern dark cyber-forensics theme (`#0F172A`)
   - Real-time stat metric counters (Total Logs, Critical Alerts, Warnings, Info)
   - Dynamic search filtering by ID, Date, Event Type, User, or Description Keyword
   - Automated Threat & Anomaly Detection modal with heuristic flags
   - Cryptographic Evidence Integrity Check (DJB2 & SHA-256 rolling master hash)
   - Live Legal Chain of Custody Audit Trail viewer (`audit_trail.log`)
   - 1-Click SIEM Exporters (HTML Web Report, JSON Feed, CSV Spreadsheet)
2. **C++ High-Performance CLI Engine (`forensic_analyzer` / `main.cpp`)**:
   - 14-option menu with ANSI color escape codes and box-drawing card formats
   - Fast in-memory parsing and $O(N \log N)$ chronological timeline reconstruction
   - Rolling cryptographic hash generation per event and dataset

---

## 2. Market-Ready Product Features

1. 🚨 **Automated Threat & Anomaly Engine**:
   - **Off-Hours Activity Rule**: Automatically detects access between 00:00 AM and 05:00 AM.
   - **USB Data Exfiltration Rule**: Flags USB peripheral insertions and external storage connections.
   - **Evidence Tampering Rule**: Detects file deletions and suspicious modifications.
   - **Severity Alerting**: Elevates `CRITICAL` records for immediate analyst triage.
2. 📜 **Legal Chain of Custody Audit Trail (`audit_trail.log`)**:
   - Every operation (Add, Search, Filter, Sort, Threat Scan, Export, Delete) is logged with immutable timestamps for court admissibility.
3. 🌐 **Multi-Format SIEM Export Feeds**:
   - `forensic_timeline.json`: Formatted JSON array for SIEM ingestion (Splunk/Elastic).
   - `forensic_timeline.csv`: Tabular spreadsheet format for forensic auditing in Excel.
   - `forensic_timeline_report.html`: Interactive executive web report with color-coded severity tags.
4. 🔒 **Cryptographic Evidence Verification**:
   - Computes rolling hashes to detect evidence tampering.

---

## 3. Project Directory Structure

```text
CPP_PROJECT/
├── ForensicEvent.h                 # Forensic event model header
├── ForensicEvent.cpp               # Forensic event model implementation
├── main.cpp                        # C++ 14-option CLI engine (678 lines)
├── forensic_gui.py                 # Python Tkinter Enterprise Desktop GUI
├── forensic_analyzer               # Compiled C++ executable
├── forensic_events.txt             # Persistent pipe-delimited database
├── audit_trail.log                 # Chain of custody audit log
├── forensic_timeline.json          # SIEM JSON export feed
├── forensic_timeline.csv           # SIEM CSV spreadsheet feed
├── forensic_timeline_report.html   # Styled executive web dashboard
├── PROJECT_REPORT.md               # Complete comprehensive project report
└── DIGITAL_FORENSIC_TIMELINE_REPORT.docx # Word doc report with full code
```

---

## 4. How to Run

### Option A: Launch Python Tkinter GUI (Recommended)
```bash
python3 forensic_gui.py
```

### Option B: Compile & Run C++ CLI Engine
```bash
# Compile with clang++ or g++
clang++ -std=c++14 -Wall ForensicEvent.cpp main.cpp -o forensic_analyzer

# Run the analyzer
./forensic_analyzer
```

---

## 5. Sample Data Format (`forensic_events.txt`)

```text
101|24-09-2026|09:15|Login|INFO|admin|User logged into the system
102|24-09-2026|09:22|File Access|INFO|admin|confidential.txt was opened
103|24-09-2026|09:30|USB Connection|INFO|admin|USB device connected
104|24-09-2026|09:45|File Modification|INFO|admin|confidential.txt modified
106|24-09-2026|10:15|Logout|INFO|admin|User logged out of system
107|24-09-2026|10:30|System Shutdown|INFO|admin|System turned off
```

---

## 6. Testing & Verification

All 10 validation scenarios pass across both CLI and Tkinter GUI interfaces:
- **TC-01 (Threat Detection)**: Flags USB exfiltration and off-hours activity.
- **TC-02 (SIEM JSON Feed)**: Exports valid JSON array.
- **TC-03 (SIEM CSV Feed)**: Exports standard CSV spreadsheet.
- **TC-04 (HTML Web Report)**: Exports styled web dashboard.
- **TC-05 (Audit Logger)**: Logs all actions with timestamps in `audit_trail.log`.
- **TC-06 (Evidence Hash)**: Computes and validates master SHA-256 / DJB2 checksum.
- **TC-07 (Chronological Sort)**: Orders events chronologically by `YYYYMMDDHHMM`.
- **TC-08 (Multi-Param Search)**: Queries records by ID, Type, Date, or User keyword.
- **TC-09 (Evidence Deletion)**: Expunges records while updating the audit trail.
- **TC-10 (Persistence)**: Loads and saves state to `forensic_events.txt`.
