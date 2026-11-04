"""Desktop emulator of the 240x160 UC1698 display with 6 hardware buttons.

Usage:
    python Emulator.py <dir_with_Display_pyd> [more dirs...] [--scale N] [--fps N]
"""
import sys
import os
import argparse
import tkinter as tk

SCREEN_W = 240
SCREEN_H = 160

# Monochrome display: 1 bit per pixel (grayscale values)
PIXEL_ON = b"\x00"   # black
PIXEL_OFF = b"\xff"  # white

# Button name -> (label, keyboard shortcuts)
BUTTONS = [
    ("UP",   "Up",   ("<Up>", "w")),
    ("DOWN", "Down", ("<Down>", "s")),
    ("OK",   "OK",   ("<Return>", "<space>")),
    ("BACK", "Back", ("<Escape>", "<BackSpace>")),
    ("VIEW", "View", ("v",)),
    ("FN",   "Fn",   ("f",)),
]


def parse_args():
    parser = argparse.ArgumentParser(description="Display emulator with 6 buttons.")
    parser.add_argument(
        "pydDirs",
        nargs="+",
        help="Directory (or directories) containing the .pyd module to add to sys.path",
    )
    parser.add_argument("--scale", type=int, default=3, help="Pixel scale factor (default: 3)")
    parser.add_argument("--fps", type=int, default=30, help="Refresh rate (default: 30)")
    args = parser.parse_args()

    for pydDir in args.pydDirs:
        pydDir = os.path.abspath(pydDir)
        if not os.path.isdir(pydDir):
            parser.error(f"Directory does not exist: {pydDir}")
        if pydDir not in sys.path:
            sys.path.append(pydDir)

    if args.scale < 1:
        parser.error("--scale must be >= 1")
    if args.fps < 1:
        parser.error("--fps must be >= 1")
    return args


# Byte -> 8 grayscale pixels. u8g2 buffer for UC1698 uses "horizontal_right_lsb":
# each byte holds 8 horizontal pixels, MSB = leftmost, row stride = SCREEN_W / 8.
_LUT = [
    b"".join(PIXEL_ON if (byte & (0x80 >> bit)) else PIXEL_OFF for bit in range(8))
    for byte in range(256)
]
_PPM_HEADER = f"P5\n{SCREEN_W} {SCREEN_H}\n255\n".encode("ascii")


def buffer_to_ppm(buf: bytes) -> bytes:
    expected = SCREEN_W * SCREEN_H // 8
    if len(buf) != expected:
        raise ValueError(f"Unexpected buffer size: {len(buf)} (expected {expected})")
    lut = _LUT
    return _PPM_HEADER + b"".join(lut[b] for b in buf)


class Emulator:
    def __init__(self, display_module, scale: int, fps: int):
        self.Display = display_module
        self.scale = scale
        self.interval_ms = max(1, 1000 // fps)

        self.controller = self.Display.DisplayController()
        self.controller.init()

        self.root = tk.Tk()
        self.root.title("Display Emulator")
        self.root.resizable(False, False)
        self.root.configure(bg="#2b2b2b")

        # Screen
        self.canvas = tk.Canvas(
            self.root,
            width=SCREEN_W * scale,
            height=SCREEN_H * scale,
            highlightthickness=4,
            highlightbackground="#111111",
            bg="#000000",
        )
        self.canvas.grid(row=0, column=0, padx=12, pady=12)
        self.image_id = self.canvas.create_image(0, 0, anchor="nw")
        self.photo = None

        # Buttons
        panel = tk.Frame(self.root, bg="#2b2b2b")
        panel.grid(row=0, column=1, padx=(0, 12), pady=12, sticky="ns")

        self.status = tk.StringVar(value="Ready")

        for row, (name, label, keys) in enumerate(BUTTONS):
            button = getattr(self.Display.Button, name)
            hint = " / ".join(k.strip("<>") for k in keys)
            tk.Button(
                panel,
                text=f"{label}\n({hint})",
                width=12,
                command=lambda b=button, n=name: self.press(b, n),
            ).grid(row=row, column=0, pady=4, sticky="ew")
            for key in keys:
                self.root.bind(key, lambda _e, b=button, n=name: self.press(b, n))

        tk.Label(
            self.root, textvariable=self.status, anchor="w", bg="#2b2b2b", fg="#cccccc"
        ).grid(row=1, column=0, columnspan=2, sticky="ew", padx=12, pady=(0, 8))

        self.root.protocol("WM_DELETE_WINDOW", self.root.destroy)

    def press(self, button, name: str):
        self.Display.onButtonPressed(button)
        self.status.set(f"Button pressed: {name}")

    def refresh(self):
        try:
            self.controller.draw()
            ppm = buffer_to_ppm(self.Display.getBuffer())
            photo = tk.PhotoImage(data=ppm, format="PPM")
            if self.scale > 1:
                photo = photo.zoom(self.scale, self.scale)
            self.photo = photo  # keep a reference, otherwise Tk drops the image
            self.canvas.itemconfigure(self.image_id, image=self.photo)
        except Exception as exc:
            self.status.set(f"Error: {exc}")
            raise
        self.root.after(self.interval_ms, self.refresh)

    def run(self):
        self.refresh()
        self.root.mainloop()


def main():
    args = parse_args()
    try:
        import Display
    except ImportError as exc:
        sys.exit(f"Failed to import Display module: {exc}")

    Emulator(Display, args.scale, args.fps).run()


if __name__ == "__main__":
    main()
