#!/usr/bin/env python

from subprocess import Popen
from subprocess import PIPE
import os
import time

make_proc = Popen(["make", "simple_taboo_search-7"])
make_proc.communicate()

exec_dir = os.getcwd()

benchmark = os.path.expanduser("~/benchmark")
if not os.path.isdir(benchmark):
  os.mkdir(benchmark)

os.chdir(benchmark)

max_size = 200
num_of_jobs = 5
condor_job = """Universe     = standard
Executable   = %s/simple_taboo_search-7
Arguments    = %d 113.ce

should_transfer_files = YES
transfer_input_files = %s/113.ce

Log          = %s/log
Error        = %s/err
Output       = %s/out

Queue
"""

for i in range(1, num_of_jobs + 1):
  outdir = "delay_from_113_out_max_" + str(max_size) + "_" + str(i)
  os.mkdir(outdir)
  job = condor_job % (exec_dir, max_size + 1, benchmark, outdir, outdir, outdir)
  p1 = Popen(["echo", "-e", job], stdout = PIPE)
  p2 = Popen("condor_submit", stdin = p1.stdout)
  p1.stdout.close()
  p2.communicate()
  time.sleep(90)
