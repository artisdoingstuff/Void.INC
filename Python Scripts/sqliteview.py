import sys
import sqlite3
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTableWidget, QTableWidgetItem,
    QFileDialog, QTextEdit, QLabel, QMessageBox, QLineEdit, QAbstractItemView
)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QIcon

class SQLiteViewer(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("SQLite Viewer")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\global.ico"
        self.setWindowIcon(QIcon(icon_path))
        self.resize(900, 600)

        self.conn = None
        self.current_table = None
        self.current_headers = []
        self.pk_column_index = 0

        self.open_btn = QPushButton("Open Database")
        self.open_btn.clicked.connect(self.open_database)

        self.tables_label = QLabel("Tables: None")
        
        self.tables_list = QTextEdit()
        self.tables_list.setReadOnly(True)
        self.tables_list.setMaximumHeight(80)

        self.query_input = QLineEdit()
        self.query_input.setPlaceholderText("Enter SQL query (e.g., SELECT * FROM table)")
        
        self.run_query_btn = QPushButton("Run Query")
        self.run_query_btn.clicked.connect(self.run_query)
        
        self.results_table = QTableWidget()
        self.results_table.setEditTriggers(QAbstractItemView.EditTrigger.AllEditTriggers)
        self.results_table.itemChanged.connect(self.on_item_changed)

        top_layout = QHBoxLayout()
        top_layout.addWidget(self.open_btn)
        top_layout.addWidget(self.tables_label)

        query_layout = QHBoxLayout()
        query_layout.addWidget(self.query_input)
        query_layout.addWidget(self.run_query_btn)

        main_layout = QVBoxLayout()
        main_layout.addLayout(top_layout)
        main_layout.addWidget(self.tables_list)
        main_layout.addLayout(query_layout)
        main_layout.addWidget(self.results_table)

        self.setLayout(main_layout)

    def open_database(self):
        path, _ = QFileDialog.getOpenFileName(self, "Open SQLite Database", "", "SQLite Files (*.sqlite *.db *.sqlite3);;All Files (*)")
        if not path:
            return
        try:
            if self.conn:
                self.conn.close()
            self.conn = sqlite3.connect(path)
            self.load_tables()
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to open database:\n{e}")

    def load_tables(self):
        cursor = self.conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [row[0] for row in cursor.fetchall()]
        self.tables_label.setText(f"Tables: {', '.join(tables) if tables else 'None'}")
        self.tables_list.setPlainText("\n".join(tables))
        if tables:
            self.load_table_data(tables[0])

    def load_table_data(self, table_name):
        self.current_table = table_name
        cursor = self.conn.cursor()
        try:
            cursor.execute(f"SELECT * FROM {table_name} LIMIT 100")
            rows = cursor.fetchall()
            headers = [description[0] for description in cursor.description]

            self.current_headers = headers
            self.results_table.blockSignals(True)
            self.display_data(headers, rows)
            self.results_table.blockSignals(False)
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load table data:\n{e}")

    def display_data(self, headers, rows):
        self.results_table.clear()
        self.results_table.setRowCount(len(rows))
        self.results_table.setColumnCount(len(headers))
        self.results_table.setHorizontalHeaderLabels(headers)

        for row_idx, row_data in enumerate(rows):
            for col_idx, cell in enumerate(row_data):
                item = QTableWidgetItem(str(cell))
                self.results_table.setItem(row_idx, col_idx, item)

    def run_query(self):
        if not self.conn:
            QMessageBox.warning(self, "Warning", "No database is opened.")
            return

        query = self.query_input.text().strip()
        if not query:
            QMessageBox.warning(self, "Warning", "Please enter an SQL query.")
            return

        cursor = self.conn.cursor()
        try:
            cursor.execute(query)
            if query.lower().startswith("select"):
                rows = cursor.fetchall()
                headers = [desc[0] for desc in cursor.description]

                self.current_headers = headers
                self.current_table = None
                self.results_table.blockSignals(True)
                self.display_data(headers, rows)
                self.results_table.blockSignals(False)
            else:
                self.conn.commit()
                QMessageBox.information(self, "Success", "Query executed successfully.")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to execute query:\n{e}")

    def on_item_changed(self, item):
        if not self.conn or not self.current_table or not self.current_headers:
            return

        row = item.row()
        col = item.column()
        new_value = item.text()

        pk_value_item = self.results_table.item(row, self.pk_column_index)
        if pk_value_item is None:
            return
        pk_value = pk_value_item.text()

        pk_column_name = self.current_headers[self.pk_column_index]
        column_name = self.current_headers[col]

        try:
            cursor = self.conn.cursor()
            query = f"UPDATE {self.current_table} SET {column_name} = ? WHERE {pk_column_name} = ?"
            cursor.execute(query, (new_value, pk_value))
            self.conn.commit()
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to update database:\n{e}")
            self.load_table_data(self.current_table)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    viewer = SQLiteViewer()
    viewer.show()
    sys.exit(app.exec())