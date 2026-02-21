# FilenameEdit

A lightweight Qt Widgets desktop application for **batch renaming files** by
removing a specified substring from file base names while preserving extensions.

---

## Features

- **Table-based preview** – shows *Original Name*, *Preview Name*, and *Status*
  for every selected file before any changes are made.
- **Base-name-only edits** – extensions are never touched.
- **Case-sensitive / case-insensitive** substring removal (toggle via checkbox).
- **Per-file validation** with colour-coded status:
  - ✓ OK (green) – rename is safe to apply.
  - — Unchanged (gray) – the substring was not found; file is skipped.
  - ✗ Error (red) – empty result name, path separator in name, duplicate target
    within the batch, or target already exists on disk.
- **Apply is blocked** until all entries are either valid or unchanged.
- **Swap/cycle-safe two-phase rename** – files are first moved to unique
  temporary names, then to their final names, so renaming A→B and B→A works.
- **Results dialog** – a summary listing every file's outcome (success or
  failure reason) is shown after applying.
- **Fusion style** with a minimal light QSS for a clean, cross-platform UI.

---

## Requirements

| Component | Version |
|-----------|---------|
| OS        | Windows 11 (also works on Linux/macOS) |
| Qt        | 6.8 (Qt Widgets module) |
| Compiler  | MSVC 2022 (x64) |
| Build tool| CMake ≥ 3.5 |
| IDE       | Qt Creator *or* Visual Studio 2022 with Qt VS Tools |

---

## Build & Run (Windows 11 + Qt 6.8 + VS 2022)

### Using Qt Creator

1. Install **Qt 6.8** via the [Qt Online Installer](https://www.qt.io/download).
   Select the *MSVC 2022 64-bit* kit.
2. Open Qt Creator → **File › Open File or Project…** → select `CMakeLists.txt`.
3. Choose the *Qt 6.8 MSVC 2022 64-bit* kit and click **Configure Project**.
4. Press **Ctrl+R** (or the green ▶ button) to build and run.

### Using the command line (Developer Command Prompt for VS 2022)

```bat
:: Set Qt path (adjust to your installation)
set QT_DIR=C:\Qt\6.8.0\msvc2022_64

:: Configure
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_PREFIX_PATH="%QT_DIR%"

:: Build
cmake --build build --config Release

:: Run
build\Release\FilenameEdit.exe
```

> **Tip:** If the executable cannot find Qt DLLs, run
> `%QT_DIR%\bin\windeployqt.exe build\Release\FilenameEdit.exe` once to copy
> the required DLLs next to the executable.

---

## Usage

1. **Add files** – click *Add Files…* (or *File › Add Files…* / `Ctrl+O`) and
   select one or more files in the dialog.  The files appear in the table.
2. **Enter a substring** – type the text you want to remove in the
   *Remove substring* field.  The table updates the *Preview Name* and *Status*
   columns in real time.  Enable *Case-insensitive* if needed.
3. **Review** – check that all rows show **OK** or **Unchanged**.  Rows with
   errors are highlighted in red and block the Apply button.
4. **Apply** – click *Apply (Rename)* to perform the renames.  A summary dialog
   reports the outcome for each file.
5. **Clear** – click *Clear* to remove all entries from the table.

---

## Notes

- Qt Creator project-specific files (`*.user`, `*.user.*`, `.qtcreator/`) are
  listed in `.gitignore` and are not tracked by this repository.
- The application uses a **two-phase rename** strategy: every file is first
  moved to a hidden temporary name, then to its final name.  This makes
  swap and cycle renames (e.g. `a.txt ↔ b.txt`) work correctly.

