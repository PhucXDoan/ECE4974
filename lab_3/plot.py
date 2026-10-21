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
        self.map_size      = float(self.map_size.removesuffix('%'))
        self.saved_crashes = int  (self.saved_crashes)
        self.saved_hangs   = int  (self.saved_hangs  )
        self.max_depth     = int  (self.max_depth    )
        self.execs_per_sec = float(self.execs_per_sec)
        self.total_execs   = int  (self.total_execs  )
        self.edges_found   = int  (self.edges_found  )
        self.total_crashes = int  (self.total_crashes)
        self.servers_count = int  (self.servers_count)



MODES   = range(2, 5)
STRIDE  = 16
COLUMNS = 2
PLOTS = [
    ('total_execs'  , 'Total Executions' ),
    ('edges_found'  , 'Edges Found'      ),
    ('corpus_count' , 'Corpus Count'     ),
    ('cycles_done'  , 'Cycles Done'      ),
    ('pending_total', 'Pending Total'    ),
    ('pending_favs' , 'Pending Favorites'),
]

CRASH_FIELD  = 'saved_crashes'
CRASH_MARKER = 'x'
CRASH_SIZE   = 9



mode_entries = {
    mode : [
        Entry(*line.split(', '))
        for line in pathlib.Path(f'./../oct-20th-out/{mode}/default/plot_data').read_text().splitlines()[1:]
        #for line in pathlib.Path(f'./../36f5a1934b603bbfa8f0a08abc79ab9f5d389d0f-out/{mode}/default/plot_data').read_text().splitlines()[1:]
        #for line in pathlib.Path(f'./lab_3/out/{mode}/default/plot_data').read_text().splitlines()[1:]
    ]
    for mode in range(5)
}



def get_datapoints(mode, field):
    return sorted([
        (
            entry.relative_time / 60 / 60,
            #entry.total_execs,
            getattr(entry, field),
        )
        for entry in mode_entries[mode][0::STRIDE]
    ])



def get_crash_datapoints(mode, field):

    datapoints = []
    previous   = 0

    for entry in mode_entries[mode]:
        current = getattr(entry, CRASH_FIELD)
        if current > previous:
            datapoints.append((
                entry.relative_time / 60 / 60,
                #entry.total_execs,
                getattr(entry, field),
            ))
        previous = current

    return sorted(datapoints)



rows = math.ceil(len(PLOTS) / COLUMNS)

figure, axes = matplotlib.pyplot.subplots(
    rows,
    COLUMNS,
    figsize     = (7 * COLUMNS, 4 * rows),
    squeeze     = False,
    sharex      = True,
    constrained_layout = True,
)

flat_axes = axes.flatten()

crash_handle = matplotlib.lines.Line2D(
    [], [],
    marker    = CRASH_MARKER,
    markersize = CRASH_SIZE,
    linestyle = '',
    color     = 'black',
    label     = 'New crash',
)

for axis, (field, label) in zip(flat_axes, PLOTS):

    for mode in MODES:
        datapoints = get_datapoints(mode, field)
        line, = axis.plot(
            [x for x, y in datapoints],
            [y for x, y in datapoints],
            label = f'Mode {mode}',
        )

        crash_datapoints = get_crash_datapoints(mode, field)
        axis.plot(
            [x for x, y in crash_datapoints],
            [y for x, y in crash_datapoints],
            linestyle       = '',
            marker          = CRASH_MARKER,
            markersize      = CRASH_SIZE,
            markeredgewidth = 2,
            color           = line.get_color(),
            zorder          = 3,
        )

    axis.set_title(label)
    axis.set_xlabel('Run Time (Hours)', fontsize = 12)
    #axis.set_xlabel('Total Executions', fontsize = 12)
    axis.set_ylabel(label             , fontsize = 12)
    axis.grid(True, linestyle = ':', alpha = 0.6)

    handles, labels = axis.get_legend_handles_labels()
    axis.legend(handles + [crash_handle], labels + ['New crash'])

for axis in flat_axes[len(PLOTS):]:
    axis.set_visible(False)

matplotlib.pyplot.show()
