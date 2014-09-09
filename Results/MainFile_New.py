import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from FileReader import OpenSpdByH

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)

spdByH = [OpenSpdByH()]

x = np.arange(spdByH[0]['xMax'], 0, -(spdByH[0]['xStep']))
ax.set_xlabel('h', fontsize=32)
ax.set_ylabel('v', fontsize=32)
ax.set_title(spdByH[0]['comment'])
plt.plot(x, spdByH[0]['data'][360-1], lw=2, label=u"noise = 0")
plt.axis([0, spdByH[0]['xMax'], 0, spdByH[0]['yMax']])
mainAxes = plt.gca()

axcolor = 'lightgoldenrodyellow'

'''axNoise = plt.axes([0.25, 0.1, 0.65, 0.03], axisbg=axcolor)
sNoise = Slider(axNoise, 'Noise', 1, spdByH[0]['paramRange'], valinit=0)'''

plt.sca(mainAxes)
plt.axis([0, spdByH[0]['xMax'], 0, spdByH[0]['yMax']])
plt.plot(x, spdByH[0]['data'][360-73], lw=2, label=u"noise = 72")
plt.plot(x, spdByH[0]['data'][360-145], lw=2, label=u"noise = 144")
plt.plot(x, spdByH[0]['data'][360-289], lw=2, label=u"noise = 288")
plt.plot(x, spdByH[0]['data'][360-360], lw=2, label=u"noise = 360")
'''def update(val):
    index = int(round(sNoise.val - spdByH[0]['paramRange'], 0))
    for i in range(0, mainPlot.__len__()):
        varIndex = index
        while varIndex > len(spdByH[i]['data']):
            varIndex -= len(spdByH[i]['data'])

        mainPlot[i].set_ydata(spdByH[i]['data'][varIndex])
    fig.canvas.draw_idle()


sNoise.on_changed(update)

resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Add Series', color=axcolor, hovercolor='0.975')


def addButtonClick(event):
    plt.sca(mainAxes)
    tmp = OpenSpdByH(spdByH[0]['xMax'])
    x = np.arange(0, tmp['xMax'], tmp['xStep'])
    plt.axis([0, tmp['xMax'], 0, tmp['yMax']])
    spdByH.append(tmp)
    mainPlot.append(plt.plot(x, tmp['data'][0], lw=2, label=tmp['comment'])[0])


button.on_clicked(addButtonClick)'''

leg = plt.legend(loc='upper center', shadow=True)
plt.show()
