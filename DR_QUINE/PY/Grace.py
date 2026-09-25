#!/usr/bin/env python3
# Grace
FILE = "Grace_kid.py"
WRITE = lambda f, s : open(f, "w").write(s)
MACRO = lambda s : WRITE(FILE, s % s)
DATA = '#!/usr/bin/env python3\n# Grace\nFILE = "Grace_kid.py"\nWRITE = lambda f, s : open(f, "w").write(s)\nMACRO = lambda s : WRITE(FILE, s %% s)\nDATA = %r\nMACRO(DATA)\n'
MACRO(DATA)
