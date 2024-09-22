import gdb
gdb.execute("file tmp/to_debug")
gdb.execute("break sha256_transform")
gdb.execute("run")
gdb.execute("p/x *state")
gdb.execute("p/x *data")
gdb.execute("continue")
