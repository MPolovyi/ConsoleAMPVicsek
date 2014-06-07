import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from FileReader import OpenSpdByH

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)

spdByH = [OpenSpdByH()]

x = np.arange(0, spdByH[0]['xMax'], spdByH[0]['xStep'])

mainPlot = plt.plot(x, [0.5]*x.size, lw=2)
plt.axis([0, spdByH[0]['xMax'], 0, spdByH[0]['yMax']])
mainAxes = plt.gca()

axcolor = 'lightgoldenrodyellow'
axNoise = plt.axes([0.25, 0.1, 0.65, 0.03], axisbg=axcolor)
sNoise = Slider(axNoise, 'Noise', 1, spdByH[0]['paramRange'], valinit=0)

def update(val):
    index = int(round(sNoise.val - spdByH[0]['paramRange'], 0))
    for i in range(0, mainPlot.__len__()):
        mainPlot[i].set_ydata(spdByH[i]['data'][index])
    fig.canvas.draw_idle()
sNoise.on_changed(update)

resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Add Series', color=axcolor, hovercolor='0.975')

def addButtonClick(event):
    plt.sca(mainAxes)
    tmp = OpenSpdByH()
    plt.axis([0, tmp['xMax'], 0, tmp['yMax']])
    spdByH.append(tmp)
    mainPlot.append(plt.plot(x, tmp['data'][0], lw=2)[0])

button.on_clicked(addButtonClick)

plt.show()
