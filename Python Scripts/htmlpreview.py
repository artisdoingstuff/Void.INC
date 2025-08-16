import sys
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QSplitter, QPlainTextEdit,
    QFileDialog, QMessageBox, QMenuBar
)
from PyQt6.QtCore import Qt, QTimer
from PyQt6.QtWebEngineWidgets import QWebEngineView
from PyQt6.QtGui import QAction
from pathlib import Path


class HTMLEditorPreview(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HTML Previewer")
        self.resize(1200, 700)

        self.current_file = None

        menu_bar = QMenuBar()
        file_menu = menu_bar.addMenu("File")

        open_action = QAction("Open", self)
        open_action.triggered.connect(self.open_file)
        file_menu.addAction(open_action)

        save_action = QAction("Save", self)
        save_action.triggered.connect(self.save_file)
        file_menu.addAction(save_action)

        self.setMenuBar(menu_bar)

        splitter = QSplitter(Qt.Orientation.Horizontal)

        self.editor = QPlainTextEdit()
        self.editor.setPlainText(
            """<html>
<head>
    <style>
        body { font-family: Arial; background-color: #f0f0f0; padding: 20px; }
        h1 { color: #4CAF50; }
        p { font-size: 18px; }
    </style>
</head>
<body>
    <h1>Hello from PyQt6!</h1>
    <p>Live preview updates as you type.</p>
</body>
</html>"""
        )

        self.preview = QWebEngineView()

        splitter.addWidget(self.editor)
        splitter.addWidget(self.preview)
        splitter.setSizes([500, 700])

        self.setCentralWidget(splitter)

        self.update_timer = QTimer(self)
        self.update_timer.setInterval(300)
        self.update_timer.setSingleShot(True)
        self.update_timer.timeout.connect(self.update_preview)

        self.editor.textChanged.connect(self.on_text_changed)

        self.update_preview()

    def on_text_changed(self):
        self.update_timer.start()

    def update_preview(self):
        html_content = self.editor.toPlainText()
        self.preview.setHtml(html_content)

    def open_file(self):
        file_path, _ = QFileDialog.getOpenFileName(
            self, "Open HTML File", "", "HTML Files (*.html *.htm);;All Files (*)"
        )
        if file_path:
            try:
                content = Path(file_path).read_text(encoding="utf-8")
                self.editor.setPlainText(content)
                self.current_file = file_path
                self.update_preview()
            except Exception as e:
                QMessageBox.critical(self, "Error", f"Could not open file:\n{e}")

    def save_file(self):
        if not self.current_file:
            file_path, _ = QFileDialog.getSaveFileName(
                self, "Save HTML File", "", "HTML Files (*.html *.htm);;All Files (*)"
            )
            if not file_path:
                return
            self.current_file = file_path

        try:
            Path(self.current_file).write_text(self.editor.toPlainText(), encoding="utf-8")
            QMessageBox.information(self, "Saved", f"File saved:\n{self.current_file}")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Could not save file:\n{e}")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = HTMLEditorPreview()
    window.show()
    sys.exit(app.exec())