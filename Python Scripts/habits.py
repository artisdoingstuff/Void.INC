import sys
import json
import sqlite3
from datetime import datetime, timedelta, date
from collections import defaultdict

from PyQt6.QtCore import Qt, QTimer
from PyQt6.QtWidgets import (
    QApplication, QWidget, QHBoxLayout, QVBoxLayout, QListWidget, QListWidgetItem,
    QPushButton, QLabel, QLineEdit, QSpinBox, QMessageBox, QComboBox, QFormLayout,
    QDialog, QDialogButtonBox, QGroupBox, QGridLayout, QFileDialog
)
from PyQt6.QtGui import QIcon

from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

DB_FILENAME = "Data/habits.db"

class DB:
    def __init__(self, path=DB_FILENAME):
        self.conn = sqlite3.connect(path, detect_types=sqlite3.PARSE_DECLTYPES)
        self.conn.row_factory = sqlite3.Row
        self.create_tables()

    def create_tables(self):
        c = self.conn.cursor()
        c.execute("""
        CREATE TABLE IF NOT EXISTS habits (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL DEFAULT 'yesno',
            target INTEGER DEFAULT 1,
            rest_days TEXT DEFAULT '',
            created TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
        """)
        c.execute("""
        CREATE TABLE IF NOT EXISTS completions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            habit_id INTEGER NOT NULL,
            day TEXT NOT NULL,
            amount INTEGER DEFAULT 1,
            confirmed INTEGER DEFAULT 1,
            FOREIGN KEY(habit_id) REFERENCES habits(id)
        )
        """)
        c.execute("""
        CREATE TABLE IF NOT EXISTS profile (
            id INTEGER PRIMARY KEY CHECK (id = 1),
            xp INTEGER DEFAULT 0,
            level INTEGER DEFAULT 1,
            coins INTEGER DEFAULT 0
        )
        """)
        c.execute("""
        CREATE TABLE IF NOT EXISTS achievements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            key TEXT UNIQUE,
            title TEXT,
            unlocked_at TIMESTAMP
        )
        """)
        c.execute("""
        CREATE TABLE IF NOT EXISTS purchases (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            item_key TEXT UNIQUE,
            bought_at TIMESTAMP
        )
        """)
        self.conn.commit()
        c.execute("INSERT OR IGNORE INTO profile (id, xp, level, coins) VALUES (1, 0, 1, 0)")
        self.conn.commit()

    def add_habit(self, name, typ='yesno', target=1, rest_days=''):
        c = self.conn.cursor()
        c.execute("INSERT INTO habits (name, type, target, rest_days) VALUES (?, ?, ?, ?)",
                  (name, typ, target, rest_days))
        self.conn.commit()
        return c.lastrowid

    def update_habit(self, hid, name, typ, target, rest_days):
        c = self.conn.cursor()
        c.execute("UPDATE habits SET name=?, type=?, target=?, rest_days=? WHERE id=?",
                  (name, typ, target, rest_days, hid))
        self.conn.commit()

    def delete_habit(self, hid):
        c = self.conn.cursor()
        c.execute("DELETE FROM habits WHERE id=?", (hid,))
        c.execute("DELETE FROM completions WHERE habit_id=?", (hid,))
        self.conn.commit()

    def get_habits(self):
        c = self.conn.cursor()
        c.execute("SELECT * FROM habits ORDER BY name")
        return [dict(r) for r in c.fetchall()]

    def get_habit(self, hid):
        c = self.conn.cursor()
        c.execute("SELECT * FROM habits WHERE id=?", (hid,))
        r = c.fetchone()
        return dict(r) if r else None

    def add_completion(self, habit_id, day, amount=1, confirmed=1):
        c = self.conn.cursor()
        c.execute("INSERT INTO completions (habit_id, day, amount, confirmed) VALUES (?, ?, ?, ?)",
                  (habit_id, day, amount, int(bool(confirmed))))
        self.conn.commit()

    def get_completions_for_day(self, day):
        c = self.conn.cursor()
        c.execute("SELECT * FROM completions WHERE day=?", (day,))
        return [dict(r) for r in c.fetchall()]

    def get_completions_for_habit(self, habit_id):
        c = self.conn.cursor()
        c.execute("SELECT * FROM completions WHERE habit_id=? ORDER BY day DESC", (habit_id,))
        return [dict(r) for r in c.fetchall()]

    def completion_exists(self, habit_id, day):
        c = self.conn.cursor()
        c.execute("SELECT 1 FROM completions WHERE habit_id=? AND day=?", (habit_id, day))
        return c.fetchone() is not None

    def get_profile(self):
        c = self.conn.cursor()
        c.execute("SELECT xp, level, coins FROM profile WHERE id=1")
        r = c.fetchone()
        return dict(r)

    def add_xp(self, xp_gain):
        c = self.conn.cursor()
        c.execute("UPDATE profile SET xp = xp + ? WHERE id=1", (xp_gain,))
        c.execute("SELECT xp, level FROM profile WHERE id=1")
        xp, level = c.fetchone()
        while xp >= level * 100:
            xp -= level * 100
            level += 1
        c.execute("UPDATE profile SET xp=?, level=? WHERE id=1", (xp, level))
        self.conn.commit()

    def add_coins(self, coins):
        c = self.conn.cursor()
        c.execute("UPDATE profile SET coins = coins + ? WHERE id=1", (coins,))
        self.conn.commit()

    def spend_coins(self, amount):
        c = self.conn.cursor()
        c.execute("SELECT coins FROM profile WHERE id=1")
        coins = c.fetchone()[0]
        if coins >= amount:
            c.execute("UPDATE profile SET coins = coins - ? WHERE id=1", (amount,))
            self.conn.commit()
            return True
        return False

    def unlock_achievement(self, key, title):
        c = self.conn.cursor()
        c.execute("INSERT OR IGNORE INTO achievements (key, title, unlocked_at) VALUES (?, ?, ?)",
                  (key, title, datetime.now().isoformat()))
        self.conn.commit()

    def get_achievements(self):
        c = self.conn.cursor()
        c.execute("SELECT * FROM achievements")
        return [dict(r) for r in c.fetchall()]

    def buy_item(self, item_key):
        c = self.conn.cursor()
        c.execute("INSERT OR IGNORE INTO purchases (item_key, bought_at) VALUES (?, ?)",
                  (item_key, datetime.now().isoformat()))
        self.conn.commit()

    def get_purchases(self):
        c = self.conn.cursor()
        c.execute("SELECT * FROM purchases")
        return [dict(r) for r in c.fetchall()]

    def export_json(self, path):
        data = {
            "habits": self.get_habits(),
            "completions": [],
            "profile": self.get_profile(),
            "achievements": self.get_achievements(),
            "purchases": self.get_purchases()
        }
        c = self.conn.cursor()
        c.execute("SELECT * FROM completions")
        for r in c.fetchall():
            data["completions"].append(dict(r))
        with open(path, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, default=str)

    def import_json(self, path):
        with open(path, "r", encoding="utf-8") as f:
            data = json.load(f)
        c = self.conn.cursor()
        for h in data.get("habits", []):
            c.execute("INSERT INTO habits (name, type, target, rest_days, created) VALUES (?, ?, ?, ?, ?)",
                      (h["name"], h.get("type", "yesno"), h.get("target", 1), h.get("rest_days", ""), h.get("created")))
        for comp in data.get("completions", []):
            c.execute("INSERT INTO completions (habit_id, day, amount, confirmed) VALUES (?, ?, ?, ?)",
                      (comp.get("habit_id"), comp.get("day"), comp.get("amount", 1), comp.get("confirmed", 1)))
        self.conn.commit()

def today_iso():
    return date.today().isoformat()

def days_ago_iso(n):
    return (date.today() - timedelta(days=n)).isoformat()

def rewards_for_completion(habit):
    base_xp = 10
    base_coins = 1
    if habit["type"] == "count":
        base_xp = 5 * (habit.get("target", 1))
        base_coins = max(1, habit.get("target", 1) // 2)
    return base_xp, base_coins

class HabitDialog(QDialog):
    def __init__(self, parent=None, habit=None):
        super().__init__(parent)
        self.setWindowTitle("Add Habit" if habit is None else "Edit Habit")
        self.habit = habit

        self.name_edit = QLineEdit(habit["name"] if habit else "")
        self.type_combo = QComboBox()
        self.type_combo.addItems(["yesno", "count"])
        if habit:
            self.type_combo.setCurrentText(habit["type"])
        self.target_spin = QSpinBox()
        self.target_spin.setRange(1, 10000)
        self.target_spin.setValue(habit["target"] if habit else 1)

        form = QFormLayout()
        form.addRow("Name:", self.name_edit)
        form.addRow("Type:", self.type_combo)
        form.addRow("Target (for count):", self.target_spin)

        buttons = QDialogButtonBox(QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel)
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)

        v = QVBoxLayout()
        v.addLayout(form)
        v.addWidget(buttons)
        self.setLayout(v)

    def values(self):
        return {
            "name": self.name_edit.text().strip(),
            "type": self.type_combo.currentText(),
            "target": self.target_spin.value()
        }

class HabitApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Habit Control")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\dashboard-icon.png"
        self.setWindowIcon(QIcon(icon_path))
        self.resize(1000, 600)
        self.db = DB()

        self.habits_list = QListWidget()
        self.habits_list.itemClicked.connect(self.refresh_today_view)

        self.add_habit_btn = QPushButton("Add Habit")
        self.add_habit_btn.clicked.connect(self.add_habit)
        self.edit_habit_btn = QPushButton("Edit Habit")
        self.edit_habit_btn.clicked.connect(self.edit_selected_habit)
        self.delete_habit_btn = QPushButton("Delete Habit")
        self.delete_habit_btn.clicked.connect(self.delete_selected_habit)

        left_v = QVBoxLayout()
        left_v.addWidget(QLabel("Habits"))
        left_v.addWidget(self.habits_list)
        left_v.addWidget(self.add_habit_btn)
        left_v.addWidget(self.edit_habit_btn)
        left_v.addWidget(self.delete_habit_btn)

        left_box = QGroupBox()
        left_box.setLayout(left_v)

        self.today_list = QListWidget()
        self.mark_done_btn = QPushButton("Mark Selected Done")
        self.mark_done_btn.clicked.connect(self.mark_selected_done)
        self.refresh_today_btn = QPushButton("Refresh Today")
        self.refresh_today_btn.clicked.connect(self.refresh_today_view)

        center_v = QVBoxLayout()
        center_v.addWidget(QLabel("Today's Habits"))
        center_v.addWidget(self.today_list)
        center_v.addWidget(self.mark_done_btn)
        center_v.addWidget(self.refresh_today_btn)
        center_box = QGroupBox()
        center_box.setLayout(center_v)

        self.xp_label = QLabel()
        self.coins_label = QLabel()
        self.level_label = QLabel()
        self.refresh_profile_labels()

        self.export_btn = QPushButton("Export Backup")
        self.export_btn.clicked.connect(self.export_backup)
        self.import_btn = QPushButton("Import Backup")
        self.import_btn.clicked.connect(self.import_backup)

        self.shop_combo = QComboBox()
        self.shop_items = {
            "theme_dark": {"title": "Dark Theme", "cost": 20},
            "theme_pastel": {"title": "Pastel Theme", "cost": 30}
        }
        for k, v in self.shop_items.items():
            self.shop_combo.addItem(f"{v['title']} - {v['cost']} coins", k)
        self.buy_btn = QPushButton("Buy Item")
        self.buy_btn.clicked.connect(self.buy_item)

        self.figure = Figure(figsize=(4, 3))
        self.canvas = FigureCanvas(self.figure)
        self.draw_weekly_chart()

        right_v = QVBoxLayout()
        right_v.addWidget(QLabel("Profile"))
        right_v.addWidget(self.level_label)
        right_v.addWidget(self.xp_label)
        right_v.addWidget(self.coins_label)
        right_v.addStretch()
        right_v.addWidget(QLabel("Shop"))
        right_v.addWidget(self.shop_combo)
        right_v.addWidget(self.buy_btn)
        right_v.addStretch()
        right_v.addWidget(QLabel("Weekly Activity"))
        right_v.addWidget(self.canvas)
        right_v.addStretch()
        right_v.addWidget(self.export_btn)
        right_v.addWidget(self.import_btn)

        right_box = QGroupBox()
        right_box.setLayout(right_v)

        h = QHBoxLayout()
        h.addWidget(left_box, 2)
        h.addWidget(center_box, 3)
        h.addWidget(right_box, 3)

        self.setLayout(h)

        self.load_habits()
        self.refresh_today_view()

        self.midnight_timer = QTimer()
        self.midnight_timer.setInterval(60_000)
        self.midnight_timer.timeout.connect(self.handle_midnight)
        self.midnight_timer.start()

    def load_habits(self):
        self.habits_list.clear()
        for h in self.db.get_habits():
            item = QListWidgetItem(f"{h['name']} ({h['type']})")
            item.setData(Qt.ItemDataRole.UserRole, h["id"])
            self.habits_list.addItem(item)

    def refresh_profile_labels(self):
        p = self.db.get_profile()
        self.level_label.setText(f"Level: {p['level']}")
        self.xp_label.setText(f"XP: {p['xp']} / {p['level']*100}")
        self.coins_label.setText(f"Coins: {p['coins']}")

    def refresh_today_view(self, item=None):
        self.today_list.clear()
        habits = self.db.get_habits()
        day = today_iso()
        completed = {c["habit_id"]: c for c in self.db.get_completions_for_day(day)}
        for h in habits:
            name = h["name"]
            hid = h["id"]
            done = self.db.completion_exists(hid, day)
            label = f"{name} [{'done' if done else '---'}]"
            if h["type"] == "count":
                label = f"{name} (target {h['target']}) [{'done' if done else '---'}]"
            item = QListWidgetItem(label)
            item.setData(Qt.ItemDataRole.UserRole, h["id"])
            self.today_list.addItem(item)

    def add_habit(self):
        dlg = HabitDialog(self)
        if dlg.exec() == QDialog.DialogCode.Accepted:
            v = dlg.values()
            if not v["name"]:
                QMessageBox.warning(self, "Validation", "Name required")
                return
            self.db.add_habit(v["name"], v["type"], v["target"], "")
            self.load_habits()
            self.refresh_today_view()

    def edit_selected_habit(self):
        it = self.habits_list.currentItem()
        if not it:
            QMessageBox.warning(self, "Select", "Select a habit to edit")
            return
        hid = it.data(Qt.ItemDataRole.UserRole)
        h = self.db.get_habit(hid)
        dlg = HabitDialog(self, habit=h)
        if dlg.exec() == QDialog.DialogCode.Accepted:
            v = dlg.values()
            self.db.update_habit(hid, v["name"], v["type"], v["target"], "")
            self.load_habits()
            self.refresh_today_view()

    def delete_selected_habit(self):
        it = self.habits_list.currentItem()
        if not it:
            QMessageBox.warning(self, "Select", "Select a habit to delete")
            return
        hid = it.data(Qt.ItemDataRole.UserRole)
        if QMessageBox.question(self, "Confirm", "Delete this habit?") == QMessageBox.StandardButton.Yes:
            self.db.delete_habit(hid)
            self.load_habits()
            self.refresh_today_view()

    def mark_selected_done(self):
        items = self.today_list.selectedItems()
        if not items:
            QMessageBox.information(self, "Select", "Select habit(s) to mark done")
            return
        day = today_iso()
        for it in items:
            hid = it.data(Qt.ItemDataRole.UserRole)
            if self.db.completion_exists(hid, day):
                continue
            name = self.db.get_habit(hid)["name"]
            if QMessageBox.question(self, "Confirm",
                                    f"Confirm you completed '{name}' today?") != QMessageBox.StandardButton.Yes:
                continue
            habit = self.db.get_habit(hid)
            self.db.add_completion(hid, day, amount=habit["target"], confirmed=1)
            xp, coins = rewards_for_completion(habit)
            self.db.add_xp(xp)
            self.db.add_coins(coins)
            total_completed = self.count_total_completions()
            if total_completed >= 10:
                self.db.unlock_achievement("ten_completions", "10 Completions")
            if total_completed >= 100:
                self.db.unlock_achievement("hundred_completions", "100 Completions")
        self.refresh_profile_labels()
        self.refresh_today_view()
        self.draw_weekly_chart()

    def count_total_completions(self):
        c = self.db.conn.cursor()
        c.execute("SELECT COUNT(*) FROM completions")
        return c.fetchone()[0]

    def buy_item(self):
        key = self.shop_combo.currentData()
        item = self.shop_items.get(key)
        if not item:
            return
        if self.db.spend_coins(item["cost"]):
            self.db.buy_item(key)
            QMessageBox.information(self, "Bought", f"You bought {item['title']}!")
            self.refresh_profile_labels()
        else:
            QMessageBox.warning(self, "No coins", "You don't have enough coins.")

    def draw_weekly_chart(self):
        days = [date.today() - timedelta(days=i) for i in range(13, -1, -1)]
        day_labels = [d.strftime("%m-%d") for d in days]
        counts = []
        c = self.db.conn.cursor()
        for d in days:
            iso = d.isoformat()
            c.execute("SELECT COUNT(*) FROM completions WHERE day=?", (iso,))
            counts.append(c.fetchone()[0])

        self.figure.clear()
        ax = self.figure.add_subplot(111)
        ax.bar(day_labels, counts)
        ax.set_title("Completions (last 14 days)")
        ax.set_xticklabels(day_labels, rotation=45, ha="right")
        ax.set_ylabel("Completions")
        self.canvas.draw()

    def export_backup(self):
        path, _ = QFileDialog.getSaveFileName(self, "Export JSON Backup", "", "JSON files (*.json);;All files (*)")
        if not path:
            return
        self.db.export_json(path)
        QMessageBox.information(self, "Exported", f"Backup saved to {path}")

    def import_backup(self):
        path, _ = QFileDialog.getOpenFileName(self, "Import JSON Backup", "", "JSON files (*.json);;All files (*)")
        if not path:
            return
        self.db.import_json(path)
        QMessageBox.information(self, "Imported", f"Backup imported from {path}")
        self.load_habits()
        self.refresh_today_view()
        self.draw_weekly_chart()

    def handle_midnight(self):
        self.refresh_today_view()
        self.draw_weekly_chart()

def main():
    app = QApplication(sys.argv)
    win = HabitApp()
    win.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()