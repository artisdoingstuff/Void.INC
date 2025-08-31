import sys
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget,
    QListWidget, QTextEdit, QPushButton,
    QHBoxLayout, QVBoxLayout, QLineEdit,
    QMessageBox, QInputDialog, QLabel
)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QIcon
import sqlite3

class NoteTakingApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Notes for notes")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\global.ico"
        self.setWindowIcon(QIcon(icon_path))
        self.resize(900, 600)

        self.conn = sqlite3.connect("Data/notes.db")
        self.create_table()

        self.current_note_id = None
        self.text_modified = False

        self.setup_ui()
        self.load_notes()

    def create_table(self):
        c = self.conn.cursor()
        c.execute("""
            CREATE TABLE IF NOT EXISTS notes (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT UNIQUE NOT NULL,
                content TEXT
            )
        """)
        self.conn.commit()

    def setup_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        self.notes_list = QListWidget()
        self.notes_list.currentRowChanged.connect(self.on_note_select)

        self.search_bar = QLineEdit()
        self.search_bar.setPlaceholderText("Search notes...")
        self.search_bar.textChanged.connect(self.on_search_change)

        self.text_edit = QTextEdit()
        self.text_edit.textChanged.connect(self.on_text_changed)

        self.add_btn = QPushButton("Add Note")
        self.add_btn.clicked.connect(self.add_note)

        self.delete_btn = QPushButton("Delete Note")
        self.delete_btn.clicked.connect(self.delete_note)

        self.save_btn = QPushButton("Save Note")
        self.save_btn.clicked.connect(self.save_note)

        left_layout = QVBoxLayout()
        left_layout.addWidget(QLabel("Notes"))
        left_layout.addWidget(self.search_bar)
        left_layout.addWidget(self.notes_list)
        left_layout.addWidget(self.add_btn)
        left_layout.addWidget(self.delete_btn)

        right_layout = QVBoxLayout()
        right_layout.addWidget(QLabel("Content"))
        right_layout.addWidget(self.text_edit)
        right_layout.addWidget(self.save_btn)

        main_layout = QHBoxLayout()
        main_layout.addLayout(left_layout, 1)
        main_layout.addLayout(right_layout, 3)

        central_widget.setLayout(main_layout)

    def load_notes(self, filter_text=""):
        self.notes_list.clear()
        c = self.conn.cursor()
        if filter_text:
            c.execute("SELECT id, title FROM notes WHERE title LIKE ? ORDER BY title", (f"%{filter_text}%",))
        else:
            c.execute("SELECT id, title FROM notes ORDER BY title")
        self.notes = c.fetchall()
        for _, title in self.notes:
            self.notes_list.addItem(title)
        self.current_note_id = None
        self.text_edit.clear()
        self.text_modified = False

    def on_note_select(self, index):
        if index < 0 or index >= len(self.notes):
            return

        if self.text_modified:
            if not self.confirm_save():
                if self.current_note_id is not None:
                    for i, (nid, _) in enumerate(self.notes):
                        if nid == self.current_note_id:
                            self.notes_list.blockSignals(True)
                            self.notes_list.setCurrentRow(i)
                            self.notes_list.blockSignals(False)
                            return
                else:
                    self.notes_list.blockSignals(True)
                    self.notes_list.clearSelection()
                    self.notes_list.blockSignals(False)
                    return

        note_id, title = self.notes[index]
        self.current_note_id = note_id

        c = self.conn.cursor()
        c.execute("SELECT content FROM notes WHERE id = ?", (note_id,))
        content = c.fetchone()[0] or ""
        self.text_edit.blockSignals(True)
        self.text_edit.setPlainText(content)
        self.text_edit.blockSignals(False)
        self.text_modified = False

    def add_note(self):
        title, ok = QInputDialog.getText(self, "New Note", "Enter note title:")
        if not ok or not title.strip():
            return
        title = title.strip()
        c = self.conn.cursor()
        try:
            c.execute("INSERT INTO notes (title, content) VALUES (?, ?)", (title, ""))
            self.conn.commit()
            self.load_notes()
            for i, (nid, t) in enumerate(self.notes):
                if t == title:
                    self.notes_list.setCurrentRow(i)
                    break
        except sqlite3.IntegrityError:
            QMessageBox.warning(self, "Error", f"A note titled '{title}' already exists.")

    def delete_note(self):
        if self.current_note_id is None:
            QMessageBox.warning(self, "Delete Note", "No note selected.")
            return
        reply = QMessageBox.question(self, "Confirm Delete", "Delete selected note?", QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No)
        if reply == QMessageBox.StandardButton.Yes:
            c = self.conn.cursor()
            c.execute("DELETE FROM notes WHERE id = ?", (self.current_note_id,))
            self.conn.commit()
            self.load_notes()

    def save_note(self):
        if self.current_note_id is None:
            QMessageBox.warning(self, "Save Note", "No note selected.")
            return
        content = self.text_edit.toPlainText()
        c = self.conn.cursor()
        c.execute("UPDATE notes SET content = ? WHERE id = ?", (content, self.current_note_id))
        self.conn.commit()
        self.text_modified = False
        QMessageBox.information(self, "Saved", "Note saved.")

    def on_text_changed(self):
        self.text_modified = True

    def confirm_save(self):
        reply = QMessageBox.question(self, "Save Changes?", "You have unsaved changes. Save now?", QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No)
        return reply == QMessageBox.StandardButton.Yes

    def on_search_change(self, text):
        self.load_notes(filter_text=text.strip())

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NoteTakingApp()
    window.show()
    sys.exit(app.exec())