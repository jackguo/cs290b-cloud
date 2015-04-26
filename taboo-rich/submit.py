#!/usr/bin/env python

from subprocess import Popen
from subprocess import PIPE
import os

make_proc = Popen(["make"])
make_proc.communicate()

exec_dir = os.getcwd()

benchmark = os.path.expanduser("~/benchmark")
if not os.path.isdir(benchmark):
  os.mkdir(benchmark)

os.chdir(benchmark)

max_size = 80
condor_job = """Universe     = vanilla
Executable   = %s/simple_taboo_search-7
Arguments    = %d

Log          = %s/7.out
Error        = %s/7.err
Output       = %s/7.out

Queue
"""

for i in range(1, 6):
  outdir = "out_max_" + str(max_size) + "_" + str(i)
  os.mkdir(outdir)
  job = condor_job % (exec_dir, max_size + 1, outdir, outdir, outdir)
  p1 = Popen(["echo", "-e", job], stdout = PIPE)
  p2 = Popen("condor_submit", stdin = p1.stdout)
  p1.stdout.close()
  p2.communicate()
