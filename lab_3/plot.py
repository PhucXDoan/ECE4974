#! /usr/bin/env python3

import math
import pathlib
import matplotlib.lines
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
        self.map_size      = float(self.map_size.removesuffix('%')) / 100
        self.saved_crashes = int  (self.saved_crashes)
        self.saved_hangs   = int  (self.saved_hangs  )
        self.max_depth     = int  (self.max_depth    )
        self.execs_per_sec = float(self.execs_per_sec)
        self.total_execs   = int  (self.total_execs  )
        self.edges_found   = int  (self.edges_found  )
        self.total_crashes = int  (self.total_crashes)
        self.servers_count = int  (self.servers_count)



shown_entries = {
    mode : {
        out : [
            Entry(*line.split(', '))
            for line in pathlib.Path(f'./lab_3/out-{out}/{mode}/default/plot_data').read_text().splitlines()[1:]
        ]
        for out in ('A', 'B', 'C')
    }
    for mode in (
#        0,
#        1,
        2,
        3,
        4,
    )
}



figure, axes = matplotlib.pyplot.subplots(
    1,
    1,
    figsize = (18, 10),
    squeeze = False,
)

crash_handle = matplotlib.lines.Line2D(
    [], [],
    marker     = 'x',
    markersize = 9,
    linestyle  = '',
    color      = 'black',
    label      = 'New crash',
)

matplotlib.rcParams['font.family'] = ['Ubuntu Mono']



for axis, (f, x_label, y_label) in zip(
    axes.flatten(),
    (
        #(lambda entry: (entry.relative_time / 60 / 60, entry.total_execs), 'Run Time (Hours)', 'Total Executions'),
        #(lambda entry: (entry.relative_time / 60 / 60, entry.corpus_count), 'Run Time (Hours)', 'Corpus Count'),
        #(lambda entry: (entry.total_execs, entry.corpus_count), 'Total Executions', 'Corpus Count'),
        #(lambda entry: (entry.relative_time / 60 / 60, entry.map_size), 'Run Time (Hours)', 'Map Size'),
        (lambda entry: (entry.total_execs, entry.map_size), 'Total Executions', 'Map Size'),
    )
):

    for mode, mode_entries in shown_entries.items():

        for out, entries in mode_entries.items():

            points  = list(zip(*map(f, entries)))
            crashes = [
                i
                for i in range(1, len(entries))
                if entries[i - 1].saved_crashes < entries[i].saved_crashes
            ]

            line, = axis.plot(
                points[0],
                points[1],
                label = f'Out-{out} Mode {mode}',
                color = {
                    0 : 'red',
                    1 : 'green',
                    2 : 'blue',
                    3 : 'orange',
                    4 : 'purple',
                }[mode],
                linestyle = {
                    'A' : '-',
                    'B' : '--',
                    'C' : ':',
                }[out],
                alpha = 1 if out == 'A' else 0.25,
                linewidth = 3,
            )

            axis.plot(
                [points[0][crash] for crash in crashes],
                [points[1][crash] for crash in crashes],
                linestyle       = '',
                marker          = 'x',
                markersize      = 16,
                markeredgewidth = 2,
                color           = line.get_color(),
                alpha           = line.get_alpha(),
                zorder          = 3,
            )

    axis.tick_params(axis = 'both', labelsize = 14)
    axis.set_xlabel(x_label, fontsize = 24)
    axis.set_ylabel(y_label, fontsize = 24)
    axis.grid(True, linestyle = ':', alpha = 0.6)
    handles, labels = axis.get_legend_handles_labels()
    axis.legend(handles + [crash_handle], labels + ['New crash'])

matplotlib.pyplot.show()
