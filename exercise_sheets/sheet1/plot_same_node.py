import matplotlib.pyplot as plt

# Data from your table
sizes = [
    2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
    8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576
]

blocking = [
    0.201, 0.199, 0.199, 0.203, 0.217, 0.232, 0.255, 0.291, 0.309, 0.357,
    0.455, 0.598, 0.755, 1.143, 1.809, 3.150, 5.813, 10.424, 16.803, 29.369
]

synchronous = [
    0.289, 0.286, 0.286, 0.287, 0.299, 0.331, 0.304, 0.358, 0.341, 0.362,
    0.470, 0.576, 0.864, 1.281, 2.231, 4.056, 6.671, 9.398, 19.201, 35.403
]

nonblocking = [
    0.155, 0.154, 0.158, 0.155, 0.163, 0.196, 0.181, 0.215, 0.215, 0.227,
    0.268, 0.341, 0.809, 1.597, 3.285, 6.292, 12.292, 24.585, 44.730, 91.587
]

# Create the plot
plt.figure(figsize=(10,6))

plt.plot(sizes, blocking, marker='o', label='Blocking')
plt.plot(sizes, synchronous, marker='s', label='Synchronous')
plt.plot(sizes, nonblocking, marker='^', label='Nonblocking')

plt.xscale('log')  # log scale for message size
plt.yscale('log')  # log scale for time (μs)

plt.xlabel('Message size (bytes)')
plt.ylabel('Time (μs)')
plt.title('MPI Point-to-Point Communication Time, Same Node')
plt.grid(True, which="both", ls="--", lw=0.5)
plt.legend()

plt.tight_layout()
plt.show()