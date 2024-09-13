import gdb

gdb.execute("file to_debug")
gdb.execute("set print pretty")
gdb.execute("break sum")
gdb.execute("run")
gdb.execute("p a")
gdb.execute("p *a@b")
gdb.execute("p b")
gdb.execute("run")
