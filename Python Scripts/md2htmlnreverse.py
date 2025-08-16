import sys
from PyQt6.QtWidgets import (
    QApplication, QWidget, QTextEdit, QPushButton,
    QHBoxLayout, QVBoxLayout
)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QFont, QIcon
import markdown
import html2text
from bs4 import BeautifulSoup

class TranslatorApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Markdown ↔ HTML Translator")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\dashboard-icon.png"
        self.setWindowIcon(QIcon(icon_path))
        self.resize(900, 600)

        self.md_to_html = True
        self.last_input = ""

        self.toggle_btn = QPushButton("Switch to HTML → Markdown")
        self.toggle_btn.clicked.connect(self.toggle_mode)

        self.input_text = QTextEdit()
        self.input_text.textChanged.connect(self.on_input_change)

        self.output_text = QTextEdit()
        self.output_text.setReadOnly(True)
        self.output_text.setStyleSheet("background-color: #f0f0f0; color: black;")

        font = QFont("Courier New")
        font.setStyleHint(QFont.StyleHint.Monospace)
        self.output_text.setFont(font)
        self.output_text.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)

        btn_layout = QHBoxLayout()
        btn_layout.addWidget(self.toggle_btn)

        text_layout = QHBoxLayout()
        text_layout.addWidget(self.input_text)
        text_layout.addWidget(self.output_text)

        main_layout = QVBoxLayout()
        main_layout.addLayout(btn_layout)
        main_layout.addLayout(text_layout)

        self.setLayout(main_layout)

    def pretty_html(self, html_str):
        try:
            soup = BeautifulSoup(html_str, "html.parser")

            for li in soup.find_all("li"):
                if li.string and "⮕" in li.string:
                    parts = li.string.split("⮕", 1)
                    main_text = parts[0].strip()
                    sub_text = parts[1].strip()

                    li.string = main_text
                    sub_ul = soup.new_tag("ul")
                    sub_li = soup.new_tag("li")
                    sub_li.string = sub_text
                    sub_ul.append(sub_li)
                    li.append(sub_ul)

                else:
                    br_tags = li.find_all("br")
                    for br in br_tags:
                        if br.next_sibling and "⮕" in str(br.next_sibling):
                            sub_text = str(br.next_sibling).replace("⮕", "").strip()
                            sub_ul = soup.new_tag("ul")
                            sub_li = soup.new_tag("li")
                            sub_li.string = sub_text
                            sub_ul.append(sub_li)
                            br.insert_after(sub_ul)
                            br.extract()
                            if br.next_sibling:
                                br.next_sibling.extract()

            return soup.prettify()
        except Exception:
            return html_str

    def on_input_change(self):
        current_input = self.input_text.toPlainText()
        if current_input == self.last_input:
            return
        self.last_input = current_input

        try:
            if self.md_to_html:
                raw_html = markdown.markdown(current_input)
                converted = self.pretty_html(raw_html)
            else:
                converter = html2text.HTML2Text()
                converter.ignore_links = False
                converted = converter.handle(current_input)
        except Exception as e:
            converted = f"Error: {e}"

        self.output_text.setPlainText(converted)

    def toggle_mode(self):
        self.md_to_html = not self.md_to_html
        if self.md_to_html:
            self.toggle_btn.setText("Switch to HTML → Markdown")
        else:
            self.toggle_btn.setText("Switch to Markdown → HTML")

        input_content = self.input_text.toPlainText()
        output_content = self.output_text.toPlainText()

        self.input_text.blockSignals(True)
        self.output_text.blockSignals(True)

        self.input_text.setPlainText(output_content)
        self.output_text.setPlainText(input_content)

        self.input_text.blockSignals(False)
        self.output_text.blockSignals(False)

        self.last_input = self.input_text.toPlainText()
        self.on_input_change()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = TranslatorApp()
    window.show()
    sys.exit(app.exec())