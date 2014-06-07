import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

x = np.linspace(0, 2 * np.pi, 100)
y1 = np.sin(x)
y2 = np.sin(3 * x)
y3 = np.sin(0.5 * x)
graph = [plt.plot(x, y1), plt.plot(x, y2)]

axcolor = 'lightgoldenrodyellow'
resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Add Series', color=axcolor, hovercolor='0.975')
def addButtonClick(event):
    graph[0][0].set_ydata(np.sin(0.5 * x))

button.on_clicked(addButtonClick)

plt.show()