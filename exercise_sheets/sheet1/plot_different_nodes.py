import matplotlib.pyplot as plt

# Data from your new table
sizes = [
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
    8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576
]

blocking = [
    3.575, 2.356, 2.383, 2.353, 2.353, 2.364, 2.337, 2.730,
    2.786, 2.709, 2.782, 2.928, 3.105, 3.456, 4.101, 7.385,
    8.731, 11.428, 16.851, 27.747, 49.433
]

synchronous = [
    5.077, 3.155, 3.119, 3.126, 3.124, 3.115, 3.105, 3.452,
    3.420, 3.461, 3.549, 3.695, 3.858, 4.215, 4.825, 8.179,
    9.467, 12.140, 17.541, 28.330, 50.047
]

nonblocking = [
    1.708, 1.703, 1.689, 1.695, 1.695, 1.704, 1.739, 1.798,
    1.983, 2.000, 2.052, 2.121, 2.159, 2.342, 2.656, 4.389,
    5.163, 6.928, 10.855, 18.797, 35.255
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
plt.title('MPI Point-to-Point Communication Time, Different Nodes')
plt.grid(True, which="both", ls="--", lw=0.5)
plt.legend()

plt.tight_layout()
plt.show()