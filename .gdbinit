set print pretty on
set print object on
set print static-members on
set print vtbl on
set print demangle on
set demangle-style gnu-v3
set print sevenbit-strings off

python
import sys
import os

# Add Qt Creator's debugging helper path
qt_creator_path = "/usr/share/qtcreator/debugger"
if os.path.exists(qt_creator_path) and qt_creator_path not in sys.path:
    sys.path.insert(0, qt_creator_path)
    try:
        import gdbbridge
        print("Qt GDB helpers loaded successfully")
    except ImportError as e:
        print(f"Could not load Qt GDB helpers: {e}")

end

printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
printf "Custom .gdbinit loaded\n"
