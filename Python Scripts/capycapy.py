import sys
import os
import json
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QListWidget, QListWidgetItem,
    QVBoxLayout, QHBoxLayout, QPushButton, QColorDialog, QLabel, QSpinBox,
    QCheckBox, QFileDialog, QMessageBox, QSlider, QToolBar
)
from PyQt6.QtGui import (
    QPainter, QImage, QColor, QPen, QPixmap, QMouseEvent, QActionGroup, QIcon, QAction
)
from PyQt6.QtCore import Qt, QRect, QPoint, QTimer, QPointF, QRectF
from PIL import Image, ImageSequence

CANVAS_PIXEL_SIZE = 124
DISPLAY_SIZE = 256
THUMBNAIL_SIZE = 64
MAX_UNDO = 20

class Frame:
    def __init__(self, w=CANVAS_PIXEL_SIZE, h=CANVAS_PIXEL_SIZE):
        self.w = w
        self.h = h
        self.image = QImage(w, h, QImage.Format.Format_RGBA8888)
        self.image.fill(Qt.GlobalColor.transparent)
        self.undo_stack = []
        self.redo_stack = []

    def push_undo(self):
        if len(self.undo_stack) >= MAX_UNDO:
            self.undo_stack.pop(0)
        self.undo_stack.append(self.image.copy())
        self.redo_stack.clear()

    def undo(self):
        if not self.undo_stack:
            return
        self.redo_stack.append(self.image.copy())
        self.image = self.undo_stack.pop()

    def redo(self):
        if not self.redo_stack:
            return
        self.undo_stack.append(self.image.copy())
        self.image = self.redo_stack.pop()

class Canvas(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumSize(DISPLAY_SIZE, DISPLAY_SIZE)
        self.frames = []
        self.current_frame_idx = -1

        self.tool = 'brush'
        self.brush_color = QColor('black')
        self.brush_size = 1
        self.eraser_size = 1
        self.onion_enabled = True
        self.onion_opacity = 0.4

        self.zoom = 1.0
        self.min_zoom = 0.25
        self.max_zoom = 8.0
        self.pan = QPoint(0,0)
        self.is_panning = False
        self.pan_start_pos = None

        self.drawing = False
        self.last_pos = None
        self.shape_start = None

        self.setMouseTracking(True)
        self.setFocusPolicy(Qt.FocusPolicy.ClickFocus)

    def add_frame(self):
        frame = Frame()
        self.frames.append(frame)
        self.current_frame_idx = len(self.frames) -1
        self.update()

    def duplicate_frame(self):
        if self.current_frame_idx < 0:
            return
        src = self.frames[self.current_frame_idx]
        dup = Frame()
        dup.image = src.image.copy()
        self.frames.insert(self.current_frame_idx + 1, dup)
        self.current_frame_idx += 1
        self.update()

    def delete_frame(self):
        if self.current_frame_idx < 0:
            return
        del self.frames[self.current_frame_idx]
        if not self.frames:
            self.current_frame_idx = -1
        else:
            self.current_frame_idx = max(0, self.current_frame_idx - 1)
        self.update()

    def move_frame(self, delta):
        idx = self.current_frame_idx
        if idx < 0:
            return
        new_idx = idx + delta
        if new_idx < 0 or new_idx >= len(self.frames):
            return
        self.frames[idx], self.frames[new_idx] = self.frames[new_idx], self.frames[idx]
        self.current_frame_idx = new_idx
        self.update()

    def set_tool(self, tool_name):
        self.tool = tool_name

    def set_brush_color(self, color):
        self.brush_color = color

    def set_brush_size(self, size):
        self.brush_size = size

    def set_eraser_size(self, size):
        self.eraser_size = size

    def toggle_onion(self, enabled):
        self.onion_enabled = enabled
        self.update()

    def set_onion_opacity(self, opacity):
        self.onion_opacity = opacity
        self.update()

    def undo(self):
        if self.current_frame_idx < 0:
            return
        self.frames[self.current_frame_idx].undo()
        self.update()

    def redo(self):
        if self.current_frame_idx < 0:
            return
        self.frames[self.current_frame_idx].redo()
        self.update()

    def change_zoom(self, factor, center=None):
        if self.current_frame_idx < 0 or not self.frames:
            return

        old_zoom = self.zoom
        self.zoom = min(self.max_zoom, max(self.min_zoom, self.zoom * factor))

        if center is not None:
            cx, cy = center
            frame = self.frames[self.current_frame_idx]
            widget_w = self.width()
            widget_h = self.height()

            scaled_w_old = frame.w * old_zoom
            scaled_h_old = frame.h * old_zoom
            scaled_w_new = frame.w * self.zoom
            scaled_h_new = frame.h * self.zoom

            left_old = (widget_w - scaled_w_old) / 2 + self.pan.x()
            top_old = (widget_h - scaled_h_old) / 2 + self.pan.y()

            img_x = (cx - left_old) / old_zoom
            img_y = (cy - top_old) / old_zoom

            left_new = cx - img_x * self.zoom
            top_new = cy - img_y * self.zoom

            self.pan.setX(int(left_new - (widget_w - scaled_w_new) / 2))
            self.pan.setY(int(top_new - (widget_h - scaled_h_new) / 2))

        self.update()

    def reset_view(self):
        self.zoom = 1.0
        self.pan = QPoint(0,0)
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(self.rect(), QColor('white'))

        if self.current_frame_idx < 0 or not self.frames:
            return

        frame = self.frames[self.current_frame_idx]

        widget_w = self.width()
        widget_h = self.height()

        scaled_w = int(frame.w * self.zoom)
        scaled_h = int(frame.h * self.zoom)

        left = (widget_w - scaled_w) // 2 + self.pan.x()
        top = (widget_h - scaled_h) // 2 + self.pan.y()

        painter.save()
        shade_color = QColor(100, 100, 100, 150)
        painter.setBrush(shade_color)
        painter.setPen(Qt.PenStyle.NoPen)

        full_rect = self.rect()
        draw_rect = QRectF(left, top, scaled_w, scaled_h)

        painter.drawRect(QRectF(full_rect.left(), full_rect.top(), full_rect.width(), draw_rect.top() - full_rect.top()))
        painter.drawRect(QRectF(full_rect.left(), draw_rect.bottom(), full_rect.width(), full_rect.bottom() - draw_rect.bottom()))
        painter.drawRect(QRectF(full_rect.left(), draw_rect.top(), draw_rect.left() - full_rect.left(), draw_rect.height()))
        painter.drawRect(QRectF(draw_rect.right(), draw_rect.top(), full_rect.right() - draw_rect.right(), draw_rect.height()))
        painter.restore()

        if self.onion_enabled and self.current_frame_idx > 0:
            prev = self.frames[self.current_frame_idx - 1].image
            onion_img = prev.scaled(
                scaled_w, scaled_h,
                Qt.AspectRatioMode.IgnoreAspectRatio,
                Qt.TransformationMode.FastTransformation)
            painter.setOpacity(self.onion_opacity)
            painter.drawImage(left, top, onion_img)
            painter.setOpacity(1.0)

        img_scaled = frame.image.scaled(
            scaled_w, scaled_h,
            Qt.AspectRatioMode.IgnoreAspectRatio,
            Qt.TransformationMode.FastTransformation)
        painter.drawImage(left, top, img_scaled)

        if self.zoom >= 2.0:
            pen = QPen(QColor(150, 150, 150, 100))
            painter.setPen(pen)
            for x in range(frame.w + 1):
                xpos = left + x * self.zoom
                painter.drawLine(QPointF(xpos, top), QPointF(xpos, top + scaled_h))
            for y in range(frame.h + 1):
                ypos = top + y * self.zoom
                painter.drawLine(QPointF(left, ypos), QPointF(left + scaled_w, ypos))

    def mousePressEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self.drawing = True
            img_x, img_y = self.screen_to_image(event.position())
            self.shape_start = (img_x, img_y)
            self.last_pos = (img_x, img_y)
            if self.current_frame_idx < 0:
                return
            self.frames[self.current_frame_idx].push_undo()
            if self.tool in ('brush', 'eraser'):
                self.draw_point(img_x, img_y)
            self.update()
        elif event.button() == Qt.MouseButton.RightButton:
            self.is_panning = True
            self.pan_start_pos = event.position()

    def mouseMoveEvent(self, event):
        if self.drawing:
            img_x, img_y = self.screen_to_image(event.position())
            if self.current_frame_idx < 0:
                return
            if self.tool == 'brush':
                self.draw_line(self.last_pos, (img_x, img_y), color=self.brush_color, size=self.brush_size)
            elif self.tool == 'eraser':
                self.draw_line(self.last_pos, (img_x, img_y), color=QColor(0,0,0,0), size=self.eraser_size, erase=True)
            else:
                pass
            self.last_pos = (img_x, img_y)
            self.update()
        elif self.is_panning:
            delta = event.position() - self.pan_start_pos
            self.pan += delta.toPoint()
            self.pan_start_pos = event.position()
            self.update()

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            if self.drawing and self.tool in ('rect', 'ellipse', 'line'):
                img_x, img_y = self.screen_to_image(event.position())
                self.draw_shape(self.shape_start[0], self.shape_start[1], img_x, img_y, self.tool)
            self.drawing = False
            self.shape_start = None
            self.last_pos = None
            self.update()
        elif event.button() == Qt.MouseButton.RightButton:
            self.is_panning = False

    def wheelEvent(self, event):
        angle = event.angleDelta().y()
        factor = 1.1 if angle > 0 else 0.9
        self.change_zoom(factor, center=(event.position().x(), event.position().y()))

    def screen_to_image(self, pos):
        if self.current_frame_idx < 0 or not self.frames:
            return -1, -1
        frame = self.frames[self.current_frame_idx]
        widget_w = self.width()
        widget_h = self.height()

        scaled_w = frame.w * self.zoom
        scaled_h = frame.h * self.zoom

        left = (widget_w - scaled_w) / 2 + self.pan.x()
        top = (widget_h - scaled_h) / 2 + self.pan.y()

        x = int((pos.x() - left) / self.zoom)
        y = int((pos.y() - top) / self.zoom)

        x = max(0, min(frame.w - 1, x))
        y = max(0, min(frame.h - 1, y))

        return x, y

    def draw_point(self, x, y, color=None, size=1, erase=False):
        if self.current_frame_idx < 0:
            return
        frame = self.frames[self.current_frame_idx]
        if color is None:
            color = self.brush_color
        painter = QPainter(frame.image)
        painter.setPen(QPen(color))
        painter.setBrush(color)
        half = size // 2
        for dx in range(-half, half + 1):
            for dy in range(-half, half + 1):
                px, py = x + dx, y + dy
                if 0 <= px < frame.w and 0 <= py < frame.h:
                    if erase:
                        frame.image.setPixelColor(px, py, QColor(0,0,0,0))
                    else:
                        frame.image.setPixelColor(px, py, color)

    def draw_line(self, start, end, color=None, size=1, erase=False):
        if self.current_frame_idx < 0:
            return
        frame = self.frames[self.current_frame_idx]
        x0,y0 = start
        x1,y1 = end
        points = self.bresenham_line(x0,y0,x1,y1)
        for px, py in points:
            self.draw_point(px, py, color, size, erase)

    def bresenham_line(self, x0,y0,x1,y1):
        points = []
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        x, y = x0, y0
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        if dx > dy:
            err = dx // 2
            while x != x1:
                points.append((x,y))
                err -= dy
                if err < 0:
                    y += sy
                    err += dx
                x += sx
            points.append((x,y))
        else:
            err = dy // 2
            while y != y1:
                points.append((x,y))
                err -= dx
                if err < 0:
                    x += sx
                    err += dy
                y += sy
            points.append((x,y))
        return points

    def draw_shape(self, x0,y0,x1,y1, tool='rect'):
        if self.current_frame_idx < 0:
            return
        frame = self.frames[self.current_frame_idx]
        if tool == 'rect':
            self.draw_rectangle(frame.image, x0,y0,x1,y1)
        elif tool == 'ellipse':
            self.draw_ellipse(frame.image, x0,y0,x1,y1)
        elif tool == 'line':
            self.draw_line((x0,y0),(x1,y1), color=self.brush_color, size=max(1,self.brush_size))

    def draw_rectangle(self, image, x0,y0,x1,y1):
        xmin, xmax = sorted((x0,x1))
        ymin, ymax = sorted((y0,y1))
        for x in range(xmin, xmax+1):
            for y in (ymin, ymax):
                if 0 <= x < image.width() and 0 <= y < image.height():
                    image.setPixelColor(x,y,self.brush_color)
        for y in range(ymin, ymax+1):
            for x in (xmin, xmax):
                if 0 <= x < image.width() and 0 <= y < image.height():
                    image.setPixelColor(x,y,self.brush_color)

    def draw_ellipse(self, image, x0,y0,x1,y1):
        xmin, xmax = sorted((x0,x1))
        ymin, ymax = sorted((y0,y1))
        a = (xmax - xmin) / 2
        b = (ymax - ymin) / 2
        xc = (xmin + xmax) / 2
        yc = (ymin + ymax) / 2
        for x in range(xmin, xmax+1):
            for y in range(ymin, ymax+1):
                val = ((x - xc)**2) / (a**2 if a!=0 else 1) + ((y - yc)**2) / (b**2 if b!=0 else 1)
                if 0.9 <= val <= 1.1:
                    if 0 <= x < image.width() and 0 <= y < image.height():
                        image.setPixelColor(x,y,self.brush_color)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("CapyCapy")
        icon_path = r"C:\\Users\\defau\\OneDrive\\Documents\\C++ Games\\Buh\\Python Scripts\\App Icons\\capycapy-icon.png"
        ### Image taken from https://www.shutterstock.com/image-vector/cute-capybara-orange-on-head-cartoon-2476753767, by https://www.shutterstock.com/g/laias
        self.setWindowIcon(QIcon(icon_path))
        self.resize(1000, 600)

        central = QWidget()
        self.setCentralWidget(central)
        main_layout = QHBoxLayout()
        central.setLayout(main_layout)

        self.thumb_list = QListWidget()
        self.thumb_list.setFixedWidth(THUMBNAIL_SIZE+20)
        main_layout.addWidget(self.thumb_list)

        self.canvas = Canvas()
        main_layout.addWidget(self.canvas, 1)

        toolbar_layout = QVBoxLayout()
        main_layout.addLayout(toolbar_layout)

        btn_new = QPushButton("New Frame")
        btn_new.clicked.connect(self.add_frame)
        toolbar_layout.addWidget(btn_new)

        btn_dup = QPushButton("Duplicate Frame")
        btn_dup.clicked.connect(self.duplicate_frame)
        toolbar_layout.addWidget(btn_dup)

        btn_del = QPushButton("Delete Frame")
        btn_del.clicked.connect(self.delete_frame)
        toolbar_layout.addWidget(btn_del)

        move_layout = QHBoxLayout()
        btn_left = QPushButton("← Move Left")
        btn_left.clicked.connect(lambda: self.move_frame(-1))
        btn_right = QPushButton("Move Right →")
        btn_right.clicked.connect(lambda: self.move_frame(1))
        move_layout.addWidget(btn_left)
        move_layout.addWidget(btn_right)
        toolbar_layout.addLayout(move_layout)

        toolbar_layout.addSpacing(10)

        tools = ['brush', 'eraser', 'rect', 'ellipse', 'line']
        self.tool_buttons = []
        self.tool_group = QActionGroup(self)
        for t in tools:
            act = QAction(t.capitalize(), self, checkable=True)
            if t == 'brush':
                act.setChecked(True)
            act.triggered.connect(lambda checked, tool=t: self.select_tool(tool))
            self.tool_group.addAction(act)
            toolbar_layout.addWidget(QPushButton(act.text(), clicked=lambda checked, tool=t: self.select_tool(tool)))

        toolbar_layout.addSpacing(10)

        self.color_btn = QPushButton("Brush Color")
        self.color_btn.clicked.connect(self.pick_color)
        toolbar_layout.addWidget(self.color_btn)

        lbl_brush = QLabel("Brush Size")
        toolbar_layout.addWidget(lbl_brush)
        self.brush_size_spin = QSpinBox()
        self.brush_size_spin.setRange(1, 16)
        self.brush_size_spin.setValue(1)
        self.brush_size_spin.valueChanged.connect(self.change_brush_size)
        toolbar_layout.addWidget(self.brush_size_spin)

        lbl_eraser = QLabel("Eraser Size")
        toolbar_layout.addWidget(lbl_eraser)
        self.eraser_size_spin = QSpinBox()
        self.eraser_size_spin.setRange(1, 16)
        self.eraser_size_spin.setValue(1)
        self.eraser_size_spin.valueChanged.connect(self.change_eraser_size)
        toolbar_layout.addWidget(self.eraser_size_spin)

        self.onion_checkbox = QCheckBox("Onion Skin")
        self.onion_checkbox.setChecked(True)
        self.onion_checkbox.stateChanged.connect(self.toggle_onion)
        toolbar_layout.addWidget(self.onion_checkbox)

        lbl_opacity = QLabel("Onion Skin Opacity")
        toolbar_layout.addWidget(lbl_opacity)
        self.opacity_slider = QSlider(Qt.Orientation.Horizontal)
        self.opacity_slider.setRange(1, 100)
        self.opacity_slider.setValue(40)
        self.opacity_slider.valueChanged.connect(self.change_opacity)
        toolbar_layout.addWidget(self.opacity_slider)

        toolbar_layout.addSpacing(10)

        undo_btn = QPushButton("Undo")
        undo_btn.clicked.connect(self.canvas.undo)
        toolbar_layout.addWidget(undo_btn)

        redo_btn = QPushButton("Redo")
        redo_btn.clicked.connect(self.canvas.redo)
        toolbar_layout.addWidget(redo_btn)

        toolbar_layout.addStretch()

        btn_export_gif = QPushButton("Export GIF")
        btn_export_gif.clicked.connect(self.export_gif)
        toolbar_layout.addWidget(btn_export_gif)

        btn_export_pngs = QPushButton("Export PNGs")
        btn_export_pngs.clicked.connect(self.export_pngs)
        toolbar_layout.addWidget(btn_export_pngs)

        btn_export_spritesheet = QPushButton("Export Spritesheet + JSON")
        btn_export_spritesheet.clicked.connect(self.export_spritesheet)
        toolbar_layout.addWidget(btn_export_spritesheet)

        self.thumb_list.currentRowChanged.connect(self.change_frame)

        self.add_frame()

    def add_frame(self):
        self.canvas.add_frame()
        self.update_thumbnails()
        self.thumb_list.setCurrentRow(self.canvas.current_frame_idx)

    def duplicate_frame(self):
        self.canvas.duplicate_frame()
        self.update_thumbnails()
        self.thumb_list.setCurrentRow(self.canvas.current_frame_idx)

    def delete_frame(self):
        self.canvas.delete_frame()
        self.update_thumbnails()
        self.thumb_list.setCurrentRow(self.canvas.current_frame_idx)

    def move_frame(self, delta):
        self.canvas.move_frame(delta)
        self.update_thumbnails()
        self.thumb_list.setCurrentRow(self.canvas.current_frame_idx)

    def change_frame(self, idx):
        if idx < 0 or idx >= len(self.canvas.frames):
            return
        self.canvas.current_frame_idx = idx
        self.canvas.update()

    def update_thumbnails(self):
        self.thumb_list.clear()
        for i, frame in enumerate(self.canvas.frames):
            pixmap = QPixmap.fromImage(frame.image).scaled(
                THUMBNAIL_SIZE, THUMBNAIL_SIZE, Qt.AspectRatioMode.KeepAspectRatio)
            item = QListWidgetItem(f"Frame {i+1}")
            item.setIcon(QIcon(pixmap))
            self.thumb_list.addItem(item)

    def select_tool(self, tool_name):
        self.canvas.set_tool(tool_name)

    def pick_color(self):
        color = QColorDialog.getColor(initial=self.canvas.brush_color, parent=self)
        if color.isValid():
            self.canvas.set_brush_color(color)
            self.color_btn.setStyleSheet(f"background-color: {color.name()}")

    def change_brush_size(self, val):
        self.canvas.set_brush_size(val)

    def change_eraser_size(self, val):
        self.canvas.set_eraser_size(val)

    def toggle_onion(self, state):
        enabled = self.onion_checkbox.isChecked()
        self.canvas.toggle_onion(enabled)

    def change_opacity(self, val):
        self.canvas.set_onion_opacity(val / 100)

    def export_gif(self):
        if not self.canvas.frames:
            QMessageBox.information(self, "No frames", "No frames to export")
            return
        path, _ = QFileDialog.getSaveFileName(self, "Save GIF", "", "GIF Files (*.gif)")
        if not path:
            return

        pil_images = []
        for frame in self.canvas.frames:
            img = frame.image
            img = img.convertToFormat(QImage.Format.Format_RGBA8888)
            ptr = img.constBits()
            ptr.setsize(img.sizeInBytes())
            pil_img = Image.frombuffer("RGBA", (img.width(), img.height()), ptr, "raw", "RGBA", 0, 1)
            pil_images.append(pil_img)

        pil_images[0].save(path, save_all=True, append_images=pil_images[1:], loop=0, duration=1000//12, disposal=2)

        QMessageBox.information(self, "Export complete", f"GIF saved to:\n{path}")

    def export_pngs(self):
        if not self.canvas.frames:
            QMessageBox.information(self, "No frames", "No frames to export")
            return
        dir_path = QFileDialog.getExistingDirectory(self, "Select Folder to Save PNGs")
        if not dir_path:
            return

        for i, frame in enumerate(self.canvas.frames):
            path = os.path.join(dir_path, f"frame_{i+1}.png")
            frame.image.save(path)

        QMessageBox.information(self, "Export complete", f"Exported PNG frames to:\n{dir_path}")

    def export_spritesheet(self):
        if not self.canvas.frames:
            QMessageBox.information(self, "No frames", "No frames to export")
            return
        path, _ = QFileDialog.getSaveFileName(self, "Save Spritesheet PNG", "", "PNG Files (*.png)")
        if not path:
            return

        w = self.canvas.frames[0].w
        h = self.canvas.frames[0].h
        count = len(self.canvas.frames)

        sheet_width = w * count
        sheet_height = h

        spritesheet = QImage(sheet_width, sheet_height, QImage.Format.Format_RGBA8888)
        spritesheet.fill(Qt.GlobalColor.transparent)

        painter = QPainter(spritesheet)
        for i, frame in enumerate(self.canvas.frames):
            painter.drawImage(i*w, 0, frame.image)
        painter.end()

        spritesheet.save(path)

        json_path = os.path.splitext(path)[0] + '.json'
        data = {
            "frame_width": w,
            "frame_height": h,
            "frame_count": count,
            "frames": [{"index": i, "x": i*w, "y": 0} for i in range(count)]
        }
        with open(json_path, 'w') as f:
            json.dump(data, f, indent=2)

        QMessageBox.information(self, "Export complete", f"Saved spritesheet and JSON:\n{path}\n{json_path}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())