from tkinter.filedialog import askopenfile
import re
import numpy as np

def OpenSpdByH(domSize=None):
    options = {'defaultextension': '.txt', 'filetypes': [('text files', '.txt')]}
    file = askopenfile(mode='r', **options)

    if not "Split" in file.name:
        return

    ret = {'comment': file.readline()}
    if domSize is None:
        domSize = re.search("[Dd]om.*?= (\d*\.\d*|\d*)", ret['comment']).group().split('=')[1]

    tmpData = [re.sub('\s+', ' ', line).split(' ')[:-4] for line in file]
    tmpData = tmpData[1:]

    tmpData = zip(*tmpData)

    data = [np.fromiter(item, dtype=np.float) for item in tmpData]
    ret['data'] = data
    ret['yMax'] = max([max(it) for it in data])
    ret['xStep'] = float(domSize) / len(data[0])
    ret['xMax'] = float(domSize) - (ret['xStep']/2)
    ret['paramRange'] = len(data)

    return ret