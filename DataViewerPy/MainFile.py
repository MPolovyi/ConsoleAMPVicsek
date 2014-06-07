import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from FileReader import OpenSpdByH

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)

spdByH = OpenSpdByH()

x = np.arange(0, spdByH['xMax'], spdByH['xStep'])
mainPlot, = plt.plot(x, [0.5]*x.size, lw=2)
plt.axis([0, spdByH['xMax'], 0, spdByH['yMax']])

axcolor = 'lightgoldenrodyellow'
axNoise = plt.axes([0.25, 0.1, 0.65, 0.03], axisbg=axcolor)

sNoise = Slider(axNoise, 'Noise', 1, spdByH['paramRange'], valinit=0)

def update(val):
    index = int(round(sNoise.val - spdByH['paramRange'], 0))
    mainPlot.set_ydata(spdByH['data'][index])
    fig.canvas.draw_idle()

sNoise.on_changed(update)

plt.show()
