#! /usr/bin/env python3

import pathlib
import matplotlib.pyplot
import dataclasses
import typing



@dataclasses.dataclass
class Entry:

    relative_time : typing.Any
    cycles_done   : typing.Any
    cur_item      : typing.Any
    corpus_count  : typing.Any
    pending_total : typing.Any
    pending_favs  : typing.Any
    map_size      : typing.Any
    saved_crashes : typing.Any
    saved_hangs   : typing.Any
    max_depth     : typing.Any
    execs_per_sec : typing.Any
    total_execs   : typing.Any
    edges_found   : typing.Any
    total_crashes : typing.Any
    servers_count : typing.Any

    def __post_init__(self):
        self.relative_time = int  (self.relative_time)
        self.cycles_done   = int  (self.cycles_done  )
        self.cur_item      = int  (self.cur_item     )
        self.corpus_count  = int  (self.corpus_count )
        self.pending_total = int  (self.pending_total)
        self.pending_favs  = int  (self.pending_favs )
        self.map_size      = float(self.map_size.removesuffix('%'))
        self.saved_crashes = int  (self.saved_crashes)
        self.saved_hangs   = int  (self.saved_hangs  )
        self.max_depth     = int  (self.max_depth    )
        self.execs_per_sec = float(self.execs_per_sec)
        self.total_execs   = int  (self.total_execs  )
        self.edges_found   = int  (self.edges_found  )
        self.total_crashes = int  (self.total_crashes)
        self.servers_count = int  (self.servers_count)

mode_entries = [
    [
        Entry(*line.split(', '))
        for line in pathlib.Path(f'./lab_3/out/{mode}/default/plot_data').read_text().splitlines()[1:]
    ]
    for mode in range(5)
]



mode_datapoints = [
    sorted([
        (
            entry.relative_time / 60 / 60,
            entry.total_execs,
            #entry.corpus_count,
        )
        for entry in entries[0::64]
    ])
    for entries in mode_entries
]



for mode, _ in enumerate(mode_entries):
    matplotlib.pyplot.plot(
        [x for x, y in mode_datapoints[mode]],
        [y for x, y in mode_datapoints[mode]],
        label = f'{mode}',
    )

matplotlib.pyplot.title(
    'TMP',
    fontsize   = 14,
    fontweight = 'bold',
)

matplotlib.pyplot.xlabel(
    'TMP',
    fontsize = 12,
)

matplotlib.pyplot.ylabel(
    'TMP',
    fontsize = 12,
)

matplotlib.pyplot.grid(
    True,
    linestyle = ':',
    alpha     = 0.6,
)

matplotlib.pyplot.legend()
matplotlib.pyplot.show()
