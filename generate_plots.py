import matplotlib.pyplot as plt

titles = ["1000 attributes", "1000000 attributes"]
data = {
    titles[0]: {
        "O(p)": 38.2249,
        "bitset": 3.75686,
        "roaring bitmap": 5.21877
    },
    titles[1]: {
        "O(p)": 324000,
        "bitset": 310.597,
        "roaring bitmap": 8.6197
    }
}

for title in titles:
    labels = list(data[title].keys())
    values = list(data[title].values())
    
    plt.figure(figsize=(6, 4))
    plt.bar(labels, values, color=['blue', 'green', 'orange'])
    plt.title(title)
    plt.ylabel('Time (seconds)')
    plt.xlabel('Methods')
    plt.tight_layout()
    plt.show()

labels = ["bitset", "roaring bitmap"]
values = [data["1000000 attributes"]["bitset"], data["1000000 attributes"]["roaring bitmap"]]

plt.figure(figsize=(6, 4))
plt.bar(labels, values, color=['green', 'orange'])
plt.title("Comparison: Bitset vs Roaring Bitmap (1,000,000 attributes)")
plt.ylabel('Time (seconds)')
plt.xlabel('Methods')
plt.tight_layout()
plt.show()
