#! /usr/bin/env python3

import fcntl
import os
import pty
import select
import shutil
import signal
import struct
import subprocess
import sys
import termios
import threading
import time
import tty
import pathlib
import pyte

CHECKPOINTS = [
    10,
    60,
    10 * 60,
    30 * 60,
    1  * 60 * 60,
    8  * 60 * 60,
    24 * 60 * 60,
]

PRINT_INTERVAL = 3
MODES          = [0, 1, 2, 3, 4]
ROWS           = 50
COLUMNS        = 100
HARNESS_PATH   = pathlib.Path('./result/lab_3_miniz_harnesses')
OUT_PATH       = pathlib.Path('./lab_3/out')

OUT_PATH.mkdir(
    parents  = True,
    exist_ok = True,
)

environment = os.environ.copy()
environment['ASAN_OPTIONS'] = 'abort_on_error=1:symbolize=0:allocator_may_return_null=1'



lock = threading.Lock()



def drain(fd, stream):
    try:
        while data := os.read(fd, 65536):
            with lock:
                stream.feed(data)
    except OSError:
        pass



processes, screens = {}, {}

for mode in MODES:

    command = f'''
        afl-fuzz
        -i ./lab_3/corpus_minimized
        -o {OUT_PATH / str(mode)}
        -s 123
        -m none
        -x ./lab_3/dictionary
    '''

    if mode == 4:
        command += f'''
            -c {HARNESS_PATH / 'miniz_harness_4.cmplog'}
            -l 2AT
        '''

    command += f'''
        -- {HARNESS_PATH / f'miniz_harness_{mode}'} @@
    '''

    print(f'Running >>> {' '.join(command.split())}')

    master, slave = pty.openpty()

    fcntl.ioctl(
        slave,
        termios.TIOCSWINSZ,
        struct.pack('HHHH', ROWS, COLUMNS, 0, 0),
    )

    processes[mode] = subprocess.Popen(
        command.split(),
        stdin            = slave,
        stdout           = slave,
        stderr           = slave,
        env              = environment,
        start_new_session= True
    )

    os.close(slave)

    screens[mode] = pyte.Screen(COLUMNS, ROWS)

    threading.Thread(
        target = drain,
        args   = (master, pyte.ByteStream(screens[mode])),
        daemon = True,
    ).start()

    time.sleep(2)



start = time.monotonic()



def read_stats(mode):

    try:
        lines = (OUT_PATH / str(mode) / 'default' / 'fuzzer_stats').read_text().splitlines()
    except FileNotFoundError:
        return None

    return dict(
        tuple(part.strip() for part in line.split(':', 1))
        for line in lines if ':' in line
    )



def print_stats():

    elapsed = int(time.monotonic() - start)

    print(f'--- stats @ {elapsed // 3600:02d}:{elapsed // 60 % 60:02d}:{elapsed % 60:02d} ---')

    for mode in MODES:

        code   = processes[mode].poll()
        status = 'alive' if code is None else f'EXITED({code})'
        stats  = read_stats(mode)

        if stats is None:
            print(f'mode {mode} | {status} | no fuzzer_stats yet')
            continue

        print(
            f'mode {mode} | {status}'
            f' | run {stats.get('run_time', '?')}s'
            f' | execs {stats.get('execs_done', '?')}'
            f' | {stats.get('execs_per_sec', '?')}/s'
            f' | corpus {stats.get('corpus_count', '?')}'
            f' | crashes {stats.get('saved_crashes', '?')}'
            f' | hangs {stats.get('saved_hangs', '?')}'
            f' | cov {stats.get('bitmap_cvg', '?')}'
        )



def snapshot(mode, label):

    directory = OUT_PATH / 'snapshots'

    directory.mkdir(
        parents  = True,
        exist_ok = True,
    )

    for fuzz_statistic_file_name in ('fuzzer_stats', 'plot_data'):

        shutil.copy2(
            OUT_PATH / str(mode) / 'default' / fuzz_statistic_file_name,
            directory / f'{mode}_{label}_{fuzz_statistic_file_name}'
        )

    print(f'mode {mode}: snapshot {label}')



stdin_fd     = sys.stdin.fileno()
old_terminal = termios.tcgetattr(stdin_fd) if sys.stdin.isatty() else None

if old_terminal:
    tty.setcbreak(stdin_fd)

try:

    next_print = 0

    for seconds in sorted(CHECKPOINTS):

        while time.monotonic() - start < seconds:

            if time.monotonic() - start >= next_print:
                print_stats()
                next_print = time.monotonic() - start + PRINT_INTERVAL

            if select.select([sys.stdin], [], [], 0.5)[0]:
                os.read(stdin_fd, 1024)
                print_stats()

        for mode in MODES:
            snapshot(mode, str(seconds))

except KeyboardInterrupt:

    print('<INTERRUPTED>')

    for mode in MODES:
        snapshot(mode, 'interrupted')

finally:

    if old_terminal:
        termios.tcsetattr(stdin_fd, termios.TCSADRAIN, old_terminal)

    for process in processes.values():

        if process.poll() is None:
            os.killpg(process.pid, signal.SIGINT)

    for process in processes.values():

        try:

            process.wait(timeout = 15)

        except subprocess.TimeoutExpired:

            os.killpg(process.pid, signal.SIGKILL)
