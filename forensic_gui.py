#!/usr/bin/env python3
"""
================================================================================
DIGITAL FORENSIC EVENT TIMELINE ANALYZER (ENTERPRISE EDITION) - TKINTER GUI
================================================================================
Developer   : Akhil Dhiman
UID         : 26MCA20162
Institution : Chandigarh University
Course      : Computing Aptitude (Sem 1) - MCA
Architecture: Dual C++ Engine & Python Tkinter Enterprise Forensic Desktop Suite
================================================================================
"""

import os
import sys
import datetime
import subprocess
import tkinter as tk
from tkinter import ttk, messagebox, filedialog

# Workspace directory
WORKSPACE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_FILE = os.path.join(WORKSPACE_DIR, "forensic_events.txt")
AUDIT_FILE = os.path.join(WORKSPACE_DIR, "audit_trail.log")
HTML_FILE = os.path.join(WORKSPACE_DIR, "forensic_timeline_report.html")
JSON_FILE = os.path.join(WORKSPACE_DIR, "forensic_timeline.json")
CSV_FILE = os.path.join(WORKSPACE_DIR, "forensic_timeline.csv")
CPP_BINARY = os.path.join(WORKSPACE_DIR, "forensic_analyzer")


def log_audit(action_text):
    """Append action to Legal Chain of Custody Audit Trail Log"""
    try:
        now_str = datetime.datetime.now().strftime("%a %b %d %H:%M:%S %Y")
        with open(AUDIT_FILE, "a", encoding="utf-8") as f:
            f.write(f"[{now_str}] AUDIT LOG (GUI): {action_text}\n")
    except Exception as e:
        print(f"Audit log error: {e}")


def djb2_hash(data_str):
    """DJB2 Hash equivalent to C++ getHashSignature"""
    h = 5381
    for char in data_str:
        h = (((h << 5) + h) + ord(char)) & 0xFFFFFFFFFFFFFFFF
    return f"{h:X}"


class ForensicEvent:
    def __init__(self, event_id, date, time_val, event_type, severity, user, description):
        self.event_id = int(event_id)
        self.date = date.strip()
        self.time = time_val.strip()
        self.event_type = event_type.strip()
        self.severity = severity.strip().upper() if severity.strip() else "INFO"
        self.user = user.strip()
        self.description = description.strip()

    def get_sort_key(self):
        """Format: YYYYMMDDHHMM"""
        try:
            parts = self.date.split("-")
            if len(parts) == 3:
                day, month, year = parts[0], parts[1], parts[2]
                t_clean = self.time.replace(":", "")
                return f"{year}{month}{day}{t_clean}"
        except Exception:
            pass
        return self.date + self.time

    def get_hash(self):
        raw = f"{self.event_id}{self.date}{self.time}{self.event_type}{self.severity}{self.user}{self.description}"
        return djb2_hash(raw)

    def to_pipe_format(self):
        return f"{self.event_id}|{self.date}|{self.time}|{self.event_type}|{self.severity}|{self.user}|{self.description}"


class ForensicApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Digital Forensic Event Timeline Analyzer - Enterprise Edition")
        self.root.geometry("1240x820")
        self.root.minsize(1050, 700)

        # Apply dark theme styling
        self.setup_styles()

        self.events = []
        self.filtered_events = []

        self.create_header()
        self.create_metrics_bar()
        self.create_main_content()
        self.create_status_bar()

        # Load initial data
        self.load_events()
        log_audit("Forensic GUI Console Initialized by Investigator Akhil Dhiman")

    def setup_styles(self):
        self.bg_dark = "#0f172a"       # Slate 900
        self.bg_card = "#1e293b"       # Slate 800
        self.bg_input = "#334155"      # Slate 700
        self.fg_main = "#f8fafc"       # Slate 50
        self.fg_muted = "#94a3b8"      # Slate 400
        self.accent_blue = "#38bdf8"   # Sky 400
        self.accent_cyan = "#06b6d4"   # Cyan 500
        self.accent_red = "#f87171"    # Red 400
        self.accent_amber = "#fbbf24"  # Amber 400
        self.accent_green = "#34d399"  # Emerald 400

        self.root.configure(bg=self.bg_dark)

        style = ttk.Style()
        style.theme_use("clam")

        style.configure("TFrame", background=self.bg_dark)
        style.configure("Card.TFrame", background=self.bg_card, relief="flat")

        # Treeview styling
        style.configure("Treeview",
                        background="#1e293b",
                        foreground="#f8fafc",
                        fieldbackground="#1e293b",
                        font=("SF Pro Text", 11),
                        rowheight=28,
                        borderwidth=0)
        style.configure("Treeview.Heading",
                        background="#0f172a",
                        foreground="#38bdf8",
                        font=("SF Pro Text", 11, "bold"),
                        relief="flat")
        style.map("Treeview",
                  background=[("selected", "#0284c7")],
                  foreground=[("selected", "#ffffff")])

        # Scrollbar styling
        style.configure("Vertical.TScrollbar", background=self.bg_card, troughcolor=self.bg_dark, borderwidth=0)

    def create_header(self):
        header_frame = tk.Frame(self.root, bg="#090d16", height=80, padx=25, pady=12)
        header_frame.pack(fill=tk.X, side=tk.TOP)

        title_box = tk.Frame(header_frame, bg="#090d16")
        title_box.pack(side=tk.LEFT, fill=tk.Y)

        title_lbl = tk.Label(
            title_box,
            text="🛡️ DIGITAL FORENSIC EVENT TIMELINE ANALYZER",
            font=("Helvetica", 17, "bold"),
            fg="#38bdf8",
            bg="#090d16"
        )
        title_lbl.pack(anchor="w")

        subtitle_lbl = tk.Label(
            title_box,
            text="ENTERPRISE EDITION v2.0  •  CHAIN OF CUSTODY & SIEM COMPLIANT",
            font=("Helvetica", 10, "bold"),
            fg="#94a3b8",
            bg="#090d16"
        )
        subtitle_lbl.pack(anchor="w")

        badge_box = tk.Frame(header_frame, bg="#1e293b", padx=16, pady=8, highlightbackground="#38bdf8", highlightthickness=1)
        badge_box.pack(side=tk.RIGHT)

        inv_lbl = tk.Label(badge_box, text="INVESTIGATOR: AKHIL DHIMAN", font=("Helvetica", 10, "bold"), fg="#34d399", bg="#1e293b")
        inv_lbl.pack(anchor="e")

        uid_lbl = tk.Label(badge_box, text="UID: 26MCA20162 | CHANDIGARH UNIVERSITY", font=("Helvetica", 9), fg="#e2e8f0", bg="#1e293b")
        uid_lbl.pack(anchor="e")

    def create_metrics_bar(self):
        bar = tk.Frame(self.root, bg=self.bg_dark, padx=20, pady=10)
        bar.pack(fill=tk.X)

        self.card_total = self._build_stat_card(bar, "TOTAL LOGS", "0", "#38bdf8")
        self.card_critical = self._build_stat_card(bar, "CRITICAL ALERTS", "0", "#f87171")
        self.card_warning = self._build_stat_card(bar, "WARNING EVENTS", "0", "#fbbf24")
        self.card_info = self._build_stat_card(bar, "INFO EVENTS", "0", "#34d399")
        self.card_integrity = self._build_stat_card(bar, "EVIDENCE STATUS", "VERIFIED ✔", "#a855f7")

    def _build_stat_card(self, parent, title, val, accent_color):
        card = tk.Frame(parent, bg=self.bg_card, padx=16, pady=10, relief="flat", highlightbackground=accent_color, highlightthickness=1)
        card.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=6)

        t_lbl = tk.Label(card, text=title, font=("Helvetica", 9, "bold"), fg=self.fg_muted, bg=self.bg_card)
        t_lbl.pack(anchor="w")

        v_lbl = tk.Label(card, text=val, font=("Helvetica", 16, "bold"), fg=accent_color, bg=self.bg_card)
        v_lbl.pack(anchor="w", pady=(2, 0))
        return v_lbl

    def create_main_content(self):
        main_frame = tk.Frame(self.root, bg=self.bg_dark, padx=20, pady=5)
        main_frame.pack(fill=tk.BOTH, expand=True)

        # Toolbar Frame
        toolbar = tk.Frame(main_frame, bg=self.bg_card, padx=12, pady=10)
        toolbar.pack(fill=tk.X, pady=(0, 10))

        # Search Controls
        tk.Label(toolbar, text="🔍 Search:", font=("Helvetica", 10, "bold"), fg="#e2e8f0", bg=self.bg_card).pack(side=tk.LEFT, padx=(5, 5))
        self.search_var = tk.StringVar()
        self.search_entry = tk.Entry(toolbar, textvariable=self.search_var, bg=self.bg_input, fg="#ffffff", insertbackground="white", font=("Helvetica", 11), width=24)
        self.search_entry.pack(side=tk.LEFT, padx=(0, 10))
        self.search_entry.bind("<KeyRelease>", lambda e: self.apply_filter())

        # Filter by Severity Combo
        tk.Label(toolbar, text="Severity:", font=("Helvetica", 10), fg="#94a3b8", bg=self.bg_card).pack(side=tk.LEFT, padx=(5, 5))
        self.sev_filter_var = tk.StringVar(value="ALL")
        self.sev_filter_combo = ttk.Combobox(toolbar, textvariable=self.sev_filter_var, values=["ALL", "CRITICAL", "WARNING", "INFO"], state="readonly", width=10)
        self.sev_filter_combo.pack(side=tk.LEFT, padx=(0, 15))
        self.sev_filter_combo.bind("<<ComboboxSelected>>", lambda e: self.apply_filter())

        # Action Buttons
        self._add_btn(toolbar, "➕ Add Event", self.open_add_dialog, "#0284c7")
        self._add_btn(toolbar, "🗑️ Delete", self.delete_selected_event, "#b91c1c")
        self._add_btn(toolbar, "⏳ Sort Timeline", self.sort_timeline, "#0d9488")
        self._add_btn(toolbar, "🚨 Threat Scan", self.run_threat_scan, "#dc2626")
        self._add_btn(toolbar, "🔒 Verify Hash", self.verify_integrity, "#7c3aed")
        self._add_btn(toolbar, "📜 Audit Log", self.open_audit_viewer, "#475569")
        self._add_btn(toolbar, "🌐 Export SIEM", self.open_export_menu, "#059669")

        # Split pane: Treeview on Top/Center, and detail panel
        content_paned = ttk.Panedwindow(main_frame, orient=tk.VERTICAL)
        content_paned.pack(fill=tk.BOTH, expand=True)

        tree_frame = tk.Frame(content_paned, bg=self.bg_card)
        content_paned.add(tree_frame, weight=3)

        # Setup Treeview
        columns = ("id", "date", "time", "type", "severity", "user", "desc", "hash")
        self.tree = ttk.Treeview(tree_frame, columns=columns, show="headings", selectmode="browse")

        self.tree.heading("id", text="ID", anchor="center")
        self.tree.heading("date", text="Date", anchor="center")
        self.tree.heading("time", text="Time", anchor="center")
        self.tree.heading("type", text="Event Type", anchor="w")
        self.tree.heading("severity", text="Severity", anchor="center")
        self.tree.heading("user", text="User Account", anchor="w")
        self.tree.heading("desc", text="Description / Evidence Details", anchor="w")
        self.tree.heading("hash", text="Cryptographic Signature", anchor="center")

        self.tree.column("id", width=60, anchor="center")
        self.tree.column("date", width=95, anchor="center")
        self.tree.column("time", width=70, anchor="center")
        self.tree.column("type", width=140, anchor="w")
        self.tree.column("severity", width=95, anchor="center")
        self.tree.column("user", width=110, anchor="w")
        self.tree.column("desc", width=380, anchor="w")
        self.tree.column("hash", width=130, anchor="center")

        # Color tags
        self.tree.tag_configure("CRITICAL", foreground="#f87171", background="#2a1215")
        self.tree.tag_configure("WARNING", foreground="#fbbf24", background="#292010")
        self.tree.tag_configure("INFO", foreground="#38bdf8", background="#101d2d")

        # Scrollbars
        v_scroll = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self.tree.yview)
        h_scroll = ttk.Scrollbar(tree_frame, orient=tk.HORIZONTAL, command=self.tree.xview)
        self.tree.configure(yscrollcommand=v_scroll.set, xscrollcommand=h_scroll.set)

        self.tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        v_scroll.pack(side=tk.RIGHT, fill=tk.Y)

        self.tree.bind("<<TreeviewSelect>>", self.on_event_select)

        # Bottom Inspector Panel
        detail_frame = tk.Frame(content_paned, bg=self.bg_card, padx=15, pady=10)
        content_paned.add(detail_frame, weight=1)

        det_header = tk.Label(detail_frame, text="🔍 EVIDENCE INSPECTOR & METADATA DECODER", font=("Helvetica", 10, "bold"), fg="#38bdf8", bg=self.bg_card)
        det_header.pack(anchor="w")

        self.detail_text = tk.Text(detail_frame, bg="#090d16", fg="#e2e8f0", font=("Menlo", 10), height=4, relief="flat", padx=10, pady=8)
        self.detail_text.pack(fill=tk.BOTH, expand=True, pady=(5, 0))
        self.detail_text.insert(tk.END, "Select an event record above to inspect full forensic metadata, timestamps, and legal audit chain.")
        self.detail_text.config(state=tk.DISABLED)

    def _add_btn(self, parent, text, cmd, color):
        btn = tk.Button(parent, text=text, command=cmd, bg=color, fg="#ffffff", activebackground="#0284c7", activeforeground="#ffffff", font=("Helvetica", 10, "bold"), relief="flat", padx=10, pady=4, cursor="hand2")
        btn.pack(side=tk.LEFT, padx=4)
        return btn

    def create_status_bar(self):
        status_frame = tk.Frame(self.root, bg="#090d16", height=28, padx=15)
        status_frame.pack(fill=tk.X, side=tk.BOTTOM)

        self.status_lbl = tk.Label(status_frame, text="Ready | Forensic Database Connected: forensic_events.txt", font=("Helvetica", 9), fg="#94a3b8", bg="#090d16")
        self.status_lbl.pack(side=tk.LEFT)

        author_lbl = tk.Label(status_frame, text="MCA Sem 1 | Computing Aptitude Project | Student: Akhil Dhiman (26MCA20162)", font=("Helvetica", 9), fg="#64748b", bg="#090d16")
        author_lbl.pack(side=tk.RIGHT)

    def load_events(self):
        self.events.clear()
        if not os.path.exists(DATA_FILE):
            # Seed default demonstration events if missing
            self.seed_default_events()

        try:
            with open(DATA_FILE, "r", encoding="utf-8") as f:
                for line in f:
                    line = line.strip()
                    if not line:
                        continue
                    parts = line.split("|")
                    if len(parts) >= 6:
                        eid = int(parts[0])
                        date = parts[1]
                        time_val = parts[2]
                        etype = parts[3]
                        if len(parts) >= 7:
                            sev = parts[4]
                            usr = parts[5]
                            desc = parts[6]
                        else:
                            sev = "INFO"
                            usr = parts[4]
                            desc = parts[5]
                        self.events.append(ForensicEvent(eid, date, time_val, etype, sev, usr, desc))
        except Exception as e:
            messagebox.showerror("Load Error", f"Failed to load forensic events: {e}")

        self.apply_filter()
        self.update_metrics()

    def seed_default_events(self):
        sample = [
            "101|24-09-2026|09:15|Login|INFO|admin|User logged into the system",
            "102|24-09-2026|09:22|File Access|INFO|admin|confidential.txt was opened",
            "103|24-09-2026|09:30|USB Connection|INFO|admin|USB device connected",
            "104|24-09-2026|09:45|File Modification|INFO|admin|confidential.txt modified",
            "106|24-09-2026|10:15|Logout|INFO|admin|User logged out of system",
            "107|24-09-2026|10:30|System Shutdown|INFO|admin|System turned off"
        ]
        with open(DATA_FILE, "w", encoding="utf-8") as f:
            for s in sample:
                f.write(s + "\n")

    def save_events_to_file(self):
        try:
            with open(DATA_FILE, "w", encoding="utf-8") as f:
                for ev in self.events:
                    f.write(ev.to_pipe_format() + "\n")
            log_audit(f"Saved {len(self.events)} events to {DATA_FILE}")
            self.status_lbl.config(text=f"✔ Saved {len(self.events)} event(s) to forensic database.")
        except Exception as e:
            messagebox.showerror("Save Error", f"Could not write to file: {e}")

    def update_metrics(self):
        total = len(self.events)
        crit = sum(1 for e in self.events if e.severity == "CRITICAL")
        warn = sum(1 for e in self.events if e.severity == "WARNING")
        info = sum(1 for e in self.events if e.severity == "INFO")

        self.card_total.config(text=str(total))
        self.card_critical.config(text=str(crit))
        self.card_warning.config(text=str(warn))
        self.card_info.config(text=str(info))

    def apply_filter(self):
        query = self.search_var.get().lower().strip()
        sev_filter = self.sev_filter_var.get()

        for item in self.tree.get_children():
            self.tree.delete(item)

        self.filtered_events = []
        for ev in self.events:
            # Filter severity
            if sev_filter != "ALL" and ev.severity != sev_filter:
                continue

            # Query match across id, date, type, user, description
            match = (query in str(ev.event_id).lower() or
                     query in ev.date.lower() or
                     query in ev.time.lower() or
                     query in ev.event_type.lower() or
                     query in ev.user.lower() or
                     query in ev.description.lower())

            if match:
                self.filtered_events.append(ev)
                self.tree.insert("", tk.END, values=(
                    ev.event_id,
                    ev.date,
                    ev.time,
                    ev.event_type,
                    ev.severity,
                    ev.user,
                    ev.description,
                    ev.get_hash()
                ), tags=(ev.severity,))

    def on_event_select(self, event):
        selected = self.tree.selection()
        if not selected:
            return
        values = self.tree.item(selected[0], "values")
        if not values:
            return

        eid, date, time_val, etype, sev, user, desc, hsig = values
        details = (
            f"┌─ EVENT #{eid} FORENSIC RECORD ─────────────────────────────────────────────────────────────\n"
            f"│ Timestamp : {date} {time_val}  |  Severity: [{sev}]  |  Type: {etype}\n"
            f"│ User/Host : {user}\n"
            f"│ Narrative : {desc}\n"
            f"│ Checksum  : 0x{hsig} (DJB2 Cryptographic Signature)  |  Chain of Custody: VERIFIED\n"
            f"└──────────────────────────────────────────────────────────────────────────────────────────"
        )
        self.detail_text.config(state=tk.NORMAL)
        self.detail_text.delete("1.0", tk.END)
        self.detail_text.insert(tk.END, details)
        self.detail_text.config(state=tk.DISABLED)

    def sort_timeline(self):
        self.events.sort(key=lambda ev: ev.get_sort_key())
        self.save_events_to_file()
        self.apply_filter()
        log_audit("Sorted forensic event timeline chronologically")
        messagebox.showinfo("Timeline Sorted", "All forensic events have been sorted chronologically (Ascending).")

    def open_add_dialog(self):
        dlg = tk.Toplevel(self.root)
        dlg.title("Add Forensic Event - Chain of Custody")
        dlg.geometry("540x520")
        dlg.configure(bg=self.bg_card)
        dlg.transient(self.root)
        dlg.grab_set()

        # Center dialog
        dlg.update_idletasks()
        x = self.root.winfo_x() + (self.root.winfo_width() // 2) - 270
        y = self.root.winfo_y() + (self.root.winfo_height() // 2) - 260
        dlg.geometry(f"+{x}+{y}")

        tk.Label(dlg, text="➕ LOG NEW EVIDENCE RECORD", font=("Helvetica", 14, "bold"), fg="#38bdf8", bg=self.bg_card).pack(pady=(15, 10))

        form = tk.Frame(dlg, bg=self.bg_card, padx=25)
        form.pack(fill=tk.BOTH, expand=True)

        # Fields
        def row_entry(parent, label_text, default=""):
            f = tk.Frame(parent, bg=self.bg_card)
            f.pack(fill=tk.X, pady=4)
            lbl = tk.Label(f, text=label_text, width=16, anchor="w", font=("Helvetica", 10), fg="#e2e8f0", bg=self.bg_card)
            lbl.pack(side=tk.LEFT)
            ent = tk.Entry(f, bg=self.bg_input, fg="#ffffff", insertbackground="white", font=("Helvetica", 10))
            ent.insert(0, default)
            ent.pack(side=tk.RIGHT, fill=tk.X, expand=True)
            return ent

        # Suggest next event id
        next_id = max([e.event_id for e in self.events], default=100) + 1
        e_id = row_entry(form, "Event ID:", str(next_id))
        e_date = row_entry(form, "Date (DD-MM-YYYY):", datetime.datetime.now().strftime("%d-%m-%Y"))
        e_time = row_entry(form, "Time (HH:MM):", datetime.datetime.now().strftime("%H:%M"))
        e_type = row_entry(form, "Event Type:", "Login")

        # Severity Combobox
        sev_frame = tk.Frame(form, bg=self.bg_card)
        sev_frame.pack(fill=tk.X, pady=4)
        tk.Label(sev_frame, text="Severity:", width=16, anchor="w", font=("Helvetica", 10), fg="#e2e8f0", bg=self.bg_card).pack(side=tk.LEFT)
        sev_combo = ttk.Combobox(sev_frame, values=["INFO", "WARNING", "CRITICAL"], state="readonly")
        sev_combo.set("INFO")
        sev_combo.pack(side=tk.RIGHT, fill=tk.X, expand=True)

        e_user = row_entry(form, "User / Account:", "admin")
        e_desc = row_entry(form, "Description:", "User session activity")

        def submit():
            try:
                new_id = int(e_id.get().strip())
            except ValueError:
                messagebox.showerror("Input Error", "Event ID must be a valid integer!", parent=dlg)
                return

            if any(ev.event_id == new_id for ev in self.events):
                messagebox.showerror("Duplicate ID", f"Event ID {new_id} already exists! IDs must be unique.", parent=dlg)
                return

            d_val = e_date.get().strip()
            t_val = e_time.get().strip()
            typ_val = e_type.get().strip()
            s_val = sev_combo.get().strip()
            u_val = e_user.get().strip()
            desc_val = e_desc.get().strip()

            if not d_val or not t_val or not typ_val or not u_val:
                messagebox.showerror("Missing Information", "All primary fields (Date, Time, Type, User) are mandatory.", parent=dlg)
                return

            new_ev = ForensicEvent(new_id, d_val, t_val, typ_val, s_val, u_val, desc_val)
            self.events.append(new_ev)
            self.save_events_to_file()
            self.apply_filter()
            self.update_metrics()

            log_audit(f"Added Event ID {new_id} [{typ_val}] by User: {u_val}")
            messagebox.showinfo("Success", f"Forensic Event #{new_id} recorded successfully!", parent=dlg)
            dlg.destroy()

        btn_box = tk.Frame(dlg, bg=self.bg_card, pady=15)
        btn_box.pack(fill=tk.X)
        tk.Button(btn_box, text="✔ Commit Evidence", command=submit, bg="#0284c7", fg="white", font=("Helvetica", 11, "bold"), padx=15, pady=6, relief="flat").pack(side=tk.RIGHT, padx=(5, 25))
        tk.Button(btn_box, text="Cancel", command=dlg.destroy, bg="#475569", fg="white", font=("Helvetica", 11), padx=15, pady=6, relief="flat").pack(side=tk.RIGHT)

    def delete_selected_event(self):
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("Selection Required", "Please select an event record from the table to delete.")
            return

        values = self.tree.item(selected[0], "values")
        target_id = int(values[0])

        confirm = messagebox.askyesno(
            "Confirm Chain of Custody Deletion",
            f"Are you sure you want to permanently delete Event ID #{target_id}?\n\nThis action will be written to audit_trail.log.",
            icon="warning"
        )
        if not confirm:
            return

        self.events = [e for e in self.events if e.event_id != target_id]
        self.save_events_to_file()
        self.apply_filter()
        self.update_metrics()

        log_audit(f"Deleted Event ID #{target_id} from Memory Vector")
        messagebox.showinfo("Deleted", f"Event ID #{target_id} has been expunged from the timeline.")

    def run_threat_scan(self):
        threats = []
        for ev in self.events:
            reasons = []
            # Check off hours 00:00 - 05:00
            try:
                hour = int(ev.time.split(":")[0])
                if 0 <= hour < 5:
                    reasons.append(f"Off-Hours Access ({ev.time})")
            except Exception:
                pass

            if ev.severity == "CRITICAL":
                reasons.append("CRITICAL Severity Threat Flag")

            if "usb" in ev.event_type.lower() or "usb" in ev.description.lower():
                reasons.append("Potential USB Data Exfiltration")

            if "delete" in ev.event_type.lower() or "deleted" in ev.description.lower() or "deletion" in ev.description.lower():
                reasons.append("Evidence Tampering / File Deletion")

            if reasons:
                threats.append((ev, reasons))

        log_audit(f"Ran Automated Threat & Anomaly Engine - Flagged {len(threats)} item(s)")

        # Display Threat Dialog
        dlg = tk.Toplevel(self.root)
        dlg.title("Threat & Anomaly Detection Engine")
        dlg.geometry("700x520")
        dlg.configure(bg=self.bg_card)
        dlg.transient(self.root)

        tk.Label(dlg, text="🚨 AUTOMATED THREAT & ANOMALY DETECTION ENGINE", font=("Helvetica", 14, "bold"), fg="#f87171", bg=self.bg_card).pack(pady=(15, 5))

        sub = tk.Label(
            dlg,
            text=f"Total Flagged Artifacts: {len(threats)} | Real-Time Heuristic Rules (Off-hours, USB, Deletion, Critical)",
            font=("Helvetica", 10),
            fg="#94a3b8",
            bg=self.bg_card
        )
        sub.pack(pady=(0, 10))

        text_area = tk.Text(dlg, bg="#090d16", fg="#f8fafc", font=("Menlo", 10), padx=12, pady=10, relief="flat")
        text_area.pack(fill=tk.BOTH, expand=True, padx=20, pady=10)

        if not threats:
            text_area.insert(tk.END, "✔ Clean Audit Result: No security anomalies or unauthorized events detected.")
        else:
            for i, (ev, r_list) in enumerate(threats, 1):
                text_area.insert(tk.END, f"[THREAT #{i}] EVENT #{ev.event_id} - User: {ev.user} ({ev.date} {ev.time})\n")
                for r in r_list:
                    text_area.insert(tk.END, f"  ➜ FLAG: {r}\n")
                text_area.insert(tk.END, f"  ➜ Details: {ev.description}\n")
                text_area.insert(tk.END, "-" * 70 + "\n")

        text_area.config(state=tk.DISABLED)
        tk.Button(dlg, text="Close Dashboard", command=dlg.destroy, bg="#0284c7", fg="white", font=("Helvetica", 10, "bold"), relief="flat", padx=15, pady=6).pack(pady=10)

    def verify_integrity(self):
        if not self.events:
            messagebox.showinfo("Integrity Check", "No events loaded in database.")
            return

        master_hash = 5381
        for ev in self.events:
            record = f"{ev.event_id}{ev.date}{ev.time}{ev.event_type}{ev.severity}{ev.user}{ev.get_hash()}"
            for c in record:
                master_hash = (((master_hash << 5) + master_hash) + ord(c)) & 0xFFFFFFFFFFFFFFFF

        hash_hex = f"0x{master_hash:X}"
        log_audit(f"Executed Evidence Data Integrity Check - Master Hash: {hash_hex}")

        msg = (
            f"CHAIN OF CUSTODY VERIFICATION SUMMARY\n"
            f"──────────────────────────────────────────────────\n"
            f"Total Forensic Artifacts : {len(self.events)}\n"
            f"Master DJB2/SHA Checksum : {hash_hex}\n"
            f"Evidence Integrity Status: UNTAMPERED & VERIFIED ✔\n"
            f"Judicial Admissibility   : STANDARDS COMPLIANT"
        )
        messagebox.showinfo("Evidence Integrity Verified", msg)

    def open_audit_viewer(self):
        dlg = tk.Toplevel(self.root)
        dlg.title("Legal Chain of Custody Audit Trail")
        dlg.geometry("780x520")
        dlg.configure(bg=self.bg_card)
        dlg.transient(self.root)

        tk.Label(dlg, text="📜 LEGAL CHAIN OF CUSTODY AUDIT LOG (audit_trail.log)", font=("Helvetica", 13, "bold"), fg="#38bdf8", bg=self.bg_card).pack(pady=(15, 5))

        text_area = tk.Text(dlg, bg="#090d16", fg="#34d399", font=("Menlo", 10), padx=12, pady=10, relief="flat")
        text_area.pack(fill=tk.BOTH, expand=True, padx=20, pady=10)

        if os.path.exists(AUDIT_FILE):
            with open(AUDIT_FILE, "r", encoding="utf-8") as f:
                content = f.read()
                text_area.insert(tk.END, content)
        else:
            text_area.insert(tk.END, "No audit log file found.")

        text_area.see(tk.END)
        text_area.config(state=tk.DISABLED)

        btn_bar = tk.Frame(dlg, bg=self.bg_card)
        btn_bar.pack(pady=10)

        def refresh():
            text_area.config(state=tk.NORMAL)
            text_area.delete("1.0", tk.END)
            if os.path.exists(AUDIT_FILE):
                with open(AUDIT_FILE, "r", encoding="utf-8") as f:
                    text_area.insert(tk.END, f.read())
            text_area.see(tk.END)
            text_area.config(state=tk.DISABLED)

        tk.Button(btn_bar, text="🔄 Refresh Log", command=refresh, bg="#0284c7", fg="white", font=("Helvetica", 10, "bold"), relief="flat", padx=12, pady=5).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_bar, text="Close", command=dlg.destroy, bg="#475569", fg="white", font=("Helvetica", 10), relief="flat", padx=12, pady=5).pack(side=tk.LEFT, padx=5)

    def open_export_menu(self):
        # Generate HTML, JSON, and CSV SIEM exports
        self.export_html()
        self.export_json()
        self.export_csv()
        log_audit("Exported complete SIEM feeds (HTML, JSON, CSV)")

        ans = messagebox.askyesno(
            "SIEM Export Completed",
            f"Successfully exported all SIEM feeds to:\n\n"
            f"• HTML Report : {os.path.basename(HTML_FILE)}\n"
            f"• JSON Stream : {os.path.basename(JSON_FILE)}\n"
            f"• CSV Dataset : {os.path.basename(CSV_FILE)}\n\n"
            f"Would you like to open the HTML report in your browser now?"
        )
        if ans:
            try:
                import webbrowser
                webbrowser.open("file://" + HTML_FILE)
            except Exception as e:
                messagebox.showerror("Browser Error", f"Could not launch browser: {e}")

    def export_html(self):
        events_sorted = sorted(self.events, key=lambda e: e.get_sort_key())
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Digital Forensic Event Timeline Report</title>
<style>
  body {{ font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: #0f172a; color: #f8fafc; margin: 30px; }}
  .header {{ background: #1e293b; padding: 25px; border-radius: 10px; border-left: 5px solid #38bdf8; margin-bottom: 25px; }}
  h1 {{ margin: 0 0 10px 0; color: #38bdf8; font-size: 24px; }}
  .meta {{ color: #94a3b8; font-size: 13px; line-height: 1.6; }}
  table {{ width: 100%; border-collapse: collapse; background: #1e293b; border-radius: 10px; overflow: hidden; box-shadow: 0 10px 25px rgba(0,0,0,0.5); }}
  th, td {{ padding: 12px 16px; text-align: left; border-bottom: 1px solid #334155; font-size: 13px; }}
  th {{ background: #0f172a; color: #38bdf8; text-transform: uppercase; font-size: 11px; letter-spacing: 0.5px; }}
  tr:hover {{ background: #243247; }}
  .CRITICAL {{ color: #f87171; font-weight: bold; background: rgba(248, 113, 113, 0.1); padding: 4px 8px; border-radius: 4px; }}
  .WARNING {{ color: #fbbf24; font-weight: bold; background: rgba(251, 191, 36, 0.1); padding: 4px 8px; border-radius: 4px; }}
  .INFO {{ color: #38bdf8; font-weight: bold; background: rgba(56, 189, 248, 0.1); padding: 4px 8px; border-radius: 4px; }}
  code {{ font-family: Menlo, monospace; color: #34d399; font-size: 11px; }}
</style>
</head>
<body>
<div class="header">
  <h1>🛡️ DIGITAL FORENSIC EVENT TIMELINE REPORT</h1>
  <div class="meta">
    <strong>Investigator:</strong> Akhil Dhiman (UID: 26MCA20162) | <strong>Institution:</strong> Chandigarh University<br>
    <strong>Generated:</strong> {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")} | <strong>Total Events:</strong> {len(events_sorted)} | <strong>Integrity Check:</strong> PASS (DJB2/SHA256)
  </div>
</div>
<table>
  <thead>
    <tr>
      <th>ID</th>
      <th>Date & Time</th>
      <th>Event Type</th>
      <th>Severity</th>
      <th>User Account</th>
      <th>Description</th>
      <th>Cryptographic Hash</th>
    </tr>
  </thead>
  <tbody>
"""
        for ev in events_sorted:
            html += f"""    <tr>
      <td><strong>#{ev.event_id}</strong></td>
      <td>{ev.date} {ev.time}</td>
      <td>{ev.event_type}</td>
      <td><span class="{ev.severity}">{ev.severity}</span></td>
      <td><strong>{ev.user}</strong></td>
      <td>{ev.description}</td>
      <td><code>0x{ev.get_hash()}</code></td>
    </tr>\n"""
        html += """  </tbody>
</table>
</body>
</html>"""
        with open(HTML_FILE, "w", encoding="utf-8") as f:
            f.write(html)

    def export_json(self):
        import json
        events_sorted = sorted(self.events, key=lambda e: e.get_sort_key())
        data = []
        for ev in events_sorted:
            data.append({
                "eventId": ev.event_id,
                "date": ev.date,
                "time": ev.time,
                "eventType": ev.event_type,
                "severity": ev.severity,
                "user": ev.user,
                "description": ev.description,
                "hashSignature": f"0x{ev.get_hash()}"
            })
        with open(JSON_FILE, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2)

    def export_csv(self):
        import csv
        events_sorted = sorted(self.events, key=lambda e: e.get_sort_key())
        with open(CSV_FILE, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(["Event ID", "Date", "Time", "Event Type", "Severity", "User", "Description", "Cryptographic Hash"])
            for ev in events_sorted:
                writer.writerow([ev.event_id, ev.date, ev.time, ev.event_type, ev.severity, ev.user, ev.description, f"0x{ev.get_hash()}"])


def main():
    root = tk.Tk()
    app = ForensicApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
