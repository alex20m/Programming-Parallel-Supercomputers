import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0,2*np.pi)
y = np.sin(x)
plt.plot(x,y,label="your plot")
plt.savefig("your_plot.png")
plt.show()
