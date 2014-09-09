import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from FileReader import OpenSpdByH

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)

spdByH = [OpenSpdByH()]

axcolor = 'lightgoldenrodyellow'
resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
AddButton = Button(resetax, 'Add Series', color=axcolor, hovercolor='0.975')

averageAx = plt.axes([0.2, 0.025, 0.1, 0.04])
AverageButton = Button(averageAx, 'Average', color=axcolor, hovercolor='0.975')

saveAx = plt.axes([0.2, 0.5, 0.1, 0.04])
SaveButton = Button(saveAx, 'Save', color=axcolor, hovercolor='0.975')


def addButtonClick(event):
    tmp = OpenSpdByH(spdByH[0]['xMax'])
    spdByH.append(tmp)

def averButtonClick(event):
    tmp = spdByH[0]['data']

    for serie in spdByH:
        for i in range(0, len(serie['data'])-1):
            for j in range(0, len(serie['data'][i])-1):
                tmp[i][j] += serie['data'][i][j]

    for i in range(0, len(tmp)-1):
        for j in range(0, len(tmp[i])-1):
            tmp[i][j] /= len(spdByH)

    spdByH.clear()
    spdByH.append(tmp)

def saveButtonClick(event):
    file = open("AveragedSplitVelocity.txt", 'w')
    file.write("Averaged data, be careful!!! Domain size = 100 \n\n")

    tmp = spdByH[0]

    for i in range(0, len(tmp[0])-1):
        for j in range(0, len(tmp)):
            file.write("%.5f" % tmp[j][i])
            file.write("     ")

        file.write("   Noise = 111 \n")

AddButton.on_clicked(addButtonClick)
AverageButton.on_clicked(averButtonClick)
SaveButton.on_clicked(saveButtonClick)

plt.legend(loc='upper center', shadow=True)
plt.show()
