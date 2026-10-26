#! /usr/bin/env python3

import pathlib
import subprocess

for out in ('A', 'B', 'C'):

    for mode in range(5):

        for item in pathlib.Path(f'./lab_3/out-{out}/{mode}/default/crashes').iterdir():

            if not item.is_file():
                continue

            if item.stem == 'README':
                continue

            subprocess.run([
                'afl-tmin',
                '-i', item,
                '-o', f'./lab_3/minimized_crashes/out-{out}_mode-{mode}_id-{int(item.stem.split(',')[0].removeprefix('id:'))}',
                '-m', 'none',
                '--', f'./result/lab_3_miniz_harnesses/miniz_harness_{mode}', '@@'
            ])
