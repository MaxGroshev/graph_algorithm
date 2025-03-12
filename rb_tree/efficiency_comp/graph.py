from   matplotlib import pyplot
from   textwrap   import wrap
import numpy
import matplotlib.ticker as ticker

#-------------------------------------READING_OF_FILE_AND_PREPARE_DATA_FOR_GRAPH--------------------------------------

whole_data    = []
num_of_elems  = []
rb_tree_time = []
set_time      = []

with open('start_data.txt') as file:
    for line in file:
        whole_data.append([float(x) for x in line.split()])

for x in (whole_data):
    num_of_elems.append(x[0])
    rb_tree_time.append(x[1])
    set_time.append(x[2])

#--------------------------------------------PIC_SETTINGS------------------------------------------------------------

fig, ax = pyplot.subplots (figsize = (16, 10), dpi = 100)

#-----------------------------------------SET_AXIS_LOCATORS-----------------------------------------------------------

ax.axis([0, max(num_of_elems) + max(num_of_elems)* 0.1, 0, max(set_time) + max(set_time) * 0.1])


#Set name of axis
ax.set_ylabel ("Run time, t (c * $10^{-4}$)", size = 20)
ax.set_xlabel ("Num of elements, n",    size = 20)

#--------------------------------------SET_BUILDING_AREA_SET----------------------------------------------------------

ax.set_title ("\n".join(wrap('Pic. 1.Comparison of efficiency of distance function on small dataset', 60)), loc = 'center', size = 30)

ax.grid(which = 'major', color = 'gray')
ax.grid(which = 'minor', color = 'gray', linestyle = ':')
ax.minorticks_on ()


ax.plot(num_of_elems, rb_tree_time, color = '#21421E', linewidth = 3, label = 'rb tree t(n)', marker = 'D', markevery = 0.005, mfc = '#21421E')
ax.plot(num_of_elems, set_time, color = '#E32636', linewidth = 3, label = 'Set t(n)', marker = 'D', markevery = 0.05, mfc = '#E32636')


#Set legend
ax.legend(shadow = False, loc = 'upper right', fontsize = 20)

#--------------------------------------SAVING_OF_GRAPH------------------------------------------------------------

fig.savefig('start_graph.png')

