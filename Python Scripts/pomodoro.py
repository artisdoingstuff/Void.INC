import sys
import sqlite3
from datetime import datetime, timedelta

from PyQt6.QtCore import Qt, QTimer, QEvent
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QPushButton, QMessageBox
)
from PyQt6.QtGui import QIcon

class PomodoroTimer(QWidget):
    INACTIVITY_LIMIT_MS = 5 * 60 * 1000
    
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Pomopomodorodoro")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\global.ico"
        self.setWindowIcon(QIcon(icon_path))
        self.resize(300, 200)

        self.pomodoro_duration_sec = 25 * 60
        self.time_left_sec = self.pomodoro_duration_sec
        self.timer_running = False
        self.last_activity_time = datetime.now()
        self.inactivity_flag = False
        self.session_start_time = None
        
        self.timer_label = QLabel(self.format_time(self.time_left_sec))
        self.timer_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.timer_label.setStyleSheet("font-size: 32px;")

        self.status_label = QLabel("Status: Ready")
        self.status_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.start_btn = QPushButton("Start")
        self.start_btn.clicked.connect(self.start_timer)

        self.pause_btn = QPushButton("Pause")
        self.pause_btn.clicked.connect(self.pause_timer)
        self.pause_btn.setEnabled(False)

        self.reset_btn = QPushButton("Reset")
        self.reset_btn.clicked.connect(self.reset_timer)
        self.reset_btn.setEnabled(False)

        btn_layout = QHBoxLayout()
        btn_layout.addWidget(self.start_btn)
        btn_layout.addWidget(self.pause_btn)
        btn_layout.addWidget(self.reset_btn)

        layout = QVBoxLayout()
        layout.addWidget(self.timer_label)
        layout.addWidget(self.status_label)
        layout.addLayout(btn_layout)

        self.setLayout(layout)

        self.countdown_timer = QTimer()
        self.countdown_timer.setInterval(1000)
        self.countdown_timer.timeout.connect(self.update_timer)

        self.inactivity_timer = QTimer()
        self.inactivity_timer.setInterval(1000)
        self.inactivity_timer.timeout.connect(self.check_inactivity)
        self.inactivity_timer.start()

        self.conn = sqlite3.connect("Data/pomo_sessions.db")
        self.create_table()

        self.installEventFilter(self)

    def create_table(self):
        cursor = self.conn.cursor()
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS sessions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                start_time TEXT,
                end_time TEXT,
                completed BOOLEAN,
                inactivity_flag BOOLEAN
            )
        """)
        self.conn.commit()

    def eventFilter(self, obj, event):
        if event.type() in (QEvent.Type.MouseMove, QEvent.Type.KeyPress):
            self.last_activity_time = datetime.now()
            if self.inactivity_flag:
                self.inactivity_flag = False
                self.status_label.setText("Status: Active")
        return super().eventFilter(obj, event)

    def start_timer(self):
        if not self.timer_running:
            self.timer_running = True
            self.countdown_timer.start()
            self.session_start_time = datetime.now()
            self.status_label.setText("Status: Running")
            self.start_btn.setEnabled(False)
            self.pause_btn.setEnabled(True)
            self.reset_btn.setEnabled(True)
            self.inactivity_flag = False
            self.last_activity_time = datetime.now()

    def pause_timer(self):
        if self.timer_running:
            self.timer_running = False
            self.countdown_timer.stop()
            self.status_label.setText("Status: Paused")
            self.start_btn.setEnabled(True)
            self.pause_btn.setEnabled(False)

    def reset_timer(self):
        self.timer_running = False
        self.countdown_timer.stop()
        self.time_left_sec = self.pomodoro_duration_sec
        self.timer_label.setText(self.format_time(self.time_left_sec))
        self.status_label.setText("Status: Ready")
        self.start_btn.setEnabled(True)
        self.pause_btn.setEnabled(False)
        self.reset_btn.setEnabled(False)
        self.inactivity_flag = False

    def update_timer(self):
        if self.timer_running:
            self.time_left_sec -= 1
            self.timer_label.setText(self.format_time(self.time_left_sec))
            if self.time_left_sec <= 0:
                self.countdown_timer.stop()
                self.timer_running = False
                self.prompt_completion()

    def prompt_completion(self):
        msg = QMessageBox(self)
        msg.setWindowTitle("Pomodoro Complete")
        msg.setText("Did you really complete the Pomodoro session?")
        msg.setStandardButtons(QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No)
        ret = msg.exec()

        completed = (ret == QMessageBox.StandardButton.Yes)
        self.log_session(completed)

        if completed:
            self.status_label.setText("Status: Completed!")
        else:
            self.status_label.setText("Status: Not completed")

        self.start_btn.setEnabled(True)
        self.pause_btn.setEnabled(False)
        self.reset_btn.setEnabled(True)

        self.time_left_sec = self.pomodoro_duration_sec
        self.timer_label.setText(self.format_time(self.time_left_sec))

    def check_inactivity(self):
        if self.timer_running:
            elapsed_ms = (datetime.now() - self.last_activity_time).total_seconds() * 1000
            if elapsed_ms > self.INACTIVITY_LIMIT_MS and not self.inactivity_flag:
                self.inactivity_flag = True
                self.pause_timer()
                self.status_label.setText("Paused due to inactivity")
                QMessageBox.warning(self, "Inactivity Detected",
                                    "No activity detected for 5 minutes. Timer paused.")

    def log_session(self, completed):
        cursor = self.conn.cursor()
        end_time = datetime.now()
        cursor.execute("""
            INSERT INTO sessions (start_time, end_time, completed, inactivity_flag)
            VALUES (?, ?, ?, ?)
        """, (self.session_start_time.isoformat(),
              end_time.isoformat(),
              int(completed),
              int(self.inactivity_flag)))
        self.conn.commit()

    @staticmethod
    def format_time(seconds):
        m, s = divmod(seconds, 60)
        return f"{int(m):02}:{int(s):02}"

if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = PomodoroTimer()
    win.show()
    sys.exit(app.exec())