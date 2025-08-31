import sys, os, glob, datetime, psutil, random, requests
from pathlib import Path
from bs4 import BeautifulSoup

from PyQt6.QtWidgets import (
    QApplication, QWidget, QPushButton, QGridLayout, QVBoxLayout, QHBoxLayout,
    QLabel, QTextEdit, QScrollArea, QFrame, QSizePolicy, QMessageBox, QLineEdit,
    QComboBox
)
from PyQt6.QtCore import Qt, QTimer, QSize, QProcess
from PyQt6.QtGui import QFont, QTextCursor, QIcon

class Dashboard(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Personal Dashboard")

        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\global.ico"
        self.setWindowIcon(QIcon(icon_path))

        # self.setWindowFlag(Qt.WindowType.WindowStaysOnTopHint, True)
        self.resize(1100, 720)

        self.project_dir = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts"
        self.notes_path = Path(self.project_dir) / "Data\\quick_notes.txt"
        self.log_path = Path(self.project_dir) / "Data\\dashboard_log.txt"

        self.processes: dict[str, QProcess] = {}
        self.quotes = [
            "Start where you are. Use what you have. Do what you can.",
            "Small steps daily beat occasional sprints.",
            "Make it work, make it right, make it fast.",
            "Focus on the next 10 minutes.",
            "Ships are safe in harbor, but that's not what ships are for.",
            "You don’t need more time, you need fewer distractions.",
            "Done is better than perfect.",
            "One hour of deep work > five hours of busy work.",
            "If it’s worth doing, it’s worth doing badly at first.",
            "Consistency compounds.",
        ]

        root = QVBoxLayout(self)
        root.setContentsMargins(12, 12, 12, 12)
        root.setSpacing(10)

        top_bar = QHBoxLayout()
        top_bar.setSpacing(12)

        self.clock_label = QLabel()
        self.clock_label.setFont(QFont("Segoe UI", 16, QFont.Weight.Bold))
        self.clock_label.setStyleSheet("color: #e8e8e8;")
        top_bar.addWidget(self.clock_label, 0, Qt.AlignmentFlag.AlignLeft)

        self.weather_label = QLabel("Loading weather…")
        self.weather_label.setStyleSheet("color: #b0bec5;")
        self.weather_label.setFont(QFont("Segoe UI", 11))
        top_bar.addWidget(self.weather_label, 0, Qt.AlignmentFlag.AlignLeft)

        top_bar.addStretch(1)

        self.quote_label = QLabel(self.random_quote())
        self.quote_label.setWordWrap(True)
        self.quote_label.setStyleSheet("color: #cfd8dc;")
        self.quote_label.setMaximumWidth(600)
        top_bar.addWidget(self.quote_label)

        refresh_quote_btn = QPushButton("↻ New quote")
        refresh_quote_btn.clicked.connect(lambda: self.quote_label.setText(self.random_quote()))
        refresh_quote_btn.setStyleSheet(self._button_style())
        top_bar.addWidget(refresh_quote_btn, 0, Qt.AlignmentFlag.AlignRight)

        root.addLayout(top_bar)

        middle = QHBoxLayout()
        middle.setSpacing(14)
        root.addLayout(middle, 1)

        left_frame = QFrame()
        left_frame.setStyleSheet("background-color: #1f1f1f; border: 1px solid #2a2a2a; border-radius: 10px;")
        left_layout = QVBoxLayout(left_frame)
        left_layout.setContentsMargins(12, 12, 12, 12)
        left_layout.setSpacing(10)

        left_header = QHBoxLayout()
        lh = QLabel("Applications")
        lh.setStyleSheet("color: #e0e0e0;")
        lh.setFont(QFont("Segoe UI", 12, QFont.Weight.Bold))
        left_header.addWidget(lh)
        left_header.addStretch(1)
        rescan_btn = QPushButton("Rescan")
        rescan_btn.setStyleSheet(self._button_style())
        rescan_btn.clicked.connect(self.populate_grid)
        left_header.addWidget(rescan_btn)
        left_layout.addLayout(left_header)

        self.scroll = QScrollArea()
        self.scroll.setWidgetResizable(True)
        self.scroll.setStyleSheet("QScrollArea { background: transparent; border: none; }")
        self.grid_host = QFrame()
        self.grid_host.setStyleSheet("background: transparent;")
        self.grid = QGridLayout(self.grid_host)
        self.grid.setSpacing(12)
        self.scroll.setWidget(self.grid_host)
        left_layout.addWidget(self.scroll, 1)

        middle.addWidget(left_frame, 2)

        right_frame = QFrame()
        right_frame.setStyleSheet("background-color: #1f1f1f; border: 1px solid #2a2a2a; border-radius: 10px;")
        right_layout = QVBoxLayout(right_frame)
        right_layout.setContentsMargins(12, 12, 12, 12)
        right_layout.setSpacing(10)

        sys_box = QFrame()
        sys_box.setStyleSheet("background-color: #262626; border: 1px solid #333; border-radius: 8px;")
        sys_layout = QVBoxLayout(sys_box)
        sys_layout.setContentsMargins(10, 10, 10, 10)
        sys_layout.setSpacing(6)
        title = QLabel("System Resources")
        title.setStyleSheet("color: #e0e0e0;")
        title.setFont(QFont("Segoe UI", 11, QFont.Weight.Bold))
        self.cpu_label = QLabel()
        self.mem_label = QLabel()
        self.disk_label = QLabel()
        for w in (self.cpu_label, self.mem_label, self.disk_label):
            w.setStyleSheet("color: #cfd8dc;")
        sys_layout.addWidget(title)
        sys_layout.addWidget(self.cpu_label)
        sys_layout.addWidget(self.mem_label)
        sys_layout.addWidget(self.disk_label)
        right_layout.addWidget(sys_box)

        search_box = QFrame()
        search_box.setStyleSheet("background-color: #262626; border: 1px solid #333; border-radius: 8px;")
        search_layout = QVBoxLayout(search_box)
        search_layout.setContentsMargins(10, 10, 10, 10)
        search_layout.setSpacing(6)
        search_title = QLabel("Web Search")
        search_title.setStyleSheet("color: #e0e0e0;")
        search_title.setFont(QFont("Segoe UI", 11, QFont.Weight.Bold))
        search_layout.addWidget(search_title)
        self.search_input = QTextEdit()
        self.search_input.setPlaceholderText("Search Query")
        self.search_input.setFixedHeight(40)
        self.search_input.setStyleSheet(
            "QTextEdit { background-color: #1f1f1f; color: #e8e8e8; "
            "border: 1px solid #333; border-radius: 6px; }"
        )
        search_layout.addWidget(self.search_input)
        search_btn = QPushButton("Search")
        search_btn.setStyleSheet(self._button_style())
        search_btn.clicked.connect(self._search)
        search_layout.addWidget(search_btn)
        right_layout.addWidget(search_box)

        notes_box = QFrame()
        notes_box.setStyleSheet("background-color: #262626; border: 1px solid #333; border-radius: 8px;")
        notes_layout = QVBoxLayout(notes_box)
        notes_layout.setContentsMargins(10, 10, 10, 10)
        notes_title_row = QHBoxLayout()
        nt = QLabel("Quick Notes")
        nt.setStyleSheet("color: #e0e0e0;")
        nt.setFont(QFont("Segoe UI", 11, QFont.Weight.Bold))
        notes_title_row.addWidget(nt)
        notes_title_row.addStretch(1)
        self.notes_status = QLabel("")
        self.notes_status.setStyleSheet("color: #9e9e9e;")
        notes_title_row.addWidget(self.notes_status)
        notes_layout.addLayout(notes_title_row)

        self.notes_edit = QTextEdit()
        self.notes_edit.setPlaceholderText("Write anything…")
        self.notes_edit.setStyleSheet("QTextEdit { background-color: #1f1f1f; color: #e8e8e8; border: 1px solid #333; border-radius: 6px; }")
        self.notes_edit.textChanged.connect(self._queue_save_notes)
        notes_layout.addWidget(self.notes_edit)
        right_layout.addWidget(notes_box, 1)

        log_box = QFrame()
        log_box.setStyleSheet("background-color: #262626; border: 1px solid #333; border-radius: 8px;")
        log_layout = QVBoxLayout(log_box)
        log_layout.setContentsMargins(10, 10, 10, 10)
        log_title_row = QHBoxLayout()
        lt = QLabel("Log Console")
        lt.setStyleSheet("color: #e0e0e0;")
        lt.setFont(QFont("Segoe UI", 11, QFont.Weight.Bold))
        log_title_row.addWidget(lt)
        log_title_row.addStretch(1)
        clear_log_btn = QPushButton("Clear Log")
        clear_log_btn.setStyleSheet(self._button_style())
        clear_log_btn.clicked.connect(self.clear_log)
        log_title_row.addWidget(clear_log_btn)
        log_layout.addLayout(log_title_row)

        self.log_view = QTextEdit()
        self.log_view.setReadOnly(True)
        self.log_view.setStyleSheet("QTextEdit { background-color: #1f1f1f; color: #e8e8e8; border: 1px solid #333; border-radius: 6px; }")
        log_layout.addWidget(self.log_view, 1)

        right_layout.addWidget(log_box, 2)

        middle.addWidget(right_frame, 3)

        self.setStyleSheet("""
        QWidget { background-color: #141414; }
        QLabel { color: #e8e8e8; }
        QPushButton { color: #e8e8e8; }
        """)

        self._save_timer = QTimer(self)
        self._save_timer.setInterval(800)
        self._save_timer.setSingleShot(True)
        self._save_timer.timeout.connect(self.save_notes)

        self._ui_timer = QTimer(self)
        self._ui_timer.setInterval(1000)
        self._ui_timer.timeout.connect(self.update_clock_and_stats)
        self._ui_timer.start()

        self._weather_timer = QTimer(self)
        self._weather_timer.setInterval(30 * 60 * 1000)
        self._weather_timer.timeout.connect(self.update_weather)
        self._weather_timer.start()

        self.update_weather()

        self.load_notes()
        self.populate_grid()
        self.load_existing_log()
        self.update_clock_and_stats()

    def _button_style(self):
        return (
            "QPushButton { background-color: #333; border: 1px solid #444; border-radius: 8px; padding: 6px 10px; }"
            "QPushButton:hover { background-color: #3d3d3d; }"
            "QPushButton:pressed { background-color: #2a2a2a; }"
        )

    def grid_button_style(self):
        return (
            "QPushButton { background-color: #2e2e2e; color: #ffffff; border: 2px solid #1d1d1d;"
            " border-radius: 10px; text-align: center; }"
            "QPushButton:hover { background-color: #4a4a4a; }"
        )

    def load_notes(self):
        try:
            if self.notes_path.exists():
                self.notes_edit.setText(self.notes_path.read_text(encoding='utf-8'))
        except Exception as e:
            self.append_log(f"[ERROR] Failed to load notes: {e}")

    def _queue_save_notes(self):
        self.notes_status.setText("Saving…")
        self._save_timer.start()

    def save_notes(self):
        try:
            self.notes_path.write_text(self.notes_edit.toPlainText(), encoding='utf-8')
            ts = datetime.datetime.now().strftime('%H:%M:%S')
            self.notes_status.setText(f"Saved {ts}")
        except Exception as e:
            self.notes_status.setText("Save failed")
            self.append_log(f"[ERROR] Failed to save notes: {e}")

    def load_existing_log(self):
        try:
            if self.log_path.exists():
                content = self.log_path.read_text(encoding='utf-8')
                if len(content) > 100_000:
                    content = content[-100_000:]
                self.log_view.setPlainText(content)
                self.log_view.moveCursor(QTextCursor.MoveOperation.End)        
        except Exception as e:
            self.append_log(f"[ERROR] Failed to read log: {e}")

    def append_log(self, text: str):
        line = f"{datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}  {text}\n"
        self.log_view.moveCursor(QTextCursor.MoveOperation.End)        
        self.log_view.insertPlainText(line)
        self.log_view.moveCursor(QTextCursor.MoveOperation.End)        
        try:
            with open(self.log_path, 'a', encoding='utf-8') as f:
                f.write(line)
        except Exception:
            pass

    def clear_log(self):
        self.log_view.clear()
        try:
            self.log_path.write_text("", encoding='utf-8')
            self.append_log("[INFO] Log cleared.")
        except Exception as e:
            QMessageBox.warning(self, "Error", f"Failed to clear log: {e}")

    def update_clock_and_stats(self):
        now = datetime.datetime.now()
        self.clock_label.setText(now.strftime("%A, %B %d, %Y  •  %I:%M:%S %p |"))
        try:
            cpu = psutil.cpu_percent(interval=None)
            mem = psutil.virtual_memory().percent
            disk = psutil.disk_usage(os.path.splitdrive(self.project_dir)[0] + os.sep).percent if os.name == 'nt' else psutil.disk_usage('/').percent
            self.cpu_label.setText(f"CPU Usage: {cpu:.0f}%")
            self.mem_label.setText(f"Memory Usage: {mem:.0f}%")
            self.disk_label.setText(f"Disk Usage: {disk:.0f}%")
        except Exception as e:
            self.cpu_label.setText("CPU: n/a")
            self.mem_label.setText("Memory: n/a")
            self.disk_label.setText("Disk: n/a")
            self.append_log(f"[WARN] psutil error: {e}")

    def update_weather(self, city="Davao,PH"):
        api_key = "c03698e63ebb41feb866715fe018858c"
        url = f"https://api.openweathermap.org/data/2.5/weather?q={city}&units=metric&appid={api_key}"
        try:
            r = requests.get(url, timeout=5)
            data = r.json()
            if "main" in data:
                temp = data["main"]["temp"]
                humidity = data["main"]["humidity"]
                desc = data["weather"][0]["description"].title()
                self.weather_label.setText(f"{temp:.1f}°C, {humidity}% • {desc}")
            else:
                self.weather_label.setText("Weather unavailable")
        except Exception as e:
            self.weather_label.setText("Weather error")
            self.append_log(f"[ERROR] Weather API: {e}")

    def _search(self):
        query = self.search_input.toPlainText().strip()
        if not query:
            return

        url = "https://api.duckduckgo.com/"
        params = {"q": query, "format": "json", "pretty": 1}

        try:
            r = requests.get(url, params=params, timeout=8)
            data = r.json()

            self.append_log(f"[SEARCH] Results for '{query}':")

            if data.get("AbstractText"):
                self.append_log(f"  {data['AbstractText']} ({data.get('AbstractURL','')})")

            results = 0
            for topic in data.get("RelatedTopics", []):
                if "Text" in topic and "FirstURL" in topic:
                    self.append_log(f"  - {topic['Text']} ({topic['FirstURL']})")
                    results += 1
                    if results >= 5:
                        break

            if not data.get("AbstractText") and results == 0:
                self.append_log("  No results found.")
        except Exception as e:
            self.append_log(f"[ERROR] DuckDuckGo search failed: {e}")

    def random_quote(self):
        return random.choice(self.quotes)

    def populate_grid(self):
        while self.grid.count():
            item = self.grid.takeAt(0)
            w = item.widget()
            if w:
                w.deleteLater()

        script_paths = sorted(glob.glob(os.path.join(self.project_dir, '*.py')))
        script_paths = [p for p in script_paths if os.path.basename(p).lower() != 'dashboard.py']
        names = [Path(p).stem for p in script_paths]

        col_count = 3
        row = 0
        col = 0

        for name, path in zip(names, script_paths):
            btn = QPushButton(self.pretty_name(name))
            btn.setFixedSize(QSize(120, 120))
            btn.setStyleSheet(self.grid_button_style())
            btn.setFont(QFont("Segoe UI", 9, QFont.Weight.Bold))
            btn.clicked.connect(lambda _, p=path: self.launch_app(p))
            self.grid.addWidget(btn, row, col)

            col += 1
            if col >= col_count:
                col = 0
                row += 1

        if not script_paths:
            placeholder = QLabel("No .py files found in:\n" + self.project_dir)
            placeholder.setStyleSheet("color: #9e9e9e;")
            self.grid.addWidget(placeholder, 0, 0)

    @staticmethod
    def pretty_name(stem: str) -> str:
        s = stem.replace('_', ' ').replace('-', ' ')
        return s.title()

    def launch_app(self, script_path: str):
        self.append_log(f"[LAUNCH] {script_path}")

        proc = QProcess(self)
        proc.setProgram(sys.executable)
        proc.setArguments(["-u", script_path])
        proc.setWorkingDirectory(str(Path(script_path).parent))
        proc.setProcessChannelMode(QProcess.ProcessChannelMode.MergedChannels)

        proc.readyReadStandardOutput.connect(lambda p=proc, sp=script_path: self._read_output(p, sp))
        proc.finished.connect(lambda code, status, sp=script_path: self._process_finished(code, status, sp))

        key = f"{script_path}-{id(proc)}"
        self.processes[key] = proc
        proc.start()

        if not proc.waitForStarted(3000):
            self.append_log(f"[ERROR] Failed to start: {script_path}")
            self.processes.pop(key, None)

    def _read_output(self, proc: QProcess, script_path: str):
        try:
            data = proc.readAllStandardOutput()
            text = bytes(data).decode(errors='replace')
            if text:
                for line in text.splitlines():
                    self.append_log(f"[{Path(script_path).name}] {line}")
        except Exception as e:
            self.append_log(f"[ERROR] Reading output: {e}")

    def _process_finished(self, exit_code: int, exit_status: QProcess.ExitStatus, script_path: str):
        status = "NormalExit" if exit_status == QProcess.ExitStatus.NormalExit else "CrashExit"
        self.append_log(f"[EXIT] {Path(script_path).name} -> code={exit_code}, status={status}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = Dashboard()
    w.show()
    sys.exit(app.exec())